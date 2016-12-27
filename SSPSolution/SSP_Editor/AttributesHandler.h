#ifndef SSPEDITOR_UI_ATTRHANDLER_H
#define SSPEDITOR_UI_ATTRHANDLER_H

/*
	Author: Martin Clementson

	This class is the main control of the attributes frame of the UI. The purpose of the class is to update the  ui information when a new 
	object is selected, or the current selected object has had its values changed, for example by moving or rotating it.
	However, This class also needs to take care of inputs that the user can give in the attributes frame. Such as manually enter a translation
	,rotation, scale , new name and in some cases change animation.


	I propose another class with a (desciptive) name such as "SelectionHandler" this potential class would handle all the information
	of the current selected object. The AttributesHandler would need to communicate with the SelectionHandler both ways, prefferably using 
	signals and slots.

*/

namespace UI {
	class AttributesHandler
	{
	public:
		AttributesHandler();
		~AttributesHandler();
	};
}

#endif