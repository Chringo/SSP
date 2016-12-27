#include "AssetTreeHandler.h"



Ui::AssetTreeHandler::AssetTreeHandler()
{
}


Ui::AssetTreeHandler::AssetTreeHandler(QTreeWidget * tree)
{
	this->m_tree = tree;

	QTreeWidgetItem* model = new QTreeWidgetItem(tree);
	model->setText(0, "Models");

	model->setTextAlignment(0, Qt::AlignCenter);
	m_tree->addTopLevelItem(model);
	m_tree->insertTopLevelItem(MODELS, model);

	QTreeWidgetItem* anim = new QTreeWidgetItem(tree);
	anim->setText(0, "Animations");

	anim->setTextAlignment(0, Qt::AlignCenter);
	m_tree->addTopLevelItem(anim);
	m_tree->insertTopLevelItem(ANIMATIONS, anim);
	m_tree->setHeaderLabels(QStringList() << "Resources");


	connect(m_tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_treeView_doubleClicked()));

}

Ui::AssetTreeHandler::~AssetTreeHandler()
{
}

bool Ui::AssetTreeHandler::AddItem(AssetCategories type, std::string name)
{

	QTreeWidgetItem *itm = new QTreeWidgetItem();
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

	std::vector<Resources::Model*>* test = DataHandler::GetInstance()->GetModels(); 


	DirectX::XMVECTOR pos = {
		0.0f,0.0f,0.0f
	};
	DirectX::XMVECTOR rot = {
		0.0f,0.0f,0.0f
	};
	LevelHandler::GetInstance()->GetCurrentLevel()->AddModelEntity(test->at(index.row())->GetId(), 0, pos, rot);

	//QFileInfo fileInfo = this->m_model->fileInfo(index);
	//QString filePath = fileInfo.filePath();

	/*send the filepath to the importer*/


}