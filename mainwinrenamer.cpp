#include "mainwinrenamer.h"
#include <QStringList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QProgressBar>
#include <QThread>
#include "exif.h"
#include <QDebug>

MainWinRenamer::MainWinRenamer(QWidget *parent)
    : QWidget(parent),currentRow(-1), prBar(nullptr), thread_(nullptr), is_auto_rename(false)
{
    setWindowTitle("Picture Renamer. Ver: "+QString(APP_VERSION));
    tblWgt = new QTableWidget(0, 4, this);
    connect(tblWgt, &QTableWidget::cellClicked, this, &MainWinRenamer::sltTblCellClicked);
    QStringList lst;
    lst << "Name" << "Size" << "EXIF Date" << "Camera";
    tblWgt->setHorizontalHeaderLabels(lst);

    editor = new EditorWgt(this);
    connect(editor, &EditorWgt::sgnCurrentDirectoryChoosed, this, &MainWinRenamer::sltCurrentDirectoryChoosed);
    connect(editor, &EditorWgt::sgnFileNameChanged, this, &MainWinRenamer::sltFileNameChanged);
    connect(editor, &EditorWgt::sgnAutoRenameFileName, this, &MainWinRenamer::sltAutoRename);

    createProgressBar();

    thread_ = new QThread;
    connect(thread_, &QThread::started, &dir_reader_, &DirectoryReader::analyzeDirectory);
    connect(&dir_reader_, &DirectoryReader::analyze_finished, thread_, &QThread::quit);

    connect(&dir_reader_, &DirectoryReader::rowDataReady, this, &MainWinRenamer::onRowReady);
    connect(&dir_reader_, &DirectoryReader::analyze_finished, this, &MainWinRenamer::onAnalyzeDirectoryFinished);


    QHBoxLayout *hMainLay = new QHBoxLayout;
    hMainLay->addWidget(tblWgt);
    hMainLay->addWidget(editor);
    setLayout(hMainLay);
}

MainWinRenamer::~MainWinRenamer()
{
    if(prBar!=nullptr)
        delete prBar;
    if(thread_!=nullptr)
    {
        thread_->quit();
        thread_->wait();
        delete thread_;
    }
}

void MainWinRenamer::clearTable()
{
    currentRow=-1;
    tblWgt->setRowCount(0);
}

void MainWinRenamer::sltCurrentDirectoryChoosed(QString cdir)
{
    dir_reader_.setDirectory(cdir);
    clearTable();
    old_file_names.clear();

    auto top_left=this->mapToGlobal(QPoint(0,0));
    prBar->move(top_left.x()+width()/2, top_left.y()+height()/2);
    prBar->show();

    dir_reader_.moveToThread(thread_);
    thread_->start();
}

void MainWinRenamer::sltTblCellClicked(int row, int col)
{
    Q_UNUSED (col)
    currentRow=row;
    // Передать для отображения в картинку и в лейбл
    currentFile.setFileName(currentDir.absolutePath()+"/"+tblWgt->item(currentRow,0)->text());
    if(currentFile.exists())
    {
        editor->sltFileClicked(tblWgt->item(currentRow,0)->text());
    }
}

void MainWinRenamer::sltFileNameChanged(const QString &newName)
{
    if(currentFile.rename(newName))
    {
        tblWgt->item(currentRow,0)->setText(newName);
    }
    else
    {
        if(!is_auto_rename)
        {
            QMessageBox::warning(nullptr,"Ошибка переименования","Файл не переименован. Возможно, его не существует или имеет такое же имя.");
        }
    }
    is_auto_rename=false;
}

void MainWinRenamer::sltAutoRename(const QString &oldName, const QString &newName)
{
    is_auto_rename=true;
    auto lstItems = tblWgt->findItems(oldName, Qt::MatchContains);
    currentRow=lstItems.at(0)->row();
    currentFile.setFileName(oldName);
    sltFileNameChanged(newName);
}

void MainWinRenamer::onRowReady(int row_num, const QString &fname, const QString &size, const QString &date, const QString &camera_brand, const QString &camera_model, int percent)
{
    prBar->setValue(percent);
    tblWgt->insertRow(row_num);
    //имя
    QTableWidgetItem *itmname = new QTableWidgetItem(fname);
    itmname->setFlags(itmname->flags()^Qt::ItemIsEditable);
    tblWgt->setItem(row_num,0,itmname);
    old_file_names.push_back(fname);

    //размер
    QTableWidgetItem *itmsize = new QTableWidgetItem(size);
    itmsize->setFlags(itmsize->flags()^Qt::ItemIsEditable);
    tblWgt->setItem(row_num, 1, itmsize);

    //дата
    QTableWidgetItem *itmdate = new QTableWidgetItem(date);
    itmdate->setFlags(itmdate->flags()^Qt::ItemIsEditable);
    tblWgt->setItem(row_num, 2, itmdate);

    //камера
    auto camera=camera_brand+", "+camera_model;
    QTableWidgetItem *itmdCamera = new QTableWidgetItem(camera);
    itmdate->setFlags(itmdCamera->flags()^Qt::ItemIsEditable);
    tblWgt->setItem(row_num, 3, itmdCamera);
}

void MainWinRenamer::onAnalyzeDirectoryFinished()
{
    tblWgt->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    editor->setFileNameList(old_file_names);
    prBar->close();
}

void MainWinRenamer::resizeEvent(QResizeEvent *event)
{
    tblWgt->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    QWidget::resizeEvent(event);
}

void MainWinRenamer::createProgressBar()
{
    if(prBar==nullptr)
    {
        prBar = new QProgressBar;
        prBar->setWindowFlags(Qt::SplashScreen);
        prBar->setWindowModality(Qt::ApplicationModal);
        prBar->setRange(0, 100);
    }
}
