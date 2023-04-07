#include <iostream>
#include <string>
#include <array>
#include "gpu_envs.hpp"


// Main car detection inference class.
class DetectMultiBackend{
    public:
    Ort::RunOptions runOptions;
    Ort::AllocatorWithDefaultOptions ort_alloc;
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Session session = sessions.SessionCar();
    std::vector<std::vector<float>> forward(std::vector<float> img)
    {   

        //as of 4th Feb 2022 Onnx only supports allocation on the CPU
        char* inputName_car = session.GetInputName(0,ort_alloc);
        char* outputName_car = session.GetOutputName(0,ort_alloc);
        const std::array<const char*, 1> inputNames_car = { inputName_car };
        const std::array<const char*, 1> outputNames_car= { outputName_car };

        const std::array<int64_t, 4> inputShape = { 1, 3, 320, 320};
        const std::array<int64_t, 3> outputShape = { 1,  6300, 85};
        
        std::array<float,1*3*320*320> input;
        // std::array<float,1*6300*84> results;
        copy(img.begin(),img.end(),input.begin());
       
        auto inputTensor = Ort::Value::CreateTensor<float>(memory_info, input.data(), input.size(), inputShape.data(), inputShape.size());
        std::vector<Ort::Value> outputTensors = session.Run(Ort::RunOptions{nullptr},
                                                    inputNames_car.data(),
                                                    &inputTensor,
                                                    1,
                                                    outputNames_car.data(),
                                                    1
                                                    );

        auto* rawOutput = outputTensors[0].GetTensorData<float>();
        std::vector<int64_t> outputShape1 = outputTensors[0].GetTensorTypeAndShapeInfo().GetShape();
        size_t count = outputTensors[0].GetTensorTypeAndShapeInfo().GetElementCount();
        std::vector<float> output(rawOutput, rawOutput + count);

        int numClasses = (int)outputShape1[2] - 5;
        int elementsInBatch = (int)(outputShape1[1] * outputShape1[2]);
        std::vector<std::vector<float>> results;
        std::vector<float> tmp;
        auto options = torch::TensorOptions().dtype(at::kFloat);
        // auto start = high_resolution_clock::now();
        for(auto it=output.begin();it != output.begin() + elementsInBatch; it+=outputShape1[2]){
            tmp.clear();
            for(int i=0;i<numClasses+5;i++){
                tmp.push_back(it[i]);
            }
            
            results.push_back(tmp);
        }

        return results;

    }

};
