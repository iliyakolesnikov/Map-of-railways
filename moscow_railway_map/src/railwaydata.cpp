#include "railwaydata.h"

RailwayData::RailwayData(QObject *parent) : QObject(parent)
{
    loadMoscowRingDistrictData();
}

void RailwayData::loadMoscowRingDistrictData()
{
    // Пример данных для Московской кольцевой железной дороги (МКЖД) и прилегающих участков
    // Координаты приблизительные, в реальном проекте нужно использовать точные данные
    
    // Добавляем станции МКЖД и прилегающих путей
    m_stations.append({
        "Москва-Товарная",
        QPointF(55.7215, 37.6389),
        "technical",
        "8 путей",
        "Постоянный ток",
        "Техническая станция, важное узловое соединение"
    });
    
    m_stations.append({
        "Канатчиково",
        QPointF(55.7089, 37.5845),
        "junction",
        "6 путей",
        "Постоянный ток",
        "Узловая станция, пересечение направлений"
    });
    
    m_stations.append({
        "Пресня",
        QPointF(55.7456, 37.5678),
        "passenger",
        "4 пути",
        "Постоянный ток",
        "Пассажирская платформа, пересадка на МЦК"
    });
    
    m_stations.append({
        "Белокаменная",
        QPointF(55.7823, 37.7234),
        "technical",
        "5 путей",
        "Постоянный ток",
        "Техническая станция, пост электрификации"
    });
    
    m_stations.append({
        "Ростокино",
        QPointF(55.8012, 37.6534),
        "passenger",
        "4 пути",
        "Постоянный ток",
        "Пассажирская станция, важная транспортная развязка"
    });
    
    m_stations.append({
        "Лихоборы",
        QPointF(55.8234, 37.5456),
        "junction",
        "7 путей",
        "Постоянный ток",
        "Крупный узел, сортировочная горка"
    });
    
    m_stations.append({
        "Петровско-Разумовское",
        QPointF(55.8456, 37.5678),
        "passenger",
        "6 путей",
        "Постоянный ток",
        "Пересадка на метро, пассажирский узел"
    });
    
    m_stations.append({
        "Окружная",
        QPointF(55.8567, 37.5234),
        "passenger",
        "4 пути",
        "Постоянный ток",
        "Станция МЦК и ж/д, важный пересадочный узел"
    });
    
    // Добавляем железнодорожные линии (упрощённо - прямые отрезки между ключевыми точками)
    RailwayLine mkadLine;
    mkadLine.name = "Московское Центральное Кольцо";
    mkadLine.points << QPointF(55.7215, 37.6389) 
                    << QPointF(55.7089, 37.5845)
                    << QPointF(55.7456, 37.5678)
                    << QPointF(55.7823, 37.7234)
                    << QPointF(55.8012, 37.6534)
                    << QPointF(55.8234, 37.5456)
                    << QPointF(55.8456, 37.5678)
                    << QPointF(55.8567, 37.5234);
    m_lines.append(mkadLine);
    
    RailwayLine smolenskoLine;
    smolenskoLine.name = "Смоленское направление";
    smolenskoLine.points << QPointF(55.7456, 37.5678)
                         << QPointF(55.7234, 37.5123)
                         << QPointF(55.7012, 37.4567);
    m_lines.append(smolenskoLine);
    
    RailwayLine yaroslavskoLine;
    yaroslavskoLine.name = "Ярославское направление";
    yaroslavskoLine.points << QPointF(55.8456, 37.5678)
                           << QPointF(55.8678, 37.6123)
                           << QPointF(55.8901, 37.6567);
    m_lines.append(yaroslavskoLine);
}

QList<Station> RailwayData::getStations() const
{
    return m_stations;
}

QList<RailwayLine> RailwayData::getLines() const
{
    return m_lines;
}
