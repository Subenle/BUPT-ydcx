#include "ydcx.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ydcx w;
    w.show();

    return a.exec();
}
