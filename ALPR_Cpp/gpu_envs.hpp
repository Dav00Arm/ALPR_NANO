#include "onnxruntime_cxx_api.h"
#include "onnxruntime/include/onnxruntime/core/providers/cuda/cuda_provider_factory.h"

class CreateSession{
    public:
    const char* car_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/yolov5n_cpu.onnx";
    const char* plate_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/plate_detection_cpu.onnx";
    const char* craft_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/craft_cpu.onnx";
    const char* refiner_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/refiner_cpu.onnx";

    Ort::Env env;    
    Ort::SessionOptions session_options;    

    CreateSession(){
        env = Ort::Env(ORT_LOGGING_LEVEL_WARNING , "test");
        // session_options.SetIntraOpNumThreads(1);
        // session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
        session_options.SetIntraOpNumThreads(1);
        session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
        // options.cudnn_conv_algo_search = OrtCudnnConvAlgoSearch::EXHAUSTIVE;
        // options.gpu_mem_limit = 2048;
        // options.arena_extend_strategy = 0;
        session_options.AppendExecutionProvider_CUDA(0, ONNXRuntimeProviderOptions_CUDA{
                                                    2*1024 * 1024 * 1024, // 2 GB
                                                    kCudaPreferShared,
                                                    kCudaGrow,
                                                    1024
                                                    });

    
    }
    ~CreateSession(){}
    Ort::Session SessionCar(){
        return Ort::Session(env, car_path, session_options);
    }

    Ort::Session SessionPlate(){
        return Ort::Session(env, plate_path, session_options);
    }

    Ort::Session SessionCraft(){
        return Ort::Session(env, craft_path, session_options);
    }

    Ort::Session SessionRefiner(){
        return Ort::Session(env, refiner_path, session_options);
    }
};

CreateSession sessions;

// // Demo code for ONNX inference with GPUs.
// // Car Detection 
// std::vector<Ort::Session> CreateSession(){

//     const char* car_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/yolov5n_cpu.onnx";
//     const char* plate_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/plate_detection_cpu.onnx";
//     const char* craft_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/craft_cpu.onnx";
//     const char* refiner_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/refiner_cpu.onnx";

//     static Ort::Env env = Ort::Env(ORT_LOGGING_LEVEL_WARNING , "test");    

//     Ort::SessionOptions session_options;    
//     session_options.SetIntraOpNumThreads(1);
//     session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
//     session_options.AppendExecutionProvider_CUDA(OrtCUDAProviderOptions{});
//     std::vector<Ort::Session> outSessions = {Ort::Session(env, car_path, session_options),
//                                              Ort::Session(env, plate_path, session_options),
//                                              Ort::Session(env, craft_path, session_options),
//                                              Ort::Session(env, refiner_path, session_options)};
//     return outSessions;
// } 


// Ort::Session CreateSessionPlate(){

//     const char* model_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/plate_detection_cpu.onnx";
//     static Ort::Env env = Ort::Env(ORT_LOGGING_LEVEL_WARNING , "test");    

//     Ort::SessionOptions session_options;    
//     session_options.SetIntraOpNumThreads(1);
//     session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
//     session_options.AppendExecutionProvider_CUDA(OrtCUDAProviderOptions{});
//     return Ort::Session(env, model_path, session_options);
// } 

// Ort::Session CreateSessionCraft(){

//     const char* model_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/craft_cpu.onnx";
//     static Ort::Env env = Ort::Env(ORT_LOGGING_LEVEL_WARNING , "test");    

//     Ort::SessionOptions session_options;    
//     session_options.SetIntraOpNumThreads(1);
//     session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
//     session_options.AppendExecutionProvider_CUDA(OrtCUDAProviderOptions{});
//     return Ort::Session(env, model_path, session_options);
// } 

// Ort::Session CreateSessionRefiner(){

//     const char* model_path = "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/models/refiner_cpu.onnx";
//     static Ort::Env env = Ort::Env(ORT_LOGGING_LEVEL_WARNING , "test");    

//     Ort::SessionOptions session_options;    
//     session_options.SetIntraOpNumThreads(1);
//     session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
//     session_options.AppendExecutionProvider_CUDA(OrtCUDAProviderOptions{});
//     return Ort::Session(env, model_path, session_options);
// } 
