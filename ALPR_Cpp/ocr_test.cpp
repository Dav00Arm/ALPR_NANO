#include <torch/torch.h>
#include <torch/script.h>
#include <iostream>
// #include <memory>


int main(){

    char* path = "traced_ocr_model.pt";
    torch::jit::Module model;
    model = torch::jit::load(path);
    torch::Tensor example = torch::rand({1,1,32,100});
    // example = torch.rand(1,1,32,100)
    torch::Tensor text_for_pred = torch::zeros({1,12},torch::kInt64);
// text_for_pred = torch.LongTensor(1, 12).fill_(0)
    std::vector<torch::jit::IValue> inputs;
    // torch::LongTe
    inputs.push_back(example);
    inputs.push_back(text_for_pred);
    // inputs.push_back();
    model.forward(inputs);
    return 0;
}