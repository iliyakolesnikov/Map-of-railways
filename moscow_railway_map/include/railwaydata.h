#ifndef RAILWAYDATA_H
#define RAILWAYDATA_H

#include <QObject>
#include <QList>
#include <QPointF>
#include <QString>

struct Station {
    QString name;
    QPointF coordinates; // Lat, Lon
    QString type; // "passenger", "technical", "junction"
    QString trackCount;
    QString electrification;
    QString notes;
};

struct RailwayLine {
    QString name;
    QList<QPointF> points; // List of coordinates forming the line
};

class RailwayData : public QObject
{
    Q_OBJECT
public:
    explicit RailwayData(QObject *parent = nullptr);
    
    QList<Station> getStations() const;
    QList<RailwayLine> getLines() const;
    
private:
    void loadMoscowRingDistrictData();
    
    QList<Station> m_stations;
    QList<RailwayLine> m_lines;
};

#endif // RAILWAYDATA_H
