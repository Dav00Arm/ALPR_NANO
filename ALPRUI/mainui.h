#ifndef MAINUI_H
#define MAINUI_H

#include <QMainWindow>
#include <vector>
#include <QLabel>
#include "camerathread.h"

// Main UI class declaration.

namespace Ui {
class MainUI;
}

class MainUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = 0);
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> whiteListData;

    ~MainUI();

private slots:
    void on_SignUp_button_clicked();
    
    void on_cameraSettings_clicked();
    
    void on_AddLine_clicked();

    void on_HomeButton_clicked();

    void on_WhiteList1_clicked();
    void on_WhiteList2_clicked();
    void on_WhiteList3_clicked();
    void on_WhiteList4_clicked();
    
    void on_SaveButton_clicked();

private:
    Ui::MainUI *ui;


};

#endif // MAINUI_H
