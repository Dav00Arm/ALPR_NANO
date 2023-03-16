#include <opencv2/opencv.hpp>
#include <iostream> 
#include <tuple>
#include <typeinfo>
#include <array>
#include <torch/torch.h>
#include <ATen/ATen.h>

// Class for drawing spots before the program starts.

// Check whether lines 12-17 are used 
std::vector<cv::Point> coords;
std::vector<std::vector<cv::Point>> spots;

cv::Point line_coords[1][2] = {cv::Point(-1,-1)};
cv::Point current_drawing[1][2];
cv::Mat ill_image;

class SpotDrawing{
    public:
    cv::Mat image;
    std::string win_name;
    bool do_break = 0;
    
    SpotDrawing(cv::Mat img, std::string window_name){
        image=img;
        win_name = window_name;
        cv::namedWindow(win_name);
        
        cv::setMouseCallback(win_name,MouseCallback,0);

    }

    static void MouseCallback(int event, int x, int y, int, void*){
        // cout << "Coords: "<< coords.size()<<endl;
        // cout << "Spots: "<< spots.size()<<endl;
        // cout << "-----------------------"<<endl;
        if (event == cv::EVENT_LBUTTONDOWN){

            if (coords.size() != 4)
            {
                coords.push_back(cv::Point(x,y));
                if (coords.size() == 4)
                {
                    spots.push_back(coords);
                }
            
            }
        }
        if (event == cv::EVENT_MOUSEMOVE && coords.size() != 4){
            if (coords.size() > 0)
            {
                line_coords[0][0] = coords[coords.size()-1];
                line_coords[0][1] = cv::Point(x,y);
                current_drawing[0][0] = coords[0];
                current_drawing[0][1] = cv::Point(x,y);
            }
        }
    }

    std::vector<cv::Point> run()
    {
        std::vector<cv::Point> return_info;
        bool do_change = true;

        while (true){
            if (line_coords[0][0] != cv::Point(-1,-1)){
  
                ill_image =  image.clone();
                do_change = false;
     
            }

            if (do_change){
                show_image(image);
                // always_draw(image);
            } 
            else{
                draw(ill_image);
                show_image(ill_image);
                always_draw(ill_image);
            }

            if (spots.size() == 1){
                return_info = spots[0];
                coords.clear();
                spots.clear();

                line_coords[0][0] = cv::Point(-1,-1);
                line_coords[0][1] = cv::Point(-1,-1);
                current_drawing[0][0] = cv::Point(-1,-1);
                current_drawing[0][1] = cv::Point(-1,-1);
                return return_info;
            }

        
        }
       
    }
    void draw(cv::Mat img){
        cv::Point first;
        if (line_coords[0][0] != cv::Point(-1,-1))
        {
            if (coords.size() >= 2)
            {
                first = coords[0];
                for(int i=0;i<coords.size();i++){
                    if (i == 0){continue;}
                    cv::line(img,first,coords[i],cv::Scalar(255,0,255),2,cv::LINE_AA);
                    first = coords[i];

                }
            }
                cv::line(img,line_coords[0][0],line_coords[0][1],cv::Scalar(255,0,255),2,cv::LINE_AA);
                cv::line(img,current_drawing[0][0],current_drawing[0][1],cv::Scalar(255,0,255),2,cv::LINE_AA);
        }
    }     
    
    void always_draw(cv::Mat img){
        cv::Point first;
        std::vector<cv::Point> draw_line;
        if (spots.size() > 0)
        {
            for(int i=0;i<spots.size();i++){
                draw_line = spots[0];
                first = draw_line[0];
                for(int j=0;j<draw_line.size();j++){
                    if (j == 0){continue;}
                    cv::line(img,first,draw_line[j],cv::Scalar(0,0,255),2,cv::LINE_AA);
                    first = draw_line[j];

                }
                cv::line(img,draw_line[0],first,cv::Scalar(0,0,255),2,cv::LINE_AA);

            }
        }
        }
    void show_image(cv::Mat img){
        imshow(win_name,img);
        cv::waitKey(22);
    }
};

// Function that reads already drawn spots from .txt file
std::vector<std::vector<std::vector<cv::Point>>> read_spots(std::string name="spots.txt")
{

    std::vector<std::vector<cv::Point>> bbox1;
    std::ifstream  MyReadFile(name);
    std::string start_st;
    // myfile.open("data.txt");
    std::vector<std::vector<std::vector<cv::Point>>> bboxes;
    std::vector<std::vector<cv::Point>> bbbox2;
    std::vector<cv::Point> bbbox1;
    std::vector<int> tmp ;
    while( getline(MyReadFile, start_st)){
        int n = start_st.length();

        char char_array[n + 1]; 
        // cout << start_st;
        if (start_st == "next")
        {
            bboxes.push_back(bbbox2);
            bbbox2.clear();
        }
        strcpy(char_array, start_st.c_str());
        char *ptr;
        ptr = strtok(char_array,",");
    
        while (ptr != NULL)  
        {   
            std::string num="";
            std::string k=ptr;
            if (k[0] == '[')
            {
                num = k.substr(1,k.length()-1);
                tmp.push_back(stoi(num));
            }
            else if (k[k.length()-1] == ']'){
                num = k.substr(0,k.length()-1);
                tmp.push_back(stoi(num));
            }
        if (tmp.size() == 2){
            bbbox1.push_back(cv::Point(tmp[0],tmp[1]));
            tmp.clear();
        }
        if (bbbox1.size() == 4){
            bbbox2.push_back(bbbox1);
            bbbox1.clear();
        }
        ptr = strtok (NULL, " , ");
        }
        
            
            }

    return bboxes;
}

// Shows all camera frames in one grid.
torch::Tensor show_images(std::vector<cv::Mat> images,int height,int width, int k=3)
{
    int j = -1;
    cv::Mat image;
    // if (images.size() == 0){return images[0];}
    std::vector<std::vector<cv::Mat>> lists;
    std::vector<cv::Mat> tmp;
    int resize_width, resize_height;

    for (int i=0;i<images.size();i++)
    {
        if (i % k == 0){
            j ++;
            tmp.push_back(images[i]);
        }
        lists.push_back(tmp);
    }
    if (lists.size() > 1){
        resize_width = (int) width / k;
        resize_height = (int) height / k;
    }
    else{   
        resize_width = 640;
        resize_height = 480;
    }

    int current_y =0 ;
    int current_x =0 ;
    int shape_h;
    int shape_w;
    shape_h = resize_height*lists.size();
    shape_w = k* resize_width;
    at::Tensor base_image = at::zeros({shape_h,shape_w,3},at::kInt);
    // at::Tensor to
    at::Tensor tensor_image;
    for(int i=0;i<lists.size();i++)
    {
           current_x = 0;
        for(int j=0;j<lists[i].size();j++)
        {
            cv::resize(lists[i][j], image, cv::Size(resize_width, resize_height), cv::INTER_LINEAR);
            // std::cout << image.rows<<" "<<image.cols<<std::endl; 
            tensor_image = torch::from_blob(image.data ,{image.rows,image.cols,image.channels()},at::kInt);
            // tensor_image = tensor_image.permute({ 2,0,1 });
            // tensor_image.unsqueeze_(0);
            //  base_image[current_y:image.shape[0]+current_y,current_x:image.shape[1]+current_x,:] = image
            // base_image.index_put_({torch::indexing::Slice(current_y,tensor_image.size(0)+current_y),torch::indexing::Slice(current_x,tensor_image.size(1)+current_x)},tensor_image);
            auto ts = base_image.index({torch::indexing::Slice(current_y,tensor_image.size(0)+current_y),torch::indexing::Slice(current_x,tensor_image.size(1)+current_x),torch::indexing::Slice()}) ;
            
            // base_image.index({torch::indexing::Slice(current_y,tensor_image.size(0)+current_y),torch::indexing::Slice(current_x,tensor_image.size(1)+current_x),torch::indexing::Slice()}) = tensor_image;
            // std:: cout << ts.size(0)<< " "<<ts.size(1)<<" "<<ts.size(2)<<std::endl;
            // std::cout << tensor_image.size(0)<< " "<<tensor_image.size(1)<<" "<<tensor_image.size(2)<<std::endl;
            // std::cout << "                                   "<<std::endl;
            current_x += tensor_image.size(1);
        }
        current_y += tensor_image.size(0);
    }
    return tensor_image;
}
