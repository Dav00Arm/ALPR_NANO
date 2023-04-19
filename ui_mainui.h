/********************************************************************************
** Form generated from reading UI file 'mainui.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUI_H
#define UI_MAINUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainUI
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainUI)
    {
        if (MainUI->objectName().isEmpty())
            MainUI->setObjectName(QString::fromUtf8("MainUI"));
        MainUI->resize(400, 300);
        menuBar = new QMenuBar(MainUI);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        MainUI->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainUI);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainUI->addToolBar(mainToolBar);
        centralWidget = new QWidget(MainUI);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainUI->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainUI);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainUI->setStatusBar(statusBar);

        retranslateUi(MainUI);

        QMetaObject::connectSlotsByName(MainUI);
    } // setupUi

    void retranslateUi(QMainWindow *MainUI)
    {
        MainUI->setWindowTitle(QCoreApplication::translate("MainUI", "MainUI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainUI: public Ui_MainUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
