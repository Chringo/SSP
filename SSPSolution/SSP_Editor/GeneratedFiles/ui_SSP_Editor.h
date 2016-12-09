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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
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
    QLabel *label_2;
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QWidget *RenderWidget;
    QWidget *RenderWidget_2;
    QTreeWidget *treeWidget;
    QMenuBar *menuBar;
    QMenu *menuEditor;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SSP_EditorClass)
    {
        if (SSP_EditorClass->objectName().isEmpty())
            SSP_EditorClass->setObjectName(QStringLiteral("SSP_EditorClass"));
        SSP_EditorClass->resize(1131, 695);
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
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(230, 10, 47, 13));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(950, 30, 161, 401));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(20, 610, 75, 23));
        RenderWidget = new QWidget(centralWidget);
        RenderWidget->setObjectName(QStringLiteral("RenderWidget"));
        RenderWidget->setGeometry(QRect(230, 30, 711, 571));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RenderWidget->sizePolicy().hasHeightForWidth());
        RenderWidget->setSizePolicy(sizePolicy);
        RenderWidget->setMinimumSize(QSize(711, 571));
        RenderWidget->setCursor(QCursor(Qt::ArrowCursor));
        RenderWidget->setMouseTracking(true);
        RenderWidget->setFocusPolicy(Qt::StrongFocus);
        RenderWidget->setAcceptDrops(true);
        RenderWidget->setAutoFillBackground(true);
        RenderWidget_2 = new QWidget(centralWidget);
        RenderWidget_2->setObjectName(QStringLiteral("RenderWidget_2"));
        RenderWidget_2->setGeometry(QRect(950, 440, 161, 161));
        sizePolicy.setHeightForWidth(RenderWidget_2->sizePolicy().hasHeightForWidth());
        RenderWidget_2->setSizePolicy(sizePolicy);
        RenderWidget_2->setCursor(QCursor(Qt::ArrowCursor));
        RenderWidget_2->setMouseTracking(true);
        RenderWidget_2->setFocusPolicy(Qt::StrongFocus);
        RenderWidget_2->setAcceptDrops(true);
        RenderWidget_2->setAutoFillBackground(true);
        treeWidget = new QTreeWidget(centralWidget);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(20, 30, 191, 571));
        SSP_EditorClass->setCentralWidget(centralWidget);
        label_2->raise();
        label->raise();
        groupBox->raise();
        pushButton->raise();
        RenderWidget->raise();
        RenderWidget_2->raise();
        treeWidget->raise();
        menuBar = new QMenuBar(SSP_EditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1131, 21));
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
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("SSP_EditorClass", "Files", 0));
        menuEditor->setTitle(QApplication::translate("SSP_EditorClass", "Editor", 0));
    } // retranslateUi

};

namespace Ui {
    class SSP_EditorClass: public Ui_SSP_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSP_EDITOR_H
