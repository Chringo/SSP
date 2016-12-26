#include "SSP_Editor.h"
#include <QtWidgets\qapplication.h>

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


int main(int argc, char *argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    QApplication a(argc, argv);
    SSP_Editor w;
    w.show();
    return a.exec();
}
