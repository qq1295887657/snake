/********************************************************************************
** Form generated from reading UI file 'MainGameWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINGAMEWINDOW_H
#define UI_MAINGAMEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainGameWindowClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainGameWindowClass)
    {
        if (MainGameWindowClass->objectName().isEmpty())
            MainGameWindowClass->setObjectName(QStringLiteral("MainGameWindowClass"));
        MainGameWindowClass->resize(600, 400);
 /*
		menuBar = new QMenuBar(MainGameWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        MainGameWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainGameWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainGameWindowClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(MainGameWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainGameWindowClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainGameWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainGameWindowClass->setStatusBar(statusBar);
		*/
        retranslateUi(MainGameWindowClass);

        QMetaObject::connectSlotsByName(MainGameWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainGameWindowClass)
    {
        MainGameWindowClass->setWindowTitle(QApplication::translate("MainGameWindowClass", "MainGameWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainGameWindowClass: public Ui_MainGameWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINGAMEWINDOW_H
