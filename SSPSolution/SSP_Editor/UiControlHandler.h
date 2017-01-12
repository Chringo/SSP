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
		AttributesHandler* m_AttrHandler = nullptr;
		AssetTreeHandler* m_AssetTree    = nullptr;
		Ui::SSP_EditorClass* ui          = nullptr;
	public:
		static UiControlHandler* GetInstance();
		int Initialize(Ui::SSP_EditorClass* ui);

		AssetTreeHandler* GetAssetTreeController() { return m_AssetTree; };
		AttributesHandler* GetAttributesHandler() { return m_AttrHandler; };
		~UiControlHandler();


	private:
		UiControlHandler();
	};
}

#endif

