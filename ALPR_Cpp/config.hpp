#pragma once
#include <iostream>
#include <cstdio>
#include <codecvt>
#include <locale>
#include <opencv2/opencv.hpp>
#include <torch/torch.h>
#include <QLabel>
#include "cameraStreamer.hpp"
#include "draw_spots.hpp"
#include "dtc.hpp"
#include "license_utils.hpp"
#include "utils.hpp"
#include "craft.hpp"
#include "ocr.hpp"
#include "client.hpp"
#include "car_color_classifier.hpp"


// Pixmap options
cv::Point org(25, 50);
cv::Scalar color(209, 121, 27);
float fontScale = 1.5;
int thickness = 2;
int fontFace = cv::FONT_HERSHEY_SIMPLEX;


// All configurations for the program. 
float max_cosine_distance = 0.5; 
// int nn_budget = -1;

float nms_max_overlap = 1.0;
std::string path = "ALPR_Cpp/models/ocr(PN).pt";
std::string spot_config = "spot_config.txt";
// New width and height

int max_spots = 2, wait_time = 0;    
cv::Mat cam_img;
// torch::Tensor a;
std::vector<std::vector<std::vector<cv::Point>>> bboxes;
std::vector<std::vector<cv::Point>> bbbox2;
std::vector<cv::Point> bbbox1;
// std::string p = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!ՊՆ$Շ&'()*+,-./:;<=>?@[\\]^_`{|}~"; //ՊՆՇՍՏ
std::string p = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!pn$s&'()*+,-./:;<=>?@[\\]^_`{|}~"; //ՊՆՇՍՏ
AttnLabelConverter converter(p);
// torch::jit::Module model_ocr;
torch::jit::Module model_ocr = torch::jit::load(path);
// std::vector<std::string> capture_source = {
// "rtsp://admin:22dvcgwqw7@37.252.72.204:555/cam/realmonitor?channel=1&subtype=0",
//        "rtsp://admin:22dvcgwqw7@37.252.72.204:556/cam/realmonitor?channel=1&subtype=0",
//         "rtsp://admin:22dvcgwqw7@37.252.72.204:557/cam/realmonitor?channel=1&subtype=0",
//     "rtsp://admin:25802580Code@37.252.72.204:558/cam/realmonitor?channel=1&subtype=0",
// //    "rtsp://admin:22dvcgwqw7@37.252.72.204:559/cam/realmonitor?channel=1&subtype=0"
// };
std::vector<std::string> mac_addresses = {"9c:14:63:64:ec:c0","9c:14:63:64:e5:f0","9c:14:63:64:e8:ac","9c:14:63:64:eb:0a","9c:14:63:64:e8:af"};

