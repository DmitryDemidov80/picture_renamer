#include "editorwgt.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QStringList>
#include "exif.h"

EditorWgt::EditorWgt(QWidget *parent) : QWidget(parent)
{
    lblOriginalName = new QLabel(this);
    QStringList lst;
    lst << "Изобразительное исскусство"
        << "Гознак"
        << "Планета"
        << "Плакат"
        << "Правда"
        << "Министерство связи СССР"
        << "Аврора"
        << "Советский художник"
        << "Художник РСФСР"
        << "ИЗОГИС";
    cmbProducer = new QComboBox(this);
    cmbProducer->addItems(lst);
    lst.clear();
    lst << "Новый год"
        << "23 февраля"
        << "8 марта"
        << "1 мая"
        << "День победы"
        << "Праздник Октября"
        << "День рождения"
        << "Свадьба"
        << "Город"
        << "Детские поздравления"
        << "Поздравление"
        << "Цветы"
        << "Театр"
        << "Картина, фото"
        << "Животные"
        << "Природа"
        << "Музей"
        << "Детские"
        << "Революция-1905"
        << "Фольклор"
        << "Школа"
        << "Техноген"
        << "Спорт"
        << "Космос"
        << "Новорожденный";

    cmbSection = new QComboBox(this);
    cmbSection->addItems(lst);

    leYear = new QLineEdit("19",this);
    bOK = new QPushButton("OK",this);
    connect(bOK,SIGNAL(clicked(bool)),SLOT(sltOKClicked()));

    bOpenFolder = new QPushButton("Open Folder",this);
    connect(bOpenFolder,SIGNAL(clicked(bool)),SLOT(sltOpenFolderClicked()));

    QFormLayout *frmLay = new QFormLayout;
    frmLay->addRow("File: ",lblOriginalName);

    lblPix = new QLabel("No Pix",this);
    lblPix->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

	grbxExifDate = new QGroupBox("Exif Date",this);
	connect(grbxExifDate, &QGroupBox::clicked, this, &EditorWgt::onGroupBoxChecked);
	grbxExifDate->setCheckable(true);
	//grbxExifDate->setFlat(true);
    QHBoxLayout *vExifDateLay = new QHBoxLayout;
    chkAutoRenameAllFiles = new QCheckBox("Autorename all files", this);
    connect(chkAutoRenameAllFiles, &QCheckBox::stateChanged, this, &EditorWgt::onAutoRenameChanged);
	leExifDate = new QLineEdit(this);
    vExifDateLay->addWidget(chkAutoRenameAllFiles);
	vExifDateLay->addWidget(leExifDate);
	grbxExifDate->setLayout(vExifDateLay);

	grbxPaint = new QGroupBox("Picture",this);
	connect(grbxPaint, &QGroupBox::clicked, this, &EditorWgt::onGroupBoxChecked);
	grbxPaint->setCheckable(true);
	grbxPaint->setChecked(false);
	//grbxPaint->setFlat(true);

	QVBoxLayout *vPaintLay = new QVBoxLayout;
	vPaintLay->addWidget(cmbProducer);
	vPaintLay->addWidget(cmbSection);
	vPaintLay->addWidget(leYear);
	grbxPaint->setLayout(vPaintLay);

    QVBoxLayout *vMainLay = new QVBoxLayout;
    vMainLay->addWidget(bOpenFolder);
    vMainLay->addWidget(lblPix,2);
    vMainLay->addLayout(frmLay);
	vMainLay->addWidget(grbxExifDate);
	vMainLay->addWidget(grbxPaint);
    vMainLay->addWidget(bOK);
    setLayout(vMainLay);
}

void EditorWgt::setFileNameList(const QStringList &lst)
{
    lstFileNames.clear();
    lstFileNames = lst;
}

void EditorWgt::sltFileClicked(QString fname)
{
    // Заполнять поля и картинку
    lblOriginalName->setText(fname);
    QString path(currentDir.absolutePath()+"/"+fname);

    if(currentImg.load(path))
    {
        QImage img1=currentImg.scaled(lblPix->width(),lblPix->height(),Qt::KeepAspectRatio);
        lblPix->clear();
        lblPix->setPixmap(QPixmap().fromImage(img1));
        QString str_date(getDateFromExif(fname));
        leExifDate->setText(str_date);
    }
    else
    {
        QMessageBox::warning(nullptr,"Ошибка загрузки изображения","Ошибка загрузки изображения.");
    }
}

void EditorWgt::sltOKClicked()
{
    QString newFName("UnknownName");
    QFileInfo fi(lblOriginalName->text());
	if(grbxExifDate->isChecked())
	{
        if(chkAutoRenameAllFiles->isChecked())
        {
            ///TODO
            /// Автоматическое переименование
            autorenameAllFilesFromtable();
        }
        else
        {
            newFName = leExifDate->text()+"."+fi.suffix().toLower();
            emit sgnFileNameChanged(newFName);
        }
	}
	else if(grbxPaint->isChecked())
	{
		newFName = fi.baseName()+"-"+cmbProducer->currentText()+"-"+cmbSection->currentText()+"-"+leYear->text()+"."+fi.suffix().toLower();
        emit sgnFileNameChanged(newFName);
	}
}

void EditorWgt::sltOpenFolderClicked()
{
    QFileDialog *fdlg = new QFileDialog(this,"Выбор директории с картинками");
    fdlg->setFileMode(QFileDialog::DirectoryOnly);
    if (fdlg->exec()==QDialog::Accepted)
    {
        //currentImg = QImage();
        lblPix->clear();
        lblPix->setText("No Pix");
        QString strPath(fdlg->directory().absolutePath());
        currentDir.setCurrent(strPath);
        // Передать текущую директорию наверх
        emit sgnCurrentDirectoryChoosed(currentDir.currentPath());
    }
	delete fdlg;
}

void EditorWgt::onGroupBoxChecked(bool checked)
{
	auto grb = dynamic_cast<QGroupBox*>(sender());
	if(grb==grbxPaint)
	{
		grbxExifDate->setChecked(!checked);
	}
	else if(grb==grbxExifDate)
	{
		grbxPaint->setChecked(!checked);
    }
}

void EditorWgt::onAutoRenameChanged(int state)
{
    switch(state)
    {
        case Qt::Checked:
        {
            leExifDate->setEnabled(false);
            leExifDate->clear();
            break;
        }
        case Qt::PartiallyChecked:
        {
            break;
        }
        case Qt::Unchecked:
        {
            leExifDate->setEnabled(true);
            break;
        }
    }
}

QString EditorWgt::convertDateForFileName(const QString &strDate)
{
	QString str;
	if(!strDate.isEmpty())
	{
		auto lstSpase=strDate.split(" ");
		QString date(lstSpase[0]);
		QString time(lstSpase[1]);
		date=date.replace(":","_");
		time=time.replace(":","_");
		str=date+"-"+time;
	}
    return  str;
}

void EditorWgt::autorenameAllFilesFromtable()
{
    for(const auto &fname : lstFileNames)
    {
        QString newFName("UnknownName");
        QFileInfo fi(fname);
        auto str_date=getDateFromExif(fname);
        if(!str_date.isEmpty())
        {
            newFName = str_date+"."+fi.suffix().toLower();
            emit sgnAutoRenameFileName(fname, newFName);
        }
        else
        {
            qDebug() << "No date in file: " << fname;
        }
    }
}

QString EditorWgt::getDateFromExif(QStringView fname)
{
    easyexif::EXIFInfo info;
    QFile file(fname.toString());
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    if (int code = info.parseFrom((unsigned char *)data.data(), data.size()))
    {
        qDebug() << "Error parsing EXIF: code " << code;
    }
    QString date(info.DateTimeOriginal.c_str());
    file.close();

    auto str=convertDateForFileName(date);
    return str;
}

void EditorWgt::resizeEvent(QResizeEvent *event)
{
    if(!currentImg.isNull())
    {
        QImage img1=currentImg.scaled(lblPix->width(),lblPix->height(),Qt::KeepAspectRatio);
        lblPix->setPixmap(QPixmap().fromImage(img1));
    }
    QWidget::resizeEvent(event);
}

