cmake_minimum_required(VERSION 3.1)
project(ALPR)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_PREFIX_PATH "ALPR_Cpp/torch" "ALPR_Cpp/" "ALPR_Cpp/tbb/") 


find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Torch REQUIRED)
find_package(TBB REQUIRED)
# find_package(JetsonGPIO)


include_directories(${PROJECT_SOURCE_DIR})
include_directories( ${Torch} )
include_directories("/usr/include/jsoncpp/")
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories("ALPR_Cpp/curl/include/")
include_directories("ALPR_Cpp/json/jsoncpp-build/CMakeFiles/Export/include/")
include_directories("ALPR_Cpp/onnxruntime/include")
link_directories("ALPR_Cpp/onnxruntime/build/Release")

link_directories("usr/local/lib/")
link_directories("/usr/bin/curl")

set(UI_FILES "ALPR_UI/mainui.ui")
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable(ALPR ALPR_Cpp/main_test.cpp ALPRUI/mainui.cpp ALPRUI/mainui.h ${UI_HEADERS})

target_link_libraries(ALPR PRIVATE Qt5::Widgets tbb ${OpenCV_LIBS} ${TORCH_LIBRARIES} jsoncpp curl onnxruntime pthread) #JetsonGPIO::JetsonGPIO
set_property(TARGET ALPR PROPERTY CXX_STANDARD 14)
