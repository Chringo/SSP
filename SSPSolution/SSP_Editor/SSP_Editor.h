#ifndef SSP_EDITOR_SSP_EDITOR_H
#define SSP_EDITOR_SSP_EDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_SSP_Editor.h"
#include <qfilesystemmodel.h>
#include <qinputdialog.h>

class SSP_Editor : public QMainWindow
{
    Q_OBJECT

public:
    SSP_Editor(QWidget *parent = Q_NULLPTR);
	~SSP_Editor();

public slots:
void on_treeView_doubleClicked();
void on_NewScene_clicked();
void on_LoadScene_clicked();
void on_SaveScene_clicked();
void on_BuildBPF_clicked();

private:
    Ui::SSP_EditorClass m_ui;
	QFileSystemModel *m_model;
	QModelIndex *m_item;
};
#endif