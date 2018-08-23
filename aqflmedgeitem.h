#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include "aqflmblockitem.h"



class AqflmEdgeItem : public QGraphicsItem
{
public:
    AqflmEdgeItem(AqflmBlockItem *sourceNode, AqflmBlockItem *destNode);

    AqflmBlockItem *sourceNode() const;
    AqflmBlockItem *destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    AqflmBlockItem *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};

#endif // EDGE_H
