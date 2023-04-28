#include <torch/script.h>
#include "futrextr.hpp"
// #include <typeinfo>
#include <codecvt>
#include <locale>
// Resize and normalize input image for OCR.
std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> charToStr;

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
    bool PN = false;
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
        torch::Tensor preds_index = std::get<1>(preds.max(2));

        preds_str = converter.decode(preds_index,length_for_pred);

        // torch::nn::functional::softmax(preds,)
        torch::Tensor preds_prob = torch::nn::functional::softmax(preds,torch::nn::functional::SoftmaxFuncOptions(2));


        torch::Tensor preds_max_prob = std::get<0>(preds_prob.max(2));
        std::string prediction="";
        torch::Tensor confidence_score = torch::zeros(10);
        // torch::Tensor image = AlignCollate(line).out_image.to(torch::kCUDA);
        // int batch_size = 1;
        // torch::Tensor length_for_pred = torch::zeros(1, torch::kCUDA);
        // torch::Tensor text_for_pred = torch::zeros({1, 12}, torch::kCUDA);
        // std::vector<torch::jit::IValue> inputs = {image.to(torch::kFloat32)};
        // torch::Tensor preds = model.forward(inputs).toTensor();
        // torch::Tensor preds_index = std::get<1>(preds.max(2));
        // preds_str = converter.decode(preds_index, length_for_pred);
        // torch::Tensor preds_prob = torch::nn::functional::softmax(preds, torch::nn::functional::SoftmaxFuncOptions(2)).to(torch::kCPU);
        // torch::Tensor preds_max_prob = std::get<0>(preds_prob.max(2));
        // std::string prediction="";
        // torch::Tensor confidence_score = torch::zeros(10, torch::kCPU);


        for(int i=0;i< preds_str.size();i++)
        {
            std::string pred = preds_str[i];
            int pred_EOS = pred.find("[s]");
            torch::Tensor pred_max_prob = preds_max_prob.index({torch::indexing::Slice(0,pred_EOS)});

            for(int j=0;j<pred_EOS;j++)
            {
                if(pred[j] == 'p' && pred[j+1] == 'n'){
                    wchar_t charP = L'Պ';
                    std::string strP = charToStr.to_bytes(charP);
                    prediction += strP;
                    PN = true;
                    continue;
                }
                if(PN){

                    if(pred[j] == 'n'){
                        wchar_t charN = L'Ն';
                        std::string strN = charToStr.to_bytes(charN);
                        prediction += strN; //Armenian 'n'
                        continue;
                    }
                    if(pred[j] == 'U'){
                        wchar_t charU = L'Ս';
                        std::string strU = charToStr.to_bytes(charU);
                        prediction += strU; //Armenian 's'
                        continue;
                    }
                    if(pred[j] == 'S'){
                        wchar_t charS = L'Տ';
                        std::string strS = charToStr.to_bytes(charS);
                        prediction += strS; //Armenian 't'
                        continue;
                    }
                    if(pred[j] == 's' || pred[j] == 'G' || pred[j] == 'C'){
                        wchar_t charSH = L'Շ';
                        std::string strSH = charToStr.to_bytes(charSH);
                        prediction += strSH; //Armenian 'sh'
                        continue;
                    }
                }


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