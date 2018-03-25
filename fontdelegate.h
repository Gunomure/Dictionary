#ifndef FONTDELEGATE_H
#define FONTDELEGATE_H

#include <QStyledItemDelegate>

class FontDelegate : public QStyledItemDelegate
{
Q_OBJECT

public:
    FontDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
    const QModelIndex &index) const override;
};

#endif // FONTDELEGATE_H
