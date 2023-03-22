#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <tbb/concurrent_queue.h>
#include <opencv2/opencv.hpp>


// Video capture with threads. 
class CameraStreamer{
public:
//this holds camera stream urls
    std::vector<std::string> camera_source;
    //this holds usb camera indices
    std::vector<int> camera_index;
    //this holds OpenCV VideoCapture pointers
    std::vector<cv::VideoCapture*> camera_capture;
    //this holds queue(s) which hold images from each camera
    std::vector<tbb::concurrent_queue<cv::Mat>*> frame_queue;

    //this holds thread(s) which run the camera capture process
    std::vector<std::thread*> camera_thread;
    std::vector<cv::Mat> initialFrames;
    //Constructor for IP Camera capture
    CameraStreamer(std::vector<std::string> stream_source)
    {
        camera_source = stream_source;
        camera_count = camera_source.size();
        isUSBCamera = false;
        
        startMultiCapture();
        }
    //Constructor for USB Camera capture
    CameraStreamer(std::vector<int> capture_index)
    {
        camera_index = capture_index;
        camera_count = capture_index.size();
        isUSBCamera = true;
        
        startMultiCapture();
    }
    //Destructor for releasing resource(s)
    ~CameraStreamer()
    {
        stopMultiCapture();
    }
    
    private:
        bool isUSBCamera;
        int camera_count;
        //initialize and start the camera capturing process(es)
        void startMultiCapture()
        {
            cv::VideoCapture *capture;
            std::thread *t;
            tbb::concurrent_queue<cv::Mat> *q;
            for (int i = 0; i < camera_count; i++)
            {
            //Make VideoCapture instance
            if (!isUSBCamera){
                cv::Mat img;
                std::string url = camera_source[i];
                capture = new cv::VideoCapture(url);
                (*capture) >> img;
                initialFrames.push_back(img);
                std::cout << "Camera Setup: " << url << std::endl;

            }
            else{
            int idx = camera_index[i];
            capture = new cv::VideoCapture(idx);
            std::cout << "Camera Setup: " << std::to_string(idx) << std::endl;
            }
            
            //Put VideoCapture to the vector
            camera_capture.push_back(capture);
            //Make thread instance
            t = new std::thread(&CameraStreamer::captureFrame, this, i);
            
            //Put thread to the vector
            camera_thread.push_back(t);
            
            //Make a queue instance
            q = new tbb::concurrent_queue<cv::Mat>; //Seg fault here when car detection works with gpu
            
            //Put queue to the vector
            frame_queue.push_back(q);

            }
        }
        //release all camera capture resource(s)
        void stopMultiCapture()
        {
            cv::VideoCapture *cap;
            for (int i = 0; i < camera_count; i++) {
                cap = camera_capture[i];
                if (cap->isOpened()){
                    //Relase VideoCapture resource
                    cap->release();
                    std::cout << "Capture " << i << " released" << std::endl;
                    }
                }
        }
        //main camera capturing process which will be done by the thread(s)
        void captureFrame(int index)
        {
            cv::VideoCapture *capture = camera_capture[index];
            while (true)
            {
                cv::Mat frame;
                //Grab frame from camera capture
                (*capture) >> frame;
                frame_queue[index]->try_pop(frame);
                //Put frame to the queue
                frame_queue[index]->push(frame);
                //relase frame resource

                frame.release();

            }
        }
};