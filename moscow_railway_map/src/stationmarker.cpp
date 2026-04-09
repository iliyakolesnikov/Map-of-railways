#include "stationmarker.h"
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QToolTip>

StationMarker::StationMarker(const QString &name, const QString &type, QPointF pos, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_name(name), m_type(type), m_hovered(false)
{
    setPos(pos);
    setAcceptHoverEvents(true);
    setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
}

QRectF StationMarker::boundingRect() const
{
    return QRectF(-10, -10, 20, 20);
}

void StationMarker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    
    // Определяем цвет в зависимости от типа станции
    QColor color;
    if (m_type == "technical") {
        color = Qt::red;
    } else if (m_type == "junction") {
        color = Qt::blue;
    } else {
        color = Qt::green;
    }
    
    painter->setBrush(color);
    painter->setPen(Qt::black);
    
    // Рисуем маркер станции
    if (m_hovered || isSelected()) {
        painter->drawEllipse(boundingRect());
        painter->drawText(boundingRect(), Qt::AlignCenter, m_name);
    } else {
        // На большом масштабе показываем только точку
        qreal scale = painter->transform().m11();
        if (scale > 1.0) {
            painter->drawEllipse(boundingRect());
        } else {
            painter->drawEllipse(QRectF(-5, -5, 10, 10));
        }
    }
}

void StationMarker::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void StationMarker::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

QVariant StationMarker::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSelectedHasChanged) {
        update();
    }
    return QGraphicsItem::itemChange(change, value);
}
