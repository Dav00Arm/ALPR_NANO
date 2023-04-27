#include "ALPR_Cpp/run_alpr.hpp"
#include "mainui.h"
#include "ui_mainui.h"
#include <QScreen>
#include <QDebug>
// All callback functions of UI.

Json::Value whiteList(Json::objectValue);
Json::Value user(Json::objectValue);
Json::Value userSignIn(Json::objectValue);


// Main class of UI.
MainUI::MainUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);
    QPixmap bckg_img("ALPRUI/images/background.jpg");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(bckg_img));
    this->setPalette(palette);
    std::ifstream jsonUser("userInfo.json");
    jsonUser >> userSignIn;
    Json::Value::Members user_ = userSignIn.getMemberNames();
    if(user_.size() != 0){
        ui->stackedWidget->setCurrentIndex(3);
    }
    jsonUser.close();

}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::loadWhiteList(std::string index){
    std::ifstream jsonFileIn("whiteList.json");
    // Json::FastWriter writer;
    jsonFileIn >> whiteList;
    Json::Value::Members members = whiteList[index].getMemberNames();
    std::vector<std::string> keys;
    for (const auto& member : members) {
        keys.push_back(member);
    }

    for(int i=0; i<keys.size(); i++){
        int numRows = ui->WhiteListTable->rowCount();
        ui->WhiteListTable->insertRow(i);
        
        QTableWidgetItem *plate = new QTableWidgetItem(QString::fromStdString(whiteList[index][keys[i]].asString()));
        QTableWidgetItem *name = new QTableWidgetItem(QString::fromStdString(keys[i]));
        
        ui->WhiteListTable->setItem(i, 0, name);
        ui->WhiteListTable->setItem(i, 1, plate);
        ui->WhiteListTable->setRowHeight(i, 30); 
    }
}

void MainUI::clearWhiteList(){
    ui->WhiteListTable->clearContents();
    ui->WhiteListTable->setRowCount(0);
}

void MainUI::SaveWhiteList(){
    std::ofstream jsonWhiteList("whiteList.json");
    whiteList.removeMember(std::to_string(currentList));

    int rowCount = ui->WhiteListTable->rowCount();
    int columnCount = ui->WhiteListTable->columnCount();
    for (int row = 0; row < rowCount; row++) {
        QTableWidgetItem* itemVal = ui->WhiteListTable->item(row, 1);
        QTableWidgetItem* itemName = ui->WhiteListTable->item(row, 0);

        if (itemVal != nullptr || itemName != nullptr) {
            std::string plate = itemVal->text().toStdString();
            std::string name = itemName->text().toStdString();
            if(plate != "" && name != ""){
                whiteList[std::to_string(currentList)][name] = plate;
            }
        }
    }

    // write the JSON string to a file
    if (jsonWhiteList.is_open()) {
        jsonWhiteList << whiteList;
        jsonWhiteList.close();
    } else {
        std::cerr << "Failed to open JSON file" << std::endl;
    }

}

// Sign up Button clicked logic.
void MainUI::on_SignUp_button_clicked()
{
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
        std::ofstream jsonUser("userInfo.json");
        user[username.toStdString()] = password.toStdString();
        if (jsonUser.is_open()) {
            jsonUser << user;
            jsonUser.close();
        } 
        else {
            std::cerr << "Failed to open JSON file" << std::endl;
        }
        ui->stackedWidget->setCurrentIndex(1);
        this->setPalette(QPalette());
    }
}

void MainUI::on_SignIn_button_clicked()
{
    std::ifstream jsonUser("userInfo.json");
    jsonUser >> userSignIn;
    Json::Value::Members user_ = userSignIn.getMemberNames();
    QString usernameSignIn = ui->Username_input_SignIn->text();
    QString passwordSignIn = ui->Password_input_SignIn->text();

    if(user_[0] != usernameSignIn.toStdString() || 
        userSignIn[user_[0]] != passwordSignIn.toStdString()) {
    
        ui->Password_mismatch_2->setText("Wrong username or password!");
    }
    else{
        ui->stackedWidget->setCurrentIndex(1);
        this->setPalette(QPalette());
    }
}


// Settings button clicked logic. (not finished)
void MainUI::on_cameraSettings_clicked()
{   
    run_status = false;
    ui->AddLineWarning->hide();
    ui->CheckBoxWarning->hide();
    ui->stackedWidget->setCurrentIndex(2);
    std::ifstream jsonURLs("URLs.json");
    Json::Value CamUrlsTmp(Json::objectValue);
    jsonURLs >> CamUrlsTmp;

    if(CamUrlsTmp.isMember("0")){
        ui->camera1LineEdit->setText(QString::fromStdString(CamUrlsTmp["0"].asString()));
    }
    if(CamUrlsTmp.isMember("1")){
        ui->camera2LineEdit->setText(QString::fromStdString(CamUrlsTmp["1"].asString()));
    }
    if(CamUrlsTmp.isMember("2")){
        ui->camera3LineEdit->setText(QString::fromStdString(CamUrlsTmp["2"].asString()));
    }
    if(CamUrlsTmp.isMember("3")){
        ui->camera4LineEdit->setText(QString::fromStdString(CamUrlsTmp["3"].asString()));
    }

    jsonURLs.close();
}

// Add line in WhiteList table. Add new "name": "License plate" pair.
void MainUI::on_AddLine_clicked()
{   
    if(currentList == -1){
        // std::cout<<"Choose a camera"<<std::endl;
        ui->AddLineWarning->show();
    }
    else{
        ui->AddLineWarning->hide();

        int numRows = ui->WhiteListTable->rowCount();

        ui->WhiteListTable->insertRow(numRows);

        ui->WhiteListTable->setItem(numRows, 0, new QTableWidgetItem(""));
        ui->WhiteListTable->setItem(numRows, 1, new QTableWidgetItem(""));
        ui->WhiteListTable->setRowHeight(numRows, 30);
        // if(numRows<10)
        // {
        //     ui->WhiteListTable->resize(270,400);
        // }
        // else if(10<=numRows<=100)
        // {
        //     ui->WhiteListTable->resize(300,400);
        // }
        // else{
        //     ui->WhiteListTable->resize(321,400);
        // }
    }
}

// Opens WhiteList table for the 1st camera. (not finished)
void MainUI::on_WhiteList1_clicked()
{   
    if(currentList != -1){
        SaveWhiteList();
    }
    ui->WhiteListLabel->show();
    ui->WhiteListLabel->setText("Camera 1 White List");
    clearWhiteList();
    currentList = 0;
    loadWhiteList("0");

}

// Opens WhiteList table for the 2nd camera. (not finished)
void MainUI::on_WhiteList2_clicked()
{
    if(currentList != -1){
        SaveWhiteList();
    }
    ui->WhiteListLabel->show();
    ui->WhiteListLabel->setText("Camera 2 White List");
    clearWhiteList();
    currentList = 1;
    loadWhiteList("1");
}

// Opens WhiteList table for the 3rd camera. (not finished)
void MainUI::on_WhiteList3_clicked()
{    
    if(currentList != -1){
        SaveWhiteList();
    }
    ui->WhiteListLabel->show();
    ui->WhiteListLabel->setText("Camera 3 White List");
    clearWhiteList();
    currentList = 2;
    loadWhiteList("2");
}

// Opens WhiteList table for the 4th camera. (not finished)
void MainUI::on_WhiteList4_clicked()
{
    if(currentList != -1){
        SaveWhiteList();
    }
    ui->WhiteListLabel->show();
    ui->WhiteListLabel->setText("Camera 4 White List");
    clearWhiteList();
    currentList = 3;
    loadWhiteList("3");

}

// Camera Settings page SAVE button clicked logic. (not finished)
void MainUI::on_SaveButton_clicked()
{
    if(currentList != -1){
        SaveWhiteList();
    }
}

// Home button clicked from camera settings page logic.
void MainUI::on_HomeButton_clicked()
{
    int delUrl = std::remove("URLs.json");
    Json::Value CamUrls(Json::objectValue);

    std::ofstream jsonURLs("URLs.json");
    if(!ui->camera1LineEdit->text().isEmpty()){
        CamUrls["0"] = ui->camera1LineEdit->text().toStdString();
    }
    if(!ui->camera2LineEdit->text().isEmpty()){
        CamUrls["1"] = ui->camera2LineEdit->text().toStdString();
    }
    if(!ui->camera3LineEdit->text().isEmpty()){
        CamUrls["2"] = ui->camera3LineEdit->text().toStdString();
    }
    if(!ui->camera4LineEdit->text().isEmpty()){
        CamUrls["3"] = ui->camera4LineEdit->text().toStdString();
    }
    if (jsonURLs.is_open()) {
        jsonURLs << CamUrls;
        jsonURLs.close();
    } 

    bool isChecked1 = ui->camera1CheckBox->isChecked();
    bool isChecked2 = ui->camera2CheckBox->isChecked();
    bool isChecked3 = ui->camera3CheckBox->isChecked();
    bool isChecked4 = ui->camera4CheckBox->isChecked();

    QString name1 = ui->cameraName1LineEdit->text();
    QString url1 = ui->camera1LineEdit->text();
    QString name2 = ui->cameraName2LineEdit->text();
    QString url2 = ui->camera2LineEdit->text();
    QString name3 = ui->cameraName3LineEdit->text();
    QString url3 = ui->camera3LineEdit->text();
    QString name4 = ui->cameraName4LineEdit->text();
    QString url4 = ui->camera4LineEdit->text();
    ui->CheckBoxWarning->hide();
    names = {};
    urls = {};
    ui->cam1->setText("No connection");
    ui->cam2->setText("No connection");
    ui->cam3->setText("No connection");
    ui->cam4->setText("No connection");
    if(isChecked1){
        if(name1 == "" || url1 == ""){
            ui->CheckBoxWarning->show();
            ui->CheckBoxWarning->setText("Camera 1: NAME or URL cannot be empty");
            return;
        }
        else{
            ui->cam1->setText("Loading...");
            names.push_back(name1.toStdString());
            urls.push_back(url1.toStdString());
        }
    }
    if(isChecked2){
        if(name2 == "" || url2 == ""){
            ui->CheckBoxWarning->show();
            ui->CheckBoxWarning->setText("Camera 2: NAME or URL cannot be empty");
            return;
        }
        else{
            ui->cam2->setText("Loading...");
            names.push_back(name2.toStdString());
            urls.push_back(url2.toStdString());
        }
    }
    if(isChecked3){
        if(name3 == "" || url3 == ""){
            ui->CheckBoxWarning->show();
            ui->CheckBoxWarning->setText("Camera 3: NAME or URL cannot be empty");
            return;
        }
        else{
            ui->cam3->setText("Loading...");
            names.push_back(name3.toStdString());
            urls.push_back(url3.toStdString());
        }
    }
    if(isChecked4){
        if(name4 == "" || url4 == ""){
            ui->CheckBoxWarning->show();
            ui->CheckBoxWarning->setText("Camera 4: NAME or URL cannot be empty");
            return;
        }
        else{
            ui->cam4->setText("Loading...");
            names.push_back(name4.toStdString());
            urls.push_back(url4.toStdString());
        }
    }
    if(currentList != -1){
        SaveWhiteList();
    }
    // std::cout<< "name_size: " << names.size()<<std::endl;
    if(names.size()>0){
        std::string cameraState = cameraChecker(names, urls);
        // std::cout<< "CameraChecker: " << (cameraState == "True")<<std::endl;
        ui->stackedWidget->setCurrentIndex(1);
        if(cameraState == "True"){
            ui->stackedWidget->setCurrentIndex(1);
            // QMetaObject::invokeMethod(ui->stackedWidget, "setCurrentIndex", Q_ARG(int, 1));
            // std::cout<< "CameraChecker: " << (cameraState == "True")<<std::endl;

            QLabel *label1 = ui->cam1;
            QLabel *label2 = ui->cam2;
            QLabel *label3 = ui->cam3;
            QLabel *label4 = ui->cam4;
            std::vector<QLabel*> labels = {label1, label2, label3, label4};
            run_status = true;  
            alpr(labels, names, urls);
        }
        else{
            ui->CheckBoxWarning->show();
            ui->CheckBoxWarning->setText(QString::fromStdString("Camera " + cameraState + " is unavailable"));
        }


    }
    else if(names.size()==0){
        ui->stackedWidget->setCurrentIndex(1);
    }

    
}

// Opens sign up page to reset username and password
void MainUI::on_resetLogin_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    
    QPixmap bckg_img("ALPRUI/images/background.jpg");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(bckg_img));
    this->setPalette(palette);
    int result = std::remove("userInfo.json");

    ui->Welcome->setText("     Reset");
    ui->Username_label->setText("New Username");
    ui->Password_label->setText("New Password");
    ui->Username_input->setText("");
    ui->Password_input->setText("");
    ui->Password2_input->setText("");
}

// Close window after "x" button 
void MainUI::closeEvent(QCloseEvent *event)  {
        // Ask the user if they really want to quit
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Quit", "Are you sure you want to quit?",
                                        QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // If the user clicked "Yes", close the window and exit the application
            event->accept();
            run_status = false;
            QApplication::exit();
        } else {
            // If the user clicked "No", ignore the close event and keep the window open
            event->ignore();
        }
    }