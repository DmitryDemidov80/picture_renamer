#include "mainwinrenamer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if(QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    {
        //QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        //QApplication::setAttribute(Qt::AA_Use96Dpi);
    }
#endif
    QApplication a(argc, argv);
    MainWinRenamer w;
    w.showMaximized();

    return a.exec();
}
