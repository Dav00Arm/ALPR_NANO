#include "onnxruntime_cxx_api.h"

class CreateSession{
    public:
    const char* car_path = "ALPR_Cpp/models/yolov5n_cpu.onnx";
    const char* plate_path = "ALPR_Cpp/models/plate_detection_cpu.onnx";
    const char* craft_path = "ALPR_Cpp/models/craft_cpu.onnx";
    const char* refiner_path = "ALPR_Cpp/models/refiner_cpu.onnx";

    Ort::Env env;    
    Ort::SessionOptions session_options;    

    CreateSession(){
        env = Ort::Env(ORT_LOGGING_LEVEL_WARNING , "test");
        // session_options.AppendExecutionProvider_CUDA(OrtCUDAProviderOptions{});
    
    }
    ~CreateSession(){}
    Ort::Session SessionCar(){
        std::cout<<"Creating Car session\n";        
        Ort::Session session = Ort::Session(env, car_path, session_options);
        std::cout<<"Created Car session\n";        

        return session;
    }

    Ort::Session SessionPlate(){
        std::cout<<"Created Plate session\n";        

        return Ort::Session(env, plate_path, session_options);
    }

    Ort::Session SessionCraft(){
        std::cout<<"Created Craft session\n";        

        return Ort::Session(env, craft_path, session_options);
    }

    Ort::Session SessionRefiner(){
        std::cout<<"Created Refiner session\n";        

        return Ort::Session(env, refiner_path, session_options);
    }
};

CreateSession sessions;
