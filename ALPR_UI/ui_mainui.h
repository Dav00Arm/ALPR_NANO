/********************************************************************************
** Form generated from reading UI file 'mainui.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUI_H
#define UI_MAINUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainUI
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *SignUp;
    QWidget *CenterWidget;
    QGridLayout *gridLayout_3;
    QWidget *widget_2;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *Welcome;
    QLabel *Username_label;
    QLineEdit *Username_input;
    QLabel *Password_label;
    QLineEdit *Password_input;
    QLabel *Password2_label;
    QLineEdit *Password2_input;
    QLabel *Password_mismatch;
    QPushButton *SignUp_button;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_3;
    QWidget *CamerView;
    QWidget *widget;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainUI)
    {
        if (MainUI->objectName().isEmpty())
            MainUI->setObjectName(QStringLiteral("MainUI"));
        MainUI->resize(1096, 600);
        MainUI->setWindowOpacity(1);
        MainUI->setLayoutDirection(Qt::LeftToRight);
        MainUI->setStyleSheet(QLatin1String("/*Background image*/\n"
"QMainWindow{\n"
"	border-image: url(/home/jets/Desktop/ALPR_UI/images/signup.jpg);\n"
"}\n"
"\n"
"/*white widget*/\n"
"#widget_2{\n"
"	border-radius: 10px;\n"
"}\n"
"\n"
"QLabel{\n"
"	color: #888888;\n"
"}\n"
"\n"
"QLineEdit{\n"
"	border: none;\n"
"	border-bottom: 1px solid #cccccc;\n"
"}\n"
"\n"
"QLineEdit:focus{\n"
"	border: none;\n"
"	border-bottom: 1px solid rgb(27, 121, 209);\n"
"}\n"
"\n"
"QPushButton{\n"
"	color: rgb(27, 121, 209);\n"
"	border: 1px solid rgb(27, 121, 209);\n"
"	border-radius: 5px;\n"
"}\n"
"\n"
""));
        centralWidget = new QWidget(MainUI);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMinimumSize(QSize(0, 0));
        centralWidget->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        SignUp = new QWidget();
        SignUp->setObjectName(QStringLiteral("SignUp"));
        SignUp->setLayoutDirection(Qt::LeftToRight);
        CenterWidget = new QWidget(SignUp);
        CenterWidget->setObjectName(QStringLiteral("CenterWidget"));
        CenterWidget->setGeometry(QRect(200, 50, 700, 500));
        CenterWidget->setMinimumSize(QSize(0, 0));
        gridLayout_3 = new QGridLayout(CenterWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        widget_2 = new QWidget(CenterWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setMinimumSize(QSize(250, 280));
        QFont font;
        font.setKerning(false);
        widget_2->setFont(font);
        widget_2->setLayoutDirection(Qt::LeftToRight);
        widget_2->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        gridLayout_2 = new QGridLayout(widget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(20);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, -1, -1, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        Welcome = new QLabel(widget_2);
        Welcome->setObjectName(QStringLiteral("Welcome"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Welcome->sizePolicy().hasHeightForWidth());
        Welcome->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setFamily(QStringLiteral("DejaVu Serif"));
        font1.setPointSize(15);
        font1.setBold(true);
        font1.setWeight(75);
        Welcome->setFont(font1);
        Welcome->setStyleSheet(QStringLiteral("color: rgb(27, 121, 209);"));

        verticalLayout->addWidget(Welcome);

        Username_label = new QLabel(widget_2);
        Username_label->setObjectName(QStringLiteral("Username_label"));
        QFont font2;
        font2.setFamily(QStringLiteral("DejaVu Serif"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        Username_label->setFont(font2);

        verticalLayout->addWidget(Username_label);

        Username_input = new QLineEdit(widget_2);
        Username_input->setObjectName(QStringLiteral("Username_input"));
        Username_input->setMinimumSize(QSize(200, 0));

        verticalLayout->addWidget(Username_input);

        Password_label = new QLabel(widget_2);
        Password_label->setObjectName(QStringLiteral("Password_label"));
        Password_label->setFont(font2);

        verticalLayout->addWidget(Password_label);

        Password_input = new QLineEdit(widget_2);
        Password_input->setObjectName(QStringLiteral("Password_input"));
        Password_input->setMinimumSize(QSize(200, 0));
        Password_input->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(Password_input);

        Password2_label = new QLabel(widget_2);
        Password2_label->setObjectName(QStringLiteral("Password2_label"));
        Password2_label->setFont(font2);
        Password2_label->setTextFormat(Qt::RichText);
        Password2_label->setMargin(0);

        verticalLayout->addWidget(Password2_label);

        Password2_input = new QLineEdit(widget_2);
        Password2_input->setObjectName(QStringLiteral("Password2_input"));
        Password2_input->setMinimumSize(QSize(200, 0));
        Password2_input->setStyleSheet(QStringLiteral(""));
        Password2_input->setEchoMode(QLineEdit::Password);

        verticalLayout->addWidget(Password2_input);


        verticalLayout_2->addLayout(verticalLayout);

        Password_mismatch = new QLabel(widget_2);
        Password_mismatch->setObjectName(QStringLiteral("Password_mismatch"));
        Password_mismatch->setStyleSheet(QStringLiteral("color: red;"));
        Password_mismatch->setMargin(0);

        verticalLayout_2->addWidget(Password_mismatch);

        SignUp_button = new QPushButton(widget_2);
        SignUp_button->setObjectName(QStringLiteral("SignUp_button"));
        SignUp_button->setMinimumSize(QSize(0, 30));
        QFont font3;
        font3.setFamily(QStringLiteral("DejaVu Serif"));
        font3.setPointSize(13);
        font3.setBold(true);
        font3.setWeight(75);
        font3.setKerning(true);
        font3.setStyleStrategy(QFont::PreferAntialias);
        SignUp_button->setFont(font3);
        SignUp_button->setCursor(QCursor(Qt::PointingHandCursor));
        SignUp_button->setAutoFillBackground(false);
        SignUp_button->setStyleSheet(QLatin1String("QPushButton:hover:pressed{\n"
"	background-color: rgb(27, 121, 209);\n"
"	color: #fff;\n"
"}\n"
"\n"
"QPushButton:hover{\n"
"	background-color: #74A3EC;\n"
"	color: #fff;\n"
"}"));
        SignUp_button->setAutoDefault(false);

        verticalLayout_2->addWidget(SignUp_button);


        gridLayout_2->addLayout(verticalLayout_2, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 2, 1, 1, 1);


        gridLayout_3->addWidget(widget_2, 1, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(149, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_4, 1, 2, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 49, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_4, 2, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(149, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 1, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 49, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer_3, 0, 1, 1, 1);

        stackedWidget->addWidget(SignUp);
        CamerView = new QWidget();
        CamerView->setObjectName(QStringLiteral("CamerView"));
        widget = new QWidget(CamerView);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(9, -1, 1071, 541));
        stackedWidget->addWidget(CamerView);

        horizontalLayout->addWidget(stackedWidget);

        MainUI->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainUI);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainUI->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainUI);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainUI->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainUI);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1096, 15));
        MainUI->setMenuBar(menuBar);

        retranslateUi(MainUI);

        SignUp_button->setDefault(false);


        QMetaObject::connectSlotsByName(MainUI);
    } // setupUi

    void retranslateUi(QMainWindow *MainUI)
    {
        MainUI->setWindowTitle(QApplication::translate("MainUI", "Sign up", Q_NULLPTR));
        Welcome->setText(QApplication::translate("MainUI", "               Welcome", Q_NULLPTR));
        Username_label->setText(QApplication::translate("MainUI", "Username", Q_NULLPTR));
        Password_label->setText(QApplication::translate("MainUI", "Password", Q_NULLPTR));
        Password2_label->setText(QApplication::translate("MainUI", "Repeat password", Q_NULLPTR));
        Password_mismatch->setText(QString());
        SignUp_button->setText(QApplication::translate("MainUI", "Sign up", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainUI: public Ui_MainUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
