/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *pushButton_L;
    QLabel *label_T_Name;
    QLabel *label_H_Name;
    QLabel *label_L_Name;
    QLabel *label_D_Name;
    QLabel *label_T;
    QLabel *label_H;
    QLabel *label_L;
    QLabel *label_D;
    QPushButton *pushButton_Flush;
    QLabel *label_L_Pic;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(753, 560);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 351, 31));
        pushButton_L = new QPushButton(centralwidget);
        pushButton_L->setObjectName(QString::fromUtf8("pushButton_L"));
        pushButton_L->setGeometry(QRect(520, 390, 93, 28));
        label_T_Name = new QLabel(centralwidget);
        label_T_Name->setObjectName(QString::fromUtf8("label_T_Name"));
        label_T_Name->setGeometry(QRect(60, 140, 72, 15));
        label_H_Name = new QLabel(centralwidget);
        label_H_Name->setObjectName(QString::fromUtf8("label_H_Name"));
        label_H_Name->setGeometry(QRect(60, 195, 72, 20));
        label_L_Name = new QLabel(centralwidget);
        label_L_Name->setObjectName(QString::fromUtf8("label_L_Name"));
        label_L_Name->setGeometry(QRect(60, 250, 72, 15));
        label_D_Name = new QLabel(centralwidget);
        label_D_Name->setObjectName(QString::fromUtf8("label_D_Name"));
        label_D_Name->setGeometry(QRect(60, 300, 72, 15));
        label_T = new QLabel(centralwidget);
        label_T->setObjectName(QString::fromUtf8("label_T"));
        label_T->setGeometry(QRect(160, 140, 72, 15));
        label_H = new QLabel(centralwidget);
        label_H->setObjectName(QString::fromUtf8("label_H"));
        label_H->setGeometry(QRect(160, 200, 72, 15));
        label_L = new QLabel(centralwidget);
        label_L->setObjectName(QString::fromUtf8("label_L"));
        label_L->setGeometry(QRect(160, 250, 101, 16));
        label_D = new QLabel(centralwidget);
        label_D->setObjectName(QString::fromUtf8("label_D"));
        label_D->setGeometry(QRect(160, 300, 72, 15));
        pushButton_Flush = new QPushButton(centralwidget);
        pushButton_Flush->setObjectName(QString::fromUtf8("pushButton_Flush"));
        pushButton_Flush->setGeometry(QRect(600, 30, 93, 28));
        label_L_Pic = new QLabel(centralwidget);
        label_L_Pic->setObjectName(QString::fromUtf8("label_L_Pic"));
        label_L_Pic->setGeometry(QRect(510, 150, 141, 201));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 753, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QString());
        pushButton_L->setText(QString());
        label_T_Name->setText(QCoreApplication::translate("MainWindow", "\346\270\251\345\272\246\357\274\232", nullptr));
        label_H_Name->setText(QCoreApplication::translate("MainWindow", "\346\271\277\345\272\246\357\274\232", nullptr));
        label_L_Name->setText(QCoreApplication::translate("MainWindow", "\345\205\211\347\205\247\346\203\205\345\206\265\357\274\232", nullptr));
        label_D_Name->setText(QCoreApplication::translate("MainWindow", "\351\227\250\347\252\227\346\203\205\345\206\265\357\274\232", nullptr));
        label_T->setText(QString());
        label_H->setText(QString());
        label_L->setText(QString());
        label_D->setText(QString());
        pushButton_Flush->setText(QCoreApplication::translate("MainWindow", "\345\210\267\346\226\260", nullptr));
        label_L_Pic->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
