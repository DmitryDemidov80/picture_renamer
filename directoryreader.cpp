#include "directoryreader.h"
#include "exif.h"
#include <QDebug>

DirectoryReader::DirectoryReader(QObject *parent) : QObject(parent)
{

}

void DirectoryReader::setDirectory(const QString &dir) noexcept
{
    currentDir.setPath(dir);
}

QDir DirectoryReader::current_directory() const noexcept
{
    return currentDir;
}

std::tuple<QString, QString, QString, QString> DirectoryReader::analyze_file(const QString &fname)
{
    easyexif::EXIFInfo info;
    QFile file(fname);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    if (int code = info.parseFrom((unsigned char *)data.data(), data.size()))
    {
        qDebug() << "Error parsing EXIF: code " << EXIF_ErrorCodeToString(code);
    }
    // Дата exif
    QString date(info.DateTimeOriginal.c_str());
    file.close();

    // Имя
    //QString name(fname);

    //Размер
    QFileInfo fi(fname);
    double size=fi.size()/1024.0/1024.0;
    QString str_size=QString().setNum(size)+"МБ";


    // камера
    auto camera_brand = info.Make;
    auto camera_model = info.Model;
    return std::tuple(str_size, date, QString(camera_brand.c_str()), QString(camera_model.c_str()));
}

void DirectoryReader::analyzeDirectory()
{
    QStringList lst;
    lst << "*.jpg" << "*.jpeg" << "*.bmp" << "*.png" << "*.tiff";
    QStringList flst(currentDir.entryList(lst, QDir::Files, QDir::Name));
    for (auto i=0; i<flst.size(); ++i)
    {
        auto percents = i*100/flst.size()+1;
        easyexif::EXIFInfo info;
        QFile file(flst[i]);
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        if (int code = info.parseFrom((unsigned char *)data.data(), data.size()))
        {
            qDebug() << "Error parsing EXIF: code " << EXIF_ErrorCodeToString(code);
        }
        // Дата exif
        QString date(info.DateTimeOriginal.c_str());
        file.close();

        // Имя
        QString name(flst[i]);

        //Размер
        QFileInfo fi(flst[i]);
        double size=fi.size()/1024.0/1024.0;
        QString str_size=QString().setNum(size)+"МБ";


        // камера
        auto camera_brand = info.Make;
        auto camera_model = info.Model;

        emit rowDataReady(i, name, str_size, date, camera_brand.c_str(), camera_model.c_str(), percents);

    }
    emit analyze_finished();
}

QString DirectoryReader::EXIF_ErrorCodeToString(int code) const
{
    QString res;
    switch (code)
    {
        case PARSE_EXIF_SUCCESS:
        {
            res="PARSE_EXIF_SUCCESS";
            break;
        }
        case PARSE_EXIF_ERROR_NO_JPEG:
        {
            res="PARSE_EXIF_ERROR_NO_JPEG";
            break;
        }
        case PARSE_EXIF_ERROR_NO_EXIF:
        {
            res="PARSE_EXIF_ERROR_NO_EXIF";
            break;
        }
        case PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN:
        {
            res="PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN";
            break;
        }
        case PARSE_EXIF_ERROR_CORRUPT:
        {
            res="PARSE_EXIF_ERROR_CORRUPT";
            break;
        }
    }
    return res;
}
