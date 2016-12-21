#ifndef SSP_EDITOR_SSP_EDITOR_H
#define SSP_EDITOR_SSP_EDITOR_H

#include "ui_SSP_Editor.h"
#include <QtWidgets\qmainwindow.h>
#include <qfilesystemmodel.h>
#include <qinputdialog.h>
#include <QKeyEvent>
#include "RenderWidget.h"
#include "FileImporter.h"
#include <qfiledialog.h>
#include <LevelHandler.h>
#include <qmessagebox.h>
#include <qdatetime.h>
	class SSP_Editor : public QMainWindow
	{
		Q_OBJECT

	
	public:
		SSP_Editor(QWidget *parent = Q_NULLPTR);
		~SSP_Editor();
	protected:
		virtual void keyPressEvent(QKeyEvent * evt);
		virtual void keyReleaseEvent(QKeyEvent *evt);
		virtual void closeEvent(QCloseEvent * event);
		virtual void mousePressEvent(QMouseEvent * evt);
		virtual void mouseReleaseEvent(QMouseEvent * evt);
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
		D3DRenderWidget* m_D3DRenderWidget;
		FileImporter* m_fileImporter;
	
		D3DRenderWidget* m_D3DRenderWidgetPreview;
		 QDateTime time;
		QString lastSave = "None made";
		bool PromptSaveLevel();
	};
#endif