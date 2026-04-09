#include "mapwidget.h"
#include "stationmarker.h"
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <cmath>

MapWidget::MapWidget(QWidget *parent)
    : QGraphicsView(parent), m_railwayData(nullptr), m_currentMode("railways"), m_currentScale(1.0)
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    
    setupScene();
    
    // Настройка вида
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    
    // Соединяем сигналы и слоты
    connect(m_scene, &QGraphicsScene::selectionChanged, this, &MapWidget::onStationSelected);
}

void MapWidget::setupScene()
{
    m_scene->setSceneRect(-2000, -2000, 4000, 4000);
    setFixedSize(800, 600);
}

void MapWidget::loadRailwayData(const RailwayData &data)
{
    m_railwayData = &data;
    
    // Очищаем сцену
    m_scene->clear();
    m_stationMarkers.clear();
    m_railwayLines.clear();
    
    // Рисуем железные дороги
    drawRailwayLines(data.getLines());
    
    // Добавляем маркеры станций
    addStationMarkers(data.getStations());
    
    // Центрируем вид на Москве
    centerOn(QPointF(0, 0));
    scale(0.5, 0.5);
}

void MapWidget::drawRailwayLines(const QList<RailwayLine> &lines)
{
    for (const auto &line : lines) {
        QPen pen(Qt::darkGray, 3);
        if (line.name.contains("МЦК") || line.name.contains("Кольцо")) {
            pen.setColor(QColor(165, 42, 42)); // brown color
            pen.setWidth(4);
        }
        
        for (int i = 0; i < line.points.size() - 1; ++i) {
            QPointF start = geoToScreen(line.points[i]);
            QPointF end = geoToScreen(line.points[i + 1]);
            
            QGraphicsLineItem *lineItem = m_scene->addLine(
                start.x(), start.y(), end.x(), end.y(), pen);
            m_railwayLines.append(lineItem);
        }
        
        // Добавляем подпись линии
        if (!line.points.isEmpty()) {
            QPointF labelPos = geoToScreen(line.points.first());
            QGraphicsTextItem *textItem = m_scene->addText(line.name);
            textItem->setPos(labelPos);
            textItem->setDefaultTextColor(Qt::black);
            m_railwayLines.append(textItem);
        }
    }
}

void MapWidget::addStationMarkers(const QList<Station> &stations)
{
    for (const auto &station : stations) {
        QPointF screenPos = geoToScreen(station.coordinates);
        StationMarker *marker = new StationMarker(station.name, station.type, screenPos);
        marker->setData(0, QVariant::fromValue(station)); // Сохраняем данные станции
        m_scene->addItem(marker);
        m_stationMarkers.append(marker);
    }
}

QPointF MapWidget::geoToScreen(const QPointF &geoCoords)
{
    // Преобразование географических координат в экранные
    // Москва примерно в центре: 55.75, 37.61
    const double moscowLat = 55.75;
    const double moscowLon = 37.61;
    const double scale = 5000; // Масштабный коэффициент
    
    double x = (geoCoords.y() - moscowLon) * scale;
    double y = -(geoCoords.x() - moscowLat) * scale; // Инвертируем Y
    
    return QPointF(x, y);
}

void MapWidget::setDisplayMode(const QString &mode)
{
    m_currentMode = mode;
    updateMarkersVisibility();
}

void MapWidget::updateMarkersVisibility()
{
    // В зависимости от режима и масштаба показываем/скрываем элементы
    for (auto *marker : m_stationMarkers) {
        if (m_currentMode == "railways") {
            marker->setVisible(true);
        } else {
            marker->setVisible(false);
        }
    }
}

void MapWidget::wheelEvent(QWheelEvent *event)
{
    // Масштабирование колесом мыши
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    
    if (!numPixels.isNull()) {
        zoom(numPixels.y());
    } else if (!numDegrees.isNull()) {
        zoom(numDegrees.y());
    }
    
    event->accept();
}

void MapWidget::zoom(int delta)
{
    const double zoomFactor = 1.1;
    if (delta > 0) {
        scale(zoomFactor, zoomFactor);
        m_currentScale *= zoomFactor;
    } else {
        scale(1.0 / zoomFactor, 1.0 / zoomFactor);
        m_currentScale /= zoomFactor;
    }
    
    emit scaleChanged(m_currentScale);
    updateMarkersVisibility();
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void MapWidget::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
}

void MapWidget::onStationSelected()
{
    auto selectedItems = m_scene->selectedItems();
    for (auto *item : selectedItems) {
        StationMarker *marker = qgraphicsitem_cast<StationMarker*>(item);
        if (marker) {
            Station station = marker->data(0).value<Station>();
            QString info = QString("Станция: %1\n"
                                   "Тип: %2\n"
                                   "Количество путей: %3\n"
                                   "Электрификация: %4\n"
                                   "Примечание: %5")
                              .arg(station.name)
                              .arg(station.type == "technical" ? "Техническая" : 
                                   station.type == "junction" ? "Узловая" : "Пассажирская")
                              .arg(station.trackCount)
                              .arg(station.electrification)
                              .arg(station.notes);
            emit stationClicked(station.name, info);
        }
    }
}
