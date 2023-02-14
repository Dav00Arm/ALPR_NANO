#include "mainui.h"
#include "ui_mainui.h"
#include "opencv2/opencv.hpp"
#include "/home/jets/Desktop/ALPR/ALPR_Cpp/main.hpp"

MainUI::MainUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUI)
{

    ui->setupUi(this);

}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::on_SignUp_button_clicked()
{
    std::cout<<"run"<<std::endl;
    run();
    std::cout<<"passed run"<<std::endl;
//    ui->stackedWidget->setCurrentIndex(1);


    QString username = ui->Username_input->text();
    QString password = ui->Password_input->text();
    QString password2 = ui->Password2_input->text();

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
    }


}
