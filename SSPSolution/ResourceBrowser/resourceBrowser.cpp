#include "resourceBrowser.h"
#include <iostream>

ResourceBrowser::ResourceBrowser()
{
}

void ResourceBrowser::OpenBrowser()
{
	std::cout<< "\n HEllo FROM browser" << std::endl;
	std::cout << "\n UPDATE!" << std::endl;
    m_browserUI.show();
}
