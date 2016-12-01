#include "SSP_Editor.h"
#include <QtWidgets\qapplication.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SSP_Editor w;
    w.show();
    return a.exec();
}
