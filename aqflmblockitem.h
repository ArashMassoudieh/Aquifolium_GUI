#ifndef AQFLMBLOCKITEM_H
#define AQFLMBLOCKITEM_H

#include "qgraphicsitem.h"
#include "qpixmap.h"
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QString>

// class for customization
class AqflmBlockItem :public QGraphicsItem
{
public:
    AqflmBlockItem();
    AqflmBlockItem(const QString &name) {objectname = name;}

    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    // item state
    bool Pressed;
    void SetObjectName(const QString &name) {objectname = name;}
    bool SetPixMap(const QString &filename)
    {
        qDebug()<<"Loading pixmap...";
        pixmap.load(filename);
    }
private:
    QPixmap pixmap;
    QString objectname;
protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // AQFLMBLOCKITEM_H
