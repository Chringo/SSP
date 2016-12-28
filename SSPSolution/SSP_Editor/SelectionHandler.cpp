#include "SelectionHandler.h"

SelectionHandler::SelectionHandler()
{
}

SelectionHandler::~SelectionHandler()
{
}

SelectionHandler * SelectionHandler::GetInstance()
{
	static SelectionHandler selectionHandler;

	return &selectionHandler;
}
