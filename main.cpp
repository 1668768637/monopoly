#include "monopolygame.h"
#include <QApplication>
#include <QLibrary>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    monopolyGame w;
    w.show();

    return a.exec();
}
