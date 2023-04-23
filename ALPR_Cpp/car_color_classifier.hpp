#include <iostream>
#include <vector>


class CarColorClassifier{
    // This class classifies car's color with opencv, supports 12 color classification 
    public:

    std::unordered_map<int, std::string> operator()(std::vector<cv::Mat> car_images, std::unordered_map<int, int> car_inds_dict, 
    float crop_size_up=0.4, float crop_size_down=0.1, float crop_size_left=0.1, float crop_size_right=0.1){
        std::unordered_map<int, std::string> preds;
        
        for(auto car_ind_pair: car_inds_dict){
            int spot_ind = car_ind_pair.first;
            int car_ind = car_ind_pair.second;
            std::string pred;
            std::vector<cv::Mat> masks, cropped_masks;
            cv::Mat hsv_image;
            
            cv::cvtColor(car_images[car_ind], hsv_image, cv::COLOR_BGR2HSV);

            masks = get_masks(hsv_image);
            cropped_masks = crope_masks(masks, crop_size_up, crop_size_down, crop_size_left, crop_size_right);
            pred = get_color(cropped_masks);
            preds[spot_ind] = pred;
        }

        return preds;
    }

    private:
    
    // Supported color names
    std::vector<std::string> colors = {
        "white", "black", "brown", "gray", "orange", "yellow", "green", "cyan", "blue", "purple", "pink", "red"
    };
    // These ranges are supported color ranges for HSV formatted images, last two ranges belongs to red color
    std::vector<cv::Scalar> range_lower_bounds = {
        cv::Scalar(0, 0, 160), cv::Scalar(0, 0, 0), cv::Scalar(0, 40, 56), cv::Scalar(0, 0, 56),
        cv::Scalar(6, 86, 86), cv::Scalar(21, 46, 56), cv::Scalar(36, 46, 56), cv::Scalar(88, 46, 56),
        cv::Scalar(103, 46, 86), cv::Scalar(127, 46, 56), cv::Scalar(152, 46, 56), cv::Scalar(0, 46, 56),
        cv::Scalar(173, 86, 56)
    };
    std::vector<cv::Scalar> range_upper_bounds = {
        cv::Scalar(179, 45, 255), cv::Scalar(179, 255, 55), cv::Scalar(65, 224, 185), cv::Scalar(179, 45, 200),
        cv::Scalar(20, 255, 255), cv::Scalar(35, 255, 255), cv::Scalar(87, 255, 255), cv::Scalar(102, 255, 255),
        cv::Scalar(131, 255, 255), cv::Scalar(157, 255, 255), cv::Scalar(176, 210, 255), cv::Scalar(5, 255, 255),
        cv::Scalar(179, 255, 255)
    };

    std::vector<cv::Mat> get_masks(cv::Mat hsv_image){
        std::vector<cv::Mat> masks;
        for(int i; i < range_lower_bounds.size() - 1; ++i){
            cv::Mat mask;
            cv::Scalar range_lower = range_lower_bounds[i], range_upper = range_upper_bounds[i];
            cv::inRange(hsv_image, range_lower, range_upper, mask);
            if(i == range_lower_bounds.size() - 2){
                cv::Mat mask2;
                cv::Scalar range_lower2 = range_lower_bounds[i + 1], range_upper2 = range_upper_bounds[i + 1];
                cv::inRange(hsv_image, range_lower2, range_upper2, mask2);
                cv::bitwise_or(mask, mask2, mask);
            }
            masks.push_back(mask);
        }
        return masks;
    }

    std::vector<cv::Mat> crope_masks(std::vector<cv::Mat> masks, float crop_size_up, float crop_size_down, float crop_size_left, float crop_size_right){
        int y_start = int(crop_size_up * masks[0].rows);
        int y_end = masks[0].rows - int(crop_size_down * masks[0].rows);

        int x_start = int(crop_size_left * masks[0].cols);
        int x_end = masks[0].cols - int(crop_size_right * masks[0].cols);

        std::vector<cv::Mat> cropped_masks;
        for (cv::Mat mask : masks) {
            cv::Mat cropped_mask = mask(cv::Range(y_start, y_end), cv::Range(x_start, x_end));
            cropped_masks.push_back(cropped_mask);
        }
        return cropped_masks;
    }

    std::string get_color(std::vector<cv::Mat> cropped_masks){
        int max_ind = 0, current_ind = 0;
        double max_value = 0.0;
        for(cv::Mat cropped_mask: cropped_masks){
            cv::Scalar cnt = cv::sum(cropped_mask);
            double total_cnt= cnt[0] + cnt[1] + cnt[2];
            if(total_cnt > max_value){
                max_value = total_cnt;
                max_ind = current_ind;
            }
            current_ind++;
        }
        return colors[max_ind];
    }
};