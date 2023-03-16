#include <torch/torch.h>
#include <torch/script.h>
#include <opencv2/opencv.hpp>
#include "futrextr.hpp"


// Resize and normalize input image for OCR.
torch::Tensor ResizeNormalize(cv::Mat image){

    cv::resize(image, image, cv::Size(100,32), cv::INTER_CUBIC);

    torch::Tensor tensor_image_ = torch::from_blob(image.data ,{image.rows,image.cols,image.channels()},torch::kByte).to(torch::kFloat32).permute({2,0,1});
    tensor_image_ = tensor_image_ / 255.0;
    tensor_image_ = (tensor_image_.sub_(0.5).div_(0.5));
    return tensor_image_;
}

// Please review this class.
class AlignCollate{
    private:
    int imgH = 32, imgW = 100;
    bool keep_ratio_with_pad = false;
    public:
    torch::Tensor out_image;
    AlignCollate(cv::Mat line_){
        torch::Tensor tensor_image = ResizeNormalize(line_);
        
        out_image = tensor_image.unsqueeze(0);
    }
};

// Main ocr call and inference function
std::tuple<std::string,float> ocr_run(std::vector<cv::Mat> lines,torch::jit::Module model,AttnLabelConverter converter){
    // Main ocr inference
    std::string plate = "";
    float conf = 0;
    std::vector<std::string> preds_str;
    for(cv::Mat line: lines){
        // cv::imshow("CAP",line);
        // cv::waitKey(0);
        torch::Tensor image = AlignCollate(line).out_image;
        
        int batch_size = 1;
        torch::Tensor length_for_pred = torch::zeros(1);
        torch::Tensor text_for_pred = torch::zeros({1, 12});
        std::vector<torch::jit::IValue> inputs = {image.to(torch::kFloat32)};

        torch::Tensor preds = model.forward(inputs).toTensor();
        // std::cout << "Model "<<preds <<std::endl;
        torch::Tensor perds_index = std::get<1>(preds.max(2));

        preds_str = converter.decode(perds_index,length_for_pred);
        // torch::nn::functional::softmax(preds,)
        torch::Tensor preds_prob = torch::nn::functional::softmax(preds,torch::nn::functional::SoftmaxFuncOptions(2));


        torch::Tensor preds_max_prob = std::get<0>(preds_prob.max(2));
        std::string prediction="";
        torch::Tensor confidence_score = torch::zeros(10);
        
       
        for(int i=0;i< preds_str.size();i++)
        {
            std::string pred = preds_str[i];
            int pred_EOS = pred.find("[s]");
            torch::Tensor pred_max_prob = preds_max_prob.index({torch::indexing::Slice(0,pred_EOS)});
            
            for(int j=0;j<pred_EOS;j++)
            {
                prediction += pred[j];
            }
            confidence_score  = pred_max_prob.cumprod(0).index({-1});
        }
        plate += prediction;
        conf += confidence_score[0].item<float>();
        
    }
        conf = conf / lines.size();
    std::tuple<std::string,float> out={plate,conf};
    return out;
}