cmake_minimum_required (VERSION 3.2)
project(main)

set(CMAKE_PREFIX_PATH "torch" "tbb/") 

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)
find_package(TBB REQUIRED)
#find_package(JetsonGPIO)

include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${Torch} )
include_directories("/usr/include/jsoncpp/")

include_directories("curl/include/")
# include_directories("json/jsoncpp-build/CMakeFiles/Export/include/")
# include_directories("onnxruntime/include")
# link_directories("onnxruntime/build/Linux/Release")

# link_directories("usr/local/lib/")
# link_directories("/usr/bin/curl")

# add_executable( main tst.cpp )
# target_link_libraries (main tbb ${OpenCV_LIBS} ${TORCH_LIBRARIES} jsoncpp curl onnxruntime )#JetsonGPIO::JetsonGPIO) 


set_property(TARGET main PROPERTY CXX_STANDARD 14)

