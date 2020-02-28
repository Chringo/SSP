/********************************************************************************
** Form generated from reading UI file 'browserUi.ui'
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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BrowserUI
{
public:
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *BrowserUI)
    {
        if (BrowserUI->objectName().isEmpty())
            BrowserUI->setObjectName(QStringLiteral("BrowserUI"));
        BrowserUI->resize(772, 567);
        label = new QLabel(BrowserUI);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(210, 180, 371, 131));
        QFont font;
        font.setPointSize(25);
        label->setFont(font);
        label_2 = new QLabel(BrowserUI);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(340, 360, 371, 131));
        label_2->setFont(font);

        retranslateUi(BrowserUI);

        QMetaObject::connectSlotsByName(BrowserUI);
    } // setupUi

    void retranslateUi(QWidget *BrowserUI)
    {
        BrowserUI->setWindowTitle(QApplication::translate("BrowserUI", "Form", 0));
        label->setText(QApplication::translate("BrowserUI", "Resource Window", 0));
        label_2->setText(QApplication::translate("BrowserUI", "NEW CHANGE", 0));
    } // retranslateUi

};

namespace Ui {
    class BrowserUI: public Ui_BrowserUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BROWSERUI_H
