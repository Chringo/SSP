#ifndef SSPEDITOR_UI_UICONTROLHANDLER_H
#define SSPEDITOR_UI_UICONTROLHANDLER_H
#include "AttributesHandler.h"
#include "AssetTreeHandler.h"
/*
	Author: Martin Clementson

	This class holds all the necessary classes to control the user interface.

*/
namespace Ui {

	class UiControlHandler
	{
	private:
		AttributesHandler* m_AttrHandler;
		Ui::SSP_EditorClass* ui;

	public:
		UiControlHandler(Ui::SSP_EditorClass* ui);
		~UiControlHandler();


	private:
		UiControlHandler();
	};
}

#endif

