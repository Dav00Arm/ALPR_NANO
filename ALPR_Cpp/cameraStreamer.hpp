#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <tbb/concurrent_queue.h>


// Video capture with threads. 
class CameraStreamer{
public:
    bool stop = true;
    //this holds camera stream urls
    std::vector<std::string> camera_source;
    //this holds OpenCV VideoCapture pointers
    std::vector<cv::VideoCapture> camera_capture;
    //this holds queue(s) which hold images from each camera
    std::vector<tbb::concurrent_queue<cv::Mat>> frame_queue;

    //this holds thread(s) which run the camera capture process
    std::vector<std::thread*> camera_thread;
    std::vector<cv::Mat> initialFrames;

    //Constructor for IP Camera capture

    CameraStreamer(std::vector<std::string> cam_names_, std::vector<std::string> stream_source)
    {

        camera_source = stream_source;
        camera_count = camera_source.size();
        
        startMultiCapture();
        }

    //Destructor for releasing resource(s)
    ~CameraStreamer()
    {
        stop = false;
        stopMultiCapture();
    }
    
    private:
        int camera_count;
        
        //initialize and start the camera capturing process(es)
        void startMultiCapture()
        {
            tbb::concurrent_queue<cv::Mat> q;
            cv::VideoCapture capture;
            std::thread* t;

            for (int i = 0; i < camera_count; i++)
            {
            //Make VideoCapture instance

                cv::Mat img;
                std::string url = camera_source[i];

                cv::VideoCapture capture(url);

                (capture) >> img;
                initialFrames.push_back(img);
                // if(!capture.isOpened()){
                //     continue;
                // }
                std::cout << "Camera Setup: " << url << std::endl;
                
                //Put VideoCapture to the vector
                camera_capture.push_back(capture);
                //Make thread instance
                t = new std::thread(&CameraStreamer::captureFrame, this, i);
                //Put thread to the vector
                camera_thread.push_back(t);
                //Make a queue instance
                tbb::concurrent_queue<cv::Mat> q; //Seg fault here when car detection works with gpu (Solved)
                //Put queue to the vector
                frame_queue.push_back(q);
            }
        }
        //release all camera capture resource(s)
        void stopMultiCapture()
        {
            // cv::VideoCapture cap;
            for (int i = 0; i < camera_count; i++) {
                cv::VideoCapture cap = camera_capture[i];
                if (cap.isOpened()){
                    //Relase VideoCapture resource
                    camera_thread[i]->join();
                    cap.release();
                    std::cout << "Capture " << i << " released" << std::endl;
                }
            }
        }

        void captureFrame(int index)
        {
            cv::VideoCapture capture = camera_capture[index];
            while (stop)
            {
                cv::Mat frame;
                //Grab frame from camera capture
                (capture) >> frame;
                std::mutex frame_queue_mutex;
                if (frame.empty()) {
                    std::cout << "Lost connection with camera " << index << ", attempting to reconnect..." << std::endl;
                    while (!capture.open(camera_source[index])) {
                        std::cout << "Failed to reconnect to camera " << index << ", will retry in 5 seconds..." << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                    }
                    std::cout << "Reconnected to camera " << index << std::endl;
                } else {
                    std::lock_guard<std::mutex> lock(frame_queue_mutex);
                    frame_queue[index].try_pop(frame);
                    //Put frame to the queue
                    frame_queue[index].push(frame);
                    //release frame resource
                    frame.release();
                }
            }
        }
};