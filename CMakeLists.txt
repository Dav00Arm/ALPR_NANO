cmake_minimum_required(VERSION 3.1)
project(ALPR)
set(CMAKE_PREFIX_PATH "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/torch" "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/" "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/tbb/") 

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Torch REQUIRED)
find_package(TBB REQUIRED)

include_directories( ${Torch} )
include_directories("/usr/include/jsoncpp/")
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories("/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/curl/include/")
include_directories("/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/json/jsoncpp-build/CMakeFiles/Export/include/")
include_directories("/home/jets/Desktop/onnx_Kareni_mot/onnxruntime/include")
link_directories("/home/jets/Desktop/onnx_Kareni_mot/onnxruntime/build/Linux/Release")

link_directories("usr/local/lib/")
link_directories("/usr/bin/curl")


add_executable(ALPR ALPR_UI/main.cpp ALPR_UI/mainui.cpp ALPR_UI/mainui.h /home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/main.hpp)

target_link_libraries(ALPR PRIVATE Qt5::Widgets tbb ${OpenCV_LIBS} ${TORCH_LIBRARIES} jsoncpp curl onnxruntime)
set_property(TARGET ALPR PROPERTY CXX_STANDARD 14)
