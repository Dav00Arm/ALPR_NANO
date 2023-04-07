// #include "nms.hpp"
#include "transforms.hpp"
#include "onnxruntime_cxx_api.h"
// #include "Helpers.cpp"
using namespace std::chrono;

// Main plate detection inference class.
class Predictor_ONNX{
    public:
    int size, candidate_size;
    float mean, std, filter_threshold;
    const int numChannels = 3, numClasses = 3000, height = 300, width = 300;
    std::vector<int64_t> inputShape {1, numChannels, height, width};
    std::vector<int64_t> outputShape1 = { 1, numClasses, 2};
    std::vector<int64_t> outputShape2 = { 1, numClasses, 4};

    const char* net = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/plate_detection_cpu.onnx";
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator,OrtMemTypeCPU);
    Ort::Env env;
    Ort::SessionOptions session_options;
    Ort::RunOptions runOptions;
    Ort::AllocatorWithDefaultOptions ort_alloc;

    Predictor_ONNX(float filter_threshold_=0.01, int candidate_size_=200)
    {
        filter_threshold = filter_threshold_;
        candidate_size = candidate_size_;
    }
    ~Predictor_ONNX(){}
    Ort::Session session = Ort::Session(env,net,Ort::SessionOptions{ nullptr });
    // Ort::Session session = sessions.SessionPlate();
    char* inputName = session.GetInputName(0,ort_alloc);
    char* outputName1 = session.GetOutputName(0,ort_alloc);
    char* outputName2 = session.GetOutputName(1,ort_alloc);
    const std::array<const char*, 1> inputNames = { inputName };

    const std::array<const char*, 2> outputNames = { outputName1, outputName2 };


    torch::Tensor predict(cv::Mat image_, int top_k=-1, float prob_threshold = 0.3){
        auto options = torch::TensorOptions().dtype(torch::kFloat32);
        torch::Tensor image = Transform(image_);
        torch::Tensor images = torch::unsqueeze(image, 0); // MAYBE PETQ CHI

        images = images.contiguous();
        std::vector<float> input(images.data_ptr<float>(), images.data_ptr<float>() + images.numel());
        auto inputTensor = Ort::Value::CreateTensor<float>(memory_info, input.data(), input.size(), inputShape.data(), inputShape.size());

        auto outputTensor = session.Run(Ort::RunOptions{nullptr},
                                                    inputNames.data(),
                                                    &inputTensor,
                                                    1,
                                                    outputNames.data(),
                                                    2                                                    
                                                    );

        auto* confOutput = outputTensor[0].GetTensorData<float>();
        std::vector<int64_t> outputShape_scores = outputTensor[0].GetTensorTypeAndShapeInfo().GetShape();
        size_t count_conf = outputTensor[0].GetTensorTypeAndShapeInfo().GetElementCount();
        std::vector<float> output_conf(confOutput, confOutput + count_conf);

        auto* boxesOutput = outputTensor[1].GetTensorData<float>();
        std::vector<int64_t> outputShape_boxes = outputTensor[1].GetTensorTypeAndShapeInfo().GetShape();
        size_t count = outputTensor[1].GetTensorTypeAndShapeInfo().GetElementCount();
        std::vector<float> output_box(boxesOutput, boxesOutput + count);


        int elementsInBatch1 = (int)(outputShape_scores[1] * outputShape_scores[2]);
        int elementsInBatch2 = (int)(outputShape_boxes[1] * outputShape_boxes[2]);

        std::vector<std::vector<float>> results_scores, results_boxes;

        torch::Tensor scores = torch::from_blob(output_conf.data(), {3000, 2}).to(torch::kFloat32);
        torch::Tensor boxes = torch::from_blob(output_box.data(), {3000, 4}).to(torch::kFloat32);

        torch::Tensor probs = scores.index({torch::indexing::Slice(0, torch::indexing::None, 1), 1});  
        torch::Tensor mask = probs > prob_threshold;
        probs = probs.index({mask});
        if(probs.size(0) == 0){
            torch::Tensor a = torch::zeros(4);

            return a;
        }

        torch::Tensor subset_boxes = boxes.index({mask, torch::indexing::Slice(0, torch::indexing::None, 1)});
        torch::Tensor box_probs = torch::cat({subset_boxes , probs.reshape({-1,1})}, 1);
        torch::Tensor picked_box_probs = hard_nms(box_probs, top_k, candidate_size);


        picked_box_probs[0] *= image_.cols;
        picked_box_probs[1] *= image_.rows;
        picked_box_probs[2] *= image_.cols;
        picked_box_probs[3] *= image_.rows;

        return picked_box_probs.index({torch::indexing::Slice(0, 4, 1)});
    }
};
