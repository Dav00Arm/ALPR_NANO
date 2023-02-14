#include "onnxruntime_cxx_api.h"

Ort::Session CreateSessionCar(){

    static Ort::Env env = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "test");
    const char* model_path = "./models/yolov5n_cpu.onnx";

    Ort::SessionOptions session_options;    
    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    OrtCUDAProviderOptions cuda_options;
    cuda_options.device_id = 0;
    // But is zero a valid value for every variable? Not quite. It is not guaranteed. In the other words: does every enum
    // type contain zero? The following line can be omitted because EXHAUSTIVE is mapped to zero in onnxruntime_c_api.h.
    cuda_options.gpu_mem_limit = static_cast<int>(SIZE_MAX * 1024 * 1024);
    cuda_options.arena_extend_strategy = 1;
    cuda_options.cudnn_conv_algo_search = OrtCudnnConvAlgoSearch::EXHAUSTIVE;
    cuda_options.do_copy_in_default_stream = 1;
    cuda_options.default_memory_arena_cfg = nullptr;
    session_options.AppendExecutionProvider_CUDA(cuda_options);
    return Ort::Session(env, model_path, session_options);
}