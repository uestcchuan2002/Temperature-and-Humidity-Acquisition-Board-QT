#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tcpThread = new QThread;
    tcpWorker = new TcpWorker;
    tcpWorker->moveToThread(tcpThread);
    tcpThread->start();

    // 1. 隐藏原生标题栏（必须）
    this->setWindowFlags(Qt::FramelessWindowHint); // 隐藏原生标题栏
    this->setAttribute(Qt::WA_TranslucentBackground, false); // 关闭透明（可选）

    // 2. 创建自定义标题栏
    // 可自定义参数：高度50px，背景色深蓝色，文字白色，按钮悬浮浅蓝
    createCustomTitleBar(50, "#323C4D", "#ffffff", "#4080FF", "多路温湿度采集板卡上位机软件", "#323C4D", 5);

    // 3. 设置界面大小
    this->setWindowState(Qt::WindowMaximized);
    this->setMinimumSize(1280, 800);

    // 4. 初始化界面内容
    initContentArea();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化界面内容区
void MainWindow::initContentArea(void)
{
    // 1.清空原有界面格式
    if (ui->centralwidget->layout() != nullptr) {
        delete ui->centralwidget->layout();
    }

    // 2.创建内容区主容器
    contentContainer = new QWidget(ui->centralwidget);
    contentContainer->setStyleSheet(R"(
                                    QWidget {
                                    background-color: #475776; /* 内容区背景色 */
                                    border: none;
                                    }
                                    )"
                                    );

    // 3. 给原centralwidget设置垂直布局（作为内容区的根布局）
    QVBoxLayout *contentRootLayout = new QVBoxLayout(ui->centralwidget);
    contentRootLayout->setContentsMargins(10, 10, 10, 10); // 内容区内边距
    contentRootLayout->setSpacing(15); // 控件间距
    contentRootLayout->addWidget(contentContainer); // 将自定义容器加入布局

    // 3.1 给自定义容器设置子布局（示例：水平布局，可换成QVBoxLayout）
    QVBoxLayout *containerLayout = new QVBoxLayout(contentContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(20);

    // 4.1-子容器1：TCP连接区域
    tcpConnectControlArea = new QWidget(contentContainer);
    tcpConnectControlArea->setMaximumHeight(400);
    tcpConnectControlArea->setStyleSheet(R"(
                                    QWidget {
                                    background-color: #ffffff; /* 内容区背景色 */
                                    border: none;
                                    border-radius: 4px;
                                    }
                                    )"
                                    );
    // 4.2-IP/端口配置行布局（核心）
    QHBoxLayout *configLayout = new QHBoxLayout(tcpConnectControlArea);
    configLayout->setContentsMargins(30, 30, 30, 30);
    configLayout->setSpacing(20); // 控件之间的间距

    // ===== 4.3 标签 + 输入框：IP地址 =====
    ipLabel = new QLabel("IP地址:", tcpConnectControlArea);
    ipLabel->setStyleSheet("font-size: 16px; color: #333333;");
    ipLabel->setFixedWidth(80); // 固定标签宽度，对齐更整齐
    ipLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter); // 文字右对齐

    ipEdit = new QLineEdit(tcpConnectControlArea);
    ipEdit->setPlaceholderText("请输入IP地址");
    ipEdit->setText("192.168.1.30"); // 默认值
    ipEdit->setFixedWidth(150); // 固定输入框宽度
    ipEdit->setStyleSheet(R"(
                          QLineEdit {
                          border: 1px solid #DCDFE6;
                          border-radius: 4px;
                          padding: 8px 12px;
                          font-size: 16px;
                          color: #333333;
                          background-color: #FFFFFF;
                          }
                          QLineEdit:focus {
                          border-color: #4080FF;
                          outline: none;
                          }
                          )");

    // ===== 4.4 标签 + 输入框：端口 =====
    portLabel = new QLabel("端口:", tcpConnectControlArea);
    portLabel->setStyleSheet("font-size: 16px; color: #333333;");
    portLabel->setFixedWidth(80);
    portLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    portEdit = new QLineEdit(tcpConnectControlArea);
    portEdit->setPlaceholderText("请输入端口号");
    portEdit->setText("8080"); // 默认值
    portEdit->setFixedWidth(120);
    portEdit->setStyleSheet(R"(
                            QLineEdit {
                            border: 1px solid #DCDFE6;
                            border-radius: 4px;
                            padding: 8px 12px;
                            font-size: 16px;
                            color: #333333;
                            background-color: #FFFFFF;
                            }
                            QLineEdit:focus {
                            border-color: #4080FF;
                            outline: none;
                            }
                            )");

    /* 限制只能输入 0 到 65535 之间的整数 */
    QIntValidator *validator = new QIntValidator(0, 65535, this);
    portEdit->setValidator(validator);

    // ===== 4.5 按钮：连接 =====
    connectBtn = new QPushButton("连接", tcpConnectControlArea);
    connectBtn->setFixedSize(100, 40); // 固定按钮大小
    connectBtn->setStyleSheet(R"(
                              QPushButton {
                              border: none;
                              border-radius: 4px;
                              background-color: #4080FF;
                              color: #FFFFFF;
                              font-size: 16px;
                              }
                              QPushButton:hover {
                              background-color: #6699FF;
                              }
                              QPushButton:pressed {
                              background-color: #3373DD;
                              }
                              QPushButton:disabled {
                              background-color: #BBBBBB;
                              }
                              )");

    // ===== 4.6 按钮：断开 =====
    disconnectBtn = new QPushButton("断开", tcpConnectControlArea);
    disconnectBtn->setFixedSize(100, 40);
    disconnectBtn->setStyleSheet(R"(
                                 QPushButton {
                                 border: 1px solid #DCDFE6;
                                 border-radius: 4px;
                                 background-color: #FFFFFF;
                                 color: #333333;
                                 font-size: 16px;
                                 }
                                 QPushButton:hover {
                                 border-color: #C0C4CC;
                                 background-color: #F5F5F5;
                                 }
                                 QPushButton:pressed {
                                 background-color: #E6E6E6;
                                 }
                                 QPushButton:disabled {
                                 color: #999999;
                                 background-color: #F5F5F5;
                                 }
                                 )");
    disconnectBtn->setEnabled(false); // 默认禁用“断开”按钮

    /* 4.7 TCP连接状态显示 */
    tcpConncetState = new QLabel(tcpConnectControlArea);
    tcpConncetState->setText("采集板卡：断开连接");
    tcpConncetState->setStyleSheet("font-size: 16px; color: #333333;");
    tcpConncetState->setFixedWidth(180);
    tcpConncetState->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    /* 4.8 文本浏览框 */
    textBrowser = new QTextBrowser(tcpConnectControlArea);
    textBrowser->setStyleSheet("font-size: 16px; color: #333333;");
    textBrowser->setFixedWidth(380);
    textBrowser->setFixedHeight(200);
    textBrowser->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    textBrowser->setStyleSheet(QString(R"(
                                         QWidget {
                                         border: 4px solid #000000; /* 边框宽度 + 颜色 */
                                         border-radius: 1px; /* 可选：窗口圆角 */
                                         background-color: #ffffff; /* 窗口内容区背景色 */
                                         }
                                         )"));

    // 发送文本框
    lineEdit = new QLineEdit(tcpConnectControlArea);
    lineEdit->setFixedWidth(280);
    lineEdit->setFixedHeight(50);
    lineEdit->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    lineEdit->setStyleSheet(QString(R"(
                                         QWidget {
                                         border: 4px solid #000000; /* 边框宽度 + 颜色 */
                                         border-radius: 1px; /* 可选：窗口圆角 */
                                         background-color: #ffffff; /* 窗口内容区背景色 */
                                         }
                                         )"));

    // 发送消息按键
    messageSendBtn = new QPushButton("发送",tcpConnectControlArea);
    messageSendBtn->setFixedSize(100, 40);
    messageSendBtn->setStyleSheet(R"(
                              QPushButton {
                              border: none;
                              border-radius: 4px;
                              background-color: #4080FF;
                              color: #FFFFFF;
                              font-size: 16px;
                              }
                              QPushButton:hover {
                              background-color: #6699FF;
                              }
                              QPushButton:pressed {
                              background-color: #3373DD;
                              }
                              QPushButton:disabled {
                              background-color: #BBBBBB;
                              }
                              )");
    // ===== 4.9 将控件添加到布局 =====
    configLayout->addWidget(ipLabel);
    configLayout->addWidget(ipEdit);
    configLayout->addWidget(portLabel);
    configLayout->addWidget(portEdit);
    configLayout->addSpacing(40); // 增加空白间距，分隔输入框和按钮
    configLayout->addWidget(connectBtn);
    configLayout->addWidget(disconnectBtn);
    configLayout->addSpacing(20);
    configLayout->addWidget(tcpConncetState);
    configLayout->addWidget(textBrowser);
    configLayout->addWidget(lineEdit);
    configLayout->addWidget(messageSendBtn);
    configLayout->addStretch(); // 右侧拉伸空白，让控件靠左对齐

    /* TCP连接与断开信号与槽函数 */
    connect(connectBtn, &QPushButton::clicked, this, &MainWindow::toConnect);
    connect(disconnectBtn, &QPushButton::clicked, this, &MainWindow::toDisConnect);
    connect(messageSendBtn, &QPushButton::clicked, this, &MainWindow::sendMessages);

    connect(this, &MainWindow::sigConnect, tcpWorker, &TcpWorker::connectToHost);
    connect(this, &MainWindow::sigSend, tcpWorker, &TcpWorker::sendData);
    connect(this, &MainWindow::sigDisconnect, tcpWorker, &TcpWorker::disconnectFromHost);
    connect(this, &MainWindow::sigUpdate, tcpWorker, &TcpWorker::startUpgrade);

    connect(tcpWorker,  &TcpWorker::sigConnected, this, &MainWindow::connected);
    connect(tcpWorker,  &TcpWorker::sigDisconnected, this, &MainWindow::disconnected);
    connect(tcpWorker,  &TcpWorker::sigRecv, this, &MainWindow::receiveMessages);
    connect(tcpWorker,  &TcpWorker::updateUpgradeProgress, this, &MainWindow::onUpgradeProgress);
    connect(tcpWorker,  &TcpWorker::updateUpgradeStatus, this, &MainWindow::onUpgradeStatus);
    // 5. ota升级模块设计
    firmwareUpdateWidget = new QWidget(contentContainer);
    firmwareUpdateWidget->setMaximumHeight(200);
    firmwareUpdateWidget->setStyleSheet(
                    R"(QWidget{
                    background-color: #ffffff;
                    border: none;
                    border-radius: 4px;
                    })"
                );

    // ========== 关键修复：给OTA模块添加布局 ==========
    // 改为垂直布局，先放原有控件行，再放进度条行
    QVBoxLayout *otaMainLayout = new QVBoxLayout(firmwareUpdateWidget);
    otaMainLayout->setContentsMargins(30, 30, 30, 30);
    otaMainLayout->setSpacing(15);

    // 第一行：原有控件的水平布局
    QHBoxLayout *otaTopLayout = new QHBoxLayout();
    otaTopLayout->setSpacing(20);

    // OTA控件1：标签
    otaLabel = new QLabel("OTA固件升级:", firmwareUpdateWidget);
    otaLabel->setStyleSheet("font-size: 16px; color: #333333;");
    otaLabel->setFixedWidth(100);
    otaLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // OTA控件2：固件文件选择按钮
    selectFileBtn = new QPushButton("选择固件", firmwareUpdateWidget);
    selectFileBtn->setFixedSize(120, 40);
    selectFileBtn->setStyleSheet(R"(
                              QPushButton {
                              border: 1px solid #DCDFE6;
                              border-radius: 4px;
                              background-color: #FFFFFF;
                              color: #333333;
                              font-size: 16px;
                              }
                              QPushButton:hover {
                              border-color: #4080FF;
                              }
                              )");

    // OTA控件3：固件路径显示
    firmwarePathEdit = new QLineEdit(firmwareUpdateWidget);
    firmwarePathEdit->setPlaceholderText("未选择固件文件");
    firmwarePathEdit->setFixedWidth(300);
    firmwarePathEdit->setStyleSheet(R"(
                          QLineEdit {
                          border: 1px solid #DCDFE6;
                          border-radius: 4px;
                          padding: 8px 12px;
                          font-size: 16px;
                          color: #333333;
                          background-color: #FFFFFF;
                          }
                          )");
    firmwarePathEdit->setReadOnly(true); // 只读，防止手动修改

    // OTA控件4：升级按钮
    updateBtn = new QPushButton("开始升级", firmwareUpdateWidget);
    updateBtn->setFixedSize(120, 40);
    updateBtn->setStyleSheet(R"(
                              QPushButton {
                              border: none;
                              border-radius: 4px;
                              background-color: #4080FF;
                              color: #FFFFFF;
                              font-size: 16px;
                              }
                              QPushButton:hover {
                              background-color: #6699FF;
                              }
                              QPushButton:pressed {
                              background-color: #3373DD;
                              }
                              QPushButton:disabled {
                              background-color: #BBBBBB;
                              }
                              )");
    updateBtn->setEnabled(false); // 默认禁用，选择固件后再启用

    // OTA控件5：升级状态
    updateStateLabel = new QLabel("升级状态：未开始", firmwareUpdateWidget);
    updateStateLabel->setStyleSheet("font-size: 16px; color: #333333;");
    updateStateLabel->setFixedWidth(180);
    updateStateLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // 将原有控件添加到顶部水平布局
    otaTopLayout->addWidget(otaLabel);
    otaTopLayout->addWidget(selectFileBtn);
    otaTopLayout->addWidget(firmwarePathEdit);
    otaTopLayout->addWidget(updateBtn);
    otaTopLayout->addWidget(updateStateLabel);
    otaTopLayout->addStretch();

    // 第二行：进度条控件
    QHBoxLayout *otaProgressLayout = new QHBoxLayout();
    otaProgressLayout->setSpacing(10);

    // 进度条标签
    QLabel *progressLabel = new QLabel("升级进度：", firmwareUpdateWidget);
    progressLabel->setStyleSheet("font-size: 16px; color: #333333;");
    progressLabel->setFixedWidth(100);
    progressLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // 升级进度条（核心新增控件）
    updateProgressBar = new QProgressBar(firmwareUpdateWidget);
    updateProgressBar->setFixedHeight(30);
    updateProgressBar->setFixedWidth(600); // 进度条宽度
    // 进度条样式美化
    updateProgressBar->setStyleSheet(R"(
                              QProgressBar {
                              border: 1px solid #DCDFE6;
                              border-radius: 4px;
                              background-color: #F5F7FA;
                              text-align: center;
                              font-size: 14px;
                              color: #333333;
                              }
                              QProgressBar::chunk {
                              border-radius: 3px;
                              background-color: #4080FF;
                              }
                              )");
    updateProgressBar->setRange(0, 100); // 进度范围0-100%
    updateProgressBar->setValue(0); // 初始进度0%


    // 将进度条控件添加到进度布局
    otaProgressLayout->addWidget(progressLabel);
    otaProgressLayout->addWidget(updateProgressBar);
    otaProgressLayout->addStretch();

    // 将顶部布局和进度布局添加到主垂直布局
    otaMainLayout->addLayout(otaTopLayout);
    otaMainLayout->addLayout(otaProgressLayout);

    connect(selectFileBtn, &QPushButton::clicked, this, [=]() {

        QString filePath = QFileDialog::getOpenFileName(
                    this,
                    "选择固件文件",
                    "",
                    "Bin文件 (*.bin);;Hex文件 (*.hex);;所有文件 (*.*)");

        if (filePath.isEmpty())
            return;

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "错误", "无法打开固件文件！");
            return;
        }

        // 读取全部数据
        m_firmwareData = file.readAll();
        file.close();

        if (m_firmwareData.isEmpty()) {
            QMessageBox::warning(this, "错误", "固件文件为空！");
            return;
        }

        // 保存路径
        m_firmwarePath = filePath;

        // 更新UI
        firmwarePathEdit->setText(filePath);
        updateBtn->setEnabled(true);
        updateProgressBar->setValue(0);
        updateStateLabel->setText("升级状态：未开始");

    });

    connect(updateBtn, &QPushButton::clicked,
            this, &MainWindow::startUpgrade);

    // 6. 将子容器添加到主容器的布局中
    contentRootLayout->addWidget(tcpConnectControlArea);
    contentRootLayout->addWidget(firmwareUpdateWidget);
    contentRootLayout->addStretch();
}

void MainWindow::toConnect()
{
    QString ip = ipEdit->text();
    QString portStr = portEdit->text();
    quint16 port = portStr.toUInt();
    emit sigConnect(ip, port);
}

void MainWindow::toDisConnect()
{
    emit sigDisconnect();
}

void MainWindow::connected()
{
    connectBtn->setEnabled(false);
    disconnectBtn->setEnabled(true);
    tcpConncetState->setText("采集板卡：已连接");
}

void MainWindow::disconnected()
{
    connectBtn->setEnabled(true);
    disconnectBtn->setEnabled(false);
    tcpConncetState->setText("采集板卡：断开连接");
}

void MainWindow::receiveMessages(QString messages)
{
    // 1. 转换为UTF-8字节数组
    QByteArray byteData = messages.toUtf8();

    // 2. 直接转换为紧凑的16进制字符串（QByteArray内置方法）
    QString hexStr = byteData.toHex().toUpper();

    // 3. 格式化显示（每两个字符加空格，更易读）
    QString formattedHex;
    for (int i = 0; i < hexStr.size(); i += 2) {
        formattedHex += hexStr.mid(i, 2) + " ";
    }

    // 4. 追加到textBrowser
    textBrowser->append("服务端（16进制）：" + formattedHex.trimmed());
}

void MainWindow::sendMessages()
{
    QString sendMessage_str = lineEdit->text();
    QByteArray sendMessage = sendMessage_str.toUtf8();
    textBrowser->append("客户端：" + sendMessage_str);
    emit sigSend(sendMessage);
}

void MainWindow::startUpgrade()
{
    emit sigUpdate(m_firmwareData);
}

void MainWindow::onUpgradeProgress(int percent)
{
    updateProgressBar->setValue(percent);
    updateProgressBar->update(); // 强制重绘
}

void MainWindow::onUpgradeStatus(QString status)
{
    updateStateLabel->setText(status);
}

void MainWindow::onUpgradeFinished(bool success, QString message)
{
    QMessageBox::information(this,
                             success ? "升级成功" : "升级失败",
                             message);
}

// 核心：创建自定义标题栏（支持全样式自定义）
void MainWindow::createCustomTitleBar(int height, QString bgColor, QString textColor, QString hoverColor, QString titleText, QString borderColor, int borderWidth)
{
    // 1. 创建标题栏容器
    QWidget *titleBar = new QWidget(this);
    titleBar->setFixedHeight(height);
    titleBar->setStyleSheet(QString("QWidget { background-color: %1; }").arg(bgColor));

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(15, 0, 10, 0);
    titleLayout->setSpacing(15);

    // ========== 新增：添加标题栏图标 ==========
    QLabel *titleFlag = new QLabel(titleBar);
    // 设置图片（资源文件路径）
    QPixmap pixmap(":/new/res/res/flag.png");
    // 缩放图片到合适大小（适配标题栏高度，保留宽高比）
    pixmap = pixmap.scaled(height - 10, height - 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    titleFlag->setPixmap(pixmap);
    // 让图片垂直居中
    titleFlag->setAlignment(Qt::AlignVCenter);

    QLabel *titleLabel = new QLabel(titleText, titleBar);
    titleLabel->setStyleSheet(QString("color: %1; font-size: 24px; font-weight: 500;").arg(textColor));

    QPushButton *minBtn = new QPushButton(titleBar);
    minBtn->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarMinButton));
    minBtn->setFixedSize(height - 10, height - 10);
    minBtn->setStyleSheet(QString(R"(
                                  QPushButton { border: none; border-radius: 4px; background-color: #696969;}
                                  QPushButton:hover { background-color: %1; }
                                  )").arg(hoverColor));

    QPushButton *closeBtn = new QPushButton(titleBar);
    closeBtn->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    closeBtn->setFixedSize(height - 10, height - 10);
    closeBtn->setStyleSheet(R"(
                            QPushButton { border: none; border-radius: 4px; background-color: #696969; }
                            QPushButton:hover { background-color: #FF4D4F; }
                            )");

    titleLayout->addWidget(titleFlag); // 图标居左
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(minBtn);
    titleLayout->addWidget(closeBtn);

    // 2. 创建主容器（核心：添加边框样式）
    QWidget *centralWidget = new QWidget(this);
    // 设置主容器样式：边框颜色、宽度、背景色（窗口内容区背景）
    centralWidget->setStyleSheet(QString(R"(
                                         QWidget {
                                         border: %1px solid %2; /* 边框宽度 + 颜色 */
                                         border-radius: 1px; /* 可选：窗口圆角 */
                                         background-color: #2A364A; /* 窗口内容区背景色 */
                                         }
                                         )").arg(borderWidth).arg(borderColor));

    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);

    centralLayout->addWidget(titleBar);
    centralLayout->addWidget(ui->centralwidget);
    this->setCentralWidget(centralWidget);

    // 绑定按钮事件
    connect(minBtn, &QPushButton::clicked, this, &MainWindow::showMinimized);
    connect(closeBtn, &QPushButton::clicked, this, &MainWindow::close);

    // 安装事件过滤器（窗口拖动）
    titleBar->installEventFilter(this);
}

// 事件过滤器：实现自定义标题栏的拖动功能
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            // 记录鼠标按下时的位置
            lastTitleBarPos = mouseEvent->globalPos() - this->frameGeometry().topLeft();
            return true;
        }
    }
    else if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->buttons() & Qt::LeftButton)
        {
            // 移动窗口
            this->move(mouseEvent->globalPos() - lastTitleBarPos);
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
