#include "SSP_Editor.h"

SSP_Editor::SSP_Editor(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	/*making a fileSystemModel to enable a file viewer in the treeView widget*/
	this->m_model = new QFileSystemModel(this);
	this->m_model->setRootPath("C:/");
	this->m_model->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	
	/*a list of filters for the treeView*/
	QStringList filters;
	filters << "*.bbf";

	/*setting the filters and disabling the user from seeing any other files*/
	this->m_model->setNameFilters(filters);
	this->m_model->setNameFilterDisables(false);

	/*setting the fileSystemModel to the treeView and connecting the signal slot*/
	m_ui.treeView->setModel(this->m_model);
	connect(m_ui.treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(treeView_doubleClicked()));

	/*connecting the rest of the buttons to the functions*/
	connect(m_ui.actionNew_scene, SIGNAL(triggered()), this, SLOT(on_NewScene_clicked()));
	connect(m_ui.actionLoad_scene, SIGNAL(triggered()), this, SLOT(on_LoadScene_clicked()));
	connect(m_ui.actionSave_scene, SIGNAL(triggered()), this, SLOT(on_SaveScene_clicked()));
	connect(m_ui.actionBuild_BPF, SIGNAL(triggered()), this, SLOT(on_BuildBPF_clicked()));

	this->m_D3DRenderWidget = new D3DRenderWidget(m_ui.graphicsView);

}


SSP_Editor::~SSP_Editor()
{
	delete this->m_model;
	delete this->m_D3DRenderWidget;
}

void SSP_Editor::on_NewScene_clicked()
{
}

void SSP_Editor::on_LoadScene_clicked()
{
}

void SSP_Editor::on_SaveScene_clicked()
{
}

void SSP_Editor::on_BuildBPF_clicked()
{
}

void SSP_Editor::on_treeView_doubleClicked()
{
	QModelIndex index = m_ui.treeView->currentIndex();
	
	/*checking to see if the selected object is valid*/
	if (!index.isValid()) return;
	
	QFileInfo fileInfo = this->m_model->fileInfo(index);
	QString filePath = fileInfo.filePath();

	/*send the filepath to the importer*/

}

D3DRenderWidget::D3DRenderWidget(QWidget* parent)
	: QWidget(parent) {
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);

	// Create Device
	CreateDevice();
}
void D3DRenderWidget::CreateDevice()
{

}
void D3DRenderWidget::paintEvent(QPaintEvent* evt) {
	//render();
}
void D3DRenderWidget::resizeEvent(QResizeEvent* evt) {
	//releaseBuffers();
	//swapChain_->ResizeBuffers(1, width(), height(), swapChainDesc_.BufferDesc.Format, 0);
	//swapChain_->GetDesc(&swapChainDesc_);
	//viewport_.Width = width();
	//viewport_.Height = height();
	//createBuffers();
}