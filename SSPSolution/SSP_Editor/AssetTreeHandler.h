#ifndef SSPEDITOR_UI_AssetTreeHandler_H
#define SSPEDITOR_UI_AssetTreeHandler_H
#include "ui_SSP_Editor.h"
#include <qobject.h>
#include "DataHandler.h"
#include "LevelHandler.h"
/*	
	Author: Martin Clementson
	This class handles the asset tree widget to serve our purpose

*/
namespace Ui {

	class AssetTreeHandler : public QObject
	{
		Q_OBJECT
	public:
		enum AssetCategories
		{
			/*things to add:
			General Assets
			Floors
			Ceilings
			Walls
			*/
			GENERAL_ASSETS = 0,
			FLOORS,
			CEILINGS,
			WALLS,
			INTERACTABLE,
			NUM_CATEGORIES
		};
		enum AssetSubCategories
		{
			BRICK = 0,
			STONE,
			PLASTER,
			IRON,
			NUM_SUB_CATEGORIES
		};

	private:
		std::vector<QTreeWidget*> m_treeWidgets;
		AssetTreeHandler();
		void AddSubCategories(QTreeWidgetItem* topLevel);
		bool IsValidItem(QTreeWidget * tree);

	public:
		AssetTreeHandler(QTreeWidget* tree);
		~AssetTreeHandler();
		bool AddItem(AssetCategories type, std::string name, QVariant itemData);
		bool AddItem(AssetCategories type, std::string name, QVariant itemData, AssetSubCategories subType);
		bool AddItem(AssetCategories type, QTreeWidgetItem* item);
		bool AddModel(Resources::Model* model,int& index);
		bool AddModels(std::vector<Resources::Model*>& models);

		void AddTreeWidget(QTreeWidget* tree);//Adds a duplicate tree widget
		//QTreeWidget* GetQTree() {return m_tree;}
	public slots:
		void on_treeView_doubleClicked();
	
	};
}

#endif
