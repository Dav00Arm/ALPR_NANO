#ifndef MAINUI_H
#define MAINUI_H

#include <QMainWindow>
#include <vector>
#include <QLabel>
#include "camerathread.h"

namespace Ui {
class MainUI;
}

class MainUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = 0);

    ~MainUI();

private slots:
    void on_SignUp_button_clicked();
    void on_cameraSettings_clicked();

    // void on_cameraSettings2_clicked();
    // void onImageReady1(cv::Mat image);
    // void onImageReady2(cv::Mat image);

private:
    Ui::MainUI *ui;
};

#endif // MAINUI_H
