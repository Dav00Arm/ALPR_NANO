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
        {   
            cam_img = cam.initialFrames[i];
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

    std::vector<std::vector<int>> plate_zone;

    for(int camera=0; camera<capture_source.size(); camera++){
        plate_zone.push_back({});
        for(int camera_spot=0; camera_spot<bboxes[camera].size(); camera_spot++){
            plate_zone[camera].push_back(-1);
        }
    }
    std::cout<<"Starting"<<std::endl;
//    call_ram_info();

    int last_req_time = 0;
    cv::Point fps_org(resized_frame.cols - 70, 40);
    while (cv::waitKey(20) != 27)
    {
        int frame_processing_start_time = 0;
        auto start = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        for (int cam_id = 0; cam_id < capture_source.size(); cam_id++)
        {
            cv::Mat frame;
            std::vector<cv::Mat> ill_frames;
            bool gotFrame = cam.frame_queue[cam_id].try_pop(frame);
            if(gotFrame)
            {
                frame_processing_start_time = time(NULL);
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

                std::vector<cv::Mat> car_images;
                std::vector<std::vector<std::vector<int>>> car_boxes;
                std::vector<std::string> pred_labels;
//                std::cout<<"Car detection proflie\n";
//                call_ram_info();
                std::tie(car_images, car_boxes, pred_labels) = car_detection_yolo_one_id(frame,32,false,320);

                std::unordered_map<int, std::tuple<std::vector<cv::Mat>, std::vector<std::vector<std::vector<int>>>>> out_plate;
                std::vector<std::vector<std::vector<int>>> bbox;
                if(car_images.size() > 0){
                    std::unordered_map<int, cv::Mat> cam_images;
                    std::vector<std::vector<int>> track_boxes;
                }
//                std::cout<<"BEFORE Plate detection proflie\n";
//                call_ram_info();
                out_plate = detect_plate_onnx_id(frame, car_images, car_boxes);
//                std::cout<<"AFTER Plate detection proflie\n";
//                call_ram_info();
//                std::cout<<"out_plate.size(): "<<out_plate.size()<<std::endl;
                if(out_plate.size()>0){
                    std::unordered_map<int, cv::Mat> spot_dict;
                    std::unordered_map<int, int> car_ind_dict;
                    std::tie(spot_dict, plate_zone, car_ind_dict) = check_box(frame, cam_id, bboxes[cam_id], out_plate, plate_zone);
                    
                    std::unordered_map<int, std::string> car_colors_dict;
                    CarColorClassifier color_classifier;
                    car_colors_dict = color_classifier(car_images, car_ind_dict);
                    for(auto one_spot_dict: spot_dict){
                        if(one_spot_dict.second.rows > 0){
//                            std::cout<<"BEFORE CRAFT proflie\n";
//                            call_ram_info();
                            std::vector<std::vector<cv::Mat>> number_images = crop_lines(one_spot_dict.second);
//                            std::cout<<"AFTER CRAFT proflie\n";
//                            call_ram_info();
                            if(number_images.size()>0){
                                plate_zone[cam_id][one_spot_dict.first] = -1;
                                for(int nm_img=0; nm_img<number_images.size(); nm_img++){
                                    std::vector<cv::Mat> lines = number_images[nm_img];
//                                    std::cout<<"BEFORE OCR proflie\n";
//                                    call_ram_info();
                                    std::tuple<std::string,float> out;

                                    out = ocr_run(lines, model_ocr, converter);
//                                    std::cout<<"AFTER OCR proflie\n";
//                                    call_ram_info();

                                    std::string prediction = RusPlateProcess(std::get<0>(out));

                                    std::string label = pred_labels[car_ind_dict[one_spot_dict.first]];

                                    std::string color = car_colors_dict[one_spot_dict.first];

                                    float conf = std::get<1>(out); 
                                    conf *= 100;
                                    std::cout<<prediction<<" "<<conf<<std::endl;
                                    if(prediction.size()<4){
                                        plate_zone[cam_id][one_spot_dict.first] = -1;
                                    }
                                    else if(conf >= ocr_conf_threshold){
                                        std::cout<<prediction<<" "<<conf<<std::endl;
                                        // PostJSON(prepare_sending_data(mac_addresses[cam_id], std::to_string(cam_id), std::to_string(one_spot_dict.first), prediction, std::to_string((int)conf), currentDateTime()));
                                        if(prediction == "41BW001" || prediction == "021LM01"){
                                            std::cout << "FINAL: " << prediction << " " << conf << " " << label << " " << color << std::endl;

                                            if(last_req_time == 0 || time(NULL) - last_req_time >= request_timeout) {
                                                std::cout << "Requesting" << std::endl;
                                                request_to_barrier(barrier_open_url);
                                                last_req_time = time(NULL);
                                                // OpenGate(1);
                                            }
                                            else{
                                                std::cout << "Can't send request, need timeout" << std::endl;
                                            }
                                        }
                                            
                                    }
                                    else if(conf < 97){
                                        plate_zone[cam_id][one_spot_dict.first] = -1;
                                    }
                                }
                            }
                        }
                    }
                }
                float fps = 1 / (time(NULL) - frame_processing_start_time + EPS);
                fps = std::round(fps * 100) / 100;
                putText(resized_frame, std::to_string(fps), fps_org, fontFace, 1.0, color, thickness);

                QImage qImage = MatToQImage(resized_frame);
                QPixmap pixmap = QPixmap::fromImage(qImage);
                labels[cam_id]->setPixmap(pixmap);
            }
        }
        wait_time = 0;

        cv::waitKey(1);
        if(run_status == false){
            return; 
        }
    }
    
    return;
}