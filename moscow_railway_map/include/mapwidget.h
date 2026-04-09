#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "railwaydata.h"

class MapWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);
    
    void loadRailwayData(const RailwayData &data);
    void setDisplayMode(const QString &mode);
    
signals:
    void stationClicked(const QString &stationName, const QString &info);
    void scaleChanged(double scale);
    
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    
private slots:
    void onStationSelected();
    void updateMarkersVisibility();
    
private:
    void setupScene();
    void drawRailwayLines(const QList<RailwayLine> &lines);
    void addStationMarkers(const QList<Station> &stations);
    QPointF geoToScreen(const QPointF &geoCoords);
    
    QGraphicsScene *m_scene;
    RailwayData m_railwayData;
    QString m_currentMode;
    double m_currentScale;
    QList<QGraphicsItem*> m_stationMarkers;
    QList<QGraphicsItem*> m_railwayLines;
};

#endif // MAPWIDGET_H
