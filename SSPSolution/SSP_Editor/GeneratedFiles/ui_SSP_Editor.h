/********************************************************************************
** Form generated from reading UI file 'SSP_Editor.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SSP_EDITOR_H
#define UI_SSP_EDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SSP_EditorClass
{
public:
    QAction *actionNew_scene;
    QAction *actionLoad_scene;
    QAction *actionSave_scene;
    QAction *actionBuild_BPF;
    QWidget *centralWidget;
    QLabel *label;
    QGraphicsView *graphicsView;
    QLabel *label_2;
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QTreeView *treeView;
    QWidget *test;
    QMenuBar *menuBar;
    QMenu *menuEditor;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SSP_EditorClass)
    {
        if (SSP_EditorClass->objectName().isEmpty())
            SSP_EditorClass->setObjectName(QStringLiteral("SSP_EditorClass"));
        SSP_EditorClass->resize(1122, 747);
        actionNew_scene = new QAction(SSP_EditorClass);
        actionNew_scene->setObjectName(QStringLiteral("actionNew_scene"));
        actionLoad_scene = new QAction(SSP_EditorClass);
        actionLoad_scene->setObjectName(QStringLiteral("actionLoad_scene"));
        actionSave_scene = new QAction(SSP_EditorClass);
        actionSave_scene->setObjectName(QStringLiteral("actionSave_scene"));
        actionBuild_BPF = new QAction(SSP_EditorClass);
        actionBuild_BPF->setObjectName(QStringLiteral("actionBuild_BPF"));
        centralWidget = new QWidget(SSP_EditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 47, 13));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(230, 30, 711, 571));
        graphicsView->setMinimumSize(QSize(10, 10));
        graphicsView->setMouseTracking(true);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(230, 10, 47, 13));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(950, 30, 161, 571));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(20, 610, 75, 23));
        treeView = new QTreeView(centralWidget);
        treeView->setObjectName(QStringLiteral("treeView"));
        treeView->setGeometry(QRect(20, 30, 201, 571));
        test = new QWidget(centralWidget);
        test->setObjectName(QStringLiteral("test"));
        test->setGeometry(QRect(230, 30, 711, 571));
        test->setMouseTracking(true);
        test->setAcceptDrops(true);
        SSP_EditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SSP_EditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1122, 21));
        menuEditor = new QMenu(menuBar);
        menuEditor->setObjectName(QStringLiteral("menuEditor"));
        SSP_EditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SSP_EditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SSP_EditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SSP_EditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SSP_EditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuEditor->menuAction());
        menuEditor->addAction(actionNew_scene);
        menuEditor->addSeparator();
        menuEditor->addAction(actionLoad_scene);
        menuEditor->addAction(actionSave_scene);
        menuEditor->addSeparator();
        menuEditor->addAction(actionBuild_BPF);

        retranslateUi(SSP_EditorClass);

        QMetaObject::connectSlotsByName(SSP_EditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *SSP_EditorClass)
    {
        SSP_EditorClass->setWindowTitle(QApplication::translate("SSP_EditorClass", "SSP_Editor", 0));
        actionNew_scene->setText(QApplication::translate("SSP_EditorClass", "New scene", 0));
        actionLoad_scene->setText(QApplication::translate("SSP_EditorClass", "Load scene", 0));
        actionSave_scene->setText(QApplication::translate("SSP_EditorClass", "Save scene", 0));
        actionBuild_BPF->setText(QApplication::translate("SSP_EditorClass", "Build .BPF", 0));
        label->setText(QApplication::translate("SSP_EditorClass", "File Editor", 0));
        label_2->setText(QApplication::translate("SSP_EditorClass", "Preview", 0));
        groupBox->setTitle(QApplication::translate("SSP_EditorClass", "Attributes", 0));
        pushButton->setText(QApplication::translate("SSP_EditorClass", "Reload", 0));
        menuEditor->setTitle(QApplication::translate("SSP_EditorClass", "Editor", 0));
    } // retranslateUi

};

namespace Ui {
    class SSP_EditorClass: public Ui_SSP_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSP_EDITOR_H
