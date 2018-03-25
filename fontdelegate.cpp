#include "fontdelegate.h"

#include <QPainter>
#include <QFontDatabase>
#include <QCoreApplication>

void FontDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem itemOption = option;


    if(index.column() == 3)
    {
        QPalette p(option.palette);
        p.setColor(QPalette::Text, Qt::red);
        p.setColor(QPalette::Base, Qt::red);
        //p.setColor(QPalette::HighlightedText, FontColor);
        //p.setColor(QPalette::Highlight, BackColor);

        QStyleOptionViewItem opt(option);
        opt.palette = p;

        painter->setPen(Qt::darkMagenta);
        painter->setFont(QFont("Times", 11, QFont::Bold)); // Шрифт
        painter->drawText(itemOption.rect, Qt::AlignLeft | Qt::AlignBottom, index.data().toString());
        //QStyledItemDelegate::paint(painter, option, index);
    }
    else if(index.column() == 2)
    {

        //QFont nelusFont("NelusBase");  // QFont c вашим шрифтом

        painter->setPen(Qt::black);
        painter->setFont(QFont("NelusBase", 30)); // Шрифт
        painter->drawText(itemOption.rect, Qt::AlignCenter | Qt::AlignVCenter, index.data().toString());
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
