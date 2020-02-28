#include "browserUi.h"
#include "ui_browserui.h"

BrowserUI::BrowserUI(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::BrowserUI)
{
    m_ui->setupUi(this);
}

BrowserUI::~BrowserUI()
{
    delete m_ui;
}

void BrowserUI::ShowUi()
{

}
