#include "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/run_alpr.hpp"
#include "mainui.h"
#include "ui_mainui.h"
#include <QScreen>

MainUI::MainUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUI)
{

    ui->setupUi(this);
    QPixmap bckg_img("/home/jets/Desktop/FULL_ALPR_NANO/ALPRUI/images/background.jpg");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(bckg_img));
    this->setPalette(palette);

    
}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::on_SignUp_button_clicked()
{
    QString username = ui->Username_input->text();
    QString password = ui->Password_input->text();
    QString password2 = ui->Password2_input->text();
    ui->stackedWidget->setCurrentIndex(1);

    this->setPalette(QPalette());
    // Check Sign up requirements.
    if(username.length() == 0){
       ui->Password_mismatch->setText("Username can not be empty!");
    }
    else if(password.length() < 6){
        ui->Password_mismatch->setText("Password must contain at least 6 characters!");
    }
    else if(password != password2){
        ui->Password_mismatch->setText("Passwords don't match!");
    }
    else if(username.length() > 0 && password == password2){

        ui->stackedWidget->setCurrentIndex(1);
        this->setPalette(QPalette());
    }
}

void MainUI::on_cameraSettings_clicked()
{

    ui->stackedWidget->setCurrentIndex(2);
    // QLabel *label1 = ui->cam1;
    // QLabel *label2 = ui->cam2;
    // QLabel *label3 = ui->cam3;
    // QLabel *label4 = ui->cam4;

    // std::vector<QLabel*> labels = {label1, label2, label3, label4};
    // alpr(labels);

}

// void MainUI::onImageReady1(cv::Mat image) {
//   QSize newSize(400,300); // New width and height
//   QImage qImage(image.data, image.cols, image.rows, QImage::Format_RGB888);
//   QImage image_res = qImage.scaled(newSize, Qt::KeepAspectRatio);
//   QPixmap pixmap = QPixmap::fromImage(image_res.rgbSwapped());
//   m_label1->setPixmap(pixmap);
// }

// void MainUI::onImageReady2(cv::Mat image) {
//   QSize newSize(400,300); // New width and height
//   QImage qImage(image.data, image.cols, image.rows, QImage::Format_RGB888);
//   QImage image_res = qImage.scaled(newSize, Qt::KeepAspectRatio);
//   QPixmap pixmap = QPixmap::fromImage(image_res.rgbSwapped());
//   m_label2->setPixmap(pixmap);
// }

// void MainUI::on_cameraSettings2_clicked()
// {
//     QLabel *label1 = ui->cam1;
//     QLabel *label2 = ui->cam2;
//     QLabel *label3 = ui->cam3;
//     QLabel *label4 = ui->cam4;
//     std::vector<QLabel*> labels = {label1, label2, label3, label4};
//     alpr(labels);


// }
