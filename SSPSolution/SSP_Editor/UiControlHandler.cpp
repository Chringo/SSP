#include "UiControlHandler.h"



Ui::UiControlHandler::UiControlHandler()
{
}




Ui::UiControlHandler * Ui::UiControlHandler::GetInstance()
{
	static UiControlHandler instance;
	return &instance;
}

int Ui::UiControlHandler::Initialize(Ui::SSP_EditorClass * ui)
{
	m_AttrHandler = new AttributesHandler(ui);
	m_AssetTree   = new AssetTreeHandler(ui->assetTree);
	
	return 1;
}

Ui::UiControlHandler::~UiControlHandler()
{
	delete m_AssetTree;
	delete m_AttrHandler;
}
