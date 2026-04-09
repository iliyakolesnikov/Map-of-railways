#ifndef STATIONMARKER_H
#define STATIONMARKER_H

#include <QObject>
#include <QGraphicsItem>
#include <QPointF>
#include <QString>

class StationMarker : public QGraphicsItem
{
public:
    StationMarker(const QString &name, const QString &type, QPointF pos, QGraphicsItem *parent = nullptr);
    
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    
    QString getName() const { return m_name; }
    QString getType() const { return m_type; }
    
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    
private:
    QString m_name;
    QString m_type;
    bool m_hovered;
};

#endif // STATIONMARKER_H
