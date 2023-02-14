#include <iostream>
#include <chrono>
#include <torch/torch.h>
#include <opencv2/opencv.hpp>
#include "onnxruntime_cxx_api.h"

using namespace std::chrono;

//PATHS OF CRAFT AND REFINER
char* craft = "./models/craft_cpu.onnx";
char* refinet = "./models/refiner_cpu.onnx";

//SESSION INFO
Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator,OrtMemTypeCPU);
Ort::Env env;
Ort::AllocatorWithDefaultOptions ort_alloc;

//CREATE SESSIONS
Ort::Session craft_session = Ort::Session(env,craft,Ort::SessionOptions{ nullptr });
Ort::Session refinet_session = Ort::Session(env,refinet,Ort::SessionOptions{ nullptr });

//DEFINE INPUT/OUTPUT SHAPES
std::vector<int64_t> inputShapeCraft {1, 3, 100, 100};
std::vector<int64_t> outputShape1Craft = { 1, 50, 50, 2}; //NOT USED
std::vector<int64_t> outputShape2Craft = { 1, 32, 50, 50}; //NOT USED

std::vector<int64_t> inputShapeRefinerY {1, 50, 50, 2};
std::vector<int64_t> inputShapeRefinerFeature {1, 32, 50, 50};


//DEFINE INPUT/OUTPUT NAMES
char* inputNameCraft = craft_session.GetInputName(0,ort_alloc);
char* outputNameCraft1 = craft_session.GetOutputName(0,ort_alloc);
char* outputNameCraft2 = craft_session.GetOutputName(1,ort_alloc);
const std::array<const char*, 1> inputNamesCraft = { inputNameCraft };
const std::array<const char*, 2> outputNamesCraft = {outputNameCraft1, outputNameCraft2};

char* inputNameRefiner1 = refinet_session.GetInputName(0,ort_alloc);
char* inputNameRefiner2 = refinet_session.GetInputName(1,ort_alloc);
char* outputNameRefiner = refinet_session.GetOutputName(0,ort_alloc);
const std::array<const char*, 2> inputNamesRefiner = { inputNameRefiner1, inputNameRefiner2 };
const std::array<const char*, 1> outputNamesRefiner = { outputNameRefiner };


