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
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
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
    QGroupBox *Values;
    QVBoxLayout *verticalLayout_3;
    QFrame *nameQFrame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *nameLabel;
    QLineEdit *lineEdit;
    QFrame *transformFrame;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *xValue_translate;
    QDoubleSpinBox *xValue_scale;
    QDoubleSpinBox *zValue_scale;
    QDoubleSpinBox *yValue_rot;
    QLabel *scalelabel;
    QLabel *rotationlabel;
    QDoubleSpinBox *zValue_rot;
    QDoubleSpinBox *yValue_translate;
    QDoubleSpinBox *zValue_translate;
    QLabel *translateLabel;
    QDoubleSpinBox *yValue_scale;
    QDoubleSpinBox *xValue_rot;
    QFrame *variousOptionsframe;
    QFormLayout *formLayout;
    QLabel *UIDTEXT;
    QLabel *uniqueIDLabel;
    QLabel *ISSTATICTEXT;
    QCheckBox *isStaticCheck;
    QComboBox *animationBox;
    QLabel *ANIMTEXT;
    QTabWidget *CustomBehaviourTabWidget;
    QWidget *Behaviour;
    QFormLayout *formLayout_3;
    QLabel *BEHAVIOURTEXT;
    QComboBox *BehaviourDropDown;
    QStackedWidget *BehaviourStackWidget;
    QWidget *Behaviour_None;
    QFormLayout *formLayout_6;
    QWidget *Behaviour_Button;
    QGridLayout *gridLayout_5;
    QSpacerItem *verticalSpacer_4;
    QLabel *tag_label;
    QLabel *tag_label_2;
    QSpinBox *button_tagBox;
    QDoubleSpinBox *button_dist_box;
    QLabel *tag_label_3;
    QDoubleSpinBox *button_timer_box;
    QWidget *Behaviour_Door;
    QGridLayout *gridLayout_4;
    QDoubleSpinBox *rotateTimeBox;
    QLabel *label_5;
    QSpacerItem *verticalSpacer_2;
    QWidget *Behaviour_Path;
    QGridLayout *gridLayout_2;
    QLabel *TIMETEXT;
    QLabel *PATTERNTEXT;
    QLabel *WAYPOINTTEXT;
    QComboBox *PatternDropDown;
    QLabel *SPEEDTEXT;
    QPushButton *AddButton;
    QDoubleSpinBox *SpeedValue;
    QDoubleSpinBox *TimeValue;
    QListWidget *WaypointListWidget;
    QPushButton *DeleteButton;
    QLabel *TRIGGERTAGTEXTPATH;
    QSpinBox *TriggerPathValue;
    QCheckBox *TriggerPathCheckBox;
    QWidget *Lever;
    QGridLayout *gridLayout_7;
    QDoubleSpinBox *lever_interactionDist;
    QLabel *label_lever_InteractionDist;
    QSpacerItem *verticalSpacer_5;
    QWidget *Wheel;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_wheelinteraction;
    QDoubleSpinBox *wheel_interactionDistBox;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_minRot;
    QDoubleSpinBox *wheel_minRotationBox;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_maxRot;
    QDoubleSpinBox *wheel_maxRotationBox;
    QFrame *frame_4;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_rotTime;
    QDoubleSpinBox *wheel_rotationTimeBox;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_timeTilReset;
    QDoubleSpinBox *wheel_timeTilResetBox;
    QFrame *frame_6;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_resetTime;
    QDoubleSpinBox *wheel_resetTimeBox;
    QSpacerItem *verticalSpacer_3;
    QWidget *Triggers;
    QFormLayout *formLayout_2;
    QComboBox *availableTriggers;
    QLabel *label;
    QFrame *addDel_Frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *AddTriggerButton;
    QPushButton *DeleteTriggerButton;
    QTableWidget *TriggerTableWidget;
    QComboBox *EventSignalBox;
    QLabel *label_3;
    QWidget *CheckPoint;
    QGridLayout *gridLayout_6;
    QLabel *checkpointTXT;
    QSpinBox *CheckPointValue;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *CheckPointADD;
    QLabel *label_4;
    QWidget *Lights;
    QGridLayout *gridLayout_8;
    QLabel *label_6;
    QComboBox *LightDropDown;
    QStackedWidget *LightWindows;
    QWidget *None;
    QWidget *Point;
    QGridLayout *gridLayout_9;
    QLabel *label_12;
    QSpinBox *R_Colorvalue;
    QLabel *label_8;
    QSpacerItem *verticalSpacer_6;
    QLabel *label_9;
    QLabel *label_10;
    QSpinBox *G_Colorvalue;
    QDoubleSpinBox *RadiusValue;
    QSlider *G_Slider;
    QSlider *R_Slider;
    QSlider *B_Slider;
    QSpinBox *B_Colorvalue;
    QLabel *label_14;
    QLabel *label_13;
    QLabel *label_7;
    QLabel *label_11;
    QDoubleSpinBox *QuadValue;
    QDoubleSpinBox *LinearValue;
    QDoubleSpinBox *ConstantValue;
    QSpacerItem *horizontalSpacer;
    QPushButton *ADD_Light_Button;
    QWidget *RenderWidget;
    QLabel *label_2;
    QPushButton *pushButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QTreeWidget *assetTree;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *scene_tree;
    QMenuBar *menuBar;
    QMenu *menuEditor;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SSP_EditorClass)
    {
        if (SSP_EditorClass->objectName().isEmpty())
            SSP_EditorClass->setObjectName(QStringLiteral("SSP_EditorClass"));
        SSP_EditorClass->resize(1171, 736);
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
"QHeaderView::section\n"
"{\n"
"spacing: 10px;\n"
"background-color:lightblue;\n"
"color: black;\n"
"margin: 1px;\n"
"border:solid;\n"
"text-align: right;\n"
"font-family: arial;\n"
"font-size:12px;\n"
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
""
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
"QTreeWidget::Item:has-children{\n"
"spacing: 3px;\n"
"border-radius: 4px;\n"
"border: 1px solid;\n"
"color: white;\n"
"background : rgb(88, 88, 88);\n"
"}\n"
"QTreeWidget::Item"
                        ":!has-children{ \n"
"spacing: 3px;\n"
"border-radius: 1px;\n"
"border: 1px solid;\n"
"background : rgb(111, 111, 111);\n"
"}\n"
"QTreeView::item:hover {\n"
"    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1);\n"
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
"  background: qlineargradient(x1: 0, y1:"
                        " 0, x2: 0, y2: 1, stop: 0 rgb(221, 263, 239), stop: 1 #cbdaf1);\n"
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
        Values = new QGroupBox(centralWidget);
        Values->setObjectName(QStringLiteral("Values"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Values->sizePolicy().hasHeightForWidth());
        Values->setSizePolicy(sizePolicy);
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
        xValue_translate = new QDoubleSpinBox(transformFrame);
        xValue_translate->setObjectName(QStringLiteral("xValue_translate"));
        xValue_translate->setWrapping(true);
        xValue_translate->setFrame(true);
        xValue_translate->setButtonSymbols(QAbstractSpinBox::NoButtons);
        xValue_translate->setDecimals(2);
        xValue_translate->setMinimum(-10000);
        xValue_translate->setMaximum(10000);

        gridLayout_3->addWidget(xValue_translate, 1, 1, 1, 1);

        xValue_scale = new QDoubleSpinBox(transformFrame);
        xValue_scale->setObjectName(QStringLiteral("xValue_scale"));
        xValue_scale->setMinimumSize(QSize(0, 0));
        xValue_scale->setWrapping(true);
        xValue_scale->setFrame(true);
        xValue_scale->setButtonSymbols(QAbstractSpinBox::NoButtons);
        xValue_scale->setDecimals(2);
        xValue_scale->setMinimum(-10000);
        xValue_scale->setMaximum(10000);

        gridLayout_3->addWidget(xValue_scale, 5, 1, 1, 1);

        zValue_scale = new QDoubleSpinBox(transformFrame);
        zValue_scale->setObjectName(QStringLiteral("zValue_scale"));
        zValue_scale->setMinimumSize(QSize(0, 0));
        zValue_scale->setWrapping(true);
        zValue_scale->setButtonSymbols(QAbstractSpinBox::NoButtons);
        zValue_scale->setDecimals(2);
        zValue_scale->setMinimum(-10000);
        zValue_scale->setMaximum(10000);

        gridLayout_3->addWidget(zValue_scale, 5, 3, 1, 1);

        yValue_rot = new QDoubleSpinBox(transformFrame);
        yValue_rot->setObjectName(QStringLiteral("yValue_rot"));
        yValue_rot->setWrapping(true);
        yValue_rot->setButtonSymbols(QAbstractSpinBox::NoButtons);
        yValue_rot->setDecimals(2);
        yValue_rot->setMinimum(-10000);
        yValue_rot->setMaximum(10000);

        gridLayout_3->addWidget(yValue_rot, 4, 2, 1, 1);

        scalelabel = new QLabel(transformFrame);
        scalelabel->setObjectName(QStringLiteral("scalelabel"));

        gridLayout_3->addWidget(scalelabel, 5, 0, 1, 1);

        rotationlabel = new QLabel(transformFrame);
        rotationlabel->setObjectName(QStringLiteral("rotationlabel"));

        gridLayout_3->addWidget(rotationlabel, 4, 0, 1, 1);

        zValue_rot = new QDoubleSpinBox(transformFrame);
        zValue_rot->setObjectName(QStringLiteral("zValue_rot"));
        zValue_rot->setWrapping(true);
        zValue_rot->setButtonSymbols(QAbstractSpinBox::NoButtons);
        zValue_rot->setDecimals(2);
        zValue_rot->setMinimum(-10000);
        zValue_rot->setMaximum(10000);

        gridLayout_3->addWidget(zValue_rot, 4, 3, 1, 1);

        yValue_translate = new QDoubleSpinBox(transformFrame);
        yValue_translate->setObjectName(QStringLiteral("yValue_translate"));
        yValue_translate->setWrapping(true);
        yValue_translate->setButtonSymbols(QAbstractSpinBox::NoButtons);
        yValue_translate->setDecimals(2);
        yValue_translate->setMinimum(-10000);
        yValue_translate->setMaximum(10000);

        gridLayout_3->addWidget(yValue_translate, 1, 2, 1, 1);

        zValue_translate = new QDoubleSpinBox(transformFrame);
        zValue_translate->setObjectName(QStringLiteral("zValue_translate"));
        zValue_translate->setWrapping(true);
        zValue_translate->setButtonSymbols(QAbstractSpinBox::NoButtons);
        zValue_translate->setDecimals(2);
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
        yValue_scale->setDecimals(2);
        yValue_scale->setMinimum(-10000);
        yValue_scale->setMaximum(10000);

        gridLayout_3->addWidget(yValue_scale, 5, 2, 1, 1);

        xValue_rot = new QDoubleSpinBox(transformFrame);
        xValue_rot->setObjectName(QStringLiteral("xValue_rot"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(xValue_rot->sizePolicy().hasHeightForWidth());
        xValue_rot->setSizePolicy(sizePolicy1);
        xValue_rot->setWrapping(true);
        xValue_rot->setFrame(true);
        xValue_rot->setButtonSymbols(QAbstractSpinBox::NoButtons);
        xValue_rot->setDecimals(2);
        xValue_rot->setMinimum(-10000);
        xValue_rot->setMaximum(10000);

        gridLayout_3->addWidget(xValue_rot, 4, 1, 1, 1);


        verticalLayout_3->addWidget(transformFrame);

        variousOptionsframe = new QFrame(Values);
        variousOptionsframe->setObjectName(QStringLiteral("variousOptionsframe"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(variousOptionsframe->sizePolicy().hasHeightForWidth());
        variousOptionsframe->setSizePolicy(sizePolicy2);
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

        formLayout->setWidget(1, QFormLayout::LabelRole, ISSTATICTEXT);

        isStaticCheck = new QCheckBox(variousOptionsframe);
        isStaticCheck->setObjectName(QStringLiteral("isStaticCheck"));
        isStaticCheck->setLayoutDirection(Qt::LeftToRight);
        isStaticCheck->setChecked(true);

        formLayout->setWidget(1, QFormLayout::FieldRole, isStaticCheck);

        animationBox = new QComboBox(variousOptionsframe);
        animationBox->setObjectName(QStringLiteral("animationBox"));
        animationBox->setEnabled(false);

        formLayout->setWidget(2, QFormLayout::FieldRole, animationBox);

        ANIMTEXT = new QLabel(variousOptionsframe);
        ANIMTEXT->setObjectName(QStringLiteral("ANIMTEXT"));

        formLayout->setWidget(2, QFormLayout::LabelRole, ANIMTEXT);


        verticalLayout_3->addWidget(variousOptionsframe);

        CustomBehaviourTabWidget = new QTabWidget(Values);
        CustomBehaviourTabWidget->setObjectName(QStringLiteral("CustomBehaviourTabWidget"));
        CustomBehaviourTabWidget->setEnabled(true);
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
        CustomBehaviourTabWidget->setPalette(palette);
        CustomBehaviourTabWidget->setStyleSheet(QStringLiteral("background-color: rgb(74, 74, 74);"));
        Behaviour = new QWidget();
        Behaviour->setObjectName(QStringLiteral("Behaviour"));
        formLayout_3 = new QFormLayout(Behaviour);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        BEHAVIOURTEXT = new QLabel(Behaviour);
        BEHAVIOURTEXT->setObjectName(QStringLiteral("BEHAVIOURTEXT"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, BEHAVIOURTEXT);

        BehaviourDropDown = new QComboBox(Behaviour);
        BehaviourDropDown->setObjectName(QStringLiteral("BehaviourDropDown"));
        BehaviourDropDown->setEnabled(false);
        BehaviourDropDown->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, BehaviourDropDown);

        BehaviourStackWidget = new QStackedWidget(Behaviour);
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
        Behaviour_Button = new QWidget();
        Behaviour_Button->setObjectName(QStringLiteral("Behaviour_Button"));
        gridLayout_5 = new QGridLayout(Behaviour_Button);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_4, 7, 2, 1, 1);

        tag_label = new QLabel(Behaviour_Button);
        tag_label->setObjectName(QStringLiteral("tag_label"));

        gridLayout_5->addWidget(tag_label, 1, 0, 1, 2);

        tag_label_2 = new QLabel(Behaviour_Button);
        tag_label_2->setObjectName(QStringLiteral("tag_label_2"));

        gridLayout_5->addWidget(tag_label_2, 2, 0, 2, 2);

        button_tagBox = new QSpinBox(Behaviour_Button);
        button_tagBox->setObjectName(QStringLiteral("button_tagBox"));
        button_tagBox->setStyleSheet(QStringLiteral(""));
        button_tagBox->setReadOnly(true);
        button_tagBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_5->addWidget(button_tagBox, 1, 2, 1, 1);

        button_dist_box = new QDoubleSpinBox(Behaviour_Button);
        button_dist_box->setObjectName(QStringLiteral("button_dist_box"));
        button_dist_box->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        button_dist_box->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_5->addWidget(button_dist_box, 2, 2, 2, 1);

        tag_label_3 = new QLabel(Behaviour_Button);
        tag_label_3->setObjectName(QStringLiteral("tag_label_3"));

        gridLayout_5->addWidget(tag_label_3, 4, 0, 1, 1);

        button_timer_box = new QDoubleSpinBox(Behaviour_Button);
        button_timer_box->setObjectName(QStringLiteral("button_timer_box"));
        button_timer_box->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        button_timer_box->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_5->addWidget(button_timer_box, 4, 2, 1, 1);

        BehaviourStackWidget->addWidget(Behaviour_Button);
        Behaviour_Door = new QWidget();
        Behaviour_Door->setObjectName(QStringLiteral("Behaviour_Door"));
        gridLayout_4 = new QGridLayout(Behaviour_Door);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        rotateTimeBox = new QDoubleSpinBox(Behaviour_Door);
        rotateTimeBox->setObjectName(QStringLiteral("rotateTimeBox"));
        rotateTimeBox->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));

        gridLayout_4->addWidget(rotateTimeBox, 0, 1, 1, 1);

        label_5 = new QLabel(Behaviour_Door);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_4->addWidget(label_5, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_2, 1, 0, 1, 1);

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

        WAYPOINTTEXT = new QLabel(Behaviour_Path);
        WAYPOINTTEXT->setObjectName(QStringLiteral("WAYPOINTTEXT"));

        gridLayout_2->addWidget(WAYPOINTTEXT, 4, 0, 1, 1);

        PatternDropDown = new QComboBox(Behaviour_Path);
        PatternDropDown->setObjectName(QStringLiteral("PatternDropDown"));
        PatternDropDown->setEnabled(true);
        PatternDropDown->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));

        gridLayout_2->addWidget(PatternDropDown, 2, 1, 1, 2);

        SPEEDTEXT = new QLabel(Behaviour_Path);
        SPEEDTEXT->setObjectName(QStringLiteral("SPEEDTEXT"));

        gridLayout_2->addWidget(SPEEDTEXT, 0, 0, 1, 1);

        AddButton = new QPushButton(Behaviour_Path);
        AddButton->setObjectName(QStringLiteral("AddButton"));
        AddButton->setStyleSheet(QStringLiteral("background-color: rgb(129, 129, 129);"));

        gridLayout_2->addWidget(AddButton, 7, 1, 1, 1);

        SpeedValue = new QDoubleSpinBox(Behaviour_Path);
        SpeedValue->setObjectName(QStringLiteral("SpeedValue"));
        SpeedValue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        SpeedValue->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_2->addWidget(SpeedValue, 0, 2, 1, 1);

        TimeValue = new QDoubleSpinBox(Behaviour_Path);
        TimeValue->setObjectName(QStringLiteral("TimeValue"));
        TimeValue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        TimeValue->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_2->addWidget(TimeValue, 1, 2, 1, 1);

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

        gridLayout_2->addWidget(WaypointListWidget, 5, 0, 1, 3);

        DeleteButton = new QPushButton(Behaviour_Path);
        DeleteButton->setObjectName(QStringLiteral("DeleteButton"));
        DeleteButton->setStyleSheet(QStringLiteral("background-color: rgb(129, 129, 129);"));

        gridLayout_2->addWidget(DeleteButton, 7, 2, 1, 1);

        TRIGGERTAGTEXTPATH = new QLabel(Behaviour_Path);
        TRIGGERTAGTEXTPATH->setObjectName(QStringLiteral("TRIGGERTAGTEXTPATH"));

        gridLayout_2->addWidget(TRIGGERTAGTEXTPATH, 10, 1, 1, 1);

        TriggerPathValue = new QSpinBox(Behaviour_Path);
        TriggerPathValue->setObjectName(QStringLiteral("TriggerPathValue"));
        TriggerPathValue->setEnabled(true);
        TriggerPathValue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        TriggerPathValue->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_2->addWidget(TriggerPathValue, 10, 2, 1, 1);

        TriggerPathCheckBox = new QCheckBox(Behaviour_Path);
        TriggerPathCheckBox->setObjectName(QStringLiteral("TriggerPathCheckBox"));
        TriggerPathCheckBox->setChecked(false);

        gridLayout_2->addWidget(TriggerPathCheckBox, 11, 1, 1, 2);

        BehaviourStackWidget->addWidget(Behaviour_Path);
        Lever = new QWidget();
        Lever->setObjectName(QStringLiteral("Lever"));
        gridLayout_7 = new QGridLayout(Lever);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        lever_interactionDist = new QDoubleSpinBox(Lever);
        lever_interactionDist->setObjectName(QStringLiteral("lever_interactionDist"));
        sizePolicy2.setHeightForWidth(lever_interactionDist->sizePolicy().hasHeightForWidth());
        lever_interactionDist->setSizePolicy(sizePolicy2);
        lever_interactionDist->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        lever_interactionDist->setMaximum(99999);

        gridLayout_7->addWidget(lever_interactionDist, 1, 1, 1, 1);

        label_lever_InteractionDist = new QLabel(Lever);
        label_lever_InteractionDist->setObjectName(QStringLiteral("label_lever_InteractionDist"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_lever_InteractionDist->sizePolicy().hasHeightForWidth());
        label_lever_InteractionDist->setSizePolicy(sizePolicy3);
        label_lever_InteractionDist->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_7->addWidget(label_lever_InteractionDist, 1, 0, 1, 1);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_5, 2, 1, 1, 1);

        BehaviourStackWidget->addWidget(Lever);
        Wheel = new QWidget();
        Wheel->setObjectName(QStringLiteral("Wheel"));
        verticalLayout_4 = new QVBoxLayout(Wheel);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        frame_3 = new QFrame(Wheel);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout_5 = new QHBoxLayout(frame_3);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_wheelinteraction = new QLabel(frame_3);
        label_wheelinteraction->setObjectName(QStringLiteral("label_wheelinteraction"));
        label_wheelinteraction->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_wheelinteraction);

        wheel_interactionDistBox = new QDoubleSpinBox(frame_3);
        wheel_interactionDistBox->setObjectName(QStringLiteral("wheel_interactionDistBox"));
        sizePolicy1.setHeightForWidth(wheel_interactionDistBox->sizePolicy().hasHeightForWidth());
        wheel_interactionDistBox->setSizePolicy(sizePolicy1);
        wheel_interactionDistBox->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        wheel_interactionDistBox->setMaximum(99999);

        horizontalLayout_5->addWidget(wheel_interactionDistBox);


        verticalLayout_4->addWidget(frame_3);

        frame = new QFrame(Wheel);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_minRot = new QLabel(frame);
        label_minRot->setObjectName(QStringLiteral("label_minRot"));
        label_minRot->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_minRot);

        wheel_minRotationBox = new QDoubleSpinBox(frame);
        wheel_minRotationBox->setObjectName(QStringLiteral("wheel_minRotationBox"));
        sizePolicy1.setHeightForWidth(wheel_minRotationBox->sizePolicy().hasHeightForWidth());
        wheel_minRotationBox->setSizePolicy(sizePolicy1);
        wheel_minRotationBox->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        wheel_minRotationBox->setMaximum(99999);

        horizontalLayout_3->addWidget(wheel_minRotationBox);


        verticalLayout_4->addWidget(frame);

        frame_2 = new QFrame(Wheel);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        horizontalLayout_4 = new QHBoxLayout(frame_2);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_maxRot = new QLabel(frame_2);
        label_maxRot->setObjectName(QStringLiteral("label_maxRot"));
        label_maxRot->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_maxRot);

        wheel_maxRotationBox = new QDoubleSpinBox(frame_2);
        wheel_maxRotationBox->setObjectName(QStringLiteral("wheel_maxRotationBox"));
        sizePolicy1.setHeightForWidth(wheel_maxRotationBox->sizePolicy().hasHeightForWidth());
        wheel_maxRotationBox->setSizePolicy(sizePolicy1);
        wheel_maxRotationBox->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        wheel_maxRotationBox->setMaximum(99999);

        horizontalLayout_4->addWidget(wheel_maxRotationBox);


        verticalLayout_4->addWidget(frame_2);

        frame_4 = new QFrame(Wheel);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        horizontalLayout_6 = new QHBoxLayout(frame_4);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_rotTime = new QLabel(frame_4);
        label_rotTime->setObjectName(QStringLiteral("label_rotTime"));
        label_rotTime->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_6->addWidget(label_rotTime);

        wheel_rotationTimeBox = new QDoubleSpinBox(frame_4);
        wheel_rotationTimeBox->setObjectName(QStringLiteral("wheel_rotationTimeBox"));
        sizePolicy1.setHeightForWidth(wheel_rotationTimeBox->sizePolicy().hasHeightForWidth());
        wheel_rotationTimeBox->setSizePolicy(sizePolicy1);
        wheel_rotationTimeBox->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        wheel_rotationTimeBox->setMaximum(99999);

        horizontalLayout_6->addWidget(wheel_rotationTimeBox);


        verticalLayout_4->addWidget(frame_4);

        frame_5 = new QFrame(Wheel);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        horizontalLayout_7 = new QHBoxLayout(frame_5);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_timeTilReset = new QLabel(frame_5);
        label_timeTilReset->setObjectName(QStringLiteral("label_timeTilReset"));
        label_timeTilReset->setLayoutDirection(Qt::RightToLeft);
        label_timeTilReset->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_7->addWidget(label_timeTilReset);

        wheel_timeTilResetBox = new QDoubleSpinBox(frame_5);
        wheel_timeTilResetBox->setObjectName(QStringLiteral("wheel_timeTilResetBox"));
        sizePolicy1.setHeightForWidth(wheel_timeTilResetBox->sizePolicy().hasHeightForWidth());
        wheel_timeTilResetBox->setSizePolicy(sizePolicy1);
        wheel_timeTilResetBox->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        wheel_timeTilResetBox->setMaximum(99999);

        horizontalLayout_7->addWidget(wheel_timeTilResetBox);


        verticalLayout_4->addWidget(frame_5);

        frame_6 = new QFrame(Wheel);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setLayoutDirection(Qt::LeftToRight);
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        horizontalLayout_8 = new QHBoxLayout(frame_6);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(-1, -1, 9, -1);
        label_resetTime = new QLabel(frame_6);
        label_resetTime->setObjectName(QStringLiteral("label_resetTime"));
        label_resetTime->setLayoutDirection(Qt::RightToLeft);
        label_resetTime->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_8->addWidget(label_resetTime);

        wheel_resetTimeBox = new QDoubleSpinBox(frame_6);
        wheel_resetTimeBox->setObjectName(QStringLiteral("wheel_resetTimeBox"));
        sizePolicy1.setHeightForWidth(wheel_resetTimeBox->sizePolicy().hasHeightForWidth());
        wheel_resetTimeBox->setSizePolicy(sizePolicy1);
        wheel_resetTimeBox->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        wheel_resetTimeBox->setMaximum(99999);

        horizontalLayout_8->addWidget(wheel_resetTimeBox);


        verticalLayout_4->addWidget(frame_6);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_3);

        BehaviourStackWidget->addWidget(Wheel);

        formLayout_3->setWidget(5, QFormLayout::FieldRole, BehaviourStackWidget);

        CustomBehaviourTabWidget->addTab(Behaviour, QString());
        Triggers = new QWidget();
        Triggers->setObjectName(QStringLiteral("Triggers"));
        formLayout_2 = new QFormLayout(Triggers);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        availableTriggers = new QComboBox(Triggers);
        availableTriggers->setObjectName(QStringLiteral("availableTriggers"));
        sizePolicy.setHeightForWidth(availableTriggers->sizePolicy().hasHeightForWidth());
        availableTriggers->setSizePolicy(sizePolicy);
        availableTriggers->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        availableTriggers->setMaxVisibleItems(20);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, availableTriggers);

        label = new QLabel(Triggers);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral(""));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label);

        addDel_Frame = new QFrame(Triggers);
        addDel_Frame->setObjectName(QStringLiteral("addDel_Frame"));
        addDel_Frame->setStyleSheet(QStringLiteral(""));
        addDel_Frame->setFrameShape(QFrame::StyledPanel);
        addDel_Frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(addDel_Frame);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        AddTriggerButton = new QPushButton(addDel_Frame);
        AddTriggerButton->setObjectName(QStringLiteral("AddTriggerButton"));
        AddTriggerButton->setStyleSheet(QStringLiteral("background-color: rgb(129, 129, 129);"));

        horizontalLayout->addWidget(AddTriggerButton);

        DeleteTriggerButton = new QPushButton(addDel_Frame);
        DeleteTriggerButton->setObjectName(QStringLiteral("DeleteTriggerButton"));
        DeleteTriggerButton->setStyleSheet(QStringLiteral("background-color: rgb(129, 129, 129);"));

        horizontalLayout->addWidget(DeleteTriggerButton);


        formLayout_2->setWidget(7, QFormLayout::SpanningRole, addDel_Frame);

        TriggerTableWidget = new QTableWidget(Triggers);
        if (TriggerTableWidget->columnCount() < 2)
            TriggerTableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignCenter);
        TriggerTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignCenter);
        TriggerTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        TriggerTableWidget->setObjectName(QStringLiteral("TriggerTableWidget"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(TriggerTableWidget->sizePolicy().hasHeightForWidth());
        TriggerTableWidget->setSizePolicy(sizePolicy4);
        TriggerTableWidget->setMaximumSize(QSize(16777215, 16777215));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush8);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        TriggerTableWidget->setPalette(palette3);
        TriggerTableWidget->setStyleSheet(QLatin1String("background-color: rgb(48, 48, 48);\n"
"\n"
"Header- color:rgb(48, 48, 48);\n"
"\n"
""));
        TriggerTableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        TriggerTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        TriggerTableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        TriggerTableWidget->setAutoScroll(false);
        TriggerTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        TriggerTableWidget->setAlternatingRowColors(false);
        TriggerTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        TriggerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        TriggerTableWidget->setSortingEnabled(false);
        TriggerTableWidget->setCornerButtonEnabled(false);
        TriggerTableWidget->setRowCount(0);
        TriggerTableWidget->setColumnCount(2);
        TriggerTableWidget->horizontalHeader()->setVisible(false);
        TriggerTableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        TriggerTableWidget->horizontalHeader()->setDefaultSectionSize(105);
        TriggerTableWidget->horizontalHeader()->setHighlightSections(true);
        TriggerTableWidget->verticalHeader()->setVisible(false);
        TriggerTableWidget->verticalHeader()->setCascadingSectionResizes(false);
        TriggerTableWidget->verticalHeader()->setHighlightSections(false);
        TriggerTableWidget->verticalHeader()->setStretchLastSection(false);

        formLayout_2->setWidget(3, QFormLayout::SpanningRole, TriggerTableWidget);

        EventSignalBox = new QComboBox(Triggers);
        EventSignalBox->setObjectName(QStringLiteral("EventSignalBox"));
        EventSignalBox->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));

        formLayout_2->setWidget(6, QFormLayout::SpanningRole, EventSignalBox);

        label_3 = new QLabel(Triggers);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_3);

        CustomBehaviourTabWidget->addTab(Triggers, QString());
        CheckPoint = new QWidget();
        CheckPoint->setObjectName(QStringLiteral("CheckPoint"));
        gridLayout_6 = new QGridLayout(CheckPoint);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        checkpointTXT = new QLabel(CheckPoint);
        checkpointTXT->setObjectName(QStringLiteral("checkpointTXT"));

        gridLayout_6->addWidget(checkpointTXT, 0, 0, 1, 1);

        CheckPointValue = new QSpinBox(CheckPoint);
        CheckPointValue->setObjectName(QStringLiteral("CheckPointValue"));
        CheckPointValue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        CheckPointValue->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_6->addWidget(CheckPointValue, 0, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer, 2, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        CheckPointADD = new QPushButton(CheckPoint);
        CheckPointADD->setObjectName(QStringLiteral("CheckPointADD"));
        CheckPointADD->setStyleSheet(QStringLiteral("background-color: rgb(129, 129, 129);"));

        gridLayout_6->addWidget(CheckPointADD, 1, 0, 1, 1);

        label_4 = new QLabel(CheckPoint);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_6->addWidget(label_4, 2, 1, 1, 1);

        CustomBehaviourTabWidget->addTab(CheckPoint, QString());
        Lights = new QWidget();
        Lights->setObjectName(QStringLiteral("Lights"));
        gridLayout_8 = new QGridLayout(Lights);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        label_6 = new QLabel(Lights);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_8->addWidget(label_6, 1, 0, 1, 1);

        LightDropDown = new QComboBox(Lights);
        LightDropDown->setObjectName(QStringLiteral("LightDropDown"));
        LightDropDown->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));

        gridLayout_8->addWidget(LightDropDown, 1, 1, 1, 1);

        LightWindows = new QStackedWidget(Lights);
        LightWindows->setObjectName(QStringLiteral("LightWindows"));
        None = new QWidget();
        None->setObjectName(QStringLiteral("None"));
        LightWindows->addWidget(None);
        Point = new QWidget();
        Point->setObjectName(QStringLiteral("Point"));
        gridLayout_9 = new QGridLayout(Point);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        label_12 = new QLabel(Point);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_9->addWidget(label_12, 5, 0, 1, 2);

        R_Colorvalue = new QSpinBox(Point);
        R_Colorvalue->setObjectName(QStringLiteral("R_Colorvalue"));
        R_Colorvalue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        R_Colorvalue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        R_Colorvalue->setMaximum(255);

        gridLayout_9->addWidget(R_Colorvalue, 1, 1, 1, 1);

        label_8 = new QLabel(Point);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_9->addWidget(label_8, 1, 0, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_9->addItem(verticalSpacer_6, 9, 2, 1, 1);

        label_9 = new QLabel(Point);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_9->addWidget(label_9, 2, 0, 1, 1);

        label_10 = new QLabel(Point);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_9->addWidget(label_10, 3, 0, 1, 1);

        G_Colorvalue = new QSpinBox(Point);
        G_Colorvalue->setObjectName(QStringLiteral("G_Colorvalue"));
        G_Colorvalue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        G_Colorvalue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        G_Colorvalue->setMaximum(255);

        gridLayout_9->addWidget(G_Colorvalue, 2, 1, 1, 1);

        RadiusValue = new QDoubleSpinBox(Point);
        RadiusValue->setObjectName(QStringLiteral("RadiusValue"));
        RadiusValue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        RadiusValue->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_9->addWidget(RadiusValue, 4, 2, 1, 2);

        G_Slider = new QSlider(Point);
        G_Slider->setObjectName(QStringLiteral("G_Slider"));
        G_Slider->setMaximum(255);
        G_Slider->setPageStep(25);
        G_Slider->setOrientation(Qt::Horizontal);

        gridLayout_9->addWidget(G_Slider, 2, 2, 1, 2);

        R_Slider = new QSlider(Point);
        R_Slider->setObjectName(QStringLiteral("R_Slider"));
        R_Slider->setMaximum(255);
        R_Slider->setPageStep(25);
        R_Slider->setOrientation(Qt::Horizontal);

        gridLayout_9->addWidget(R_Slider, 1, 2, 1, 2);

        B_Slider = new QSlider(Point);
        B_Slider->setObjectName(QStringLiteral("B_Slider"));
        B_Slider->setMaximum(255);
        B_Slider->setPageStep(25);
        B_Slider->setOrientation(Qt::Horizontal);

        gridLayout_9->addWidget(B_Slider, 3, 2, 1, 2);

        B_Colorvalue = new QSpinBox(Point);
        B_Colorvalue->setObjectName(QStringLiteral("B_Colorvalue"));
        B_Colorvalue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        B_Colorvalue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        B_Colorvalue->setMaximum(255);

        gridLayout_9->addWidget(B_Colorvalue, 3, 1, 1, 1);

        label_14 = new QLabel(Point);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_9->addWidget(label_14, 8, 0, 1, 2);

        label_13 = new QLabel(Point);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_9->addWidget(label_13, 7, 0, 1, 2);

        label_7 = new QLabel(Point);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_9->addWidget(label_7, 6, 0, 1, 2);

        label_11 = new QLabel(Point);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_9->addWidget(label_11, 4, 0, 1, 2);

        QuadValue = new QDoubleSpinBox(Point);
        QuadValue->setObjectName(QStringLiteral("QuadValue"));
        QuadValue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        QuadValue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        QuadValue->setMaximum(1);
        QuadValue->setSingleStep(0.01);

        gridLayout_9->addWidget(QuadValue, 8, 2, 1, 1);

        LinearValue = new QDoubleSpinBox(Point);
        LinearValue->setObjectName(QStringLiteral("LinearValue"));
        LinearValue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        LinearValue->setFrame(true);
        LinearValue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        LinearValue->setMaximum(1);
        LinearValue->setSingleStep(0.01);

        gridLayout_9->addWidget(LinearValue, 7, 2, 1, 1);

        ConstantValue = new QDoubleSpinBox(Point);
        ConstantValue->setObjectName(QStringLiteral("ConstantValue"));
        ConstantValue->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        ConstantValue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ConstantValue->setMaximum(1);
        ConstantValue->setSingleStep(0.01);

        gridLayout_9->addWidget(ConstantValue, 6, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer, 6, 3, 1, 1);

        LightWindows->addWidget(Point);

        gridLayout_8->addWidget(LightWindows, 4, 0, 1, 2);

        ADD_Light_Button = new QPushButton(Lights);
        ADD_Light_Button->setObjectName(QStringLiteral("ADD_Light_Button"));
        ADD_Light_Button->setStyleSheet(QStringLiteral("background-color: rgb(129, 129, 129);"));

        gridLayout_8->addWidget(ADD_Light_Button, 0, 0, 1, 2);

        CustomBehaviourTabWidget->addTab(Lights, QString());

        verticalLayout_3->addWidget(CustomBehaviourTabWidget);


        gridLayout->addWidget(Values, 2, 3, 3, 1);

        RenderWidget = new QWidget(centralWidget);
        RenderWidget->setObjectName(QStringLiteral("RenderWidget"));
        sizePolicy4.setHeightForWidth(RenderWidget->sizePolicy().hasHeightForWidth());
        RenderWidget->setSizePolicy(sizePolicy4);
        RenderWidget->setMinimumSize(QSize(1, 1));
        RenderWidget->setCursor(QCursor(Qt::ArrowCursor));
        RenderWidget->setMouseTracking(true);
        RenderWidget->setFocusPolicy(Qt::StrongFocus);
        RenderWidget->setAcceptDrops(true);
        RenderWidget->setAutoFillBackground(false);

        gridLayout->addWidget(RenderWidget, 2, 2, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 4, 1, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
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
        QObject::connect(LightDropDown, SIGNAL(currentIndexChanged(int)), LightWindows, SLOT(setCurrentIndex(int)));
        QObject::connect(R_Colorvalue, SIGNAL(valueChanged(int)), R_Slider, SLOT(setValue(int)));
        QObject::connect(R_Slider, SIGNAL(sliderMoved(int)), R_Colorvalue, SLOT(setValue(int)));
        QObject::connect(G_Colorvalue, SIGNAL(valueChanged(int)), G_Slider, SLOT(setValue(int)));
        QObject::connect(G_Slider, SIGNAL(sliderMoved(int)), G_Colorvalue, SLOT(setValue(int)));
        QObject::connect(B_Colorvalue, SIGNAL(valueChanged(int)), B_Slider, SLOT(setValue(int)));
        QObject::connect(B_Slider, SIGNAL(sliderMoved(int)), B_Colorvalue, SLOT(setValue(int)));
        QObject::connect(G_Slider, SIGNAL(valueChanged(int)), G_Colorvalue, SLOT(setValue(int)));
        QObject::connect(B_Slider, SIGNAL(valueChanged(int)), B_Colorvalue, SLOT(setValue(int)));
        QObject::connect(R_Slider, SIGNAL(valueChanged(int)), R_Colorvalue, SLOT(setValue(int)));

        CustomBehaviourTabWidget->setCurrentIndex(3);
        BehaviourStackWidget->setCurrentIndex(0);
        LightWindows->setCurrentIndex(0);
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
        nameLabel->setText(QApplication::translate("SSP_EditorClass", "Level Name", 0));
        scalelabel->setText(QApplication::translate("SSP_EditorClass", "Scale", 0));
        rotationlabel->setText(QApplication::translate("SSP_EditorClass", "Rotation", 0));
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
         << QApplication::translate("SSP_EditorClass", "Button", 0)
         << QApplication::translate("SSP_EditorClass", "Door", 0)
         << QApplication::translate("SSP_EditorClass", "Path", 0)
         << QApplication::translate("SSP_EditorClass", "Lever", 0)
         << QApplication::translate("SSP_EditorClass", "Wheel", 0)
        );
        tag_label->setText(QApplication::translate("SSP_EditorClass", " Tag", 0));
        tag_label_2->setText(QApplication::translate("SSP_EditorClass", "Distance of interaction", 0));
        tag_label_3->setText(QApplication::translate("SSP_EditorClass", "Reset timer (Seconds)", 0));
        label_5->setText(QApplication::translate("SSP_EditorClass", "Rotation time :", 0));
        TIMETEXT->setText(QApplication::translate("SSP_EditorClass", "Time (ms)", 0));
        PATTERNTEXT->setText(QApplication::translate("SSP_EditorClass", "Pattern", 0));
        WAYPOINTTEXT->setText(QApplication::translate("SSP_EditorClass", "Waypoints", 0));
        PatternDropDown->clear();
        PatternDropDown->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "Linear", 0)
         << QApplication::translate("SSP_EditorClass", "Circular", 0)
         << QApplication::translate("SSP_EditorClass", "Round Trip", 0)
         << QApplication::translate("SSP_EditorClass", "Random", 0)
        );
        SPEEDTEXT->setText(QApplication::translate("SSP_EditorClass", "Speed", 0));
        AddButton->setText(QApplication::translate("SSP_EditorClass", "ADD", 0));
        DeleteButton->setText(QApplication::translate("SSP_EditorClass", "DEL", 0));
        TRIGGERTAGTEXTPATH->setText(QApplication::translate("SSP_EditorClass", "Trigger Tag", 0));
        TriggerPathCheckBox->setText(QApplication::translate("SSP_EditorClass", "Not Triggered", 0));
        label_lever_InteractionDist->setText(QApplication::translate("SSP_EditorClass", "Dist. of interaction :", 0));
        label_wheelinteraction->setText(QApplication::translate("SSP_EditorClass", "Interaction Dist:", 0));
        label_minRot->setText(QApplication::translate("SSP_EditorClass", "Min Rotation:", 0));
        label_maxRot->setText(QApplication::translate("SSP_EditorClass", "Max Rotation:", 0));
        label_rotTime->setText(QApplication::translate("SSP_EditorClass", "Rotation Time:", 0));
        label_timeTilReset->setText(QApplication::translate("SSP_EditorClass", "Time til reset (s):", 0));
        label_resetTime->setText(QApplication::translate("SSP_EditorClass", "Reset time (s):", 0));
        CustomBehaviourTabWidget->setTabText(CustomBehaviourTabWidget->indexOf(Behaviour), QApplication::translate("SSP_EditorClass", "Behaviour", 0));
        availableTriggers->clear();
        availableTriggers->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "None", 0)
        );
        label->setText(QApplication::translate("SSP_EditorClass", "Scene Triggers : ", 0));
        AddTriggerButton->setText(QApplication::translate("SSP_EditorClass", "ADD", 0));
        DeleteTriggerButton->setText(QApplication::translate("SSP_EditorClass", "DEL", 0));
        QTableWidgetItem *___qtablewidgetitem = TriggerTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("SSP_EditorClass", "Trigger", 0));
        QTableWidgetItem *___qtablewidgetitem1 = TriggerTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("SSP_EditorClass", "Event Signal", 0));
        EventSignalBox->clear();
        EventSignalBox->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "None", 0)
        );
        label_3->setText(QApplication::translate("SSP_EditorClass", "Event Signal", 0));
        CustomBehaviourTabWidget->setTabText(CustomBehaviourTabWidget->indexOf(Triggers), QApplication::translate("SSP_EditorClass", "Triggers", 0));
        checkpointTXT->setText(QApplication::translate("SSP_EditorClass", "Checkpoint ID", 0));
        CheckPointADD->setText(QApplication::translate("SSP_EditorClass", "Add CheckPoint", 0));
        label_4->setText(QApplication::translate("SSP_EditorClass", "Empty Space", 0));
        CustomBehaviourTabWidget->setTabText(CustomBehaviourTabWidget->indexOf(CheckPoint), QApplication::translate("SSP_EditorClass", "Checkpoints", 0));
        label_6->setText(QApplication::translate("SSP_EditorClass", "Light Type", 0));
        LightDropDown->clear();
        LightDropDown->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "None", 0)
         << QApplication::translate("SSP_EditorClass", "Point", 0)
         << QApplication::translate("SSP_EditorClass", "Directional", 0)
         << QApplication::translate("SSP_EditorClass", "Area", 0)
         << QApplication::translate("SSP_EditorClass", "Spot", 0)
         << QApplication::translate("SSP_EditorClass", "Ambient", 0)
        );
        label_12->setText(QApplication::translate("SSP_EditorClass", "Falloff", 0));
        label_8->setText(QApplication::translate("SSP_EditorClass", "R", 0));
        label_9->setText(QApplication::translate("SSP_EditorClass", "G", 0));
        label_10->setText(QApplication::translate("SSP_EditorClass", "B", 0));
        label_14->setText(QApplication::translate("SSP_EditorClass", "Quadratic", 0));
        label_13->setText(QApplication::translate("SSP_EditorClass", "Linear", 0));
        label_7->setText(QApplication::translate("SSP_EditorClass", "Constant", 0));
        label_11->setText(QApplication::translate("SSP_EditorClass", "Radius", 0));
        ADD_Light_Button->setText(QApplication::translate("SSP_EditorClass", "ADD NEW LIGHT", 0));
        CustomBehaviourTabWidget->setTabText(CustomBehaviourTabWidget->indexOf(Lights), QApplication::translate("SSP_EditorClass", "Lights", 0));
        label_2->setText(QApplication::translate("SSP_EditorClass", "Preview", 0));
        pushButton->setText(QApplication::translate("SSP_EditorClass", "Reload", 0));
        QTreeWidgetItem *___qtreewidgetitem = assetTree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("SSP_EditorClass", "Files", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SSP_EditorClass", "Asset Browser", 0));
        QTreeWidgetItem *___qtreewidgetitem1 = scene_tree->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("SSP_EditorClass", "Files", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SSP_EditorClass", "Scene info", 0));
        menuEditor->setTitle(QApplication::translate("SSP_EditorClass", "Editor", 0));
    } // retranslateUi

};

namespace Ui {
    class SSP_EditorClass: public Ui_SSP_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSP_EDITOR_H
