#include "aug.hpp"
#include <vector>
#include "yolov5.hpp"
#include "genr.hpp"

DetectMultiBackend model;

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

std::tuple<std::vector<cv::Mat>, std::vector<std::vector<std::vector<int>>>> car_detection_yolo_one_id(cv::Mat image, int stride,bool pt,int imgsz)
{
    int bs = 1;
    cv::Mat img,im0,cropped;
    std::vector<int> confd;
    std::vector<int> classes;
    std::vector<std::vector<float>> image_processed;
    torch::Tensor im,det,inst,xyxys,xyxy,conf;
    std::vector<double> im0_shape;
    std::vector<cv::Mat> out;
    std::vector<std::vector<std::vector<int>>> draw_boxes;

    img = std::get<0>(letterbox(image,imgsz,pt,false,true,stride));

    image_processed.push_back(onnxInput(img));
    std::array<float,1*3*320*320> a;
    std::vector<std::vector<float>> y;
    std::vector<torch::Tensor> pred;
    torch::Tensor cls = torch::zeros(4);
    cls[0] = 2;
    cls[1] = 3;
    cls[2] = 5;
    cls[3] = 7;
    
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
                
                out.push_back(cropped);
                std::vector<std::vector<int>> one_box = {{xyxy[0].item<int>(), xyxy[1].item<int>()},{xyxy[2].item<int>(), xyxy[3].item<int>()}};
                draw_boxes.push_back({one_box});

            }
        }
    }
    return {out, draw_boxes};
}


// int main()
// {   
//     cv::Mat img = cv::imread("traffic.webp");
//     // char* path = "./yolov5_cpu.onnx";
//     cv::resize(img,img,cv::Size(1280,720),cv::INTER_AREA);
//     // cv::imshow("Original",img);
//     // cv::waitKey(0);
//     car_detection_yolo_one_id(img,32,false,320);

//     return 0;
// }
//     // while(true){
//     // torch::Tensor boxes = torch::zeros({12000,4});
//     // torch::Tensor output_box = torch::zeros(12000);
    
//     // auto start = high_resolution_clock::now();

//     // for(int it=0; it < 12000; it+=2){
//     //     boxes[it][0] = output_box[it];
//     //     boxes[it][1] = output_box[it];
//     //     boxes[it][2] = output_box[it];
//     //     boxes[it][3] = output_box[it];
//     // }
//     // // std::cout<<ar<<std::endl;
//     // // // for(int i=0; i<=12000; i++){
//     // // //     i = (torch::Tensor)i;
//     // // //     torch::cat({ i, ar }, 0);
//     // // // }
//     // auto stop = high_resolution_clock::now();
//     // auto duration = duration_cast<microseconds>(stop - start);
//     // std::cout << "Time taken by function: "<< duration.count()<< " seconds" << std::endl;
//     // }
//     // a =  a.reshape({25,80});
//     // std::cout << a.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(5,torch::indexing::None)})<<std::endl;
//     // a.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(5,torch::indexing::None)}) = a.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(5,torch::indexing::None)}) * a.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(4,5)});
//     // std::cout << a.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(5,torch::indexing::None)})<<std::endl;
//     // std::cout << a.index({torch::indexing::Slice(0,torch::indexing::None),torch::indexing::Slice(5,torch::indexing::None)}).max(1,true)<<std::endl;
// }