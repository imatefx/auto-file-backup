#include <QtGui>
#include "droptablewidget.h"

DropTableWidget::DropTableWidget(QWidget *parent) : QTableWidget(parent)
{
    //set widget default properties:
        setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        setDragDropMode(QAbstractItemView::DropOnly);
        setDefaultDropAction(Qt::IgnoreAction);
        setAlternatingRowColors(true);
        setSelectionMode(QAbstractItemView::NoSelection);
        setShowGrid(false);
        setWordWrap(false);
        setAcceptDrops(true);
}

void DropTableWidget::dragEnterEvent(QDragEnterEvent *event) {
    qDebug() << "drag Enter Event";
    event->acceptProposedAction();
    emit changed(event->mimeData());
}

void DropTableWidget::dragMoveEvent(QDragMoveEvent *event) {
    qDebug() << "drag Move Event ";
    event->acceptProposedAction();
}

void DropTableWidget::dropEvent(QDropEvent *event) {
    qDebug() << "drag Event ";
    event->acceptProposedAction();
    emit dropped(event->mimeData());
}

void DropTableWidget::dragLeaveEvent(QDragLeaveEvent *event) {
    qDebug() << "drag Leave Event ";
    event->accept();
}

void DropTableWidget::clear() {
    qDebug() << "drag Clear";
    emit changed();
}
