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
			MODELS = 0,
			ANIMATIONS,
			LEVELS,
			NUM_CATEGORIES
		};

	private:
		QTreeWidget* m_tree;
		AssetTreeHandler();
	public:
		AssetTreeHandler(QTreeWidget* tree);
		~AssetTreeHandler();
		bool AddItem(AssetCategories type, std::string name);
		bool AddItem(AssetCategories type, QTreeWidgetItem* item);
		
	public slots:
		void on_treeView_doubleClicked();
	
	};
}

#endif
