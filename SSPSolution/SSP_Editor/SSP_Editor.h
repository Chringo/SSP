#ifndef SSP_EDITOR_SSP_EDITOR_H
#define SSP_EDITOR_SSP_EDITOR_H

#include "ui_SSP_Editor.h"
#include <QtWidgets\qmainwindow.h>
#include <qfilesystemmodel.h>
#include <qinputdialog.h>
#include <QKeyEvent>
#include "D3DRenderWidget.h"
#include "FileImporter.h"
#include <qfiledialog.h>
#include <LevelHandler.h>
#include <qmessagebox.h>
#include <qdatetime.h>
#include "UiControlHandler.h"
#include "ResourceLibExporter.h"
#include "resourcebrowser.h"
	class SSP_Editor : public QMainWindow
	{
		Q_OBJECT

	
	public:
		SSP_Editor(QApplication* app,QWidget *parent = Q_NULLPTR);
		~SSP_Editor();
		//void OpenBrowser();
	protected:
		bool eventFilter(QObject *target, QEvent *evt);
		virtual void keyPressEvent(QKeyEvent * evt);
		virtual void keyReleaseEvent(QKeyEvent *evt);
		virtual void closeEvent(QCloseEvent * event);
		virtual void resizeEvent(QResizeEvent *event);
		virtual void mousePressEvent(QMouseEvent * evt);
		virtual void mouseReleaseEvent(QMouseEvent * evt);


		
	public slots:
	void on_Browse_clicked();
	void on_NewScene_clicked();
	void on_LoadScene_clicked();
	void on_SaveScene_clicked();
	void on_BuildBPF_clicked();


	private:
		ResourceBrowser	m_resBrowser;
		Ui::SSP_EditorClass m_ui;
		QFileSystemModel *m_model;
		QModelIndex *m_item;
		D3DRenderWidget* m_D3DRenderWidget;
		FileImporter* m_fileImporter;
		ResourceLibExporter* m_resourceLibExporter = ResourceLibExporter::GetInstance();
		QApplication* m_qApp;
		D3DRenderWidget* m_D3DRenderWidgetPreview;
		 QDateTime time;
		QString lastSave = "None made";
		bool PromptSaveLevel();

		
	};
#endif