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
    event->acceptProposedAction();
    emit changed(event->mimeData());
}

void DropTableWidget::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void DropTableWidget::dropEvent(QDropEvent *event) {
    event->acceptProposedAction();
    emit dropped(event->mimeData());
}

void DropTableWidget::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

void DropTableWidget::clear() {
    emit changed();
}
