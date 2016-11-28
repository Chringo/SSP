#include "SSP_Editor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Editor::SSP_Editor w;
    w.show();
    return a.exec();
}
