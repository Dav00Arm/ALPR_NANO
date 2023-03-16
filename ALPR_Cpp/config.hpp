#include <iostream>
#include <opencv2/opencv.hpp>
#include "cameraStreamer.hpp"
#include "draw_spots.hpp"
#include "onnxruntime_cxx_api.h"
#include "dtc.hpp"
#include "license_utils.hpp"
#include "utils.hpp"
#include "craft.hpp"
#include "ocr.hpp"
#include "client.hpp"

// All configurations for the program. 

double max_cosine_distance = 0.5;
// int nn_budget = -1;
double nms_max_overlap = 1.0;
std::string path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/traced_ocr_model_fixed.pt";
std::string spot_config = "spot_config.txt";
//  // New width and height

int max_spots = 2, wait_time = 0;    
cv::Mat cam_img;
// torch::Tensor a;
std::vector<std::vector<std::vector<cv::Point>>> bboxes;
std::vector<std::vector<cv::Point>> bbbox2;
std::vector<cv::Point> bbbox1;
std::string p = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
AttnLabelConverter converter(p);
// torch::jit::Module model_ocr;
torch::jit::Module model_ocr = torch::jit::load(path);
std::vector<std::string> capture_source = {
"rtsp://admin:22dvcgwqw7@37.252.72.204:555/cam/realmonitor?channel=1&subtype=0",
       "rtsp://admin:22dvcgwqw7@37.252.72.204:556/cam/realmonitor?channel=1&subtype=0",
        "rtsp://admin:22dvcgwqw7@37.252.72.204:557/cam/realmonitor?channel=1&subtype=0",
    "rtsp://admin:25802580Code@37.252.72.204:558/cam/realmonitor?channel=1&subtype=0",
//    "rtsp://admin:22dvcgwqw7@37.252.72.204:559/cam/realmonitor?channel=1&subtype=0"
};
std::vector<std::string> mac_addresses = {"9c:14:63:64:ec:c0","9c:14:63:64:e5:f0","9c:14:63:64:e8:ac","9c:14:63:64:eb:0a","9c:14:63:64:e8:af"};
