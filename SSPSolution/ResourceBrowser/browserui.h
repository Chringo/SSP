#ifndef BROWSERUI_H
#define BROWSERUI_H

#include <QWidget>

namespace Ui {
class BrowserUI;
}

class BrowserUI : public QWidget
{
    Q_OBJECT

public:
    explicit BrowserUI(QWidget *parent = 0);
    ~BrowserUI();
    void ShowUi();

private:
    Ui::BrowserUI *m_ui;
};

#endif // BROWSERUI_H
