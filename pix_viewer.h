#ifndef PIXVIEWER_H
#define PIXVIEWER_H

#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>

class PixViewer : public QLabel
{
    Q_OBJECT
public:
    PixViewer(const QString &text, QWidget *parent = nullptr);

    // QWidget interface
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

signals:
    void file_dropped(const QString &fname);
};

#endif // PIXVIEWER_H
