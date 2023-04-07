#ifndef MAINUI_H
#define MAINUI_H
 
#include <QMainWindow>
#include <vector>
#include <QLabel>
#include <unordered_map>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>

// Main UI class declaration.

namespace Ui {
class MainUI;
}

class MainUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = 0);
    int currentList = -1;
    std::vector<std::string> names;
    std::vector<std::string> urls;

    ~MainUI();

private slots:
    void on_SignUp_button_clicked();
    
    void on_cameraSettings_clicked();
    
    void loadWhiteList(std::string index);
    void clearWhiteList();
    void SaveWhiteList();
    void on_AddLine_clicked();

    void on_HomeButton_clicked();

    void on_WhiteList1_clicked();
    void on_WhiteList2_clicked();
    void on_WhiteList3_clicked();
    void on_WhiteList4_clicked();
    
    void on_SaveButton_clicked();

    void on_SignIn_button_clicked();

    void on_resetLogin_clicked();

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainUI *ui;

};

#endif // MAINUI_H
