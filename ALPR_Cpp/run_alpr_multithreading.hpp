#include <iostream>
#include <vector>


class ALPRMultithreading{
public:
    bool processing_status = true;
    std::vector<std::thread*> processing_threads;
    std::vector<std::vector<std::vector<cv::Point>>> spot_bboxes;
    std::vector<QLabel*> labels;
    std::vector<std::string> cam_names;
    cv::Size label_size;

    // Constructor for ALPR Multithreading
    ALPRMultithreading(std::vector<std::vector<std::vector<cv::Point>>> bboxes, std::vector<QLabel*> q_labels,
    cv::Size stream_label_size, std::vector<std::string> camera_names, CameraStreamer& camera_streamer){
        spot_bboxes = bboxes;
        labels = q_labels;
        cam_names = camera_names;
        label_size = stream_label_size;

        start_multithreading(camera_streamer);

    }

    // Destructor for ALPR Multithreading
    ~ALPRMultithreading(){
        processing_status = false;
        sleep(3);
        stop_multithreading();
    }

private:

    void start_multithreading(CameraStreamer& camera_streamer){
        for(int cam_id=0; cam_id < cam_names.size(); ++cam_id){
            std::thread* processing_thread;
            processing_thread = new std::thread(&ALPRMultithreading::run, this, cam_id, spot_bboxes[cam_id], labels[cam_id], label_size, cam_names[cam_id], std::ref(camera_streamer));
            processing_threads.push_back(processing_thread);
        }
    }

    void stop_multithreading(){
        std::cout<<"HEEEEEEEEEEEY, I'm JOINING!!!!!!!"<<std::endl;
        sleep(5);
        for (int cam_id=0; cam_id < cam_names.size(); ++cam_id) {
            processing_threads[cam_id]->join();
        }
    }



    void run(int cam_id, std::vector<std::vector<cv::Point>> spot_bboxes, QLabel* label,
    cv::Size label_size, std::string cam_name, CameraStreamer& cam){
        std::cout<<"Initializing status"<<std::endl;
        std::vector<std::string> status;
        std::vector<int> fix_times, last_idss, plate_zone_read; 
        std::vector<std::vector<std::vector<int>>> plate_zone;
        std::vector<bool> responses;
        std::unordered_map<int, std::string> changed, prev_changed;
        std::vector<std::string> last_preds;

        for(int camera_spot=0; camera_spot<spot_bboxes.size(); camera_spot++){
            fix_times.push_back(0);
            last_preds.push_back("None");
            responses.push_back(false);
            plate_zone.push_back({});
            plate_zone_read.push_back(-1);
            status.push_back("None");
        }
        std::vector<std::string> prev_status = status;

        std::cout<<"Starting"<<std::endl;
        
        while(processing_status){
            cv::Mat frame;
            std::vector<cv::Mat> ill_frames;
            std::mutex frame_queue_read_mutex;
            bool gotFrame;
            {
                std::lock_guard<std::mutex> lock(frame_queue_read_mutex);
                gotFrame = cam.frame_queue[cam_id].try_pop(frame);
            }
            std::cout<<"gotFrame == "<<gotFrame<<" "<<cam_id<<std::endl;
            
            if(gotFrame)
            {
                for(int j=0;j<spot_bboxes.size();j++)
                {
                    bbbox1 = spot_bboxes[j];
                    cv::line(frame,spot_bboxes[j][1], spot_bboxes[j][2], cv::Scalar(0,0,255),2);
                    cv::line(frame,spot_bboxes[j][0], spot_bboxes[j][1], cv::Scalar(0,0,255),2);
                    cv::line(frame,spot_bboxes[j][2], spot_bboxes[j][3], cv::Scalar(0,0,255),2);
                    cv::line(frame,spot_bboxes[j][3], spot_bboxes[j][0], cv::Scalar(0,0,255),2);
                }

                cv::Mat resized_frame;
                cv::resize(frame, resized_frame, label_size, cv::INTER_AREA);
                putText(resized_frame, cam_name, org, fontFace, fontScale, color, thickness);

                QImage qImage = MatToQImage(resized_frame);
                QPixmap pixmap = QPixmap::fromImage(qImage); 
                label->setPixmap(pixmap);

                std::vector<cv::Mat> car_images;
                std::vector<std::vector<std::vector<int>>> car_boxes;
                std::vector<std::string> pred_labels;
                std::cout<<"Car detection proflie\n";
                call_ram_info();
                std::tie(car_images, car_boxes, pred_labels) = car_detection_yolo_one_id(frame,32,false,320);

                std::unordered_map<int, std::tuple<std::vector<cv::Mat>, std::vector<std::vector<std::vector<int>>>>> out_plate;
                std::vector<std::vector<std::vector<int>>> bbox;
                if(car_images.size() > 0){
                    std::unordered_map<int, cv::Mat> cam_images;
                    std::vector<std::vector<int>> track_boxes;
                }
                std::cout<<"BEFORE Plate detection proflie\n";
                call_ram_info();
                out_plate = detect_plate_onnx_id(frame, car_images, car_boxes);
                std::cout<<"AFTER Plate detection proflie\n";
                call_ram_info();
                if(out_plate.size()>0){

                    std::unordered_map<int, cv::Mat> spot_dict;
                    std::unordered_map<int, std::string> current_spot_dict;
                    std::unordered_map<int, int> car_ind_dict;
                    std::tie(spot_dict, plate_zone_read, current_spot_dict, plate_zone, car_ind_dict) = check_box(frame, spot_bboxes, out_plate, plate_zone_read, plate_zone);
                    
                    std::unordered_map<int, std::string> car_colors_dict;
                    CarColorClassifier color_classifier;
                    car_colors_dict = color_classifier(car_images, car_ind_dict);
                    
                    for(auto spot_dict_pair: current_spot_dict){
                        auto key = spot_dict_pair.first;
                        status[key] = spot_dict_pair.second;
                        if(changed.size()>0){
                            if(changed[key] == "Busy"){
                                cv::line(frame, spot_bboxes[key][1], spot_bboxes[key][2], cv::Scalar(0,0,255),2);
                                cv::line(frame, spot_bboxes[key][0], spot_bboxes[key][1], cv::Scalar(0,0,255),2);
                                cv::line(frame, spot_bboxes[key][2], spot_bboxes[key][3], cv::Scalar(0,0,255),2);
                                cv::line(frame, spot_bboxes[key][3], spot_bboxes[key][0], cv::Scalar(0,0,255),2);
                            }
                        }
                    }
                    
                    if(prev_status != status){
                        for(int spot_i=0; spot_i<status.size(); spot_i++){
                            if(prev_status[spot_i] == "Busy" && status[spot_i] == "Free"){
                                fix_times[spot_i] = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                            }
                            else if(prev_status[spot_i] == "Free" && status[spot_i] == "Busy"){
                                fix_times[spot_i] = -1;
                            }
                        }
                        prev_status = status;
                    }
                    
                    for(int spot_time=0; spot_time<fix_times.size(); spot_time++){
                        if(fix_times[spot_time] == -1){
                            changed[spot_time] = "Busy";
                        }
                        else if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - fix_times[spot_time] - wait_time > 10){
                            changed[spot_time] = "Free";
                        }
                    }
                    
                    for(auto one_spot_dict: spot_dict){
                        if(one_spot_dict.second.rows > 0){
                            std::cout<<"BEFORE CRAFT proflie\n";
                            call_ram_info();
                            std::vector<std::vector<cv::Mat>> number_images = crop_lines(one_spot_dict.second);
                            std::cout<<"AFTER CRAFT proflie\n";
                            call_ram_info();
                            if(number_images.size()>0){
                                for(int nm_img=0; nm_img<number_images.size(); nm_img++){
                                    std::vector<cv::Mat> lines = number_images[nm_img];
                                    std::cout<<"BEFORE OCR proflie\n";
                                    call_ram_info();

                                    std::tuple<std::string,float> out = ocr_run(lines, model_ocr, converter);

                                    std::cout<<"AFTER OCR proflie\n";
                                    call_ram_info();

                                    std::string prediction = RusPlateProcess(std::get<0>(out));

                                    std::string obj_label = pred_labels[car_ind_dict[one_spot_dict.first]];
                                    std::cout << obj_label << std::endl;

                                    std::string color = car_colors_dict[one_spot_dict.first];
                                    std::cout << color << std::endl;
                                    
                                    float conf = std::get<1>(out); 
                                    conf *= 100;
                                    std::cout<<prediction<<" "<<conf<<std::endl;

                                    if(prediction.size()<4){
                                        plate_zone[one_spot_dict.first] = {};
                                    }
                                    else if(prediction != last_preds[one_spot_dict.first] && conf >= 97){
                                        last_preds[one_spot_dict.first] = prediction;
                                        std::cout<<prediction<<std::endl;
                                        responses[one_spot_dict.first] = true;
                                        // PostJSON(prepare_sending_data(mac_addresses[cam_id], std::to_string(cam_id), std::to_string(one_spot_dict.first), prediction, std::to_string((int)conf), currentDateTime()));
                                    }
                                    else if(conf < 97){
                                        plate_zone[one_spot_dict.first] = {};
                                    }
                                }
                            }
                        }
                    }
                }
            }
            

            wait_time = 0;
            if(prev_changed != changed){
                std::unordered_map<std::string, std::string> frees, frees_spot;
                for(auto changed_pair: changed){
                    int key = changed_pair.first;
                    if(changed[key] == "Free"){
                        if(prev_changed.size() > 0 && prev_changed[key] != "Free" && responses[key]){
                            frees["mac_address"] = mac_addresses[cam_id];
                            frees_spot[std::to_string(key)] = "Free";
                            frees["time"] = currentDateTime(); 
                            responses[key] = true;//PostJSON(frees, frees_spot);
                        }
                        if(prev_changed.size() == 0){
                            if(status[key] == "Free"){
                            frees["mac_address"] = mac_addresses[cam_id];
                            frees_spot[std::to_string(key)] = "Free";
                            frees["time"] = currentDateTime(); 
                            responses[key] = true;//PostJSON(frees, frees_spot);
                            }
                        }
                    }
                }
                std::cout<<status<<std::endl;
                prev_changed = changed;
            }
        } 
        return;
    }

};
