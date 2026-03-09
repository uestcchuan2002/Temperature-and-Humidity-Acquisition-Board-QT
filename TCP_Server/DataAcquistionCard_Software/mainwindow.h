#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QMouseEvent>
#include <QLineEdit>
#include <QIntValidator>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QTcpSocket>
#include <QTextBrowser>
#include <QLineEdit>
#include <QProgressBar> // 新增：用于显示升级进度
#include <QFile>        // 新增：用于打开和读取文件
#include <QTimer>       // 新增：用于定时发送数据
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QThread>

#include "tcpworker.h"
#include "firmwareupgradeworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void sigConnect(QString ip, quint16 port);
    void sigDisconnect();
    void sigSend(QByteArray data);
    void sigUpdate(QByteArray data);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 事件过滤函数，用于实现窗口拖动
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;

    QPoint lastTitleBarPos;     /* 定义标题栏的位置记录（用于拖动窗口） */

    /* ota升级 */
    QThread *tcpThread = nullptr;
    TcpWorker *tcpWorker = nullptr;

    /* 界面控件 */
    QWidget *contentContainer;      /* 总的界面容器 */
    QWidget *tcpConnectControlArea; /* tcp连接控制模块 */
    QWidget *firmwareUpdateWidget;  /* ota升级模块 */

    /* tcp连接控制模块控件------------------------------start */
    QLabel *ipLabel;                /* IP地址标签 */
    QLineEdit *ipEdit;              /* IP地址输入框 */
    QLabel *portLabel;              /* 端口号标签 */
    QLineEdit *portEdit;            /* 端口号输入框 */
    QPushButton *connectBtn;        /* TCP连接按键 */
    QPushButton *disconnectBtn;     /* TCP断开连接按键 */
    QLabel *tcpConncetState;        /* TCP连接状态显示标签 */
    QTextBrowser *textBrowser;      /* 文本浏览器 */
    QLineEdit *lineEdit;            /* 文本输入框 */
    QPushButton *messageSendBtn;    /* 消息发送按钮 */
    /* tcp连接控制模块控件------------------------------end */

    /* ota升级模块控件---------------------------------start */
    QLabel *otaLabel;                   /* OTA显示标签 */
    QPushButton *selectFileBtn;         /* 固件文件选择按钮 */
    QLineEdit *firmwarePathEdit;        /* 固件路径显示框 */
    QPushButton *updateBtn;             /* 升级按钮 */
    QLabel *updateStateLabel;           /* 升级状态标签 */
    QProgressBar *updateProgressBar;    /* 升级进度条 */
    QByteArray m_firmwareData;          /* 固件数据缓存 */
    QString    m_firmwarePath;          /* 固件路径 */
    /* ota升级模块控件---------------------------------end */

    // 创建自定义标题栏的函数
    void createCustomTitleBar(int height = 40,
                                  QString bgColor = "#2c3e50",
                                  QString textColor = "#ffffff",
                                  QString hoverColor = "#34495e",
                                  QString titleText = "水质检测仪",
                                  QString borderColor = "#165DFF",  // 边框颜色
                                  int borderWidth = 1);             // 边框宽度
    // 初始化内容区域
    void initContentArea(void);

private slots:
    void toConnect();           /* 建立连接 */
    void toDisConnect();        /* 断开连接 */
    void connected();           /* 已连接 */
    void disconnected();        /* 已断开连接 */
    void receiveMessages(QString messages);     /* 接收消息 */
    void sendMessages();        /* 发送消息 */
    void startUpgrade();        /* 开始升级 */
    void onUpgradeProgress(int percent);        /* 升级进度更新 */
    void onUpgradeStatus(QString status);       /* 升级状态更新 */
    void onUpgradeFinished(bool success, QString message); /* 升级完成回调 */

    // --- 新增结束 ---
};
#endif // MAINWINDOW_H
