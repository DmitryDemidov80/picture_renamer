#include "mainwinrenamer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    //QApplication::setAttribute(Qt::AA_Use96Dpi);
    QApplication a(argc, argv);
    MainWinRenamer w;
    w.showMaximized();

    return a.exec();
}
