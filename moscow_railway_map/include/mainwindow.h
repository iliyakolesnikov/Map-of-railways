#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include "mapwidget.h"
#include "railwaydata.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    
private slots:
    void onModeChanged(const QString &mode);
    void onStationClicked(const QString &name, const QString &info);
    void onScaleChanged(double scale);
    void onZoomIn();
    void onZoomOut();
    void onResetView();
    
private:
    void setupUI();
    void createMenuBar();
    void createToolBar();
    void createStatusBar();
    
    MapWidget *m_mapWidget;
    RailwayData *m_railwayData;
    QComboBox *m_modeComboBox;
    QLabel *m_scaleLabel;
    QTextEdit *m_infoPanel;
};

#endif // MAINWINDOW_H
