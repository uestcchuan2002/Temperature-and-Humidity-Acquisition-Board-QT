#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void setupRealtimeDataSlot(); // 初始化图表
    void updatePlot();            // 定时刷新槽函数

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *mDataTimer;           // 刷新定时器
    double mStartTime;            // 记录启动时间
};
#endif // MAINWINDOW_H
