#ifndef EDITORWGT_H
#define EDITORWGT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDir>
#include <QComboBox>
#include <QStringList>

class QImage;
class QGroupBox;
class QCheckBox;
class EditorWgt : public QWidget
{
    Q_OBJECT

    QLabel *lblOriginalName;
    QLabel*lblPix;
    QLineEdit *leYear;
    QPushButton *bOK;
    QPushButton *bOpenFolder;
    QComboBox *cmbProducer; // Издательство
    QComboBox *cmbSection; // Раздел
    QDir currentDir;
    QImage currentImg;
	QLineEdit *leExifDate;
    QCheckBox *chkAutoRename;
	QGroupBox *grbxExifDate;
	QGroupBox *grbxPaint;
    QStringList lstFileNames;

public:
    explicit EditorWgt(QWidget *parent = nullptr);
    void setFileNameList(const QStringList &lst);

signals:
    void sgnFileNameChanged(QString newName);
    void sgnCurrentDirectoryChoosed(QString cdir);
    void sgnAutoRenameFileName(const QString &oldName, const QString &newName);

public slots:
    void sltFileClicked(QString fname);
    void sltOKClicked();
    void sltOpenFolderClicked();

private slots:
	void onGroupBoxChecked(bool checked);
    void onAutoRenameChanged(int state);

private:
	QString convertDateForFileName(const QString &strDate);
    void autorenameAllFilesFromtable();
    QString getDateFromExif(QStringView fname);

protected:
    virtual void resizeEvent(QResizeEvent * event);
};

#endif // EDITORWGT_H
