#include "pix_viewer.h"
#include <QFileInfo>

PixViewer::PixViewer(const QString &text, QWidget *parent):QLabel(text, parent)
{
    setAcceptDrops(true);
}


void PixViewer::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->urls().size()==1)
    {
        QStringList lst;
        lst << "jpg" << "jpeg" << "bmp" << "png" << "tiff";
        QFileInfo f(event->mimeData()->urls().at(0).toString());
        auto s = f.suffix();
        auto it = std::find(lst.cbegin(), lst.cend(), s);
        if(it != lst.cend())
            event->accept();
        else
            event->ignore();
    }
    else
        event->ignore();
}

void PixViewer::dropEvent(QDropEvent *event)
{
    auto url_files = event->mimeData()->urls();
    QStringList files;
    for(const auto &u : url_files)
    {
        files.append(u.toLocalFile());
    }
    emit file_dropped(files.at(0));
}
