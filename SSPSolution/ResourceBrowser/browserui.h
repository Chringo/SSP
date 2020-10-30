#ifndef BROWSERUI_H
#define BROWSERUI_H

#include <QWidget>
#include <qtreewidget.h>
namespace Ui {
class BrowserUI;
}

class BrowserUI : public QWidget
{
    Q_OBJECT

private:
    Ui::BrowserUI *m_ui;	
public:
    explicit BrowserUI(QWidget *parent = 0);
    ~BrowserUI();

	
	QTreeWidget* GetAssetTree() {return nullptr/*m_ui->assetTree*/;}
};

#endif // BROWSERUI_H
