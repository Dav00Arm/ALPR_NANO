#include "config.hpp"
#include "run_alpr_multithreading.hpp"

// Main ALPR function. The whole logic of recognition.  
void alpr(std::vector<QLabel*> labels, std::vector<std::string> cam_names, std::vector<std::string> capture_source){
    std::cout<<"STARTING ALPR\n";
    call_ram_info();
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

    ALPRMultithreading program(bboxes, labels, label_size, cam_names, cam);

    while(run_status){}
    std::cout<<"run_status: "<<run_status<<std::endl;
    
    return;
}