#include "/home/jets/Desktop/FULL_ALPR_NANO/ALPRUI/mainui.h"
#include <QApplication>
#include <QtWidgets>

// MAIN function
int main(int argc, char *argv[])
{   
    // UI
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
    QWidget *CentralWidget2 = w.findChild<QWidget*>("CenterWidget_2"); //getting the widget by its name
    int x = (width - CentralWidget->width()) / 2;
    int y = (height - CentralWidget->height()) / 2;
    CentralWidget->move(x, y);
    CentralWidget2->move(x, y);
    QLabel *label_1 = w.findChild<QLabel*>("cam1"); 
    QLabel *label_2 = w.findChild<QLabel*>("cam2"); 
    QLabel *label_3 = w.findChild<QLabel*>("cam3"); 
    QLabel *label_4 = w.findChild<QLabel*>("cam4"); 

    label_1->move(0, 0); 
    label_1->resize(width/2-35, height/2-35);
    
    label_2->move(width/2-35, 0); 
    label_2->resize(label_1->width(), label_1->height());
    
    label_3->move(0, height/2-35); 
    label_3->resize(label_1->width(), label_1->height());
    
    label_4->move(width/2-35, height/2-35); 
    label_4->resize(label_1->width(), label_1->height());

    QPushButton *settings = w.findChild<QPushButton*>("cameraSettings");
    settings->setIcon(QIcon("/home/jets/Desktop/FULL_ALPR_NANO/ALPRUI/images/settings.png")); // set the icon
    settings->setIconSize(QSize(30, 30)); // set the icon size      
    settings->setFixedSize(30, 30); // set the same width and height
    settings->move(width-60, 0);

    QLabel *camera1Label = w.findChild<QLabel*>("camera1Label");
    QLabel *camera2Label = w.findChild<QLabel*>("camera2Label");
    QLabel *camera3Label = w.findChild<QLabel*>("camera3Label");
    QLabel *camera4Label = w.findChild<QLabel*>("camera4Label"); 

    QLineEdit *cameraName1LineEdit = w.findChild<QLineEdit*>("cameraName1LineEdit");
    cameraName1LineEdit->setPlaceholderText("Enter camera name");
    QLineEdit *cameraName2LineEdit = w.findChild<QLineEdit*>("cameraName2LineEdit");
    cameraName2LineEdit->setPlaceholderText("Enter camera name");
    QLineEdit *cameraName3LineEdit = w.findChild<QLineEdit*>("cameraName3LineEdit");
    cameraName3LineEdit->setPlaceholderText("Enter camera name");
    QLineEdit *cameraName4LineEdit = w.findChild<QLineEdit*>("cameraName4LineEdit"); 
    cameraName4LineEdit->setPlaceholderText("Enter camera name");

    QPushButton *whiteList1 = w.findChild<QPushButton*>("WhiteList1"); 
    QPushButton *whiteList2 = w.findChild<QPushButton*>("WhiteList2"); 
    QPushButton *whiteList3 = w.findChild<QPushButton*>("WhiteList3"); 
    QPushButton *whiteList4 = w.findChild<QPushButton*>("WhiteList4"); 

    QLineEdit *camera1LineEdit = w.findChild<QLineEdit*>("camera1LineEdit");
    camera1LineEdit->setPlaceholderText("Enter camera URL");
    QLineEdit *camera2LineEdit = w.findChild<QLineEdit*>("camera2LineEdit");
    camera2LineEdit->setPlaceholderText("Enter camera URL");
    QLineEdit *camera3LineEdit = w.findChild<QLineEdit*>("camera3LineEdit");
    camera3LineEdit->setPlaceholderText("Enter camera URL");
    QLineEdit *camera4LineEdit = w.findChild<QLineEdit*>("camera4LineEdit"); 
    camera4LineEdit->setPlaceholderText("Enter camera URL");

    QCheckBox *camera1CheckBox = w.findChild<QCheckBox*>("camera1CheckBox");
    QCheckBox *camera2CheckBox = w.findChild<QCheckBox*>("camera2CheckBox");
    QCheckBox *camera3CheckBox = w.findChild<QCheckBox*>("camera3CheckBox");
    QCheckBox *camera4CheckBox = w.findChild<QCheckBox*>("camera4CheckBox");

    camera1Label->setFixedSize(50, 25);
    camera1Label->move(5, 0);
    cameraName1LineEdit->setFixedSize(200, 20);
    cameraName1LineEdit->move(5, 27);
    whiteList1->setFixedSize(80, 20);
    whiteList1->move(210, 27);
    camera1LineEdit->setFixedSize(200, 20);
    camera1LineEdit->move(5, 54);
    camera1CheckBox->setFixedSize(60, 20);
    camera1CheckBox->move(210, 54);

    camera2Label->setFixedSize(50, 25);
    camera2Label->move(5, 72);
    cameraName2LineEdit->setFixedSize(200, 20);
    cameraName2LineEdit->move(5, 99);
    whiteList2->setFixedSize(80, 20);
    whiteList2->move(210, 99);
    camera2LineEdit->setFixedSize(200, 20);
    camera2LineEdit->move(5, 126);
    camera2CheckBox->setFixedSize(60, 20);
    camera2CheckBox->move(210, 126);

    camera3Label->setFixedSize(50, 25);
    camera3Label->move(5, 149);
    cameraName3LineEdit->setFixedSize(200, 20);
    cameraName3LineEdit->move(5, 176);
    whiteList3->setFixedSize(80, 20);
    whiteList3->move(210, 176);
    camera3LineEdit->setFixedSize(200, 20);
    camera3LineEdit->move(5, 203);
    camera3CheckBox->setFixedSize(60, 20);
    camera3CheckBox->move(210, 203);

    camera4Label->setFixedSize(50, 25);
    camera4Label->move(5, 220);
    cameraName4LineEdit->setFixedSize(200, 20);
    cameraName4LineEdit->move(5, 247);
    whiteList4->setFixedSize(80, 20);
    whiteList4->move(210, 247);
    camera4LineEdit->setFixedSize(200, 20);
    camera4LineEdit->move(5, 274);
    camera4CheckBox->setFixedSize(60, 20);
    camera4CheckBox->move(210, 274);


    QTableWidget *table = w.findChild<QTableWidget*>("WhiteListTable");
    QPushButton *addLine = w.findChild<QPushButton*>("AddLine");
    QLabel *AddLineWarning = w.findChild<QLabel*>("AddLineWarning"); 
    QLabel *CheckBoxWarning = w.findChild<QLabel*>("CheckBoxWarning"); 
    QLabel *WhiteListLabel = w.findChild<QLabel*>("WhiteListLabel"); 

    addLine->move(width/2-130,450);
    
    AddLineWarning->move(width/2-30, 450);
    CheckBoxWarning->move(5, 360);
    WhiteListLabel->move(width/2-130,10);

    table->move(width/2-130,40);
    table->setRowCount(0);
    table->setColumnCount(2);
    table->setColumnWidth(0, 140);
    table->setColumnWidth(1, 120);

    table->resize(300, 400);
    table->setHorizontalHeaderLabels(QStringList() << "Name" << "License Number");

    QPushButton *saveButton = w.findChild<QPushButton*>("SaveButton");
    saveButton->setFixedSize(60,30);
    saveButton->move(5, 300);

    QPushButton *homeButton = w.findChild<QPushButton*>("HomeButton");
    homeButton->setFixedSize(70,30);
    homeButton->move(70, 300);
    
    QPushButton *resetLogin = w.findChild<QPushButton*>("resetLogin");
    resetLogin->setFixedSize(60,30);
    resetLogin->move(145, 300);
    w.show();

    return a.exec();
}
