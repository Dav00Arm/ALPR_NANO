#include <iostream>
#include "onnxruntime_cxx_api.h"
#include <string>
#include <array>
#include <tuple>
#include <cmath>

// Augment image for car detection
std::tuple<cv::Mat,std::vector<float>,float,float> letterbox(cv::Mat im, int new_shape=640,bool auto_=true, bool scaleFill=false, bool scaleup=true, int stride=32){
    std::vector<float> shape;
    std::vector<float> new_shapes;
    std::vector<float> new_unpad;
    std::vector<float> ratio;
    float dw,dh;
    shape.push_back(im.size().height);
    shape.push_back(im.size().width);
    new_shapes.push_back(new_shape);
    new_shapes.push_back(new_shape);
    double r = std::min(new_shapes[0] / shape[0],new_shapes[1] / shape[1]);
    
    if (!scaleup){r = std::min(r, 1.0);}
    
    ratio.push_back(r);
    ratio.push_back(r);

    new_unpad.push_back(int(std::round(shape[1]*r)));
    new_unpad.push_back(int(std::round(shape[0]*r)));
    dw = new_shapes[1] - new_unpad[0];
    dh = new_shapes[0] - new_unpad[1];

    if (auto_){
        dw = int(dw) % stride;
        dh = int(dh) % stride;
    }
    else if (scaleFill){
        dw = 0.0;
        dh = 0.0;
        new_unpad[0] = new_shapes[1];
        new_unpad[1] = new_shapes[0];
        ratio[0] = new_shapes[1] / shape[1];
        ratio[1] = new_shapes[0] / shape[0];

    }

    dw /= 2;
    dh /= 2;

    if ((shape[0] != new_unpad[1]) && (shape[1] != new_unpad[0])){
        cv::resize(im,im,cv::Size(new_unpad[0],new_unpad[1]),cv::INTER_LINEAR);
    }
    int top = int(std::round(dh-0.1));
    int bottom = int(std::round(dh+0.1));
    int left = int(std::round(dw-0.1));
    int right = int(std::round(dw+0.1));
    cv::copyMakeBorder(im,im,top,bottom,left,right,cv::BORDER_CONSTANT,cv::Scalar(114,114,114));
     
    return {im,ratio,dw,dh};    
}
