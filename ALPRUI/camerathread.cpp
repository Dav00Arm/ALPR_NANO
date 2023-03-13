#include "camerathread.h"
#include <iostream>
CameraThread::CameraThread(int deviceNum, QObject *parent) : QThread(parent), m_deviceNum(deviceNum) {}

CameraThread::~CameraThread() {}

void CameraThread::run() {
  cv::VideoCapture cap(m_deviceNum);
  if (!cap.isOpened()) {
    qDebug() << "Failed to open camera " << m_deviceNum;
    return;
  }
  else{
      qDebug() << "Camera is successfully open " << m_deviceNum;
  }

  while (true) {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
      break;
    }
    emit imageReady(frame);
//    this->msleep(100);

  }
}
