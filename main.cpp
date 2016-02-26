#include <QApplication>

#include "grapher2d.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Grapher2D w;
    w.show();

    return a.exec();
}
