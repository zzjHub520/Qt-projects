#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QItemDelegate>
#include <QApplication>
#include <QMouseEvent>
#include <QDialog>

class MyDelegate : public QItemDelegate {
    Q_OBJECT
public:
    MyDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    bool editorEvent(QEvent *event,
                     QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index);
};

#endif // MYDELEGATE_H
