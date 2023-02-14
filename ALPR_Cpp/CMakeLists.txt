cmake_minimum_required (VERSION 3.2)
project (main)
set(CMAKE_PREFIX_PATH "/home/jets/Desktop/ALPR_Cpp/torch" "/home/jets/Desktop/ALPR_Cpp/" "/home/jets/Desktop/ALPR_Cpp/tbb/") 

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)
find_package(TBB REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${Torch} )
include_directories("/usr/include/jsoncpp/")

include_directories("/home/jets/Desktop/ALPR_Cpp/curl/include/")
include_directories("/home/jets/Desktop/ALPR_Cpp/json/jsoncpp-build/CMakeFiles/Export/include/")
include_directories("/home/jets/Desktop/onnx_Kareni_mot/onnxruntime/include")
link_directories("/home/jets/Desktop/onnx_Kareni_mot/onnxruntime/build/Linux/Release")

link_directories("usr/local/lib/")
link_directories("/usr/bin/curl")

add_executable( main main.cpp )
target_link_libraries (main tbb ${OpenCV_LIBS} ${TORCH_LIBRARIES} jsoncpp curl onnxruntime ) 
set_property(TARGET main PROPERTY CXX_STANDARD 14)

