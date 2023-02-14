#include "check_polygon.hpp"
#include <opencv2/opencv.hpp>

int time_now(){
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::tuple<std::unordered_map<int, cv::Mat>, std::vector<std::vector<int>>,
    std::unordered_map<int, std::string>,std::vector<std::vector<std::vector<std::vector<int>>>>>
    check_box(cv::Mat frame, int cam_id, std::vector<std::vector<cv::Point>> spots, 
    std::unordered_map<int, std::tuple<std::vector<cv::Mat>, std::vector<std::vector<std::vector<int>>>>> all_coordinates, std::vector<std::vector<int>> plate_read,
    std::vector<std::vector<std::vector<std::vector<int>>>> plate_zone){
    
    std::unordered_map<int, cv::Mat> spot_dict; // NEW PLATES TO READ
    std::unordered_map<int, std::string> current_spot_dict;
    
    for(int s=0; s<spots.size(); s++){
        current_spot_dict[s] = "Free";
    }
    
    for(auto pair: all_coordinates){
        std::vector<cv::Mat> imgs = std::get<0>(pair.second);
        std::vector<std::vector<std::vector<int>>> boxes = std::get<1>(pair.second);
        
        for(int j=0; j<boxes.size(); j++){
            for(int i=0; i<spots.size(); i++){
                bool plate_in_spot = checkInside(spots[i], boxes[j]);
                if(plate_in_spot){
                    current_spot_dict[i] = "Busy";

                    // A new car came, and plate isn't recognized yet.
                    if(plate_zone[cam_id][i].size() == 0){
                        int x1 = boxes[j][0][0]-10;
                        int y1 = boxes[j][0][1]-10;
                        int x2 = boxes[j][1][0]+10;
                        int y2 = boxes[j][1][1]+10;
                        plate_zone[cam_id][i] = {{x1, y1}, {x2, y2}};
                        spot_dict[i] = imgs[j];
                    }

                    // The plate is not in the zone
                    else if(plate_zone[cam_id][i].size() != 0 ){
                        bool plate_in_zone = checkInside(plate_zone[cam_id][i], boxes[j]);
                        if(plate_in_zone == false){
                            if(plate_read[cam_id][i] == -1){
                                plate_read[cam_id][i] = time_now();
                            }
                            if(time_now() - plate_read[cam_id][i] >= 3){
                                plate_zone[cam_id][i] = {};
                                plate_read[cam_id][i] = -1;
                            }
                        }
                    }
                }
                // Draw the zone 
                if(plate_zone[cam_id][i].size() != 0){
                    cv::Point p1(plate_zone[cam_id][i][0][0], plate_zone[cam_id][i][0][1]);
                    cv::Point p2(plate_zone[cam_id][i][1][0], plate_zone[cam_id][i][1][1]);
                    cv::rectangle(frame, p1, p2, cv::Scalar(0,255,255), 2);
                }
            }
        }
    }
    return {spot_dict, plate_read, current_spot_dict, plate_zone};
}

std::vector<std::string> split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

const std::string currentDateTime() {
    time_t     now = time(0);
    tm* tstruct;
    char       buf[80];
    tstruct = localtime(&now);
    // std::cout << tstructtm_hour<<std::endl;
    std::string time_string = "";
    strftime(buf, sizeof(buf), "%Y-%m-%d", tstruct);
    time_string += buf;
    time_string += " ";
    if (tstruct->tm_hour < 10){
        time_string += "0";
    }
    time_string += std::to_string(tstruct->tm_hour);
    time_string += ":";
    if (tstruct->tm_min < 10){
        time_string += "0";
    }
    time_string += std::to_string(tstruct->tm_min);
    time_string += ":";
    if (tstruct->tm_sec < 10){
        time_string += "0";
    }
    time_string += std::to_string(tstruct->tm_sec);
    
    // std::cout <<<std::endl;
    return time_string;
}

std::unordered_map<std::string, std::string> prepare_sending_data(std::string mac_address, std::string camera_id,
    std::string spot_id, std::string license_number, std::string confidence, std::string dtime){
    std::unordered_map<std::string, std::string> data;
    data["mac_address"] = mac_address;
    data["camera_id"] = camera_id;
    data["spot_id"] = spot_id;
    data["license_number"] = license_number;
    data["confidence"] = confidence;
    data["time"] = dtime;

    return data;
}