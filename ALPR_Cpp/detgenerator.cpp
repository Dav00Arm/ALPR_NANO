#include <torch/torch.h>
#include <opencv2/opencv.hpp>

// void run_in_batches(std::unordered_map<int>, torch::Tensor out, int batch_size){
//     int data_len = out.sizes()[0];
//     int num_batches = (data_len/batch_size);
//     int s = 0, e = 0;
//     for(int i=0; i<num_batches; i++){
//         s = i * batch_size;
//         e = (i + 1) * batch_size;
        
//     }
// }

cv::Mat extract_image_patch(torch::Tensor image, std::vector<float> bbox, std::vector<int> patch_shape){
    if(patch_shape.size()==0){
        float target_aspect = patch_shape[1] / patch_shape[0];
        float new_width = target_aspect * bbox[3];
        bbox[0] -= (new_width - bbox[2]) / 2;
        bbox[2] = new_width;
    }
    bbox[2] += bbox[0];
    bbox[3] += bbox[1];
    std::vector<int> bbox_int(bbox.begin(), bbox.end());

    bbox_int[0] = std::max({0, bbox_int[0]});
    bbox_int[1] = std::max({0, bbox_int[1]});
    int l1 = image.sizes()[1]-1;
    int l2 = image.sizes()[0]-1;
    bbox_int[2] = std::min({l1, bbox_int[2]});
    bbox_int[3] = std::min({l2, bbox_int[3]});
    if(bbox_int[0] >= bbox_int[2] || bbox_int[1] >= bbox_int[3]){
        cv::Mat none;
        return none;
    }

    int sx = bbox_int[0], sy = bbox_int[1], ex = bbox_int[2], ey = bbox_int[3];
    image = image.index({torch::indexing::Slice(sy, ey, 1), torch::indexing::Slice(sx, ex, 1)});
    image = image.to(torch::kByte);
    // torch::Tensor to cv::Mat
    cv::Mat output_mat(cv::Size{ image.sizes()[1], image.sizes()[0] }, CV_8UC3, image.data_ptr<uchar>());
    cv::Mat out;
    cv::resize(output_mat, out, {patch_shape[1], patch_shape[0]});
    out.convertTo(out, CV_64F);
    return out;

}



int main()
{
//    cv::Mat cv_mat = cv::Mat::eye(3,3,CV_32F);
    torch::Tensor out_tensor = torch::arange({270000}, torch::TensorOptions().dtype(torch::kByte)).reshape({300,300,3});

    return 0;
}
