#include "aqflmblockitem.h"
#include "qdebug.h"


AqflmBlockItem::AqflmBlockItem()
{
    Pressed = false;
    setFlag(ItemIsMovable);
}

QRectF AqflmBlockItem::boundingRect() const
{
    // outer most edges
    return QRectF(0,0,100,100);
}

void AqflmBlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QRectF source(0.0, 0.0, 450, 612);
    qDebug()<<"In painter!";
    painter->drawPixmap(rect, pixmap,source);
    painter->drawText(rect,objectname);
    painter->drawEllipse(rect);
    if(Pressed)
    {
        QPen pen(Qt::white, 3);
        painter->setPen(pen);
        //QBrush brush(Qt::green);
        //painter->setBrush(brush);

    }
    else
    {
        QPen pen(Qt::white, 3);
        painter->setPen(pen);
        //QBrush brush(Qt::blue);
        //painter->setBrush(brush);
    }
}

void AqflmBlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = true;
    qDebug()<<"Pressed!"<<endl;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void AqflmBlockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = false;
    qDebug()<<"UnPressed!"<<endl;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
