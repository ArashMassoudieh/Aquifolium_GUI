#include "aqflmblockitem.h"
#include "qdebug.h"

#include <QtWidgets>
#include "mainwindow.h"

AqflmBlockItem::AqflmBlockItem(const QString &name, int x, int y)
{
    this->objectname = name;
    this->x = x;
    this->y = y;
    //this->color = color;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

}

AqflmBlockItem::AqflmBlockItem()
{
    Pressed = false;
    setFlag(ItemIsMovable);
}

QRectF AqflmBlockItem::boundingRect() const
{
    // outer most edges
    return bounds.adjusted(0, 0, 2, 2);
}

void AqflmBlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    Q_UNUSED(option);

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 40));

    //painter->drawRoundRect(bounds.translated(2, 2),50,50);

    if (fillRect)
        painter->setBrush(QApplication::palette().brush(QPalette::Window));
    else
        painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 1));
//  painter->drawRoundRect(bounds);

    if (!pixmap.isNull()) {
            //pixmap = pixmap.scaledToWidth(bounds.width());
            painter->scale(1, 1);
            painter->drawPixmap(-pixmap.width() / 2*0, -pixmap.height() / 2*0, pixmap);
        }
    painter->drawText(0,0,objectname);
/*    QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
    if (option->state & QStyle::State_MouseOver)
        fillColor = fillColor.light(125);

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if (lod < 0.2) {
        if (lod < 0.125) {
            painter->fillRect(QRectF(-50, -50, 50, 50), fillColor);
            return;
        }

        QBrush b = painter->brush();
        painter->setBrush(fillColor);
        painter->drawRect(-50, -50, 50, 50);
        painter->setBrush(b);
        return;
    }

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)
        width += 2;

    pen.setWidth(width);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->drawRect(QRect(-50, -50, 50, 50));
    painter->setBrush(b);

    if (lod >= 1) {
        painter->setPen(QPen(Qt::gray, 1));
        painter->drawLine(15, 54, 94, 54);
        painter->drawLine(94, 53, 94, 15);
        painter->setPen(QPen(Qt::black, 0));
    }

    // Draw text
    if (lod >= 2) {
        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(0.1, 0.1);
        painter->drawText(170, 180, QString("Model: VSC-2000 (Very Small Chip) at %1x%2").arg(x).arg(y));
        painter->drawText(170, 200, QString("Serial number: DLWR-WEER-123L-ZZ33-SDSJ"));
        painter->drawText(170, 220, QString("Manufacturer: Chip Manufacturer"));
        painter->restore();
    }

    // Draw lines
    QVarLengthArray<QLineF, 36> lines;

    // Draw red ink
    if (stuff.size() > 1) {
        QPen p = painter->pen();
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->setBrush(Qt::NoBrush);
        QPainterPath path;
        path.moveTo(stuff.first());
        for (int i = 1; i < stuff.size(); ++i)
            path.lineTo(stuff.at(i));
        painter->drawPath(path);
        painter->setPen(p);
    }*/
}

void AqflmBlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

void AqflmBlockItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier) {
        stuff << event->pos();
        _parent->additemselected(this);
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void AqflmBlockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}

QPainterPath AqflmBlockItem::shape() const
{
    QPainterPath path;
    path.addRect(bounds);
    return path;
}
