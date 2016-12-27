#ifndef SSPEDITOR_UI_AssetTreeHandler_H
#define SSPEDITOR_UI_AssetTreeHandler_H
#include "ui_SSP_Editor.h"
/*	
	Author: Martin Clementson
	This class handles the asset tree widget to serve our purpose

*/
namespace Ui {

	class AssetTreeHandler
	{
	public:
		enum AssetCategories
		{
			MODELS = 0,
			ANIMATIONS,
			LEVELS,
			NUM_CATEGORIES
		};

	private:
		QTreeWidget* tree;
		AssetTreeHandler();
	public:
		AssetTreeHandler(QTreeWidget* tree);
		~AssetTreeHandler();
	};
}

#endif
