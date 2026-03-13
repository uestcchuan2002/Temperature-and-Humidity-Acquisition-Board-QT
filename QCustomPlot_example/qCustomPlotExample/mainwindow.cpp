#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setupRealtimeDataSlot() {
    // 1. 添加一条曲线并设置样式
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setName("传感器数据");

    // 2. 配置 X 轴为时间轴
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);

    // 3. 基础交互配置
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-1.2, 1.2); // 根据传感器量程设置

    // 4. 启动定时器 (每30毫秒刷新一次)
    mDataTimer = new QTimer(this);
    connect(mDataTimer, &QTimer::timeout, this, &MainWindow::updatePlot);
    mDataTimer->start(30);

    mStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
}

void MainWindow::updatePlot() {
    // 计算当前相对时间（单位：秒）
    double currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
    double key = currentTime - mStartTime;

    // --- 模拟数据获取 ---
    // 在实际应用中，这里替换为你从串口/网口收到的最新变量
    double value = qSin(key);
    // ------------------

    // 1. 添加数据点到曲线
    ui->customPlot->graph(0)->addData(key, value);

    // 2. 移除旧数据（可选）：保留最近 60 秒的数据，防止内存无限增长
    ui->customPlot->graph(0)->data()->removeBefore(key - 60);

    // 3. 关键步：移动 X 轴范围
    // 参数说明：(当前最新点, 范围宽度, 对齐方式)
    double windowWidth = 8.0; // 屏幕显示 8 秒宽的数据
    ui->customPlot->xAxis->setRange(key, windowWidth, Qt::AlignRight);

    // 4. 只有在需要时才调用重绘
    ui->customPlot->replot();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupRealtimeDataSlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

