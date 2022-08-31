#ifndef DIRECTORYREADER_H
#define DIRECTORYREADER_H
#include <QString>
#include <QObject>
#include <QFile>
#include <QDir>
#include <tuple>

class DirectoryReader : public QObject
{
    Q_OBJECT
public:
    explicit DirectoryReader(QObject *parent = nullptr);
    void setDirectory(const QString &dir) noexcept;
    QDir current_directory() const noexcept;

    std::tuple<QString, QString, QString, QString, QString> analyze_file(const QString &fname);

signals:

    void analyze_finished();
    void rowDataReady(int row_num,const QString &fname, const QString &size, const QString &date, const QString &camera_brand, const QString &camera_model, int percent);

public slots:
    void analyzeDirectory();

private:
    QString EXIF_ErrorCodeToString(int code) const;

private:
    QFile currentFile;
    QDir currentDir;
};

#endif // DIRECTORYREADER_H
