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
	m_AttrHandler = new AttributesHandler(ui);;
	
	return 1;
}

Ui::UiControlHandler::~UiControlHandler()
{
	delete m_AttrHandler;
}
