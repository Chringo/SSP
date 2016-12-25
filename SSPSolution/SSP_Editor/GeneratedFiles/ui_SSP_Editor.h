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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
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
    QWidget *RenderWidget;
    QLabel *label_2;
    QPushButton *pushButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QTreeWidget *treeWidget;
    QWidget *tab_2;
    QGroupBox *Values;
    QVBoxLayout *verticalLayout_3;
    QFrame *nameQFrame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *nameLabel;
    QLineEdit *lineEdit;
    QFrame *transformFrame;
    QGridLayout *gridLayout_3;
    QLabel *rotationlabel;
    QDoubleSpinBox *xValue_translate;
    QDoubleSpinBox *yValue_translate;
    QLabel *scalelabel;
    QDoubleSpinBox *zValue_translate;
    QLabel *translateLabel;
    QDoubleSpinBox *xValue_scale;
    QDoubleSpinBox *xValue_rot;
    QDoubleSpinBox *yValue_scale;
    QDoubleSpinBox *yValue_rot;
    QDoubleSpinBox *zValue_scale;
    QDoubleSpinBox *zValue_rot;
    QFrame *variousOptionsframe;
    QFormLayout *formLayout;
    QLabel *UIDTEXT;
    QLabel *uniqueIDLabel;
    QLabel *ANIMTEXT;
    QComboBox *comboBox;
    QCheckBox *checkBox;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QMenu *menuEditor;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SSP_EditorClass)
    {
        if (SSP_EditorClass->objectName().isEmpty())
            SSP_EditorClass->setObjectName(QStringLiteral("SSP_EditorClass"));
        SSP_EditorClass->resize(1171, 703);
        SSP_EditorClass->setAutoFillBackground(false);
        SSP_EditorClass->setStyleSheet(QLatin1String("\n"
"QMainWindow,QMessageBox{\n"
"background:    rgb(68, 68, 68);\n"
"border: black;\n"
"}\n"
"QMenuBar{\n"
"background:   rgb(68, 68, 68);\n"
"color: white;\n"
"border: black;\n"
"}\n"
"QMenu{\n"
"background:   rgb(68, 68, 68);\n"
"color: white;\n"
"border: black;\n"
"}\n"
"QMenu::item:selected {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1);\n"
"	color:black;\n"
"}\n"
"\n"
"QGroupBox{\n"
"background:   rgb(68, 68, 68);\n"
"color: white;\n"
"border: 1px solid black;\n"
"border-radius : 1px;\n"
"}\n"
"QGroupBox::title{\n"
"background:   transparent;\n"
"color: white;\n"
"}\n"
"\n"
"QDoubleSpinBox{\n"
"background:  rgb(48, 48, 48);\n"
"}\n"
"\n"
"QLineEdit{\n"
"background:  rgb(48, 48, 48);\n"
"}\n"
"QComboBox{\n"
"background:  rgb(48, 48, 48);\n"
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
"QTabB"
                        "ar::tab{\n"
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
"background : rgb(111, 111, 111);\n"
"}\n"
"QTreeView::item:hover {\n"
"    background: qlineargradient(x1: 0, y1: 0, x"
                        "2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1);\n"
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
"#nameQFrame{\n"
" margin-top: 10px;\n"
"}\n"
"\n"
"#transformFrame{\n"
"	background : rgb(74, 74, 74);\n"
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
"    border: 1px solid #bfcde4;}\n"
"\n"
"QPushButton:pressed{\n"
"  background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(221, 263, 239), stop: 1 #cbdaf1);\n"
"    border: 1px solid #bfcde4;}\n"
"\n"
"background:   rgb(68, 68, 68);\n"
"color: white;\n"
"border: black;"));
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
        RenderWidget = new QWidget(centralWidget);
        RenderWidget->setObjectName(QStringLiteral("RenderWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RenderWidget->sizePolicy().hasHeightForWidth());
        RenderWidget->setSizePolicy(sizePolicy);
        RenderWidget->setMinimumSize(QSize(1, 1));
        RenderWidget->setCursor(QCursor(Qt::ArrowCursor));
        RenderWidget->setMouseTracking(true);
        RenderWidget->setFocusPolicy(Qt::StrongFocus);
        RenderWidget->setAcceptDrops(true);
        RenderWidget->setAutoFillBackground(false);

        gridLayout->addWidget(RenderWidget, 1, 2, 3, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 3, 1, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setMinimumSize(QSize(200, 0));
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

        gridLayout->addWidget(tabWidget, 1, 1, 1, 1);

        Values = new QGroupBox(centralWidget);
        Values->setObjectName(QStringLiteral("Values"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(Values->sizePolicy().hasHeightForWidth());
        Values->setSizePolicy(sizePolicy2);
        Values->setMinimumSize(QSize(250, 571));
        Values->setMaximumSize(QSize(250, 16777215));
        Values->setAutoFillBackground(false);
        Values->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        Values->setFlat(false);
        verticalLayout_3 = new QVBoxLayout(Values);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        nameQFrame = new QFrame(Values);
        nameQFrame->setObjectName(QStringLiteral("nameQFrame"));
        nameQFrame->setFrameShape(QFrame::StyledPanel);
        nameQFrame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(nameQFrame);
        horizontalLayout_2->setSpacing(4);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        nameLabel = new QLabel(nameQFrame);
        nameLabel->setObjectName(QStringLiteral("nameLabel"));

        horizontalLayout_2->addWidget(nameLabel);

        lineEdit = new QLineEdit(nameQFrame);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setCursorMoveStyle(Qt::VisualMoveStyle);

        horizontalLayout_2->addWidget(lineEdit);


        verticalLayout_3->addWidget(nameQFrame);

        transformFrame = new QFrame(Values);
        transformFrame->setObjectName(QStringLiteral("transformFrame"));
        transformFrame->setFrameShape(QFrame::StyledPanel);
        transformFrame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(transformFrame);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        rotationlabel = new QLabel(transformFrame);
        rotationlabel->setObjectName(QStringLiteral("rotationlabel"));

        gridLayout_3->addWidget(rotationlabel, 4, 0, 1, 1);

        xValue_translate = new QDoubleSpinBox(transformFrame);
        xValue_translate->setObjectName(QStringLiteral("xValue_translate"));
        xValue_translate->setWrapping(true);
        xValue_translate->setFrame(true);
        xValue_translate->setButtonSymbols(QAbstractSpinBox::NoButtons);
        xValue_translate->setDecimals(4);
        xValue_translate->setMinimum(-10000);
        xValue_translate->setMaximum(10000);

        gridLayout_3->addWidget(xValue_translate, 1, 1, 1, 1);

        yValue_translate = new QDoubleSpinBox(transformFrame);
        yValue_translate->setObjectName(QStringLiteral("yValue_translate"));
        yValue_translate->setWrapping(true);
        yValue_translate->setButtonSymbols(QAbstractSpinBox::NoButtons);
        yValue_translate->setDecimals(4);
        yValue_translate->setMinimum(-10000);
        yValue_translate->setMaximum(10000);

        gridLayout_3->addWidget(yValue_translate, 1, 2, 1, 1);

        scalelabel = new QLabel(transformFrame);
        scalelabel->setObjectName(QStringLiteral("scalelabel"));

        gridLayout_3->addWidget(scalelabel, 5, 0, 1, 1);

        zValue_translate = new QDoubleSpinBox(transformFrame);
        zValue_translate->setObjectName(QStringLiteral("zValue_translate"));
        zValue_translate->setWrapping(true);
        zValue_translate->setButtonSymbols(QAbstractSpinBox::NoButtons);
        zValue_translate->setDecimals(4);
        zValue_translate->setMinimum(-10000);
        zValue_translate->setMaximum(10000);

        gridLayout_3->addWidget(zValue_translate, 1, 3, 1, 1);

        translateLabel = new QLabel(transformFrame);
        translateLabel->setObjectName(QStringLiteral("translateLabel"));

        gridLayout_3->addWidget(translateLabel, 1, 0, 1, 1);

        xValue_scale = new QDoubleSpinBox(transformFrame);
        xValue_scale->setObjectName(QStringLiteral("xValue_scale"));
        xValue_scale->setMinimumSize(QSize(0, 0));
        xValue_scale->setWrapping(true);
        xValue_scale->setFrame(true);
        xValue_scale->setButtonSymbols(QAbstractSpinBox::NoButtons);
        xValue_scale->setDecimals(4);
        xValue_scale->setMinimum(-10000);
        xValue_scale->setMaximum(10000);

        gridLayout_3->addWidget(xValue_scale, 5, 1, 1, 1);

        xValue_rot = new QDoubleSpinBox(transformFrame);
        xValue_rot->setObjectName(QStringLiteral("xValue_rot"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(xValue_rot->sizePolicy().hasHeightForWidth());
        xValue_rot->setSizePolicy(sizePolicy3);
        xValue_rot->setWrapping(true);
        xValue_rot->setFrame(true);
        xValue_rot->setButtonSymbols(QAbstractSpinBox::NoButtons);
        xValue_rot->setDecimals(4);
        xValue_rot->setMinimum(-10000);
        xValue_rot->setMaximum(10000);

        gridLayout_3->addWidget(xValue_rot, 4, 1, 1, 1);

        yValue_scale = new QDoubleSpinBox(transformFrame);
        yValue_scale->setObjectName(QStringLiteral("yValue_scale"));
        yValue_scale->setMinimumSize(QSize(0, 0));
        yValue_scale->setWrapping(true);
        yValue_scale->setButtonSymbols(QAbstractSpinBox::NoButtons);
        yValue_scale->setDecimals(4);
        yValue_scale->setMinimum(-10000);
        yValue_scale->setMaximum(10000);

        gridLayout_3->addWidget(yValue_scale, 5, 2, 1, 1);

        yValue_rot = new QDoubleSpinBox(transformFrame);
        yValue_rot->setObjectName(QStringLiteral("yValue_rot"));
        yValue_rot->setWrapping(true);
        yValue_rot->setButtonSymbols(QAbstractSpinBox::NoButtons);
        yValue_rot->setDecimals(4);
        yValue_rot->setMinimum(-10000);
        yValue_rot->setMaximum(10000);

        gridLayout_3->addWidget(yValue_rot, 4, 2, 1, 1);

        zValue_scale = new QDoubleSpinBox(transformFrame);
        zValue_scale->setObjectName(QStringLiteral("zValue_scale"));
        zValue_scale->setMinimumSize(QSize(0, 0));
        zValue_scale->setWrapping(true);
        zValue_scale->setButtonSymbols(QAbstractSpinBox::NoButtons);
        zValue_scale->setDecimals(4);
        zValue_scale->setMinimum(-10000);
        zValue_scale->setMaximum(10000);

        gridLayout_3->addWidget(zValue_scale, 5, 3, 1, 1);

        zValue_rot = new QDoubleSpinBox(transformFrame);
        zValue_rot->setObjectName(QStringLiteral("zValue_rot"));
        zValue_rot->setWrapping(true);
        zValue_rot->setButtonSymbols(QAbstractSpinBox::NoButtons);
        zValue_rot->setDecimals(4);
        zValue_rot->setMinimum(-10000);
        zValue_rot->setMaximum(10000);

        gridLayout_3->addWidget(zValue_rot, 4, 3, 1, 1);


        verticalLayout_3->addWidget(transformFrame);

        variousOptionsframe = new QFrame(Values);
        variousOptionsframe->setObjectName(QStringLiteral("variousOptionsframe"));
        variousOptionsframe->setFrameShape(QFrame::StyledPanel);
        variousOptionsframe->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(variousOptionsframe);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        UIDTEXT = new QLabel(variousOptionsframe);
        UIDTEXT->setObjectName(QStringLiteral("UIDTEXT"));

        formLayout->setWidget(0, QFormLayout::LabelRole, UIDTEXT);

        uniqueIDLabel = new QLabel(variousOptionsframe);
        uniqueIDLabel->setObjectName(QStringLiteral("uniqueIDLabel"));

        formLayout->setWidget(0, QFormLayout::FieldRole, uniqueIDLabel);

        ANIMTEXT = new QLabel(variousOptionsframe);
        ANIMTEXT->setObjectName(QStringLiteral("ANIMTEXT"));

        formLayout->setWidget(4, QFormLayout::LabelRole, ANIMTEXT);

        comboBox = new QComboBox(variousOptionsframe);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setEnabled(false);

        formLayout->setWidget(4, QFormLayout::FieldRole, comboBox);

        checkBox = new QCheckBox(variousOptionsframe);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setLayoutDirection(Qt::LeftToRight);

        formLayout->setWidget(3, QFormLayout::FieldRole, checkBox);

        label = new QLabel(variousOptionsframe);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label);


        verticalLayout_3->addWidget(variousOptionsframe);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        gridLayout->addWidget(Values, 1, 3, 3, 1);

        SSP_EditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SSP_EditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1171, 21));
        menuEditor = new QMenu(menuBar);
        menuEditor->setObjectName(QStringLiteral("menuEditor"));
        SSP_EditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SSP_EditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(false);
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

        tabWidget->setCurrentIndex(1);


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
        pushButton->setText(QApplication::translate("SSP_EditorClass", "Reload", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("SSP_EditorClass", "Files", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SSP_EditorClass", "Asset Browser", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SSP_EditorClass", "Scene info", Q_NULLPTR));
        Values->setTitle(QApplication::translate("SSP_EditorClass", "Attributes", Q_NULLPTR));
        nameLabel->setText(QApplication::translate("SSP_EditorClass", "Name", Q_NULLPTR));
        rotationlabel->setText(QApplication::translate("SSP_EditorClass", "Rotation", Q_NULLPTR));
        scalelabel->setText(QApplication::translate("SSP_EditorClass", "Scale", Q_NULLPTR));
        translateLabel->setText(QApplication::translate("SSP_EditorClass", "Translate", Q_NULLPTR));
        UIDTEXT->setText(QApplication::translate("SSP_EditorClass", "Unique ID:", Q_NULLPTR));
        uniqueIDLabel->setText(QApplication::translate("SSP_EditorClass", "0", Q_NULLPTR));
        ANIMTEXT->setText(QApplication::translate("SSP_EditorClass", "Animation:", Q_NULLPTR));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "None", Q_NULLPTR)
        );
        checkBox->setText(QString());
        label->setText(QApplication::translate("SSP_EditorClass", "Is Static:", Q_NULLPTR));
        menuEditor->setTitle(QApplication::translate("SSP_EditorClass", "Editor", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SSP_EditorClass: public Ui_SSP_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSP_EDITOR_H
