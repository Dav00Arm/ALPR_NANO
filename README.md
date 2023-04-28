# ALPR with UI for Jetson Nano
main folders` ALPR_Cpp, ALPRUI.
main file ALPR_Cpp/main_test.cpp.
See package installation steps in file "installation". 
REMINDER: steps in "installation" file is for Jetson Nano. Installation steps on another board may differ.

## How to use?
Do the installation steps, then in FULL_ALPR_NANO/ run ./ALPR in terminal.
SignUp into your account. After SignUp button you'll see settings button in the top left corner. In the settings page you
can give URLs to your streams, and give names for your cameras. Click "manage white list" button and add plate numbers and their corresponding names, click "Add line" to add one more line for adding a new member. After configuration click "Home". Your cameras will be visible in small windows.

## How to make?
Change paths to your libs in CMakeLists.txt file then run "make".

## How It's made?
### Stages
Stream->car detection->plate detection->craft+refiner->ocr.

### Stream.
We get frames from stream with CameraStreamer class. It is creating independent threads for every camera stream to avoid delays and be synchronized with each other.

### Car detection.
Yolov5n onnx version is used for car detection.

### Plate detection.
Fine tuned Mobilenet_v2_ssd_lite is used for plate detection. https://github.com/KarAleks/ALPR/tree/main/training/pytorch-ssd

### Craft and refiner. Line splitting.
Fine tuned Craft is used for splitting lines of 2 line license plates. Refiner module is used to improve Craft's output.

### OCR. Optical character recognition.
Fine tuned easyocr model for recognizing letters and digits after craft.

### Performance
2 spots per camera
#### PC(CPU)
1 camera - 1.1 FPS \n
4 cameras - 0.28 FPS 
#### NANO(CPU)
1 camera - \n
4 cameras - Luchshe ne nada
#### NANO(GPU)
1 camera - 1 FPS \n
4 cameras - 0.4 FPS

## DO NOT DELETE "URLs.json" "userInfo.json" "whiteList.json" files
