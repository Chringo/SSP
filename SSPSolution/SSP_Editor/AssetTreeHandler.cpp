#include "AssetTreeHandler.h"



Ui::AssetTreeHandler::AssetTreeHandler()
{
}

void Ui::AssetTreeHandler::AddSubCategories(QTreeWidgetItem * topLevel)
{
	QTreeWidgetItem* brick = new QTreeWidgetItem();
	brick->setText(0, "Bricks");
	brick->setTextAlignment(0, Qt::AlignCenter);
	topLevel->addChild(brick);
	topLevel->insertChild(BRICK, brick);

	QTreeWidgetItem* stone = new QTreeWidgetItem();
	stone->setText(0, "Stones");
	stone->setTextAlignment(0, Qt::AlignCenter);
	topLevel->addChild(stone);
	topLevel->insertChild(STONE, stone);

	QTreeWidgetItem* plaster = new QTreeWidgetItem();
	plaster->setText(0, "Plaster");
	plaster->setTextAlignment(0, Qt::AlignCenter);
	topLevel->addChild(plaster);
	topLevel->insertChild(PLASTER, plaster);

	QTreeWidgetItem* iron = new QTreeWidgetItem();
	iron->setText(0, "Iron");
	iron->setTextAlignment(0, Qt::AlignCenter);
	topLevel->addChild(iron);
	topLevel->insertChild(IRON, iron);
}

bool Ui::AssetTreeHandler::IsValidItem(QTreeWidget * tree)
{
	if (tree->currentItem()->parent() == NULL) //If a category window is clicked
		return false;
	if (tree->currentItem()->text(0) == QString("Bricks"))
		return false;
	if (tree->currentItem()->text(0) == QString("Stones"))
		return false;
	if (tree->currentItem()->text(0) == QString("Plaster"))
		return false;
	if (tree->currentItem()->text(0) == QString("Iron"))
		return false;
	return true;
}


Ui::AssetTreeHandler::AssetTreeHandler(QTreeWidget * tree)
{
	/*Defining m_tree and disabling sorting for manual sorting*/
	m_treeWidgets.push_back(tree);

	auto* m_tree = m_treeWidgets.front();
	m_tree->setSortingEnabled(false);


	/*Creating the "General Assets" tab*/
	QTreeWidgetItem* g_Assets = new QTreeWidgetItem(tree);
	g_Assets->setText(0, "General Assets");

	g_Assets->setTextAlignment(0, Qt::AlignCenter);
	m_tree->addTopLevelItem(g_Assets);
	m_tree->insertTopLevelItem(GENERAL_ASSETS, g_Assets);


	/*Creating the "Floors" tab also adding subcategories to this category*/
	QTreeWidgetItem* floors = new QTreeWidgetItem(tree);
	floors->setText(0, "Floors");
	floors->setTextAlignment(0, Qt::AlignCenter);

	this->AddSubCategories(floors);

	m_tree->addTopLevelItem(floors);
	m_tree->insertTopLevelItem(FLOORS, floors);
	m_tree->setHeaderLabels(QStringList() << "Resources");


	/*Creating the "Ceilings" tab also adding subcategories to this category*/
	QTreeWidgetItem* ceilings = new QTreeWidgetItem(tree);
	ceilings->setText(0, "Ceilings");
	ceilings->setTextAlignment(0, Qt::AlignCenter);

	this->AddSubCategories(ceilings);

	m_tree->addTopLevelItem(ceilings);
	m_tree->insertTopLevelItem(CEILINGS, ceilings);


	/*Creating the "Walls" tab also adding subcategories to this category*/
	QTreeWidgetItem* walls = new QTreeWidgetItem(tree);
	walls->setText(0, "Walls");
	walls->setTextAlignment(0, Qt::AlignCenter);

	this->AddSubCategories(walls);

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
	for (size_t i = 0; i < m_treeWidgets.size(); i++)
	{
		auto * m_tree = m_treeWidgets[i];
		QTreeWidgetItem *itm = new QTreeWidgetItem();
		
		itm->setData(0, Qt::ItemDataRole::UserRole, itemData);
		itm->setText(0, name.substr(0, name.rfind(".")).c_str());
		m_tree->topLevelItem((int)type)->addChild(itm);
	}
	return true;
}

bool Ui::AssetTreeHandler::AddItem(AssetCategories type, std::string name, QVariant itemData, AssetSubCategories subType)
{
	for (size_t i = 0; i < m_treeWidgets.size(); i++)
	{
		auto * m_tree = m_treeWidgets[i];
		QTreeWidgetItem *itm = new QTreeWidgetItem();

		itm->setData(0, Qt::ItemDataRole::UserRole, itemData);
		itm->setText(0, name.substr(0, name.rfind(".")).c_str());
		m_tree->topLevelItem((int)type)->child((int)subType)->addChild(itm);
	}
	return true;
}

bool Ui::AssetTreeHandler::AddItem(AssetCategories type, QTreeWidgetItem * item)
{
	for (size_t i = 0; i < m_treeWidgets.size(); i++)
	{
		auto * m_tree = m_treeWidgets[i];
		m_tree->topLevelItem((int)type)->addChild(item);
	}
	return true;
}

bool Ui::AssetTreeHandler::AddModel(Resources::Model * model, int& index)
{
	

	return AddItem(AssetCategories::GENERAL_ASSETS, std::to_string(model->GetId()), QVariant(index));
}

bool Ui::AssetTreeHandler::AddModels(std::vector<Resources::Model*>& models)
{

	for (int i = 0; i < models.size(); i++)
	{
		if (!this->AddModel(models[i], i)) {
			_CrtDbgBreak();
		}
	}

	return true;
}

void Ui::AssetTreeHandler::AddTreeWidget(QTreeWidget * tree)
{
}

void Ui::AssetTreeHandler::on_treeView_doubleClicked() 
{
	//Qt::ItemFlag::ItemIsDropEnabled
	
	auto * m_tree = m_treeWidgets.front();
	if (!this->IsValidItem(m_tree))
		return;		//If a category window is clicked

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