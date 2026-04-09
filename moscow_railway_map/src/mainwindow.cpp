#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Интерактивная карта железных дорог Москвы");
    resize(1200, 800);
    
    m_railwayData = new RailwayData(this);
    
    setupUI();
    createMenuBar();
    createToolBar();
    createStatusBar();
}

void MainWindow::setupUI()
{
    // Центральный виджет
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    
    // Левая панель с картой
    m_mapWidget = new MapWidget(this);
    m_mapWidget->loadRailwayData(*m_railwayData);
    
    // Правая панель с информацией
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(5, 5, 5, 5);
    
    // Панель управления режимами
    QWidget *controlPanel = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    
    controlLayout->addWidget(new QLabel("Режим:"));
    m_modeComboBox = new QComboBox();
    m_modeComboBox->addItem("Железные дороги", "railways");
    m_modeComboBox->addItem("Высота зданий", "buildings"); // Будущий режим
    m_modeComboBox->addItem("Транспорт", "transport"); // Будущий режим
    controlLayout->addWidget(m_modeComboBox);
    
    QPushButton *zoomInBtn = new QPushButton("+");
    zoomInBtn->setMaximumWidth(40);
    QPushButton *zoomOutBtn = new QPushButton("-");
    zoomOutBtn->setMaximumWidth(40);
    QPushButton *resetBtn = new QPushButton("Сброс");
    
    controlLayout->addWidget(zoomInBtn);
    controlLayout->addWidget(zoomOutBtn);
    controlLayout->addWidget(resetBtn);
    
    rightLayout->addWidget(controlPanel);
    
    // Инфо-панель
    rightLayout->addWidget(new QLabel("Информация о станции:"));
    m_infoPanel = new QTextEdit();
    m_infoPanel->setReadOnly(true);
    m_infoPanel->setMinimumWidth(250);
    rightLayout->addWidget(m_infoPanel);
    
    // Индикатор масштаба
    m_scaleLabel = new QLabel("Масштаб: 100%");
    rightLayout->addWidget(m_scaleLabel);
    
    rightLayout->addStretch();
    
    // Разделитель
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(m_mapWidget);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(splitter);
    
    // Соединяем сигналы и слоты
    connect(m_modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
        QString mode = m_modeComboBox->itemData(index).toString();
        onModeChanged(mode);
    });
    
    connect(m_mapWidget, &MapWidget::stationClicked, this, &MainWindow::onStationClicked);
    connect(m_mapWidget, &MapWidget::scaleChanged, this, &MainWindow::onScaleChanged);
    connect(zoomInBtn, &QPushButton::clicked, this, &MainWindow::onZoomIn);
    connect(zoomOutBtn, &QPushButton::clicked, this, &MainWindow::onZoomOut);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::onResetView);
}

void MainWindow::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu("Файл");
    fileMenu->addAction("Открыть", [](){ /* TODO */ });
    fileMenu->addSeparator();
    fileMenu->addAction("Выход", this, &QWidget::close);
    
    QMenu *viewMenu = menuBar()->addMenu("Вид");
    viewMenu->addAction("Приблизить", this, &MainWindow::onZoomIn);
    viewMenu->addAction("Отдалить", this, &MainWindow::onZoomOut);
    viewMenu->addAction("Сбросить вид", this, &MainWindow::onResetView);
    
    QMenu *helpMenu = menuBar()->addMenu("Справка");
    helpMenu->addAction("О программе", [this]() {
        QMessageBox::about(this, "О программе",
            "Интерактивная карта железных дорог Москвы\n"
            "Версия 1.0\n\n"
            "Программа для отображения железнодорожной инфраструктуры\n"
            "Московского региона.");
    });
}

void MainWindow::createToolBar()
{
    QToolBar *toolBar = addToolBar("Инструменты");
    toolBar->setMovable(false);
    
    QAction *zoomInAction = toolBar->addAction("Приблизить");
    connect(zoomInAction, &QAction::triggered, this, &MainWindow::onZoomIn);
    
    QAction *zoomOutAction = toolBar->addAction("Отдалить");
    connect(zoomOutAction, &QAction::triggered, this, &MainWindow::onZoomOut);
    
    QAction *resetAction = toolBar->addAction("Сброс");
    connect(resetAction, &QAction::triggered, this, &MainWindow::onResetView);
    
    toolBar->addSeparator();
    toolBar->addWidget(new QLabel("Режим: "));
    toolBar->addWidget(m_modeComboBox);
}

void MainWindow::createStatusBar()
{
    QStatusBar *status = statusBar();
    status->showMessage("Готов к работе");
}

void MainWindow::onModeChanged(const QString &mode)
{
    m_mapWidget->setDisplayMode(mode);
    statusBar()->showMessage("Режим: " + mode);
}

void MainWindow::onStationClicked(const QString &name, const QString &info)
{
    m_infoPanel->setText(info);
    statusBar()->showMessage("Выбрана станция: " + name);
}

void MainWindow::onScaleChanged(double scale)
{
    int percent = qRound(scale * 100);
    m_scaleLabel->setText("Масштаб: " + QString::number(percent) + "%");
}

void MainWindow::onZoomIn()
{
    m_mapWidget->zoom(120); // Положительное значение для увеличения
}

void MainWindow::onZoomOut()
{
    m_mapWidget->zoom(-120); // Отрицательное значение для уменьшения
}

void MainWindow::onResetView()
{
    m_mapWidget->resetTransform();
    m_mapWidget->centerOn(QPointF(0, 0));
    m_mapWidget->scale(0.5, 0.5);
    onScaleChanged(0.5);
}
