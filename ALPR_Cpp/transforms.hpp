#include <iostream>
#include <torch/torch.h>
#include <opencv2/opencv.hpp>

torch::Tensor Transform(cv::Mat image){
    cv::Mat cv_mat;
    cv::resize(image, cv_mat, {300,300}, 0,0,1);
    auto tensor_image = torch::from_blob(cv_mat.data, { cv_mat.rows, cv_mat.cols, cv_mat.channels() }, at::kByte);
    tensor_image = tensor_image.permute({ 2,0,1 });
    tensor_image = tensor_image.to(torch::kFloat32).sub(127).div(128.0);
    tensor_image = tensor_image.unsqueeze(0);
    return tensor_image;
}

