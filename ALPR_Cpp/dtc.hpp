#include "aug.hpp"
#include <vector>
#include "yolov5.hpp"
#include "genr.hpp"

DetectMultiBackend model;

// Prepare input image for ONNX inference. 
std::vector<float> onnxInput(cv::Mat img){
    img.convertTo(img,CV_32F,1.0/ 255);
    cv::Mat channels[3];
    cv::split(img,channels);
    std::vector<float> inputTensorValues;
    for(int i=0;i<img.channels();i++)
    {
        std::vector<float> data = std::vector<float>(channels[2-i].reshape(1,img.cols*img.rows));
        inputTensorValues.insert(inputTensorValues.end(),data.begin(),data.end());
    }
    return inputTensorValues;

}

// Main car detection call function.
std::tuple<std::vector<cv::Mat>, std::vector<std::vector<std::vector<int>>>, std::vector<std::string>> car_detection_yolo_one_id(cv::Mat image, int stride,bool pt,int imgsz)
{
    int bs = 1;
    cv::Mat img,im0,cropped;
    std::vector<int> confd;
    std::vector<int> classes;
    std::vector<std::vector<float>> image_processed;
    torch::Tensor im,det,inst,xyxys,xyxy,conf;
    int label;
    std::vector<double> im0_shape;
    std::vector<cv::Mat> out;
    std::vector<std::vector<std::vector<int>>> draw_boxes;
    std::vector<std::string> labels; 

    img = std::get<0>(letterbox(image,imgsz,pt,false,true,stride));

    image_processed.push_back(onnxInput(img));
    std::array<float,1*3*320*320> a;
    std::vector<std::vector<float>> y;
    std::vector<torch::Tensor> pred;
    torch::Tensor cls = torch::zeros(4);
    std::unordered_map<int, std::string> vehicle_classes = {{2, "car"}, {3, "motorcycle"}, {5, "bus"}, {7, "truck"}};
    cls[0] = 2; // car
    cls[1] = 3; // motorcycle
    cls[2] = 5; // bus
    cls[3] = 7; // truck
    
    y = model.forward(image_processed[0]);
    pred = non_max_suppression(y,0.40,0.5,cls,false,false,{},100);
    
    for(int i=0;i<pred.size();i++){
        im0 = image;
        im0_shape.push_back(im0.size[0]);
        im0_shape.push_back(im0.size[1]);
        det = pred[i];

        if (det.sizes()[0] > 0){

            det.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(0,4)}) = \
                                    scale_coords({320,320},det.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(0,4)}),im0_shape);
            xyxys = det.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(0,4)});

            for(int j=0;j<det.sizes()[0];j++){
                xyxy = xyxys[j];
                conf = det[j][4];        
                cropped = box_crop(im0,xyxy);
                label = det[j][5].item<int>();
                
                out.push_back(cropped);
                labels.push_back(vehicle_classes[label]);
                std::vector<std::vector<int>> one_box = {{xyxy[0].item<int>(), xyxy[1].item<int>()},{xyxy[2].item<int>(), xyxy[3].item<int>()}};
                draw_boxes.push_back({one_box});

            }
        }
    }
    return {out, draw_boxes, labels};
}