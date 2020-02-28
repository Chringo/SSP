#ifndef RESOURCEBROWSER_H
#define RESOURCEBROWSER_H

#include "resourceBrowser_global.h"
#include "browserUi.h"
class RESOURCEBROWSERSHARED_EXPORT ResourceBrowser
{
private:
    BrowserUI m_browserUI;
public:
    ResourceBrowser();

    void OpenBrowser();
};

#endif // RESOURCEBROWSER_H
