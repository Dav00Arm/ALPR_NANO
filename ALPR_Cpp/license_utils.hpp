#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "predictor.hpp"
using namespace std::chrono;


// Draw license plates' prediction box. 
void draw_plates(cv::Mat image, std::vector<std::vector<std::vector<int>>> bboxes){
    // std::cout<<"Image size"<<std::endl<<image.cols<<" "<<image.rows<<std::endl;
    // std::cout<<"all boxes---------------------"<<std::endl;
    // std::cout<<bboxes<<std::endl;
    for(int idx=0; idx<bboxes.size(); idx++){
        std::vector<std::vector<int>> boxes = bboxes[idx];
        // std::cout<<boxes<<std::endl;

        // for(int box=0; box<boxes.size(); box++){
        int x1 = boxes[0][0];
        int y1 = boxes[0][1]; 
        int x2 = boxes[1][0]; 
        int y2 = boxes[1][1]; 
        cv::Point pt1(x1, y1), pt2(x2, y2);
            // cv::Point x1(boxes[box][0]), x2(boxes[box][1]);
            // std::cout<<"x1"<<std::endl<<boxes[box]<<"\nx2\n"<<boxes[box]<<std::endl;
            // std::cout<<x1.x<<" "<< x1.y<<" "<<x2.x<<" "<<x2.y<<std::endl;
            // cv::putText(image,std::to_string(idx),(x1.x,x2.y-10),0,0.75,(255,255,255),2)
        // }
    }
}

// License plate detection main call function
std::unordered_map<int, std::tuple<std::vector<cv::Mat>, std::vector<std::vector<std::vector<int>>>>> 
    detect_plate_onnx_id(cv::Mat cur_frame, std::vector<cv::Mat> frames, std::vector<std::vector<std::vector<int>>> car_boxes){
    
    Predictor_ONNX model;
    std::unordered_map<int, std::tuple<std::vector<cv::Mat>, std::vector<std::vector<std::vector<int>>>>> out;
    // std::unordered_map<int, std::vector<std::vector<int>>> car_id;
    std::vector<cv::Mat> img;
    std::vector<int> conf;
    std::vector<std::vector<std::vector<int>>> bbox;
       
    for(int j = 0; j<frames.size(); j++){
        int x1 = car_boxes[j][0][0], y1 = car_boxes[j][0][1]; 
        
        torch::Tensor box = model.predict(frames[j], 1, 0.4);
        if(box.size(0)>0){
            int maximum_y = frames[j].rows, maximum_x=frames[j].cols;
            int x_min = std::max(0, box[0].item<int>());
            int x_max = std::min(box[2].item<int>(), maximum_x);
            int y_min = std::max(0, box[1].item<int>());
            int y_max = std::min(box[3].item<int>(), maximum_y);
            cv::Mat cropped = frames[j](cv::Range(y_min, y_max), cv::Range(x_min, x_max));

            bbox.push_back({{x1 + x_min,y1 + y_min}, {x1 + x_max,y1 + y_max}});
            cv::Point pt1(x1 + x_min,y1 + y_min);
            cv::Point pt2(x1 + x_max,y1 + y_max);
            cv::rectangle(cur_frame, pt1, pt2, cv::Scalar(255,0,0),2);
            img.push_back(cropped);
            
        }
        out[j] = {img, bbox};
        img.clear();
        bbox.clear();
    }

    return out;
}