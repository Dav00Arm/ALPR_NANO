#include "/home/jets/Desktop/FULL_ALPR_NANO/ALPR_Cpp/run_alpr.hpp"
#include "mainui.h"
#include "ui_mainui.h"
#include <QScreen>

// All callback functions of UI.

Json::Value whiteList(Json::objectValue);

// Creates WhiteList table instance. (not finished)
class WhiteList{
    public:
    QTableWidget *table;

    WhiteList(std::string tableName){
        table = ui->findChild<QTableWidget*>(tableName);
    }

    QPushButton *addLine = ui->findChild<QPushButton*>("AddLine");
    addLine->move(width/2-130,420);
    table->move(width/2-130,10);
    table->setRowCount(0);
    table->setColumnCount(2);
    table->setColumnWidth(0, 140);
    table->setColumnWidth(1, 120);

    table->resize(300, 400);
    table->setHorizontalHeaderLabels(QStringList() << "Name" << "License Number");
};

// Main class of UI.
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

// Sign up Button clicked logic.
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

// Settings button clicked logic. (not finished)
void MainUI::on_cameraSettings_clicked()
{

    ui->stackedWidget->setCurrentIndex(2);
    std::ofstream jsonFile("whiteList.json");

    // QLabel *label1 = ui->cam1;
    // QLabel *label2 = ui->cam2;
    // QLabel *label3 = ui->cam3;
    // QLabel *label4 = ui->cam4;

    // std::vector<QLabel*> labels = {label1, label2, label3, label4};
    // alpr(labels);

}

// Add line in WhiteList table. Add new "name": "License plate" pair.
void MainUI::on_AddLine_clicked()
{   
    int numRows = ui->WhiteListTable->rowCount();

    ui->WhiteListTable->insertRow(numRows);

    ui->WhiteListTable->setItem(numRows, 0, new QTableWidgetItem(""));
    ui->WhiteListTable->setItem(numRows, 1, new QTableWidgetItem(""));
    ui->WhiteListTable->setRowHeight(numRows, 30);
    if(numRows<10)
    {
        ui->WhiteListTable->resize(270,400);
    }
    else if(10<=numRows<=100)
    {
        ui->WhiteListTable->resize(300,400);
    }
    // else{
    //     ui->WhiteListTable->resize(321,400);
    // }

}

// Camera Settings page SAVE button clicked logic. (not finished)
void MainUI::on_SaveButton_clicked()
{
    std::ofstream jsonFile("whiteList.json");
    int rowCount = ui->WhiteListTable->rowCount();
    int columnCount = ui->WhiteListTable->columnCount();
    for (int row = 0; row < rowCount; row++) {
        QTableWidgetItem* itemVal = ui->WhiteListTable->item(row, 1);
        QTableWidgetItem* itemName = ui->WhiteListTable->item(row, 0);

        if (itemVal != nullptr || itemName != nullptr) {
            std::string plate = itemVal->text().toStdString();
            std::string name = itemName->text().toStdString();
            whiteList["0"][name] = plate;
        }
    }

    std::string jsonString = whiteList.toStyledString();
    std::cout << jsonString << std::endl;

    // write the JSON string to a file
    if (jsonFile.is_open()) {
        jsonFile << jsonString;
        jsonFile.close();
    } else {
        std::cerr << "Failed to open JSON file" << std::endl;
    }

}

// Opens WhiteList table for the 1st camera. (not finished)
void MainUI::on_WhiteList1_clicked()
{
    std::ofstream jsonFile("whiteList.json");
    
}

// Opens WhiteList table for the 2nd camera. (not finished)
void MainUI::on_WhiteList2_clicked()
{
    std::ofstream jsonFile("whiteList.json");

}

// Opens WhiteList table for the 3rd camera. (not finished)
void MainUI::on_WhiteList3_clicked()
{
    std::ofstream jsonFile("whiteList.json");

}

// Opens WhiteList table for the 4th camera. (not finished)
void MainUI::on_WhiteList4_clicked()
{
    std::ofstream jsonFile("whiteList.json");

}

// Home button clicked from camera settings page logic.
void MainUI::on_HomeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}
