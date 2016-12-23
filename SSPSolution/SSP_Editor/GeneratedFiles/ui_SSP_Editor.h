/********************************************************************************
** Form generated from reading UI file 'SSP_Editor.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SSP_EDITOR_H
#define UI_SSP_EDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
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
    QGridLayout *gridLayout;
    QLabel *label_2;
    QWidget *RenderWidget;
    QWidget *RenderWidget_2;
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QWidget *tab_2;
    QMenuBar *menuBar;
    QMenu *menuEditor;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SSP_EditorClass)
    {
        if (SSP_EditorClass->objectName().isEmpty())
            SSP_EditorClass->setObjectName(QStringLiteral("SSP_EditorClass"));
        SSP_EditorClass->resize(1131, 695);
        SSP_EditorClass->setAutoFillBackground(false);
        SSP_EditorClass->setStyleSheet(QLatin1String("\n"
"QMainWindow{\n"
"background:    rgb(68, 68, 68);\n"
"border: black;\n"
"}\n"
"QMenuBar{\n"
"background:   rgb(68, 68, 68);\n"
"color: white;\n"
"border: black;\n"
"}\n"
"\n"
"\n"
"QToolBar{\n"
"border: black;}\n"
"\n"
"QTabWidget::pane { /* The tab widget frame */\n"
"    border: 1px solid black;\n"
"	background :  rgb(68, 68, 68);\n"
"	color : green;\n"
"}\n"
"\n"
"\n"
"QTabBar::tab{\n"
"    background:  rgb(48, 48, 48);\n"
"    border: 1px solid rgb(68, 68, 68) ;\n"
"    border-bottom-color:  rgb(68, 68, 68); /* same as the pane color */\n"
"    border-top-left-radius: 4px;\n"
"    border-top-right-radius: 4px;\n"
"    min-width: 8ex;\n"
"    padding: 5px;\n"
" 	color : white;\n"
"}\n"
"QTabBar::tab:selected{\n"
"    background : rgb(111, 111, 111);\n"
"}\n"
"QTabBar::tab:!selected {\n"
"    margin-top: 2px; /* make non-selected tabs look smaller */\n"
"}\n"
"\n"
"\n"
"QTreeWidget{\n"
"background :  rgb(68, 68, 68);\n"
"spacing: 10px;\n"
"show-decoration-selected: 1; \n"
"border:black;\n"
" \n"
"}\n"
"\n"
""
                        "QTreeWidget::Item:has-children{\n"
"spacing: 3px;\n"
"border-radius: 4px;\n"
"border: 1px solid;\n"
"color: white;\n"
"background : rgb(88, 88, 88);\n"
"}\n"
"QTreeWidget::Item:!has-children{ \n"
"spacing: 3px;\n"
"border-radius: 1px;\n"
"border: 1px solid;\n"
"background : rgb(222, 222, 222);\n"
"}\n"
"QTreeView::item:hover {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1);\n"
"    border: 1px solid #bfcde4;\n"
"}\n"
"QTreeView::item:pressed {\n"
"    background: green;\n"
"    border: 1px solid #bfcde4;\n"
"}\n"
"\n"
"\n"
"\n"
"#centralWidget{\n"
"background:  rgb(68, 68, 68);\n"
"border : 1px solid black;\n"
"}\n"
"\n"
"QLabel{\n"
"color : white;\n"
"}\n"
"\n"
"QPushButton{\n"
" color: white;\n"
" background-color : rgb(111, 111, 111);\n"
" border-radius : 2px;\n"
"padding :5px;\n"
" border:  rgb(115, 115, 115) ;\n"
"}\n"
"QPushButton:hover{\n"
"   background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1);\n"
"    border: 1px "
                        "solid #bfcde4;}\n"
"\n"
"QPushButton:pressed{\n"
"  background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(221, 263, 239), stop: 1 #cbdaf1);\n"
"    border: 1px solid #bfcde4;}\n"
"\n"
""));
        SSP_EditorClass->setAnimated(true);
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
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        RenderWidget = new QWidget(centralWidget);
        RenderWidget->setObjectName(QStringLiteral("RenderWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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

        gridLayout->addWidget(RenderWidget, 1, 2, 2, 1);

        RenderWidget_2 = new QWidget(centralWidget);
        RenderWidget_2->setObjectName(QStringLiteral("RenderWidget_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(RenderWidget_2->sizePolicy().hasHeightForWidth());
        RenderWidget_2->setSizePolicy(sizePolicy1);
        RenderWidget_2->setCursor(QCursor(Qt::ArrowCursor));
        RenderWidget_2->setMouseTracking(true);
        RenderWidget_2->setFocusPolicy(Qt::StrongFocus);
        RenderWidget_2->setAcceptDrops(true);
        RenderWidget_2->setAutoFillBackground(true);

        gridLayout->addWidget(RenderWidget_2, 2, 3, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));

        gridLayout->addWidget(groupBox, 1, 3, 1, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy2);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        treeWidget = new QTreeWidget(tab);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setLayoutDirection(Qt::LeftToRight);
        treeWidget->setTextElideMode(Qt::ElideNone);
        treeWidget->setSortingEnabled(true);
        treeWidget->setHeaderHidden(true);
        treeWidget->header()->setCascadingSectionResizes(false);

        verticalLayout->addWidget(treeWidget);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 2, 1, 1, 1);

        SSP_EditorClass->setCentralWidget(centralWidget);
        label_2->raise();
        groupBox->raise();
        RenderWidget->raise();
        RenderWidget_2->raise();
        pushButton->raise();
        tabWidget->raise();
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

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SSP_EditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *SSP_EditorClass)
    {
        SSP_EditorClass->setWindowTitle(QApplication::translate("SSP_EditorClass", "SSP_Editor", Q_NULLPTR));
        actionNew_scene->setText(QApplication::translate("SSP_EditorClass", "New scene", Q_NULLPTR));
        actionLoad_scene->setText(QApplication::translate("SSP_EditorClass", "Load scene", Q_NULLPTR));
        actionSave_scene->setText(QApplication::translate("SSP_EditorClass", "Save scene", Q_NULLPTR));
        actionBuild_BPF->setText(QApplication::translate("SSP_EditorClass", "Build .BPF", Q_NULLPTR));
        label_2->setText(QApplication::translate("SSP_EditorClass", "Preview", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("SSP_EditorClass", "Attributes", Q_NULLPTR));
        pushButton->setText(QApplication::translate("SSP_EditorClass", "Reload", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("SSP_EditorClass", "Files", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SSP_EditorClass", "Asset Browser", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SSP_EditorClass", "Scene info", Q_NULLPTR));
        menuEditor->setTitle(QApplication::translate("SSP_EditorClass", "Editor", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SSP_EditorClass: public Ui_SSP_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSP_EDITOR_H
