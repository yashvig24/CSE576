/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *img1Tab;
    QLabel *img1Display;
    QWidget *img2Tab;
    QLabel *img2Display;
    QWidget *imgSTab;
    QLabel *imgSDisplay;
    QPushButton *openButton;
    QPushButton *saveButton;
    QPushButton *harrisButton;
    QPushButton *RANSACButton;
    QPushButton *stitchButton;
    QDoubleSpinBox *harrisSpinBox;
    QDoubleSpinBox *harrisThresSpinBox;
    QLabel *label;
    QLabel *label_2;
    QFrame *line;
    QPushButton *matchButton;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *line_4;
    QDoubleSpinBox *RANSACThresSpinBox;
    QLabel *label_3;
    QLabel *label_4;
    QSpinBox *iterationsBox;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1128, 747);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(180, 0, 921, 611));
        img1Tab = new QWidget();
        img1Tab->setObjectName(QString::fromUtf8("img1Tab"));
        img1Display = new QLabel(img1Tab);
        img1Display->setObjectName(QString::fromUtf8("img1Display"));
        img1Display->setGeometry(QRect(15, 12, 890, 565));
        tabWidget->addTab(img1Tab, QString());
        img2Tab = new QWidget();
        img2Tab->setObjectName(QString::fromUtf8("img2Tab"));
        img2Display = new QLabel(img2Tab);
        img2Display->setObjectName(QString::fromUtf8("img2Display"));
        img2Display->setGeometry(QRect(15, 12, 890, 565));
        tabWidget->addTab(img2Tab, QString());
        imgSTab = new QWidget();
        imgSTab->setObjectName(QString::fromUtf8("imgSTab"));
        imgSDisplay = new QLabel(imgSTab);
        imgSDisplay->setObjectName(QString::fromUtf8("imgSDisplay"));
        imgSDisplay->setGeometry(QRect(15, 12, 890, 565));
        tabWidget->addTab(imgSTab, QString());
        openButton = new QPushButton(centralWidget);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        openButton->setGeometry(QRect(180, 20, 101, 31));
        saveButton = new QPushButton(centralWidget);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(290, 20, 101, 31));
        harrisButton = new QPushButton(centralWidget);
        harrisButton->setObjectName(QString::fromUtf8("harrisButton"));
        harrisButton->setGeometry(QRect(10, 100, 91, 41));
        RANSACButton = new QPushButton(centralWidget);
        RANSACButton->setObjectName(QString::fromUtf8("RANSACButton"));
        RANSACButton->setGeometry(QRect(10, 340, 91, 41));
        stitchButton = new QPushButton(centralWidget);
        stitchButton->setObjectName(QString::fromUtf8("stitchButton"));
        stitchButton->setGeometry(QRect(10, 460, 91, 41));
        harrisSpinBox = new QDoubleSpinBox(centralWidget);
        harrisSpinBox->setObjectName(QString::fromUtf8("harrisSpinBox"));
        harrisSpinBox->setGeometry(QRect(10, 170, 62, 22));
        harrisThresSpinBox = new QDoubleSpinBox(centralWidget);
        harrisThresSpinBox->setObjectName(QString::fromUtf8("harrisThresSpinBox"));
        harrisThresSpinBox->setGeometry(QRect(90, 170, 62, 22));
        harrisThresSpinBox->setMaximum(999.990000000000009);
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 150, 46, 13));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(90, 150, 46, 13));
        line = new QFrame(centralWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(10, 200, 161, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        matchButton = new QPushButton(centralWidget);
        matchButton->setObjectName(QString::fromUtf8("matchButton"));
        matchButton->setGeometry(QRect(10, 220, 91, 41));
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(10, 320, 161, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(10, 440, 161, 16));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_4 = new QFrame(centralWidget);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(10, 560, 161, 16));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        RANSACThresSpinBox = new QDoubleSpinBox(centralWidget);
        RANSACThresSpinBox->setObjectName(QString::fromUtf8("RANSACThresSpinBox"));
        RANSACThresSpinBox->setGeometry(QRect(90, 410, 62, 22));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 390, 46, 13));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(90, 390, 46, 13));
        iterationsBox = new QSpinBox(centralWidget);
        iterationsBox->setObjectName(QString::fromUtf8("iterationsBox"));
        iterationsBox->setGeometry(QRect(10, 410, 42, 22));
        iterationsBox->setMaximum(9999);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1128, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        img1Display->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(img1Tab), QApplication::translate("MainWindow", "Image 1", nullptr));
        img2Display->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(img2Tab), QApplication::translate("MainWindow", "Image 2", nullptr));
        imgSDisplay->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(imgSTab), QApplication::translate("MainWindow", "Stitched Image", nullptr));
        openButton->setText(QApplication::translate("MainWindow", "Open Image", nullptr));
        saveButton->setText(QApplication::translate("MainWindow", "Save Image", nullptr));
        harrisButton->setText(QApplication::translate("MainWindow", "Harris Corners", nullptr));
        RANSACButton->setText(QApplication::translate("MainWindow", "RANSAC", nullptr));
        stitchButton->setText(QApplication::translate("MainWindow", "Stitch", nullptr));
        label->setText(QApplication::translate("MainWindow", "Sigma", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Threshold", nullptr));
        matchButton->setText(QApplication::translate("MainWindow", "Find Matches", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Iterations", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Threshold", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
