#ifndef MAINWINRENAMER_H
#define MAINWINRENAMER_H

#include <QWidget>
#include <QTableWidget>
#include <QFile>
#include <QDir>
#include <QStringList>
#include "editorwgt.h"
#include "directoryreader.h"

class EditorWgt;
class QProgressBar;
class QThread;

/**
 * @brief The MainWinRenamer class - основное окно программы.
 */
class MainWinRenamer : public QWidget
{
    Q_OBJECT

    QTableWidget *tblWgt;
    EditorWgt *editor;
    int currentRow;
    QFile currentFile;
    QProgressBar *prBar;
    DirectoryReader dir_reader_;
    QThread *thread_;
    QStringList old_file_names;
    bool is_auto_rename;

    /**
     * @brief clearTable - удаляет содержимое таблицы вместе со строками.
     */
    void clearTable();
    //QString EXIF_ErrorCodeToString(int code);
public:
    MainWinRenamer(QWidget *parent = nullptr);
    ~MainWinRenamer();

private slots:
    void sltCurrentDirectoryChoosed(const QString &cdir);
    void sltTblCellClicked(int row, int col);
    void sltFileNameChanged(const QString &newName);
    void sltAutoRename(const QString &oldName, const QString &newName);
    //void fillTable();
    void onRowReady(int row_num, const QString &fname, const QString &size, const QString &date, const QString &camera_brand, const QString &camera_model, int percent);
    void onAnalyzeDirectoryFinished();
    void file_dropped_from_folder(const QString &fname);

signals:
    void sgnPicClicked(QString fname);
    void filling_finished();

protected:
    virtual void resizeEvent(QResizeEvent * event);

private:

    void createProgressBar();

};

#endif // MAINWINRENAMER_H
