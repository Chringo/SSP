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
		D3DRenderWidget* m_D3DRenderWidget;
	};

	class D3DRenderWidget : public QWidget {
		Q_OBJECT
			Q_DISABLE_COPY(D3DRenderWidget)
	public:
		D3DRenderWidget(QWidget* parent = NULL);
		virtual ~D3DRenderWidget();
		virtual QPaintEngine* paintEngine() const { return NULL; }
	protected:
		virtual void resizeEvent(QResizeEvent* evt);
		virtual void paintEvent(QPaintEvent* evt);
		void CreateDevice();
	};

#endif