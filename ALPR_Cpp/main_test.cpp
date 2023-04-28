#include "ALPRUI/mainui.h"
#include <QtWidgets>
#include <iostream>
#include <cuda_runtime.h>

// MAIN function
int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    MainUI w;
    
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int height = screenGeometry.height()-30; //Height: 1050 NanoJetpack 4.6
    int width = screenGeometry.width()-60; //Width: 1860 NanoJetpack 4.6
    w.setWindowTitle("Locator ALPR");

    w.setFixedSize(width, height);

    QFont font = a.font();
    font.setPointSize(10);
    a.setFont(font);

    QLabel *WelcomeLabel = w.findChild<QLabel*>("Welcome");
    WelcomeLabel->setText("      Welcome");
    QLabel *WelcomeLabel2 = w.findChild<QLabel*>("Welcome_2");
    WelcomeLabel2->setText("      Welcome");
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
    settings->setIcon(QIcon("ALPRUI/images/settings.png")); // set the icon
    settings->setIconSize(QSize(width/47, height/30)); // set the icon size      
    settings->setFixedSize(width/47, height/30); // set the same width and height
    settings->move(width-65, 0);

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

    QSpinBox *spotCount1SpinBox = w.findChild<QSpinBox*>("spinBox1");
    QSpinBox *spotCount2SpinBox = w.findChild<QSpinBox*>("spinBox2");
    QSpinBox *spotCount3SpinBox = w.findChild<QSpinBox*>("spinBox3");
    QSpinBox *spotCount4SpinBox = w.findChild<QSpinBox*>("spinBox4");
    spotCount1SpinBox->setRange(0, 3);
    spotCount1SpinBox->setSpecialValueText("Number of zones");
    spotCount2SpinBox->setRange(0, 3);
    spotCount2SpinBox->setSpecialValueText("Number of zones");
    spotCount3SpinBox->setRange(0, 3);
    spotCount3SpinBox->setSpecialValueText("Number of zones");
    spotCount4SpinBox->setRange(0, 3);
    spotCount4SpinBox->setSpecialValueText("Number of zones");

    int cameraLabelPosHeight = 0;
    int cameraNameLineEdit = 27;
    int whiteList = 90;
    int cameraLineEdit = 59;
    int cameraCheckBox = 59;
    int spotCountSpinBox = 90;


    //Group 1
    camera1Label->setFixedSize(width/29, height/35);
    camera1Label->move(5, cameraLabelPosHeight);

    cameraName1LineEdit->setFixedSize(width/7, height/35);
    cameraName1LineEdit->move(5, cameraNameLineEdit);

    whiteList1->setFixedSize(width/14, height/35);
    whiteList1->move(width/13.3, whiteList);

    camera1LineEdit->setFixedSize(width/7, height/35);
    camera1LineEdit->move(5, cameraLineEdit);

    camera1CheckBox->setFixedSize(width/16, height/35);
    camera1CheckBox->move(width/7+15, cameraCheckBox);

    spotCount1SpinBox->setFixedSize(width/14, height/35);
    spotCount1SpinBox->move(5, spotCountSpinBox);

    cameraLabelPosHeight += 120;
    cameraNameLineEdit += 120;
    whiteList += 120;
    cameraLineEdit += 120;
    cameraCheckBox += 120;
    spotCountSpinBox += 120;

    //Group 2
    camera2Label->setFixedSize(width/29, height/35);
    camera2Label->move(5, cameraLabelPosHeight);

    cameraName2LineEdit->setFixedSize(width/7, height/35);
    cameraName2LineEdit->move(5, cameraNameLineEdit);

    whiteList2->setFixedSize(width/14, height/35);
    whiteList2->move(width/13.3, whiteList); 

    camera2LineEdit->setFixedSize(width/7, height/35);
    camera2LineEdit->move(5, cameraLineEdit);

    camera2CheckBox->setFixedSize(width/16, height/35);
    camera2CheckBox->move(width/7+15, cameraCheckBox);
    
    spotCount2SpinBox->setFixedSize(width/14, height/35);
    spotCount2SpinBox->move(5, spotCountSpinBox);

    cameraLabelPosHeight += 120;
    cameraNameLineEdit += 120;
    whiteList += 120;
    cameraLineEdit += 120;
    cameraCheckBox += 120;
    spotCountSpinBox += 120;

    //Group 3
    camera3Label->setFixedSize(width/29, height/35);
    camera3Label->move(5, cameraLabelPosHeight);

    cameraName3LineEdit->setFixedSize(width/7, height/35);
    cameraName3LineEdit->move(5, cameraNameLineEdit);

    whiteList3->setFixedSize(width/14, height/35);
    whiteList3->move(width/13.3, whiteList);

    camera3LineEdit->setFixedSize(width/7, height/35);
    camera3LineEdit->move(5, cameraLineEdit);

    camera3CheckBox->setFixedSize(width/16, height/35);
    camera3CheckBox->move(width/7+15, cameraCheckBox);

    spotCount3SpinBox->setFixedSize(width/14, height/35);
    spotCount3SpinBox->move(5, spotCountSpinBox);

    cameraLabelPosHeight += 120;
    cameraNameLineEdit += 120;
    whiteList += 120;
    cameraLineEdit += 120;
    cameraCheckBox += 120;
    spotCountSpinBox += 120;

    //Group 4
    camera4Label->setFixedSize(width/29, height/35);
    camera4Label->move(5, cameraLabelPosHeight);

    cameraName4LineEdit->setFixedSize(width/7, height/35);
    cameraName4LineEdit->move(5, cameraNameLineEdit);

    whiteList4->setFixedSize(width/14, height/35);
    whiteList4->move(width/13.3, whiteList);

    camera4LineEdit->setFixedSize(width/7, height/35);
    camera4LineEdit->move(5, cameraLineEdit);

    camera4CheckBox->setFixedSize(width/16, height/35);
    camera4CheckBox->move(width/7+15, cameraCheckBox);

    spotCount4SpinBox->setFixedSize(width/14, height/35);
    spotCount4SpinBox->move(5, spotCountSpinBox);

    int buttonsPos = spotCountSpinBox + 35;

    QTableWidget *table = w.findChild<QTableWidget*>("WhiteListTable");
    QPushButton *addLine = w.findChild<QPushButton*>("AddLine");
    QLabel *AddLineWarning = w.findChild<QLabel*>("AddLineWarning"); 
    QLabel *CheckBoxWarning = w.findChild<QLabel*>("CheckBoxWarning"); 
    QLabel *WhiteListLabel = w.findChild<QLabel*>("WhiteListLabel"); 

    addLine->move(width/2-130,40+height/2.5+10);
    addLine->setFixedSize(width/20, height/40);
    
    AddLineWarning->move(width/2-30, 450);
    AddLineWarning->setFixedSize(width/10, height/40);

    CheckBoxWarning->move(5, buttonsPos+60);
    CheckBoxWarning->setFixedSize(400, 20);

    WhiteListLabel->move(width/2-130, 15);
    WhiteListLabel->setFixedSize(width/10, height/40);

    table->move(width/2-130,40);
    table->setRowCount(0);
    table->setColumnCount(2);
    table->setColumnWidth(0, width/12);
    table->setColumnWidth(1, width/12);

    table->resize(width/6+30, height/2.5);
    table->setHorizontalHeaderLabels(QStringList() << "Name" << "License Number");

    QFont fontButtons("Arial", 11, QFont::Black);
    
    QPushButton *saveButton = w.findChild<QPushButton*>("SaveButton");
    saveButton->setFixedSize(width/22,height/24);
    saveButton->move(5, buttonsPos);

    QPushButton *homeButton = w.findChild<QPushButton*>("HomeButton");
    homeButton->setFixedSize(width/22,height/24);
    homeButton->move(width/22+10, buttonsPos);
    
    QPushButton *resetLogin = w.findChild<QPushButton*>("resetLogin");
    resetLogin->setFixedSize(width/21.1,height/24);
    resetLogin->move(width/22+10+width/22+5, buttonsPos);
    
    saveButton->setFont(fontButtons);
    homeButton->setFont(fontButtons);
    resetLogin->setFont(fontButtons);

    w.show();

    return a.exec();
}
