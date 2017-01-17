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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
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
    QGroupBox *Values;
    QVBoxLayout *verticalLayout_3;
    QFrame *nameQFrame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *nameLabel;
    QLineEdit *lineEdit;
    QFrame *transformFrame;
    QGridLayout *gridLayout_3;
    QLabel *rotationlabel;
    QLabel *scalelabel;
    QDoubleSpinBox *xValue_translate;
    QDoubleSpinBox *yValue_translate;
    QDoubleSpinBox *zValue_translate;
    QLabel *translateLabel;
    QDoubleSpinBox *yValue_scale;
    QDoubleSpinBox *xValue_rot;
    QDoubleSpinBox *xValue_scale;
    QDoubleSpinBox *zValue_scale;
    QDoubleSpinBox *yValue_rot;
    QDoubleSpinBox *zValue_rot;
    QFrame *variousOptionsframe;
    QFormLayout *formLayout;
    QLabel *UIDTEXT;
    QLabel *uniqueIDLabel;
    QLabel *ISSTATICTEXT;
    QCheckBox *isStaticCheck;
    QComboBox *animationBox;
    QLabel *ANIMTEXT;
    QFrame *CustomBehaviourFrame;
    QFormLayout *formLayout_3;
    QLabel *BEHAVIOURTEXT;
    QComboBox *BehaviourDropDown;
    QStackedWidget *BehaviourStackWidget;
    QWidget *Behaviour_None;
    QFormLayout *formLayout_6;
    QWidget *Behaviour_Trigger;
    QFormLayout *formLayout_2;
    QLabel *TRIGGERTEXT;
    QWidget *Behaviour_Door;
    QFormLayout *formLayout_4;
    QLabel *DOORTEXT;
    QWidget *Behaviour_Path;
    QGridLayout *gridLayout_2;
    QLabel *TIMETEXT;
    QLabel *PATTERNTEXT;
    QLabel *SPEEDTEXT;
    QLabel *WAYPOINTTEXT;
    QPushButton *DeleteButton;
    QPushButton *AddButton;
    QPushButton *UpButton;
    QPushButton *DownButton;
    QListWidget *WaypointListWidget;
    QComboBox *PatternDropDown;
    QDoubleSpinBox *TimeValue;
    QDoubleSpinBox *SpeedValue;
    QSpacerItem *verticalSpacer;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QTreeWidget *assetTree;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *scene_tree;
    QPushButton *pushButton;
    QLabel *label_2;
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
"#variousOptionsframe{\n"
"background : rgb(74, 74, 74);\n"
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

        gridLayout->addWidget(RenderWidget, 2, 2, 1, 1);

        Values = new QGroupBox(centralWidget);
        Values->setObjectName(QStringLiteral("Values"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Values->sizePolicy().hasHeightForWidth());
        Values->setSizePolicy(sizePolicy1);
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

        scalelabel = new QLabel(transformFrame);
        scalelabel->setObjectName(QStringLiteral("scalelabel"));

        gridLayout_3->addWidget(scalelabel, 5, 0, 1, 1);

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

        yValue_scale = new QDoubleSpinBox(transformFrame);
        yValue_scale->setObjectName(QStringLiteral("yValue_scale"));
        yValue_scale->setMinimumSize(QSize(0, 0));
        yValue_scale->setWrapping(true);
        yValue_scale->setButtonSymbols(QAbstractSpinBox::NoButtons);
        yValue_scale->setDecimals(4);
        yValue_scale->setMinimum(-10000);
        yValue_scale->setMaximum(10000);

        gridLayout_3->addWidget(yValue_scale, 5, 2, 1, 1);

        xValue_rot = new QDoubleSpinBox(transformFrame);
        xValue_rot->setObjectName(QStringLiteral("xValue_rot"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(xValue_rot->sizePolicy().hasHeightForWidth());
        xValue_rot->setSizePolicy(sizePolicy2);
        xValue_rot->setWrapping(true);
        xValue_rot->setFrame(true);
        xValue_rot->setButtonSymbols(QAbstractSpinBox::NoButtons);
        xValue_rot->setDecimals(4);
        xValue_rot->setMinimum(-10000);
        xValue_rot->setMaximum(10000);

        gridLayout_3->addWidget(xValue_rot, 4, 1, 1, 1);

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

        zValue_scale = new QDoubleSpinBox(transformFrame);
        zValue_scale->setObjectName(QStringLiteral("zValue_scale"));
        zValue_scale->setMinimumSize(QSize(0, 0));
        zValue_scale->setWrapping(true);
        zValue_scale->setButtonSymbols(QAbstractSpinBox::NoButtons);
        zValue_scale->setDecimals(4);
        zValue_scale->setMinimum(-10000);
        zValue_scale->setMaximum(10000);

        gridLayout_3->addWidget(zValue_scale, 5, 3, 1, 1);

        yValue_rot = new QDoubleSpinBox(transformFrame);
        yValue_rot->setObjectName(QStringLiteral("yValue_rot"));
        yValue_rot->setWrapping(true);
        yValue_rot->setButtonSymbols(QAbstractSpinBox::NoButtons);
        yValue_rot->setDecimals(4);
        yValue_rot->setMinimum(-10000);
        yValue_rot->setMaximum(10000);

        gridLayout_3->addWidget(yValue_rot, 4, 2, 1, 1);

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
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(variousOptionsframe->sizePolicy().hasHeightForWidth());
        variousOptionsframe->setSizePolicy(sizePolicy3);
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

        ISSTATICTEXT = new QLabel(variousOptionsframe);
        ISSTATICTEXT->setObjectName(QStringLiteral("ISSTATICTEXT"));

        formLayout->setWidget(3, QFormLayout::LabelRole, ISSTATICTEXT);

        isStaticCheck = new QCheckBox(variousOptionsframe);
        isStaticCheck->setObjectName(QStringLiteral("isStaticCheck"));
        isStaticCheck->setLayoutDirection(Qt::LeftToRight);
        isStaticCheck->setChecked(true);

        formLayout->setWidget(3, QFormLayout::FieldRole, isStaticCheck);

        animationBox = new QComboBox(variousOptionsframe);
        animationBox->setObjectName(QStringLiteral("animationBox"));
        animationBox->setEnabled(false);

        formLayout->setWidget(4, QFormLayout::FieldRole, animationBox);

        ANIMTEXT = new QLabel(variousOptionsframe);
        ANIMTEXT->setObjectName(QStringLiteral("ANIMTEXT"));

        formLayout->setWidget(4, QFormLayout::LabelRole, ANIMTEXT);


        verticalLayout_3->addWidget(variousOptionsframe);

        CustomBehaviourFrame = new QFrame(Values);
        CustomBehaviourFrame->setObjectName(QStringLiteral("CustomBehaviourFrame"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(74, 74, 74, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        CustomBehaviourFrame->setPalette(palette);
        CustomBehaviourFrame->setStyleSheet(QStringLiteral("background-color: rgb(74, 74, 74);"));
        CustomBehaviourFrame->setFrameShape(QFrame::StyledPanel);
        CustomBehaviourFrame->setFrameShadow(QFrame::Raised);
        formLayout_3 = new QFormLayout(CustomBehaviourFrame);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        BEHAVIOURTEXT = new QLabel(CustomBehaviourFrame);
        BEHAVIOURTEXT->setObjectName(QStringLiteral("BEHAVIOURTEXT"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, BEHAVIOURTEXT);

        BehaviourDropDown = new QComboBox(CustomBehaviourFrame);
        BehaviourDropDown->setObjectName(QStringLiteral("BehaviourDropDown"));
        BehaviourDropDown->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, BehaviourDropDown);

        BehaviourStackWidget = new QStackedWidget(CustomBehaviourFrame);
        BehaviourStackWidget->setObjectName(QStringLiteral("BehaviourStackWidget"));
        BehaviourStackWidget->setEnabled(true);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(111, 111, 111, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(92, 92, 92, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(37, 37, 37, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(49, 49, 49, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        QBrush brush6(QColor(0, 0, 0, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
        BehaviourStackWidget->setPalette(palette1);
        BehaviourStackWidget->setStyleSheet(QStringLiteral("background-color: rgb(74, 74, 74);"));
        Behaviour_None = new QWidget();
        Behaviour_None->setObjectName(QStringLiteral("Behaviour_None"));
        formLayout_6 = new QFormLayout(Behaviour_None);
        formLayout_6->setSpacing(6);
        formLayout_6->setContentsMargins(11, 11, 11, 11);
        formLayout_6->setObjectName(QStringLiteral("formLayout_6"));
        BehaviourStackWidget->addWidget(Behaviour_None);
        Behaviour_Trigger = new QWidget();
        Behaviour_Trigger->setObjectName(QStringLiteral("Behaviour_Trigger"));
        formLayout_2 = new QFormLayout(Behaviour_Trigger);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        TRIGGERTEXT = new QLabel(Behaviour_Trigger);
        TRIGGERTEXT->setObjectName(QStringLiteral("TRIGGERTEXT"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, TRIGGERTEXT);

        BehaviourStackWidget->addWidget(Behaviour_Trigger);
        Behaviour_Door = new QWidget();
        Behaviour_Door->setObjectName(QStringLiteral("Behaviour_Door"));
        formLayout_4 = new QFormLayout(Behaviour_Door);
        formLayout_4->setSpacing(6);
        formLayout_4->setContentsMargins(11, 11, 11, 11);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        DOORTEXT = new QLabel(Behaviour_Door);
        DOORTEXT->setObjectName(QStringLiteral("DOORTEXT"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, DOORTEXT);

        BehaviourStackWidget->addWidget(Behaviour_Door);
        Behaviour_Path = new QWidget();
        Behaviour_Path->setObjectName(QStringLiteral("Behaviour_Path"));
        gridLayout_2 = new QGridLayout(Behaviour_Path);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        TIMETEXT = new QLabel(Behaviour_Path);
        TIMETEXT->setObjectName(QStringLiteral("TIMETEXT"));

        gridLayout_2->addWidget(TIMETEXT, 1, 0, 1, 1);

        PATTERNTEXT = new QLabel(Behaviour_Path);
        PATTERNTEXT->setObjectName(QStringLiteral("PATTERNTEXT"));

        gridLayout_2->addWidget(PATTERNTEXT, 2, 0, 1, 1);

        SPEEDTEXT = new QLabel(Behaviour_Path);
        SPEEDTEXT->setObjectName(QStringLiteral("SPEEDTEXT"));

        gridLayout_2->addWidget(SPEEDTEXT, 0, 0, 1, 1);

        WAYPOINTTEXT = new QLabel(Behaviour_Path);
        WAYPOINTTEXT->setObjectName(QStringLiteral("WAYPOINTTEXT"));

        gridLayout_2->addWidget(WAYPOINTTEXT, 4, 0, 1, 1);

        DeleteButton = new QPushButton(Behaviour_Path);
        DeleteButton->setObjectName(QStringLiteral("DeleteButton"));
        DeleteButton->setStyleSheet(QStringLiteral("background-color: rgb(129, 129, 129);"));

        gridLayout_2->addWidget(DeleteButton, 6, 1, 1, 1);

        AddButton = new QPushButton(Behaviour_Path);
        AddButton->setObjectName(QStringLiteral("AddButton"));
        AddButton->setStyleSheet(QStringLiteral("background-color: rgb(129, 129, 129);"));

        gridLayout_2->addWidget(AddButton, 6, 0, 1, 1);

        UpButton = new QPushButton(Behaviour_Path);
        UpButton->setObjectName(QStringLiteral("UpButton"));
        UpButton->setStyleSheet(QStringLiteral("background-color: rgb(129, 129, 129);"));

        gridLayout_2->addWidget(UpButton, 6, 2, 1, 1);

        DownButton = new QPushButton(Behaviour_Path);
        DownButton->setObjectName(QStringLiteral("DownButton"));
        DownButton->setStyleSheet(QStringLiteral("background-color: rgb(129, 129, 129);"));

        gridLayout_2->addWidget(DownButton, 6, 3, 1, 1);

        WaypointListWidget = new QListWidget(Behaviour_Path);
        WaypointListWidget->setObjectName(QStringLiteral("WaypointListWidget"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush8(QColor(48, 48, 48, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        WaypointListWidget->setPalette(palette2);
        WaypointListWidget->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));

        gridLayout_2->addWidget(WaypointListWidget, 5, 0, 1, 4);

        PatternDropDown = new QComboBox(Behaviour_Path);
        PatternDropDown->setObjectName(QStringLiteral("PatternDropDown"));
        PatternDropDown->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));

        gridLayout_2->addWidget(PatternDropDown, 2, 1, 1, 3);

        TimeValue = new QDoubleSpinBox(Behaviour_Path);
        TimeValue->setObjectName(QStringLiteral("TimeValue"));
        TimeValue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        TimeValue->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_2->addWidget(TimeValue, 1, 3, 1, 1);

        SpeedValue = new QDoubleSpinBox(Behaviour_Path);
        SpeedValue->setObjectName(QStringLiteral("SpeedValue"));
        SpeedValue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        SpeedValue->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_2->addWidget(SpeedValue, 0, 3, 1, 1);

        BehaviourStackWidget->addWidget(Behaviour_Path);

        formLayout_3->setWidget(4, QFormLayout::FieldRole, BehaviourStackWidget);


        verticalLayout_3->addWidget(CustomBehaviourFrame);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        gridLayout->addWidget(Values, 2, 3, 3, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy4);
        tabWidget->setMinimumSize(QSize(200, 0));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        assetTree = new QTreeWidget(tab);
        assetTree->setObjectName(QStringLiteral("assetTree"));
        assetTree->setLayoutDirection(Qt::LeftToRight);
        assetTree->setTextElideMode(Qt::ElideNone);
        assetTree->setSortingEnabled(true);
        assetTree->setHeaderHidden(true);
        assetTree->header()->setCascadingSectionResizes(false);

        verticalLayout->addWidget(assetTree);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        scene_tree = new QTreeWidget(tab_2);
        scene_tree->setObjectName(QStringLiteral("scene_tree"));
        scene_tree->setLayoutDirection(Qt::LeftToRight);
        scene_tree->setTextElideMode(Qt::ElideNone);
        scene_tree->setSortingEnabled(true);
        scene_tree->setHeaderHidden(true);
        scene_tree->header()->setCascadingSectionResizes(false);

        verticalLayout_2->addWidget(scene_tree);

        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 2, 1, 1, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 4, 1, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

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
        QObject::connect(BehaviourDropDown, SIGNAL(currentIndexChanged(int)), BehaviourStackWidget, SLOT(setCurrentIndex(int)));

        BehaviourStackWidget->setCurrentIndex(0);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SSP_EditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *SSP_EditorClass)
    {
        SSP_EditorClass->setWindowTitle(QApplication::translate("SSP_EditorClass", "SSP_Editor", 0));
        actionNew_scene->setText(QApplication::translate("SSP_EditorClass", "New scene", 0));
        actionLoad_scene->setText(QApplication::translate("SSP_EditorClass", "Load scene", 0));
        actionSave_scene->setText(QApplication::translate("SSP_EditorClass", "Save scene", 0));
        actionBuild_BPF->setText(QApplication::translate("SSP_EditorClass", "Build .BPF", 0));
        Values->setTitle(QApplication::translate("SSP_EditorClass", "Attributes", 0));
        nameLabel->setText(QApplication::translate("SSP_EditorClass", "Name", 0));
        rotationlabel->setText(QApplication::translate("SSP_EditorClass", "Rotation", 0));
        scalelabel->setText(QApplication::translate("SSP_EditorClass", "Scale", 0));
        translateLabel->setText(QApplication::translate("SSP_EditorClass", "Translate", 0));
        UIDTEXT->setText(QApplication::translate("SSP_EditorClass", "Unique ID:", 0));
        uniqueIDLabel->setText(QApplication::translate("SSP_EditorClass", "0", 0));
        ISSTATICTEXT->setText(QApplication::translate("SSP_EditorClass", "Is Static:", 0));
        isStaticCheck->setText(QString());
        animationBox->clear();
        animationBox->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "None", 0)
        );
        ANIMTEXT->setText(QApplication::translate("SSP_EditorClass", "Animation:", 0));
        BEHAVIOURTEXT->setText(QApplication::translate("SSP_EditorClass", "Behaviour Type", 0));
        BehaviourDropDown->clear();
        BehaviourDropDown->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "None", 0)
         << QApplication::translate("SSP_EditorClass", "Trigger", 0)
         << QApplication::translate("SSP_EditorClass", "Door", 0)
         << QApplication::translate("SSP_EditorClass", "Path", 0)
        );
        TRIGGERTEXT->setText(QApplication::translate("SSP_EditorClass", "Trigger", 0));
        DOORTEXT->setText(QApplication::translate("SSP_EditorClass", "Door", 0));
        TIMETEXT->setText(QApplication::translate("SSP_EditorClass", "Time (ms)", 0));
        PATTERNTEXT->setText(QApplication::translate("SSP_EditorClass", "Pattern", 0));
        SPEEDTEXT->setText(QApplication::translate("SSP_EditorClass", "Speed", 0));
        WAYPOINTTEXT->setText(QApplication::translate("SSP_EditorClass", "Waypoints", 0));
        DeleteButton->setText(QApplication::translate("SSP_EditorClass", "DEL", 0));
        AddButton->setText(QApplication::translate("SSP_EditorClass", "ADD", 0));
        UpButton->setText(QApplication::translate("SSP_EditorClass", "UP", 0));
        DownButton->setText(QApplication::translate("SSP_EditorClass", "DOWN", 0));
        PatternDropDown->clear();
        PatternDropDown->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "Circular", 0)
         << QApplication::translate("SSP_EditorClass", "One Way", 0)
         << QApplication::translate("SSP_EditorClass", "Round Trip", 0)
        );
        QTreeWidgetItem *___qtreewidgetitem = assetTree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("SSP_EditorClass", "Files", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SSP_EditorClass", "Asset Browser", 0));
        QTreeWidgetItem *___qtreewidgetitem1 = scene_tree->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("SSP_EditorClass", "Files", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SSP_EditorClass", "Scene info", 0));
        pushButton->setText(QApplication::translate("SSP_EditorClass", "Reload", 0));
        label_2->setText(QApplication::translate("SSP_EditorClass", "Preview", 0));
        menuEditor->setTitle(QApplication::translate("SSP_EditorClass", "Editor", 0));
    } // retranslateUi

};

namespace Ui {
    class SSP_EditorClass: public Ui_SSP_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSP_EDITOR_H
