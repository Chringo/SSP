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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
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
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QTreeWidget *assetTree;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *scene_tree;
    QPushButton *pushButton;
    QLabel *label_2;
    QGroupBox *Values;
    QVBoxLayout *verticalLayout_3;
    QFrame *nameQFrame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *nameLabel;
    QLineEdit *lineEdit;
    QFrame *transformFrame;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *xValue_scale;
    QDoubleSpinBox *zValue_scale;
    QDoubleSpinBox *yValue_rot;
    QDoubleSpinBox *zValue_rot;
    QLabel *rotationlabel;
    QLabel *scalelabel;
    QDoubleSpinBox *xValue_translate;
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
    QSpinBox *button_tagBox;
    QLabel *tag_label_2;
    QSpinBox *button_dist_box;
    QWidget *Behaviour_Door;
    QGridLayout *gridLayout_4;
    QListWidget *TriggerListWidget;
    QComboBox *availableTriggers;
    QFrame *addDel_Frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *AddTriggerButton;
    QPushButton *DeleteTriggerButton;
    QLabel *label;
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
    QWidget *CheckPoint;
    QGridLayout *gridLayout_6;
    QLabel *checkpointTXT;
    QSpinBox *CheckPointValue;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *CheckPointADD;
    QLabel *label_4;
    QWidget *RenderWidget;
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
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
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

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 4, 1, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        Values = new QGroupBox(centralWidget);
        Values->setObjectName(QStringLiteral("Values"));
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
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(xValue_rot->sizePolicy().hasHeightForWidth());
        xValue_rot->setSizePolicy(sizePolicy1);
        xValue_rot->setWrapping(true);
        xValue_rot->setFrame(true);
        xValue_rot->setButtonSymbols(QAbstractSpinBox::NoButtons);
        xValue_rot->setDecimals(4);
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

        gridLayout_5->addItem(verticalSpacer_4, 6, 2, 1, 1);

        tag_label = new QLabel(Behaviour_Button);
        tag_label->setObjectName(QStringLiteral("tag_label"));

        gridLayout_5->addWidget(tag_label, 1, 0, 1, 2);

        button_tagBox = new QSpinBox(Behaviour_Button);
        button_tagBox->setObjectName(QStringLiteral("button_tagBox"));
        button_tagBox->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        button_tagBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_5->addWidget(button_tagBox, 1, 2, 1, 1);

        tag_label_2 = new QLabel(Behaviour_Button);
        tag_label_2->setObjectName(QStringLiteral("tag_label_2"));

        gridLayout_5->addWidget(tag_label_2, 2, 0, 2, 2);

        button_dist_box = new QSpinBox(Behaviour_Button);
        button_dist_box->setObjectName(QStringLiteral("button_dist_box"));
        button_dist_box->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));
        button_dist_box->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_5->addWidget(button_dist_box, 2, 2, 2, 1);

        BehaviourStackWidget->addWidget(Behaviour_Button);
        Behaviour_Door = new QWidget();
        Behaviour_Door->setObjectName(QStringLiteral("Behaviour_Door"));
        gridLayout_4 = new QGridLayout(Behaviour_Door);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        TriggerListWidget = new QListWidget(Behaviour_Door);
        TriggerListWidget->setObjectName(QStringLiteral("TriggerListWidget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(TriggerListWidget->sizePolicy().hasHeightForWidth());
        TriggerListWidget->setSizePolicy(sizePolicy3);
        TriggerListWidget->setMaximumSize(QSize(16777215, 16777215));
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
        TriggerListWidget->setPalette(palette2);
        TriggerListWidget->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));

        gridLayout_4->addWidget(TriggerListWidget, 12, 0, 1, 4);

        availableTriggers = new QComboBox(Behaviour_Door);
        availableTriggers->setObjectName(QStringLiteral("availableTriggers"));
        availableTriggers->setStyleSheet(QStringLiteral("background-color: rgb(48, 48, 48);"));

        gridLayout_4->addWidget(availableTriggers, 5, 1, 1, 3);

        addDel_Frame = new QFrame(Behaviour_Door);
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


        gridLayout_4->addWidget(addDel_Frame, 6, 1, 1, 1);

        label = new QLabel(Behaviour_Door);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral(""));

        gridLayout_4->addWidget(label, 5, 0, 1, 1);

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
        WaypointListWidget->setPalette(palette3);
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

        formLayout_3->setWidget(5, QFormLayout::FieldRole, BehaviourStackWidget);

        CustomBehaviourTabWidget->addTab(Behaviour, QString());
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

        verticalLayout_3->addWidget(CustomBehaviourTabWidget);


        gridLayout->addWidget(Values, 2, 3, 3, 1);

        RenderWidget = new QWidget(centralWidget);
        RenderWidget->setObjectName(QStringLiteral("RenderWidget"));
        sizePolicy3.setHeightForWidth(RenderWidget->sizePolicy().hasHeightForWidth());
        RenderWidget->setSizePolicy(sizePolicy3);
        RenderWidget->setMinimumSize(QSize(1, 1));
        RenderWidget->setCursor(QCursor(Qt::ArrowCursor));
        RenderWidget->setMouseTracking(true);
        RenderWidget->setFocusPolicy(Qt::StrongFocus);
        RenderWidget->setAcceptDrops(true);
        RenderWidget->setAutoFillBackground(false);

        gridLayout->addWidget(RenderWidget, 2, 2, 1, 1);

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

        tabWidget->setCurrentIndex(0);
        CustomBehaviourTabWidget->setCurrentIndex(0);
        BehaviourStackWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SSP_EditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *SSP_EditorClass)
    {
        SSP_EditorClass->setWindowTitle(QApplication::translate("SSP_EditorClass", "SSP_Editor", Q_NULLPTR));
        actionNew_scene->setText(QApplication::translate("SSP_EditorClass", "New scene", Q_NULLPTR));
        actionLoad_scene->setText(QApplication::translate("SSP_EditorClass", "Load scene", Q_NULLPTR));
        actionSave_scene->setText(QApplication::translate("SSP_EditorClass", "Save scene", Q_NULLPTR));
        actionBuild_BPF->setText(QApplication::translate("SSP_EditorClass", "Build .BPF", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = assetTree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("SSP_EditorClass", "Files", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("SSP_EditorClass", "Asset Browser", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem1 = scene_tree->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("SSP_EditorClass", "Files", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("SSP_EditorClass", "Scene info", Q_NULLPTR));
        pushButton->setText(QApplication::translate("SSP_EditorClass", "Reload", Q_NULLPTR));
        label_2->setText(QApplication::translate("SSP_EditorClass", "Preview", Q_NULLPTR));
        Values->setTitle(QApplication::translate("SSP_EditorClass", "Attributes", Q_NULLPTR));
        nameLabel->setText(QApplication::translate("SSP_EditorClass", "Level Name", Q_NULLPTR));
        rotationlabel->setText(QApplication::translate("SSP_EditorClass", "Rotation", Q_NULLPTR));
        scalelabel->setText(QApplication::translate("SSP_EditorClass", "Scale", Q_NULLPTR));
        translateLabel->setText(QApplication::translate("SSP_EditorClass", "Translate", Q_NULLPTR));
        UIDTEXT->setText(QApplication::translate("SSP_EditorClass", "Unique ID:", Q_NULLPTR));
        uniqueIDLabel->setText(QApplication::translate("SSP_EditorClass", "0", Q_NULLPTR));
        ISSTATICTEXT->setText(QApplication::translate("SSP_EditorClass", "Is Static:", Q_NULLPTR));
        isStaticCheck->setText(QString());
        animationBox->clear();
        animationBox->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "None", Q_NULLPTR)
        );
        ANIMTEXT->setText(QApplication::translate("SSP_EditorClass", "Animation:", Q_NULLPTR));
        BEHAVIOURTEXT->setText(QApplication::translate("SSP_EditorClass", "Behaviour Type", Q_NULLPTR));
        BehaviourDropDown->clear();
        BehaviourDropDown->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "None", Q_NULLPTR)
         << QApplication::translate("SSP_EditorClass", "Button", Q_NULLPTR)
         << QApplication::translate("SSP_EditorClass", "Door", Q_NULLPTR)
         << QApplication::translate("SSP_EditorClass", "Path", Q_NULLPTR)
         << QApplication::translate("SSP_EditorClass", "Lever", Q_NULLPTR)
         << QApplication::translate("SSP_EditorClass", "Wheel", Q_NULLPTR)
         << QApplication::translate("SSP_EditorClass", "Pressure plate", Q_NULLPTR)
        );
        tag_label->setText(QApplication::translate("SSP_EditorClass", " Tag", Q_NULLPTR));
        tag_label_2->setText(QApplication::translate("SSP_EditorClass", "Distance of interaction", Q_NULLPTR));
        availableTriggers->clear();
        availableTriggers->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "None", Q_NULLPTR)
        );
        AddTriggerButton->setText(QApplication::translate("SSP_EditorClass", "ADD", Q_NULLPTR));
        DeleteTriggerButton->setText(QApplication::translate("SSP_EditorClass", "DEL", Q_NULLPTR));
        label->setText(QApplication::translate("SSP_EditorClass", "Scene Triggers : ", Q_NULLPTR));
        TIMETEXT->setText(QApplication::translate("SSP_EditorClass", "Time (ms)", Q_NULLPTR));
        PATTERNTEXT->setText(QApplication::translate("SSP_EditorClass", "Pattern", Q_NULLPTR));
        WAYPOINTTEXT->setText(QApplication::translate("SSP_EditorClass", "Waypoints", Q_NULLPTR));
        PatternDropDown->clear();
        PatternDropDown->insertItems(0, QStringList()
         << QApplication::translate("SSP_EditorClass", "Linear", Q_NULLPTR)
         << QApplication::translate("SSP_EditorClass", "Circular", Q_NULLPTR)
         << QApplication::translate("SSP_EditorClass", "Round Trip", Q_NULLPTR)
         << QApplication::translate("SSP_EditorClass", "Random", Q_NULLPTR)
        );
        SPEEDTEXT->setText(QApplication::translate("SSP_EditorClass", "Speed", Q_NULLPTR));
        AddButton->setText(QApplication::translate("SSP_EditorClass", "ADD", Q_NULLPTR));
        DeleteButton->setText(QApplication::translate("SSP_EditorClass", "DEL", Q_NULLPTR));
        TRIGGERTAGTEXTPATH->setText(QApplication::translate("SSP_EditorClass", "Trigger Tag", Q_NULLPTR));
        TriggerPathCheckBox->setText(QApplication::translate("SSP_EditorClass", "Not Triggered", Q_NULLPTR));
        CustomBehaviourTabWidget->setTabText(CustomBehaviourTabWidget->indexOf(Behaviour), QApplication::translate("SSP_EditorClass", "Behaviour", Q_NULLPTR));
        checkpointTXT->setText(QApplication::translate("SSP_EditorClass", "Checkpoint ID", Q_NULLPTR));
        CheckPointADD->setText(QApplication::translate("SSP_EditorClass", "Add CheckPoint", Q_NULLPTR));
        label_4->setText(QApplication::translate("SSP_EditorClass", "Empty Space", Q_NULLPTR));
        CustomBehaviourTabWidget->setTabText(CustomBehaviourTabWidget->indexOf(CheckPoint), QApplication::translate("SSP_EditorClass", "Checkpoints", Q_NULLPTR));
        menuEditor->setTitle(QApplication::translate("SSP_EditorClass", "Editor", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SSP_EditorClass: public Ui_SSP_EditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SSP_EDITOR_H
