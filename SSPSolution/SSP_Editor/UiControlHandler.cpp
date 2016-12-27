#include "UiControlHandler.h"



Ui::UiControlHandler::UiControlHandler()
{
}


Ui::UiControlHandler::UiControlHandler(Ui::SSP_EditorClass * ui)
{
	this->m_AttrHandler = new AttributesHandler(ui);
}

Ui::UiControlHandler::~UiControlHandler()
{
	delete m_AttrHandler;
}
