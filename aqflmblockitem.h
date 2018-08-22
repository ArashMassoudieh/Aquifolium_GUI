#ifndef AQFLMBLOCKITEM_H
#define AQFLMBLOCKITEM_H

#include "qgraphicsitem.h"
#include "qpixmap.h"
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QString>

// class for customization
class AqflmBlockItem :public QGraphicsObject
{
public:
    AqflmBlockItem();
    AqflmBlockItem(const QString &name) {objectname = name;}
    AqflmBlockItem(const QString &name, int x, int y);

    QRectF boundingRect() const override;

    // overriding paint()
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    QPainterPath shape() const override;


    // item state



    bool Pressed;
    void SetObjectName(const QString &name) {objectname = name;}
    bool SetPixMap(const QString &filename)
    {
        qDebug()<<"Loading pixmap...";
        pixmap.load(filename);
        pixmap = pixmap.scaledToHeight(100);
    }
private:
    QPixmap pixmap;
    QString objectname;
    int x;
    int y;
    QColor color = Qt::red;
    QVector<QPointF> stuff;
    QRectF bounds = QRectF(0,0,100,100);
    bool fillRect;
    QLinearGradient gradient;
protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // AQFLMBLOCKITEM_H
