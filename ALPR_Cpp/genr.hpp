#include <iostream>
#include "nms.hpp"
// #include <opencv2/opencv.hpp>

using namespace std::chrono;

// Convert (x,y,w,h) to (x1,y1,x2,y2).
torch::Tensor xywh2xyxy(torch::Tensor x)
{
    torch::Tensor y = x.clone();
    y.index({torch::indexing::Slice(0,torch::indexing::None),0}) = \
              x.index({torch::indexing::Slice(0,torch::indexing::None),0}) - x.index({torch::indexing::Slice(0,torch::indexing::None),2}) / 2;
    y.index({torch::indexing::Slice(0,torch::indexing::None),1}) = \
              x.index({torch::indexing::Slice(0,torch::indexing::None),1}) - x.index({torch::indexing::Slice(0,torch::indexing::None),3}) / 2;
    y.index({torch::indexing::Slice(0,torch::indexing::None),2}) = \
              x.index({torch::indexing::Slice(0,torch::indexing::None),0}) + x.index({torch::indexing::Slice(0,torch::indexing::None),2}) / 2;
    y.index({torch::indexing::Slice(0,torch::indexing::None),3}) = \
              x.index({torch::indexing::Slice(0,torch::indexing::None),1}) + x.index({torch::indexing::Slice(0,torch::indexing::None),3}) / 2;

    return y;
}

// Scaling coordinates
torch::Tensor scale_coords(std::vector<double> img1_shape,torch::Tensor coords,std::vector<double> img0_shape){
    
    float gain = std::min((double) (img1_shape[0] / img0_shape[0]),double(img1_shape[1] / img0_shape[1]));
    float pad1 = (img1_shape[1] - img0_shape[1]*gain)/ 2;
    float pad2 = (img1_shape[0] - img0_shape[0]*gain)/ 2; 

    coords.index({torch::indexing::Slice(0,torch::indexing::None),0}) -= pad1;
    coords.index({torch::indexing::Slice(0,torch::indexing::None),2}) -= pad1;
    coords.index({torch::indexing::Slice(0,torch::indexing::None),1}) -= pad2;
    coords.index({torch::indexing::Slice(0,torch::indexing::None),3}) -= pad2;
    coords.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(0,4)}) /= gain;

    return coords;
}

// Crop image with given coordinates.
cv::Mat box_crop(cv::Mat img,torch::Tensor coords){
    int maximum_x = img.cols;
    int maximum_y = img.rows;
    int x_min = std::max(0, coords[0].item<int>());
    int x_max = std::min(coords[2].item<int>(), maximum_x);
    int y_min = std::max(0, coords[1].item<int>());
    int y_max = std::min(coords[3].item<int>(), maximum_y);
    return img(cv::Range(y_min,y_max),cv::Range(x_min,x_max));
}

// NMS for car detection.
std::vector<torch::Tensor> non_max_suppression(std::vector<std::vector<float>> predictions,
                        float conf_thres=0.25,
                        float iou_thres=0.45,
                        torch::Tensor classes={},
                        bool agnostic=false,
                        bool multi_label=false,
                        std::vector<int> labels={},
                        int max_det=300)
{
    std::vector<cv::Rect> boxes;
    std::vector<float> confs;
    std::vector<int> classIds;

    torch::Tensor x;

    // std::cout << predictions.size()<<" " <<predictions[0].size()<<std::endl;     
    int n = predictions.size();
    int m = predictions[0].size();  
    auto options = torch::TensorOptions().dtype(at::kFloat);
    
    torch::Tensor pred = torch::zeros({n,m}, options);
    for (int i = 0; i < n; i++){
        pred.slice(0, i,i+1) = torch::from_blob(predictions[i].data(), {m}, options);
    }

    pred = pred.unsqueeze(0);
    int bs = pred.sizes()[0];
    int nc = pred.sizes()[2] - 5;
    torch::Tensor xc = pred.index({"...",4}) > conf_thres;


    assert( (0 <= conf_thres && conf_thres<= 1));
    assert( (0 <= iou_thres && iou_thres<= 1));

    int max_wh = 7680;
    int max_nms = 30000;
    bool redundant = true;
    multi_label = multi_label && nc > 1;
    bool merge = false;
    std::vector<torch::Tensor> outputs;
    for(int i=0;i<bs;i++){
        outputs.push_back(torch::zeros({0,6}));
    }

    for(int xi=0;xi<bs;xi++)
    {
        x = pred[xi];
        x = x.index({xc[xi]});

        x.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(5,torch::indexing::None)}) = \
                                            x.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(5,torch::indexing::None)}) \
                                                                                                                    * x.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(4,5)});
        torch::Tensor box = xywh2xyxy(x.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(0,4)}));
        std::tuple<torch::Tensor,torch::Tensor> k = x.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(5,torch::indexing::None)}).max(1,true);
        torch::Tensor conf = std::get<0>(k); 
        torch::Tensor j = std::get<1>(k);
        x = torch::cat({box,conf,j},1).index({conf.view(-1) > conf_thres});
        if (classes.sizes()[0] > 0)
        {
            x = x.index({(x.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(5,6)}) == classes).any(1)});
        }
        int n = x.sizes()[0];
        if (!n) {continue;}
        else if(n > max_nms){
            x = x.index({x.index({torch::indexing::Slice(0,torch::indexing::None),4}).argsort(true)}).index({torch::indexing::Slice(0,max_nms)});
        }

        torch::Tensor c = x.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(5,6)})*max_wh;
        torch::Tensor boxes = x.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(0,4)}) + c;
        torch::Tensor scores = x.index({torch::indexing::Slice(0,torch::indexing::None),4});
        std::vector<cv::Rect> cv_boxes;
        std::vector<float> cv_scores;
        for(int i=0;i<boxes.size(0);i++){

            cv_boxes.push_back(cv::Rect(boxes[i][0].item<int>(),boxes[i][1].item<int>(),boxes[i][2].item<int>()-boxes[i][0].item<int>(),boxes[i][3].item<int>()-boxes[i][1].item<int>()));
            cv_scores.push_back(scores[i].item<float>());
        }
        std::vector<int> nms_result;
        
        cv::dnn::NMSBoxes(cv_boxes,cv_scores,0.25,0.45,nms_result);
        int m = nms_result.size();
        torch::Tensor i = torch::ones(m).to(torch::kInt64);
           
        for(int j=0;j<m;j++){
            i[j] = nms_result[j];
        }

        if (i.sizes()[0] > max_det){
            i.index({torch::indexing::Slice(0,max_det)});
        }
        outputs[xi] = x.index({i});

    return outputs;
    }
}
