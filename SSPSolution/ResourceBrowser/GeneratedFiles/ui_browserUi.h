/********************************************************************************
** Form generated from reading UI file 'browserui.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BROWSERUI_H
#define UI_BROWSERUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BrowserUI
{
public:
    QHBoxLayout *horizontalLayout;
    QTreeWidget *assetTree;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *BrowserUI)
    {
        if (BrowserUI->objectName().isEmpty())
            BrowserUI->setObjectName(QStringLiteral("BrowserUI"));
        BrowserUI->resize(844, 567);
        BrowserUI->setAutoFillBackground(false);
        BrowserUI->setStyleSheet(QLatin1String("\n"
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
"#RenderWidget{\n"
"background:  rgb(255, 170, 127);\n"
"}\n"
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
"QPushBut"
                        "ton:pressed{\n"
"  background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(221, 263, 239), stop: 1 #cbdaf1);\n"
"    border: 1px solid #bfcde4;}\n"
"\n"
"background:   rgb(68, 68, 68);\n"
"color: white;\n"
"border: black;"));
        horizontalLayout = new QHBoxLayout(BrowserUI);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        assetTree = new QTreeWidget(BrowserUI);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        assetTree->setHeaderItem(__qtreewidgetitem);
        assetTree->setObjectName(QStringLiteral("assetTree"));

        horizontalLayout->addWidget(assetTree);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(BrowserUI);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(25);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        label_2 = new QLabel(BrowserUI);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        horizontalLayout->addWidget(label_2);


        retranslateUi(BrowserUI);

        QMetaObject::connectSlotsByName(BrowserUI);
    } // setupUi

    void retranslateUi(QWidget *BrowserUI)
    {
        BrowserUI->setWindowTitle(QApplication::translate("BrowserUI", "Resource Browser", 0));
        label->setText(QApplication::translate("BrowserUI", "Resource Window", 0));
        label_2->setText(QApplication::translate("BrowserUI", "NEW CHANGE", 0));
    } // retranslateUi

};

namespace Ui {
    class BrowserUI: public Ui_BrowserUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BROWSERUI_H
