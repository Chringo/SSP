#include "SSP_Editor.h"

SSP_Editor::SSP_Editor(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);
	/*making a fileSystemModel to enable a file viewer in the treeView widget*/
	this->m_model = new QFileSystemModel(this);
	this->m_model->setRootPath("C:/");
	this->m_model->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	setFocusPolicy(Qt::StrongFocus);

	
	Ui::UiControlHandler::GetInstance()->Initialize(&m_ui);
	/*a list of filters for the treeView*/
	QStringList filters;
	filters << "*.bbf";

	/*setting the filters and disabling the user from seeing any other files*/
	this->m_model->setNameFilters(filters);
	this->m_model->setNameFilterDisables(false);

	/*setting the fileSystemModel to the treeView and connecting the signal slot*/
	

	//QTreeWidgetItem* model = new QTreeWidgetItem(m_ui.assetTree);
	//model->setText(0, "Models");	
	//
	//model->setTextAlignment(0,Qt::AlignCenter);
	//m_ui.assetTree->addTopLevelItem(model);		
	//m_ui.assetTree->insertTopLevelItem(0, model);
	//
	//QTreeWidgetItem* anim = new QTreeWidgetItem(m_ui.assetTree);
	//anim->setText(0, "Animations");
	//		
	//anim->setTextAlignment(0, Qt::AlignCenter);
	//m_ui.assetTree->addTopLevelItem(anim);	
	//m_ui.assetTree->insertTopLevelItem(1,anim);
	//m_ui.assetTree->setHeaderLabels(QStringList() << "Resources" );
	//
	//connect(m_ui.assetTree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_treeView_doubleClicked()));
	

	/*connecting the rest of the buttons to the functions*/
	connect(m_ui.actionNew_scene,  SIGNAL(triggered()), this, SLOT(on_NewScene_clicked()));
	connect(m_ui.actionLoad_scene, SIGNAL(triggered()), this, SLOT(on_LoadScene_clicked()));
	connect(m_ui.actionSave_scene, SIGNAL(triggered()), this, SLOT(on_SaveScene_clicked()));
	connect(m_ui.actionBuild_BPF,  SIGNAL(triggered()), this, SLOT(on_BuildBPF_clicked()));

	this->m_fileImporter    = new FileImporter(m_ui.assetTree);
	this->m_D3DRenderWidget = new D3DRenderWidget(m_ui.RenderWidget, this->m_fileImporter);
	this->m_fileImporter->Initialize();
	 
	//COMMENT ME BACK TO RENDER TO 2nd WIDGET
	//this->m_D3DRenderWidgetPreview = new D3DRenderWidget(m_ui.RenderWidget_2);
	QString title = "Level: ";
	title.append(QString::fromStdString(*LevelHandler::GetInstance()->GetCurrentLevel()->GetName()));
	this->window()->setWindowTitle(title);
}

void SSP_Editor::keyPressEvent(QKeyEvent * evt)
{
	this->m_D3DRenderWidget->keyPressEvent(evt);
}
void SSP_Editor::keyReleaseEvent(QKeyEvent *evt)
{
	this->m_D3DRenderWidget->keyReleaseEvent(evt);
}
void SSP_Editor::closeEvent(QCloseEvent * event)
{
	if (LevelHandler::GetInstance()->GetCurrentLevel()->isEmpty() == false)
	{
		PromptSaveLevel();
	}
}
void SSP_Editor::resizeEvent(QResizeEvent * event)
{
	m_D3DRenderWidget->resizeEvent(event);
}
void SSP_Editor::mousePressEvent(QMouseEvent * evt)
{
	this->m_D3DRenderWidget->mousePressEvent(evt);
}
void SSP_Editor::mouseReleaseEvent(QMouseEvent * evt)
{
	this->m_D3DRenderWidget->mouseReleaseEvent(evt);
}
SSP_Editor::~SSP_Editor()
{
	delete this->m_model;
	delete this->m_fileImporter;
	delete m_D3DRenderWidget;
	m_ui.assetTree->clear();

}



void SSP_Editor::on_NewScene_clicked()
{
	//Prompt the user if they want to save the current level before creating a new scene
	if (LevelHandler::GetInstance()->GetCurrentLevel()->isEmpty() == false)
	{
		if (!PromptSaveLevel()) //returns false if the user cancels the operation
			return;
	}
	//Create the new level
	LevelHandler::GetInstance()->NewLevel();
	QString title = "Level: untitled_level";
	this->window()->setWindowTitle(title);
	lastSave = "None performed";
}

void SSP_Editor::on_LoadScene_clicked()
{
	if (LevelHandler::GetInstance()->GetCurrentLevel()->isEmpty() == false)
	{
		if (!PromptSaveLevel()) //returns false if the user cancels the operation
			return;
	}
	
	lastSave = "None performed";

	LevelData::LevelStatus stat = LevelHandler::GetInstance()->ImportLevelFile();
	if (stat == LevelData::LevelStatus::L_OK)
	{
		QString title = "Level: "; //Set new window title
		title.append(QString::fromStdString(*LevelHandler::GetInstance()->GetCurrentLevel()->GetName()));
		this->window()->setWindowTitle(title);
	}
}

void SSP_Editor::on_SaveScene_clicked()
{

	LevelData::LevelStatus stat = LevelHandler::GetInstance()->ExportLevelFile();
	if (stat == LevelData::LevelStatus::L_OK)
	{
		QString format = "hh:mm:ss";
		lastSave = time.currentDateTime().toString(format);
		QString title = "Level: ";
		title.append(QString::fromStdString(*LevelHandler::GetInstance()->GetCurrentLevel()->GetName()));
		this->window()->setWindowTitle(title);
	}

}

void SSP_Editor::on_BuildBPF_clicked()
{
}

bool SSP_Editor::PromptSaveLevel()
{
	QString format = "hh:mm:ss";
	QMessageBox msgBox(this);
	msgBox.setText("Do you want to save the current level?");
	QString last = "Last save performed at : ";
	last.append(lastSave);
	msgBox.setInformativeText(last);
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
	switch (ret) {
	case QMessageBox::Save:
	{

		LevelData::LevelStatus stat = LevelHandler::GetInstance()->ExportLevelFile();
		if (stat == LevelData::LevelStatus::L_OK)
		{
			QString format = "hh:mm:ss";
			lastSave = time.currentDateTime().toString(format);
			QString title = "Level: ";
			title.append(QString::fromStdString(*LevelHandler::GetInstance()->GetCurrentLevel()->GetName()));
			this->window()->setWindowTitle(title);
		}
		break;
	}
	case QMessageBox::Cancel:
		return false; 
		break;
	}
	return true;
}

void SSP_Editor::on_treeView_doubleClicked()
{
	if (m_ui.assetTree->currentItem()->parent() == NULL) //If a category window is clicked
		return;
	QModelIndex index = m_ui.assetTree->currentIndex();
	
	//use index.r to get the right mesh
	/*checking to see if the selected object is valid*/
	if (!index.isValid()) return;

	std::vector<Resources::Model*>* test = DataHandler::GetInstance()->GetModels(); //m_fileImporter->get_M_models();


	DirectX::XMVECTOR pos = {
		0.0f,0.0f,0.0f
	};
	DirectX::XMVECTOR rot = {
		0.0f,0.0f,0.0f
	};

	this->m_D3DRenderWidget->getCommunicator()->AddModel(test->at(index.row())->GetId(),0,pos, rot);

	//QFileInfo fileInfo = this->m_model->fileInfo(index);
	//QString filePath = fileInfo.filePath();

	/*send the filepath to the importer*/

}
