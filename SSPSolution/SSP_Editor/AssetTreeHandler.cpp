#include "AssetTreeHandler.h"



Ui::AssetTreeHandler::AssetTreeHandler()
{
}


Ui::AssetTreeHandler::AssetTreeHandler(QTreeWidget * tree)
{
	/*Defining m_tree and disabling sorting for manual sorting*/
	this->m_tree = tree;
	m_tree->setSortingEnabled(false);


	/*Creating the "General Assets" tab*/
	QTreeWidgetItem* g_Assets = new QTreeWidgetItem(tree);
	g_Assets->setText(0, "General Assets");

	g_Assets->setTextAlignment(0, Qt::AlignCenter);
	m_tree->addTopLevelItem(g_Assets);
	m_tree->insertTopLevelItem(GENERAL_ASSETS, g_Assets);


	/*Creating the "Floors" tab*/
	QTreeWidgetItem* floors = new QTreeWidgetItem(tree);
	floors->setText(0, "Floors");

	floors->setTextAlignment(0, Qt::AlignCenter);
	m_tree->addTopLevelItem(floors);
	m_tree->insertTopLevelItem(FLOORS, floors);
	m_tree->setHeaderLabels(QStringList() << "Resources");


	/*Creating the "Ceilings" tab*/
	QTreeWidgetItem* ceilings = new QTreeWidgetItem(tree);
	ceilings->setText(0, "Ceilings");

	ceilings->setTextAlignment(0, Qt::AlignCenter);
	m_tree->addTopLevelItem(ceilings);
	m_tree->insertTopLevelItem(CEILINGS, ceilings);
	

	/*Creating the "Walls" tab*/
	QTreeWidgetItem* walls = new QTreeWidgetItem(tree);
	walls->setText(0, "Walls");

	walls->setTextAlignment(0, Qt::AlignCenter);
	m_tree->addTopLevelItem(walls);
	m_tree->insertTopLevelItem(WALLS, walls);


	/*Creating the "Interactable" tab*/
	QTreeWidgetItem* interactable = new QTreeWidgetItem(tree);
	interactable->setText(0, "Interactable");

	interactable->setTextAlignment(0, Qt::AlignCenter);
	m_tree->addTopLevelItem(interactable);
	m_tree->insertTopLevelItem(INTERACTABLE, interactable);

	/*connecting the doubleclicked signal to the "on_treeview_doubleclicked function*/
	connect(m_tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_treeView_doubleClicked()));

}

Ui::AssetTreeHandler::~AssetTreeHandler()
{

}

bool Ui::AssetTreeHandler::AddItem(AssetCategories type, std::string name, QVariant itemData)
{

	QTreeWidgetItem *itm = new QTreeWidgetItem();
	
	itm->setData(0, Qt::ItemDataRole::UserRole, itemData);
	itm->setText(0, name.substr(0, name.rfind(".")).c_str());
	m_tree->topLevelItem((int)type)->addChild(itm);
	return true;
}

bool Ui::AssetTreeHandler::AddItem(AssetCategories type, QTreeWidgetItem * item)
{
	m_tree->topLevelItem((int)type)->addChild(item);
	return true;
}

void Ui::AssetTreeHandler::on_treeView_doubleClicked() {

	if (m_tree->currentItem()->parent() == NULL) //If a category window is clicked
		return;
	QModelIndex index = m_tree->currentIndex();

	//use index.r to get the right mesh
	/*checking to see if the selected object is valid*/
	
	if (!index.isValid()) return;
	int modelIndex = m_tree->currentItem()->data(0, Qt::ItemDataRole::UserRole).toInt();
	std::vector<Resources::Model*>* test = DataHandler::GetInstance()->GetModels(); 


	DirectX::XMVECTOR pos = {
		0.0f,0.0f,0.0f
	};
	DirectX::XMVECTOR rot = {
		0.0f,0.0f,0.0f
	};
	LevelHandler::GetInstance()->GetCurrentLevel()->AddModelEntity(test->at(modelIndex)->GetId(), pos, rot);

	//QFileInfo fileInfo = this->m_model->fileInfo(index);
	//QString filePath = fileInfo.filePath();

	/*send the filepath to the importer*/


}