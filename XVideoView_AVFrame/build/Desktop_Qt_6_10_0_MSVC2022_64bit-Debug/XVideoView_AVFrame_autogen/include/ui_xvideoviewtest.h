/********************************************************************************
** Form generated from reading UI file 'xvideoviewtest.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XVIDEOVIEWTEST_H
#define UI_XVIDEOVIEWTEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XVideoViewTest
{
public:
    QAction *action0_5X;
    QAction *action0_75X;
    QAction *action1_0X;
    QAction *action1_25X;
    QAction *action1_5X;
    QAction *action2_0X;
    QAction *action3_0X;
    QWidget *centralwidget;
    QLabel *videoLB;
    QLabel *fpsLB;
    QMenuBar *menuBar;
    QMenu *videomMenu;
    QMenu *menu_2;

    void setupUi(QMainWindow *XVideoViewTest)
    {
        if (XVideoViewTest->objectName().isEmpty())
            XVideoViewTest->setObjectName("XVideoViewTest");
        XVideoViewTest->resize(800, 600);
        action0_5X = new QAction(XVideoViewTest);
        action0_5X->setObjectName("action0_5X");
        action0_75X = new QAction(XVideoViewTest);
        action0_75X->setObjectName("action0_75X");
        action1_0X = new QAction(XVideoViewTest);
        action1_0X->setObjectName("action1_0X");
        action1_25X = new QAction(XVideoViewTest);
        action1_25X->setObjectName("action1_25X");
        action1_5X = new QAction(XVideoViewTest);
        action1_5X->setObjectName("action1_5X");
        action2_0X = new QAction(XVideoViewTest);
        action2_0X->setObjectName("action2_0X");
        action3_0X = new QAction(XVideoViewTest);
        action3_0X->setObjectName("action3_0X");
        centralwidget = new QWidget(XVideoViewTest);
        centralwidget->setObjectName("centralwidget");
        videoLB = new QLabel(centralwidget);
        videoLB->setObjectName("videoLB");
        videoLB->setGeometry(QRect(100, 140, 441, 331));
        fpsLB = new QLabel(centralwidget);
        fpsLB->setObjectName("fpsLB");
        fpsLB->setGeometry(QRect(20, 40, 50, 20));
        fpsLB->setStyleSheet(QString::fromUtf8(""));
        XVideoViewTest->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(XVideoViewTest);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        videomMenu = new QMenu(menuBar);
        videomMenu->setObjectName("videomMenu");
        menu_2 = new QMenu(videomMenu);
        menu_2->setObjectName("menu_2");
        XVideoViewTest->setMenuBar(menuBar);

        menuBar->addAction(videomMenu->menuAction());
        videomMenu->addAction(menu_2->menuAction());
        menu_2->addAction(action0_5X);
        menu_2->addAction(action0_75X);
        menu_2->addAction(action1_0X);
        menu_2->addAction(action1_25X);
        menu_2->addAction(action1_5X);
        menu_2->addAction(action2_0X);
        menu_2->addAction(action3_0X);

        retranslateUi(XVideoViewTest);

        QMetaObject::connectSlotsByName(XVideoViewTest);
    } // setupUi

    void retranslateUi(QMainWindow *XVideoViewTest)
    {
        XVideoViewTest->setWindowTitle(QCoreApplication::translate("XVideoViewTest", "XVideoViewTest", nullptr));
        action0_5X->setText(QCoreApplication::translate("XVideoViewTest", "0.5X", nullptr));
        action0_75X->setText(QCoreApplication::translate("XVideoViewTest", "0.75X", nullptr));
        action1_0X->setText(QCoreApplication::translate("XVideoViewTest", "1.0X", nullptr));
        action1_25X->setText(QCoreApplication::translate("XVideoViewTest", "1.25X", nullptr));
        action1_5X->setText(QCoreApplication::translate("XVideoViewTest", "1.5X", nullptr));
        action2_0X->setText(QCoreApplication::translate("XVideoViewTest", "2.0X", nullptr));
        action3_0X->setText(QCoreApplication::translate("XVideoViewTest", "3.0X", nullptr));
        videoLB->setText(QString());
        fpsLB->setText(QString());
        videomMenu->setTitle(QCoreApplication::translate("XVideoViewTest", "\350\247\206\351\242\221", nullptr));
        menu_2->setTitle(QCoreApplication::translate("XVideoViewTest", "\345\200\215\346\225\260\346\222\255\346\224\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XVideoViewTest: public Ui_XVideoViewTest {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XVIDEOVIEWTEST_H
