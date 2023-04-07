#-------------------------------------------------
#
# Project created by QtCreator 2023-02-15T16:37:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ALPR_UI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=  \
        main.cpp \
        mainui.cpp \

HEADERS += \
        mainui.h \

FORMS += \
        mainui.ui

INCLUDEPATH += /usr/include/opencv4 \
#               /home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/torch/include/torch/csrc/api/include \
#               /home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/torch/include/torch \
#               /home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/torch/include/torch/csrc/api/include/torch \


LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_video -lopencv_videoio

#LIBS += -L/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/torch/lib -ltorch -ltorch_cuda -ltorch_cpu -ltorch_python -lc10 -lc10_cuda
