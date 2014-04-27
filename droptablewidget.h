#ifndef DROPTABLEWIDGET_H
#define DROPTABLEWIDGET_H

#include <QTableWidget>

class QMimeData;

class DropTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit DropTableWidget(QWidget *parent = 0);

signals:
    void changed(const QMimeData *mimeData = 0);
    void dropped(const QMimeData *mimeData = 0);

public slots:
    void clear();

protected:
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void dragLeaveEvent(QDragLeaveEvent *event);
        void dropEvent(QDropEvent *event);

    private:
        QTableWidget *tablewidget;

};

#endif // DROPTABLEWIDGET_H
