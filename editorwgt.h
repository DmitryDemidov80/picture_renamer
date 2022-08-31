#ifndef EDITORWGT_H
#define EDITORWGT_H

#include <QWidget>
#include <QDir>
#include <QStringList>

class QImage;
class QGroupBox;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;
class PixViewer;

class EditorWgt : public QWidget
{
    Q_OBJECT

    QLabel *lblOriginalName;
    //QLabel*lblPix;
    PixViewer *lblPix;
    QLineEdit *leYear;
    QPushButton *bOK;
    QPushButton *bOpenFolder;
    QComboBox *cmbProducer; // Издательство
    QComboBox *cmbSection; // Раздел
    QDir currentDir;
    QImage currentImg;
	QLineEdit *leExifDate;
    QCheckBox *chkAutoRenameAllFiles;
	QGroupBox *grbxExifDate;
	QGroupBox *grbxPaint;
    QStringList lstFileNames;

public:
    explicit EditorWgt(QWidget *parent = nullptr);
    void setFileNameList(const QStringList &lst);

signals:
    void sgnFileNameChanged(QString newName);
    void sgnCurrentDirectoryChoosed(const QString &cdir);
    void sgnAutoRenameFileName(const QString &oldName, const QString &newName);
    void file_dropped(const QString &fname);

public slots:
    void sltFileClicked(QString fname);
    void sltOKClicked();
    void sltOpenFolderClicked();

private slots:
	void onGroupBoxChecked(bool checked);
    void onAutoRenameChanged(int state);
    void file_dropped_from_folder(const QString &fname) noexcept;

private:
	QString convertDateForFileName(const QString &strDate);
    void autorenameAllFilesFromtable();
    QString getDateFromExif(QStringView fname);

protected:
    virtual void resizeEvent(QResizeEvent * event);
};

#endif // EDITORWGT_H
