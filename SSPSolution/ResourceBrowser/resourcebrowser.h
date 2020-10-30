#ifndef RESOURCEBROWSER_H
#define RESOURCEBROWSER_H

#include "resourceBrowser_global.h"
#include "browserUi.h"
#include <qtreewidget.h>
class RESOURCEBROWSERSHARED_EXPORT ResourceBrowser
{
private:
    BrowserUI m_browserUI;
public:
    ResourceBrowser();

    void OpenBrowser();
	
	BrowserUI* GetUI() {return &m_browserUI;};
};

#endif // RESOURCEBROWSER_H
