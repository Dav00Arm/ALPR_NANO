#include <iostream>
#include <torch/torch.h>
#include <onnxruntime_cxx_api.h>

// struct Net : torch::nn::Module{
//     Net(std::string weights="yolov5.onnx",torch::DeviceType device=torch::kCPU){
//     auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
//     input_tensor_ = Ort::Value::CreateTensor<float>(memory_info, input_image_.data(), input_image_.size(), input_shape_.data(), input_shape_.size());
//     output_tensor_ = Ort::Value::CreateTensor<float>(memory_info, results_.data(), results_.size(), output_shape_.data(), output_shape_.size());
//     }
//     torch::Tensor input
// };