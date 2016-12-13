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
	QTreeWidgetItem *model = new QTreeWidgetItem(m_ui.treeWidget);
	model->setText(0, "Models");
	m_ui.treeWidget->addTopLevelItem(model);
	QTreeWidgetItem *anim = new QTreeWidgetItem(m_ui.treeWidget);
	anim->setText(0, "Animations");
	m_ui.treeWidget->addTopLevelItem(anim);
	connect(m_ui.treeWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_treeView_doubleClicked()));


	/*connecting the rest of the buttons to the functions*/
	connect(m_ui.actionNew_scene, SIGNAL(triggered()), this, SLOT(on_NewScene_clicked()));
	connect(m_ui.actionLoad_scene, SIGNAL(triggered()), this, SLOT(on_LoadScene_clicked()));
	connect(m_ui.actionSave_scene, SIGNAL(triggered()), this, SLOT(on_SaveScene_clicked()));
	connect(m_ui.actionBuild_BPF, SIGNAL(triggered()), this, SLOT(on_BuildBPF_clicked()));

	this->m_fileImporter = new FileImporter(m_ui.treeWidget);
	this->m_D3DRenderWidget = new D3DRenderWidget(m_ui.RenderWidget, this->m_fileImporter);
	this->m_fileImporter->ImportFromServer();
	this->m_fileImporter->LoadImportedFiles();
	//COMMENT ME BACK TO RENDER TO 2nd WIDGET
	//this->m_D3DRenderWidgetPreview = new D3DRenderWidget(m_ui.RenderWidget_2);
}


SSP_Editor::~SSP_Editor()
{
	delete this->m_model;
	delete this->m_fileImporter;
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
	QModelIndex index = m_ui.treeWidget->currentIndex();
	
	//use index.r to get the right mesh
	/*checking to see if the selected object is valid*/
	if (!index.isValid()) return;
	
	std::vector<Resources::Model*>* test = m_fileImporter->get_M_models();


	DirectX::XMVECTOR pos = {
		-10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - -10))),
		-10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - -10))),
		-10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - -10)))
	};

	this->m_D3DRenderWidget->getCommunicator()->AddModel(test->at(index.row())->GetId(),0,pos,0.0f);

	//QFileInfo fileInfo = this->m_model->fileInfo(index);
	//QString filePath = fileInfo.filePath();

	/*send the filepath to the importer*/

}
