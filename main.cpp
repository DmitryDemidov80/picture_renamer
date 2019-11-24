#include "mainwinrenamer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWinRenamer w;
    w.show();

    return a.exec();
}
