#include "config.hpp"

bool run_status = true;

// Main ALPR function. The whole logic of recognition.  
void alpr(std::vector<QLabel*> labels, std::vector<std::string> cam_names, std::vector<std::string> capture_source){
    model_ocr.to(device);

    std::cout<<"STARTING ALPR\n";
//    call_ram_info();
    std::ifstream file;
    file.open(spot_config);
    cv::Size label_size(labels[0]->width(), labels[0]->height());
 
    std::cout<<"Reading cameras"<<std::endl;
    CameraStreamer cam(cam_names ,capture_source);
    std::cout<<"Cameras are read successfully"<<std::endl;

    if (!file)
    {
        std::ofstream file;
        file.open(spot_config);
        for(int i=0;i<cam.initialFrames.size();i++)
        // for(int i=0;i<1;i++)
        {   
            cam_img = cam.initialFrames[i];
            // cv::Mat cam_img = cv::imread("PN_car.jpg");
            for(int j=0;j<max_spots;j++)
            {
                SpotDrawing ui(cam_img,"CCTV "+std::to_string(i)); 
                bbbox1 = ui.run();
                bbbox2.push_back(bbbox1);
                cv::line(cam_img,bbbox1[0],bbbox1[1],(255,0,84),2);
                cv::line(cam_img,bbbox1[1],bbbox1[2],(255,0,84),2);
                cv::line(cam_img,bbbox1[2],bbbox1[3],(255,0,84),2);
                cv::line(cam_img,bbbox1[3],bbbox1[0],(255,0,84),2);
                file << bbbox1[0]<< " "<<bbbox1[1]<<" "<< bbbox1[2]<<" "<<bbbox1[3]<<" ";
            }

            file << std::endl;
            file << "next"<<std::endl;
            bboxes.push_back(bbbox2);
        }
        file.close();
        cv::destroyAllWindows();
    }


    bboxes = read_spots(spot_config);
    std::vector<std::vector<std::string>> status;
    
    for(int i=0; i<capture_source.size(); i++){ 
        status.push_back({});
        for(int j=0; j<bboxes[i].size(); j++){
            status[i].push_back("None");
        }
    }
    
    std::cout<<"Initializing status"<<std::endl;
    std::vector<std::vector<std::string>> prev_status = status;
    std::vector<std::vector<int>> fix_times, last_idss, plate_zone_read; 
    std::vector<std::vector<std::vector<std::vector<int>>>> plate_zone;
    std::vector<std::vector<bool>> responses;
    std::unordered_map<std::string, std::string> changed, prev_changed;
    std::vector<std::vector<std::string>> last_preds;

    for(int camera=0; camera<capture_source.size(); camera++){
        std::cout<<"Camera "<<camera<<std::endl;
        fix_times.push_back({});
        last_preds.push_back({});
        responses.push_back({});
        plate_zone.push_back({});
        plate_zone_read.push_back({});
        for(int camera_spot=0; camera_spot<bboxes[camera].size(); camera_spot++){
            fix_times[camera].push_back(0);
            last_preds[camera].push_back("None");
            responses[camera].push_back(false);
            plate_zone[camera].push_back({});
            plate_zone_read[camera].push_back(-1);
        }
    }
    std::cout<<"Starting"<<std::endl;
    auto start_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//    call_ram_info();
    while (cv::waitKey(20) != 27)
    {
        auto start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        for (int cam_id = 0; cam_id < capture_source.size(); cam_id++)
        {
            cv::Mat frame;
            std::vector<cv::Mat> ill_frames;
            bool gotFrame = cam.frame_queue[cam_id].try_pop(frame);
            // if (cam.frame_queue[cam_id].try_pop(frame))
//            std::cout<<"gotFrame == "<<gotFrame<<std::endl;
            // std::cout<<"frame.empty(): "<<frame.empty()<<std::endl;
            if(gotFrame) // && !frame.empty()
            {                
                // frame = cv::imread("PN_car2.jpg");
                std::vector<std::vector<cv::Point>> camera_spots = bboxes[cam_id];
                for(int j=0;j<camera_spots.size();j++)
                {
                    bbbox1 = camera_spots[j];
                    cv::line(frame,bboxes[cam_id][j][1],bboxes[cam_id][j][2],cv::Scalar(0,0,255),2);
                    cv::line(frame,bboxes[cam_id][j][0],bboxes[cam_id][j][1],cv::Scalar(0,0,255),2);
                    cv::line(frame,bboxes[cam_id][j][2],bboxes[cam_id][j][3],cv::Scalar(0,0,255),2);
                    cv::line(frame,bboxes[cam_id][j][3],bboxes[cam_id][j][0],cv::Scalar(0,0,255),2);
                }
                cv::Mat resized_frame;
                cv::resize(frame, resized_frame, label_size, cv::INTER_AREA);
                putText(resized_frame, cam_names[cam_id], org, fontFace, fontScale, color, thickness);

                QImage qImage = MatToQImage(resized_frame);
                QPixmap pixmap = QPixmap::fromImage(qImage); 
                labels[cam_id]->setPixmap(pixmap);

                std::vector<cv::Mat> car_images;
                std::vector<std::vector<std::vector<int>>> car_boxes;
                std::vector<std::string> pred_labels;
                std::cout<<"Car detection proflie\n";
//                call_ram_info();
                std::tie(car_images, car_boxes, pred_labels) = car_detection_yolo_one_id(frame,32,false,320);

                std::unordered_map<int, std::tuple<std::vector<cv::Mat>, std::vector<std::vector<std::vector<int>>>>> out_plate;
                std::vector<std::vector<std::vector<int>>> bbox;
                if(car_images.size() > 0){
                    std::unordered_map<int, cv::Mat> cam_images;
                    std::vector<std::vector<int>> track_boxes;
                }
                std::cout<<"BEFORE Plate detection proflie\n";
//                call_ram_info();
                out_plate = detect_plate_onnx_id(frame, car_images, car_boxes);
                std::cout<<"AFTER Plate detection proflie\n";
//                call_ram_info();
                if(out_plate.size()>0){
                    std::unordered_map<int, cv::Mat> spot_dict;
                    std::unordered_map<int, std::string> current_spot_dict;
                    std::unordered_map<int, int> car_ind_dict;
                    std::tie(spot_dict, plate_zone_read, current_spot_dict, plate_zone, car_ind_dict) = check_box(frame, cam_id, bboxes[cam_id], out_plate, plate_zone_read, plate_zone);
                    
                    std::unordered_map<int, std::string> car_colors_dict;
                    CarColorClassifier color_classifier;
                    car_colors_dict = color_classifier(car_images, car_ind_dict);

                    for(auto spot_dict_pair: current_spot_dict){
                        auto key = spot_dict_pair.first;
                        status[cam_id][key] = spot_dict_pair.second;
                        if(changed.size()>0){
                            if(changed[std::to_string(cam_id)+"_"+std::to_string(key)] == "Busy"){
                                cv::line(frame,bboxes[cam_id][key][1],bboxes[cam_id][key][2],cv::Scalar(0,0,255),2);
                                cv::line(frame,bboxes[cam_id][key][0],bboxes[cam_id][key][1],cv::Scalar(0,0,255),2);
                                cv::line(frame,bboxes[cam_id][key][2],bboxes[cam_id][key][3],cv::Scalar(0,0,255),2);
                                cv::line(frame,bboxes[cam_id][key][3],bboxes[cam_id][key][0],cv::Scalar(0,0,255),2);
                            }
                        }
                    }
                    if(prev_status != status){
                        for(int spots_i=0; spots_i<status.size(); spots_i++){
                            for(int spot_i=0; spot_i<status[spots_i].size(); spot_i++){
                                if(prev_status[spots_i][spot_i] == "Busy" && status[spots_i][spot_i] == "Free"){
                                    fix_times[spots_i][spot_i] = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                                }
                                else if(prev_status[spots_i][spot_i] == "Free" && status[spots_i][spot_i] == "Busy"){
                                    fix_times[spots_i][spot_i] = -1;
                                }
                            }
                        }
                        prev_status = status;
                    }

                    for(int spot_times=0; spot_times<fix_times.size(); spot_times++){
                        for(int spot_time=0; spot_time<fix_times[spot_times].size(); spot_time++){
                            if(fix_times[spot_times][spot_time] == -1){
                                changed[std::to_string(spot_times)+"_"+std::to_string(spot_time)] = "Busy";
                            }
                            else if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - fix_times[spot_times][spot_time] - wait_time > 10){
                                changed[std::to_string(spot_times)+"_"+std::to_string(spot_time)] = "Free";
                            }
                        }
                    }
                    for(auto one_spot_dict: spot_dict){
                        if(one_spot_dict.second.rows > 0){
                            std::cout<<"BEFORE CRAFT proflie\n";
//                            call_ram_info();
                            std::vector<std::vector<cv::Mat>> number_images = crop_lines(one_spot_dict.second);
                            std::cout<<"AFTER CRAFT proflie\n";
//                            call_ram_info();
                            if(number_images.size()>0){
                                for(int nm_img=0; nm_img<number_images.size(); nm_img++){
                                    std::vector<cv::Mat> lines = number_images[nm_img];
                                    std::cout<<"BEFORE OCR proflie\n";
//                                    call_ram_info();
                                    std::tuple<std::string,float> out;
                                    for(int i=0; i<15; i++){
                                        auto start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                                        out = ocr_run(lines, model_ocr, converter);
                                        auto end_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                                        std::cout<<"FPS: "<<1000.0/(end_time-start_time)<<std::endl;
                                    }
                                    std::cout<<"AFTER OCR proflie\n";
//                                    call_ram_info();

                                    std::string prediction = RusPlateProcess(std::get<0>(out));

                                    std::string label = pred_labels[car_ind_dict[one_spot_dict.first]];
                                    std::cout << label << std::endl;

                                    std::string color = car_colors_dict[one_spot_dict.first];
                                    std::cout << color << std::endl;
                                    
                                    float conf = std::get<1>(out); 
                                    conf *= 100;
                                    std::cout<<prediction<<" "<<conf<<std::endl;
                                    if(prediction.size()<4){
                                        plate_zone[cam_id][one_spot_dict.first] = {};
                                    }
                                    // else if(prediction != last_preds[cam_id][one_spot_dict.first] && conf >= 97){
                                    else if(conf >= 97){
                                        last_preds[cam_id][one_spot_dict.first] = prediction;
                                        std::cout<<prediction<<std::endl;
                                        responses[cam_id][one_spot_dict.first] = true;
                                        // PostJSON(prepare_sending_data(mac_addresses[cam_id], std::to_string(cam_id), std::to_string(one_spot_dict.first), prediction, std::to_string((int)conf), currentDateTime()));
                                        if(prediction == "35QP431"){
                                            std::cout<<"OPENING THE GATE\n";
                                            OpenGate(1);
                                        }
                                            
                                    }
                                    else if(conf < 97){
                                        plate_zone[cam_id][one_spot_dict.first] = {};
                                    }
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
                std::string key = changed_pair.first;
                if(changed[key] == "Free"){
                    int cam_ = stoi(split(key, "_")[0]);
                    int spot_ = stoi(split(key, "_")[1]);
                    if(prev_changed.size() > 0 && prev_changed[key] != "Free" && responses[cam_][spot_]){
                        frees["mac_address"] = mac_addresses[stoi(split(key, "_")[0])];
                        frees_spot[split(key, "_")[1]] = "Free";
                        frees["time"] = currentDateTime(); 
                        responses[stoi(split(key, "_")[0])][stoi(split(key, "_")[1])] = true;//PostJSON(frees, frees_spot);
                    }
                    if(prev_changed.size() == 0){
                        if(status[stoi(split(key, "_")[0])][stoi(split(key, "_")[1])] == "Free"){
                        frees["mac_address"] = mac_addresses[stoi(split(key, "_")[0])];
                        frees_spot[split(key, "_")[1]] = "Free";
                        frees["time"] = currentDateTime(); 
                        responses[stoi(split(key, "_")[0])][stoi(split(key, "_")[1])] = true;//PostJSON(frees, frees_spot);
                        }
                    }
                }
            }
            std::cout<<status<<std::endl;
            prev_changed = changed;
        }
        cv::waitKey(1);
        if(run_status == false){
            return; 
        }
            std::cout<<"FPS: "<<1.0/(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-start)*1000<<std::endl;
    }
    
    return;
}