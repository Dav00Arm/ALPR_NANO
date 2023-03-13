#include "mainui.h"
#include <QApplication>
#include <QtWidgets>
#include <opencv2/opencv.hpp>


int run_ui(int argc, char *argv[]) //
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

    QGridLayout *CameraLayout = w.findChild<QGridLayout*>("CameraGrid"); //getting the widget by its name
    CameraLayout->setContentsMargins(0, 0, 0, 0); 
    CameraLayout.resize(width-20, height-20);
    CameraLayout->move(x, y);


    
    w.show();
    return a.exec();
}
