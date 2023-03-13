#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <opencv2/opencv.hpp>


class CameraThread : public QThread {
  Q_OBJECT

public:
  CameraThread(int deviceNum, QObject *parent = nullptr);
  ~CameraThread();

signals:
  void imageReady(cv::Mat image);

protected:
  void run();

private:
  int m_deviceNum;
};

#endif // CAMERATHREAD_H
