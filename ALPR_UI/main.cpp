#include "mainui.h"
#include <QApplication>
#include <QScreen>
#include <QtWidgets>
#include "opencv2/opencv.hpp"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainUI w;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    w.setWindowTitle("Locator ALPR");

    w.setFixedSize(width, height);


    //Center the Sign up box
    QWidget *CentralWidget = w.findChild<QWidget*>("CenterWidget"); //getting the widget by its name
    int x = (width - CentralWidget->width()) / 2;
    int y = (height - CentralWidget->height()) / 2;
    CentralWidget->move(x, y);

    //Camera view page
    QLabel *label = new QLabel;
//    cv::VideoCapture cap(0);

    /*while (true)
       {
           cv::Mat frame = cv::imread("images/signup.jpg");
//           cap >> frame;

           if (frame.empty()) break;

           QImage img = QImage((const uchar*)frame.data, frame.cols, frame.rows, QImage::Format_RGB888).rgbSwapped();

           label->setPixmap(QPixmap::fromImage(img));
           label->show();

           a.processEvents();
       }*/
    w.show();
    return a.exec();
}
