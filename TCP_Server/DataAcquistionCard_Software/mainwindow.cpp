#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaType>


#include <QMetaType>

// 在任何 connect 语句执行之前调用

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qRegisterMetaType<QVector<uint8_t>>("QVector<uint8_t>");

    ui->setupUi(this);

    // 初始化 TCP 线程与 Worker
    tcpThread = new QThread;
    tcpWorker = new TcpWorker;
    tcpWorker->moveToThread(tcpThread);
    tcpThread->start();

    // 初始化 文件 线程与 Worker
    fileThread = new QThread;
    fileWorker = new FileWorker;
    fileWorker->moveToThread(fileThread);
    fileThread->start();

    // TCP 线程解析出数据后，直接发给文件线程

    bool ok = connect(tcpWorker, &TcpWorker::sendSensorDataToPrevent,
                     fileWorker, &FileWorker::onSaveSensorData);
    if(!ok) {
        qDebug() << "Signal-Slot connection failed!";
    }


    // 资源清理连接
    connect(fileThread, &QThread::finished, fileWorker, &QObject::deleteLater);

    // 1. 隐藏原生标题栏（必须）
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, false); // 关闭透明（可选）

    // 2. 创建自定义标题栏
    // 参数：高度50px，背景色深蓝色，文字白色，按钮悬浮浅蓝
    createCustomTitleBar(50, "#323C4D", "#ffffff", "#4080FF", "多路温湿度采集板卡上位机软件", "#323C4D", 5);

    // 3. 设置界面大小
    this->setWindowState(Qt::WindowMaximized);
    this->setMinimumSize(1280, 800);

    // 4. 初始化界面内容
    initContentArea();
}

MainWindow::~MainWindow()
{
    // 退出线程处理
    if (tcpThread->isRunning()) {
        tcpThread->quit();
        tcpThread->wait();
    }
    delete tcpWorker;
    delete tcpThread;

    if (fileThread->isRunning()) {
        fileThread->quit();
        fileThread->wait();
    }
    delete fileWorker;
    delete fileThread;

    delete ui;
}

// 初始化界面内容区域
void MainWindow::initContentArea(void)
{
    // 1. 清空原有界面格式
    if (ui->centralwidget->layout() != nullptr)
    {
        delete ui->centralwidget->layout();
    }

    // 2. 创建内容区主容器
    contentContainer = new QWidget(ui->centralwidget);
    contentContainer->setStyleSheet(R"(
        QWidget {
            background-color: #475776; /* 内容区背景色 */
            border: none;
        }
    )");

    // 3. 给原 centralwidget 设置垂直布局（作为内容区的根布局）
    QVBoxLayout *contentRootLayout = new QVBoxLayout(ui->centralwidget);
    contentRootLayout->setContentsMargins(10, 10, 10, 10); // 内容区内边距
    contentRootLayout->setSpacing(10);                     // 控件间距
    contentRootLayout->addWidget(contentContainer);

    // 整体为垂直布局
    QVBoxLayout *containerLayout = new QVBoxLayout(contentContainer);
    containerLayout->setContentsMargins(10, 10, 10, 10);
    containerLayout->setSpacing(0);

    // 第一行：控制区域 --> 横向分布
    contolArea = new QWidget(contentContainer);
    contolArea->setMaximumHeight(230);

    QHBoxLayout *contolAreaLayout = new QHBoxLayout(contolArea);
    contolAreaLayout->setContentsMargins(10, 10, 10, 5);
    contolAreaLayout->setSpacing(10);

    tcpConnectWindow();
    firmwareUpdateWindow();

    contolAreaLayout->addWidget(tcpConnectControlArea, 0, Qt::AlignTop);
    contolAreaLayout->addWidget(firmwareUpdateWidget, 0, Qt::AlignTop);

    // 第二行：采集参数与数据显示区域 --> 横向布局
    parameterAndSensorArea = new QWidget(contentContainer);

    QHBoxLayout *parameterAndSensorAreaLayout = new QHBoxLayout(parameterAndSensorArea);
    parameterAndSensorAreaLayout->setContentsMargins(10, 5, 10, 10);
    parameterAndSensorAreaLayout->setSpacing(10);

    parametersWindow();
    sensorDataPreventWindow();

    parameterAndSensorAreaLayout->addWidget(parametersWidget, 0, Qt::AlignTop);
    parameterAndSensorAreaLayout->addWidget(sensorDataPreventWidget, 0, Qt::AlignTop);

    //******************************************************
    containerLayout->addWidget(contolArea);
    containerLayout->addWidget(parameterAndSensorArea);
    containerLayout->addStretch();
}

void MainWindow::tcpConnectWindow()
{
    // 1. 容器整体样式：固定高度，增加浅色边框增强层次感
    tcpConnectControlArea = new QWidget(contentContainer);
    tcpConnectControlArea->setFixedHeight(120); // 压缩高度，消除空白感
    tcpConnectControlArea->setStyleSheet(R"(
        QWidget {
            background-color: #FFFFFF;
            border: 1px solid #DCDFE6;
            border-radius: 6px;
        }
    )");

    // 2. 主垂直布局：设置紧凑的边距和间距
    QVBoxLayout *tcpLayout = new QVBoxLayout(tcpConnectControlArea);
    tcpLayout->setContentsMargins(20, 15, 20, 15);
    tcpLayout->setSpacing(12);

    // --- 第一行：网络配置参数 ---
    QWidget *row1 = new QWidget(tcpConnectControlArea);
    row1->setStyleSheet("border: none;");
    QHBoxLayout *layout1 = new QHBoxLayout(row1);
    layout1->setContentsMargins(0, 0, 0, 0);
    layout1->setSpacing(10);

    // 统一输入框样式
    QString editStyle = R"(
        QLineEdit {
            border: 1px solid #DCDFE6;
            border-radius: 4px;
            padding: 5px 10px;
            font-size: 14px;
            background-color: #F5F7FA;
        }
        QLineEdit:focus { border-color: #4080FF; background-color: #FFFFFF; }
    )";

    ipLabel = new QLabel("IP地址:", row1);
    ipLabel->setFixedWidth(50);
    ipEdit = new QLineEdit("192.168.1.30", row1);
    ipEdit->setFixedWidth(130);
    ipEdit->setStyleSheet(editStyle);

    portLabel = new QLabel("端口:", row1);
    portLabel->setFixedWidth(40);
    portEdit = new QLineEdit("8080", row1);
    portEdit->setFixedWidth(70);
    portEdit->setStyleSheet(editStyle);
    portEdit->setValidator(new QIntValidator(0, 65535, this));

    connectBtn = new QPushButton("连接", row1);
    connectBtn->setFixedSize(160, 32);
    connectBtn->setStyleSheet(R"(
        QPushButton { background-color: #409EFF; color: white; border-radius: 4px; font-weight: bold; border:none; }
        QPushButton:hover { background-color: #66B1FF; }
        QPushButton:pressed { background-color: #3A8EE6; }
        QPushButton:disabled { background-color: #C0C4CC; }
    )");

    disconnectBtn = new QPushButton("断开", row1);
    disconnectBtn->setFixedSize(160, 32);
    disconnectBtn->setEnabled(false);
    disconnectBtn->setStyleSheet(R"(
        QPushButton { background-color: #FFFFFF; color: #F56C6C; border: 1px solid #F56C6C; border-radius: 4px; }
        QPushButton:hover { background-color: #FEF0F0; }
        QPushButton:pressed { background-color: #FBC4C4; }
    )");

    tcpConncetState = new QLabel("● 断开连接", row1);
    tcpConncetState->setStyleSheet("color: #909399; font-weight: bold; margin-left: 10px;");

    layout1->addWidget(ipLabel);
    layout1->addWidget(ipEdit);
    layout1->addWidget(portLabel);
    layout1->addWidget(portEdit);
    layout1->addSpacing(10);
    layout1->addWidget(connectBtn);
    layout1->addWidget(disconnectBtn);
    layout1->addStretch();
    layout1->addWidget(tcpConncetState);

    // --- 第二行：指令发送区域 ---
    QWidget *row2 = new QWidget(tcpConnectControlArea);
    row2->setStyleSheet("border: none;");
    QHBoxLayout *layout2 = new QHBoxLayout(row2);
    layout2->setContentsMargins(0, 0, 0, 0);
    layout2->setSpacing(10);

    lineEdit = new QLineEdit(row2);
    lineEdit->setPlaceholderText("在此输入待发送的 16 进制或字符串指令...");
    lineEdit->setFixedHeight(35);
    lineEdit->setStyleSheet(editStyle);

    messageSendBtn = new QPushButton("发送数据", row2);
    messageSendBtn->setFixedSize(100, 35);
    messageSendBtn->setStyleSheet(R"(
        QPushButton { background-color: #67C23A; color: white; border-radius: 4px; font-weight: bold; border:none; }
        QPushButton:hover { background-color: #85CE61; }
    )");

    layout2->addWidget(lineEdit);
    layout2->addWidget(messageSendBtn);

    // 3. 将行装入主容器布局
    tcpLayout->addWidget(row1);
    tcpLayout->addWidget(row2);

    // 4. 信号槽绑定
    connect(connectBtn, &QPushButton::clicked, this, &MainWindow::toConnect);
    connect(disconnectBtn, &QPushButton::clicked, this, &MainWindow::toDisConnect);
    connect(messageSendBtn, &QPushButton::clicked, this, &MainWindow::sendMessages);

    connect(this, &MainWindow::sigConnect, tcpWorker, &TcpWorker::connectToHost);
    connect(this, &MainWindow::sigSend, tcpWorker, &TcpWorker::sendData);
    connect(this, &MainWindow::sigDisconnect, tcpWorker, &TcpWorker::disconnectFromHost);
    connect(this, &MainWindow::sigSendParametersData, tcpWorker, &TcpWorker::onSendParametersData);

    connect(tcpWorker, &TcpWorker::sigConnected, this, &MainWindow::connected);
    connect(tcpWorker, &TcpWorker::sigDisconnected, this, &MainWindow::disconnected);
    connect(tcpWorker, &TcpWorker::sigRecv, this, &MainWindow::receiveMessages);
    connect(tcpWorker, &TcpWorker::sendSensorDataToPrevent, this, &MainWindow::receSensorDataToPrevent);
}

void MainWindow::firmwareUpdateWindow()
{
    // 1. 容器整体样式：与 TCP 控制框保持完全一致
    firmwareUpdateWidget = new QWidget(contentContainer);
    firmwareUpdateWidget->setFixedHeight(120); // 固定高度，消除空白感
    firmwareUpdateWidget->setStyleSheet(R"(
        QWidget {
            background-color: #FFFFFF;
            border: 1px solid #DCDFE6;
            border-radius: 6px;
        }
    )");

    // 2. 主垂直布局：设置紧凑边距
    QVBoxLayout *otaMainLayout = new QVBoxLayout(firmwareUpdateWidget);
    otaMainLayout->setContentsMargins(20, 15, 20, 15);
    otaMainLayout->setSpacing(12);

    // 统一输入框与按钮的基础样式
    QString editStyle = R"(
        QLineEdit {
            border: 1px solid #DCDFE6;
            border-radius: 4px;
            padding: 5px 10px;
            font-size: 14px;
            background-color: #F5F7FA;
            color: #333333;
        }
        QLineEdit:focus { border-color: #4080FF; background-color: #FFFFFF; }
    )";

    // --- 第一行：文件选择与操作 ---
    QHBoxLayout *otaTopLayout = new QHBoxLayout();
    otaTopLayout->setSpacing(10);

    otaLabel = new QLabel("固件升级:", firmwareUpdateWidget);
    otaLabel->setFixedWidth(70); // 与进度条标签对齐
    otaLabel->setStyleSheet("font-size: 14px; color: #606266; font-weight: bold; border:none;");

    selectFileBtn = new QPushButton("选择文件", firmwareUpdateWidget);
    selectFileBtn->setFixedSize(80, 32);
    selectFileBtn->setStyleSheet(R"(
        QPushButton { background-color: #FFFFFF; border: 1px solid #DCDFE6; border-radius: 4px; color: #606266; }
        QPushButton:hover { border-color: #4080FF; color: #4080FF; }
    )");

    firmwarePathEdit = new QLineEdit(firmwareUpdateWidget);
    firmwarePathEdit->setPlaceholderText("未选择固件路径...");
    firmwarePathEdit->setReadOnly(true);
    firmwarePathEdit->setMinimumWidth(500);
    firmwarePathEdit->setFixedHeight(35);
    firmwarePathEdit->setStyleSheet(editStyle);

    updateBtn = new QPushButton("开始升级", firmwareUpdateWidget);
    updateBtn->setFixedSize(90, 32);
    updateBtn->setEnabled(false);
    updateBtn->setStyleSheet(R"(
        QPushButton { background-color: #4080FF; color: white; border-radius: 4px; font-weight: bold; border:none; }
        QPushButton:hover { background-color: #66B1FF; }
        QPushButton:disabled { background-color: #C0C4CC; }
    )");

    updateStateLabel = new QLabel("等待中", firmwareUpdateWidget);
    updateStateLabel->setFixedWidth(120);
    updateStateLabel->setStyleSheet("color: #909399; font-size: 13px; font-weight: bold; border:none; margin-left:5px;");

    otaTopLayout->addWidget(otaLabel);
    otaTopLayout->addWidget(selectFileBtn);
    otaTopLayout->addWidget(firmwarePathEdit);
    otaTopLayout->addWidget(updateBtn);
    otaTopLayout->addStretch();
    otaTopLayout->addWidget(updateStateLabel);

    // --- 第二行：进度条展示 ---
    QHBoxLayout *otaProgressLayout = new QHBoxLayout();
    otaProgressLayout->setSpacing(10);

    QLabel *progressLabel = new QLabel("升级进度:", firmwareUpdateWidget);
    progressLabel->setFixedWidth(70);
    progressLabel->setStyleSheet("font-size: 14px; color: #606266; font-weight: bold; border:none;");

    updateProgressBar = new QProgressBar(firmwareUpdateWidget);
    updateProgressBar->setFixedHeight(25); // 更加纤细现代
    updateProgressBar->setRange(0, 100);
    updateProgressBar->setValue(0);
    updateProgressBar->setTextVisible(true);
    updateProgressBar->setAlignment(Qt::AlignCenter);
    updateProgressBar->setStyleSheet(R"(
        QProgressBar {
            border: none;
            background-color: #EBEEF5;
            border-radius: 9px;
            text-align: center;
            color: #333333;
            font-size: 11px;
        }
        QProgressBar::chunk {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4080FF, stop:1 #6699FF);
            border-radius: 9px;
        }
    )");

    otaProgressLayout->addWidget(progressLabel);
    otaProgressLayout->addWidget(updateProgressBar);

    // 3. 将行装入主布局
    otaMainLayout->addLayout(otaTopLayout);
    otaMainLayout->addLayout(otaProgressLayout);

    // 4. 信号槽逻辑
    connect(selectFileBtn, &QPushButton::clicked, this, [=]() {
        QString filePath = QFileDialog::getOpenFileName(this, "选择固件", "", "Bin (*.bin);;All (*.*)");
        if (filePath.isEmpty()) return;

        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            m_firmwareData = file.readAll();
            file.close();
            if (!m_firmwareData.isEmpty()) {
                m_firmwarePath = filePath;
                firmwarePathEdit->setText(filePath);
                updateBtn->setEnabled(true);
                updateStateLabel->setText("就绪");
                updateStateLabel->setStyleSheet("color: #67C23A; font-weight: bold; border:none;"); // 变绿
            }
        }
    });

    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::startUpgrade);
}

void MainWindow::parametersWindow()
{
    parametersWidget = new QWidget(contentContainer);
    parametersWidget->setMaximumWidth(250);
    parametersWidget->setStyleSheet(R"(
                                    QWidget {
                                    background-color: #FFFFFF;
                                    border: 1px solid #DCDFE6;
                                    border-radius: 6px;
                                    }
                                    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(parametersWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // 1. 标题部分
    sensorOpenLabel = new QLabel("传感器通讯开关:", parametersWidget);
    sensorOpenLabel->setStyleSheet("font-size: 18px; color: #333333; font-weight: bold; border:none;");
    mainLayout->addWidget(sensorOpenLabel);

    // 2. 创建网格容器
    QWidget *gridContainer = new QWidget(parametersWidget);
    gridContainer->setStyleSheet("border:none;");
    QGridLayout *gridLayout = new QGridLayout(gridContainer);
    gridLayout->setSpacing(15); // 控制开关之间的间距
    gridLayout->setContentsMargins(5, 5, 5, 5);

    // 统一复选框样式（美化为类似开关的勾选效果）
    QString checkStyle = R"(
                         QCheckBox {
                         font-size: 14px;
                         color: #606266;
                         spacing: 8px;
                         border: none;
                         }
                         QCheckBox::indicator {
                         width: 18px;
                         height: 18px;
                         border: 1px solid #DCDFE6;
                         border-radius: 3px;
                         }
                         QCheckBox::indicator:unchecked { background-color: #FFFFFF; }
                         QCheckBox::indicator:checked { background-color: #4080FF; image: url(:/new/res/res/check.png); border-color: #4080FF; }
                         QCheckBox::indicator:hover { border-color: #4080FF; }
                         )";

    // 3. 循环生成 16 路开关
    for (int i = 0; i < 16; ++i) {
        // 创建开关，编号从 1 开始
        sensorCheckBoxes[i] = new QCheckBox(QString("通道 %1").arg(i + 1), gridContainer);
        sensorCheckBoxes[i]->setStyleSheet(checkStyle);
        sensorCheckBoxes[i]->setCursor(Qt::PointingHandCursor);
        if (i < 3) {
            sensorCheckBoxes[i]->setChecked(true);
        }
        gridLayout->addWidget(sensorCheckBoxes[i], i / 2, i % 2);   // 计算网格位置：每行 2 个，共 8 行
    }
    mainLayout->addWidget(gridContainer);

    line = new QFrame(parametersWidget);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #EBEEF5;");
    mainLayout->addWidget(line);

    thresholdTitle = new QLabel("环境预警阈值设置:", parametersWidget);
    thresholdTitle->setStyleSheet("font-size: 18px; color: #333333; font-weight: bold; border:none; margin-top:5px;");
    mainLayout->addWidget(thresholdTitle);

    // 容器布局：两行设置
    QWidget *thresholdArea = new QWidget(parametersWidget);
    thresholdArea->setStyleSheet("border:none;");
    QGridLayout *thresholdLayout = new QGridLayout(thresholdArea);
    thresholdLayout->setContentsMargins(5, 5, 5, 5);
    thresholdLayout->setSpacing(10);

    // 统一输入框样式
    QString thresholdEditStyle = R"(
                                 QLineEdit {
                                 border: 1px solid #DCDFE6;
                                 border-radius: 4px;
                                 padding: 3px 5px;
                                 background-color: #F5F7FA;
                                 font-size: 13px;
                                 }
                                 QLineEdit:focus { border-color: #4080FF; background-color: #FFFFFF; }
                                 )";

    auto createThresholdRow = [&](int row, QString name, QLineEdit* &minEdit, QLineEdit* &maxEdit) {
        QLabel *label = new QLabel(name, thresholdArea);
        label->setFixedWidth(60);

        minEdit = new QLineEdit(thresholdArea);
        minEdit->setPlaceholderText("下限");
        minEdit->setFixedWidth(60);
        minEdit->setStyleSheet(thresholdEditStyle);

        minEdit->setValidator(new QIntValidator(0, 100, this));

        QLabel *waveLabel = new QLabel("~", thresholdArea);

        maxEdit = new QLineEdit(thresholdArea);
        maxEdit->setPlaceholderText("上限");
        maxEdit->setFixedWidth(60);
        maxEdit->setStyleSheet(thresholdEditStyle);
        maxEdit->setValidator(new QIntValidator(-100, 100, this));

        thresholdLayout->addWidget(label, row, 0);
        thresholdLayout->addWidget(minEdit, row, 1);
        thresholdLayout->addWidget(waveLabel, row, 2);
        thresholdLayout->addWidget(maxEdit, row, 3);
        thresholdLayout->setColumnStretch(4, 1); // 占位
    };

    createThresholdRow(0, "温度阈值:", tempMinEdit, tempMaxEdit);
    createThresholdRow(1, "湿度阈值:", humiMinEdit, humiMaxEdit);

    tempMinEdit->setText("16");
    tempMaxEdit->setText("56");
    humiMinEdit->setText("20");
    humiMaxEdit->setText("90");
    mainLayout->addWidget(thresholdArea);

    line1 = new QFrame(parametersWidget);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    line1->setStyleSheet("background-color: #EBEEF5;");
    mainLayout->addWidget(line1);

    rateTitle = new QLabel("数据采集速率选择:", parametersWidget);
    rateTitle->setStyleSheet("font-size: 18px; color: #333333; font-weight: bold; border:none;");
    mainLayout->addWidget(rateTitle);

    rateGroup = new QButtonGroup(this);
    rateGroup->setExclusive(true); // 开启互斥功能

    QWidget *rateArea = new QWidget(parametersWidget);
    rateArea->setStyleSheet("border:none;");
    QGridLayout *rateLayout = new QGridLayout(rateArea);
    rateLayout->setContentsMargins(5, 0, 5, 0);
    rateLayout->setSpacing(10);

    QString radioStyle = R"(
                         QRadioButton {
                         font-size: 14px;
                         color: #606266;
                         spacing: 8px;
                         }
                         QRadioButton::indicator {
                         width: 18px;
                         height: 18px;
                         }
                         QRadioButton::indicator:unchecked {
                         border: 1px solid #DCDFE6;
                         border-radius: 9px;
                         background: #FFFFFF;
                         }
                         QRadioButton::indicator:checked {
                         border: 1px solid #4080FF;
                         border-radius: 9px;
                         background: #4080FF;
                         image: url(:/new/res/res/radio_dot.png); /* 如果没有资源图，Qt默认也会显示圆点 */
                         }
                         )";

    struct RateOption {
        QString text;
        int row;
        int col;
    };
    QList<RateOption> options = {
        {"0.5s 采样", 0, 0}, {"1.0s 采样", 0, 1},
        {"2.0s 采样", 1, 0}, {"4.0s 采样", 1, 1}
    };

    for (int i = 0; i < options.size(); ++i) {
        rateButtons[i] = new QRadioButton(options[i].text, rateArea);
        rateButtons[i]->setStyleSheet(radioStyle);
        rateGroup->addButton(rateButtons[i], i); // 加入组并分配ID
        rateLayout->addWidget(rateButtons[i], options[i].row, options[i].col);
    }

    // 默认选中 1.0s
    rateButtons[1]->setChecked(true);
    mainLayout->addWidget(rateArea);


    line2 = new QFrame(parametersWidget);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    line2->setStyleSheet("background-color: #EBEEF5;");
    mainLayout->addWidget(line2);




    QLabel *storageTitle = new QLabel("传感器数据存储控制:", parametersWidget);
    storageTitle->setStyleSheet("font-size: 18px; color: #333333; font-weight: bold; border:none;");
    mainLayout->addWidget(storageTitle);

    // 第二层：当前储存数据条数
    QHBoxLayout *countLayout = new QHBoxLayout();
    QLabel *countLabel = new QLabel("当前存储条数:", parametersWidget);
    countLabel->setStyleSheet("font-size: 14px; color: #606266; border:none;");

    dataCountEdit = new QLineEdit("0", parametersWidget);
    dataCountEdit->setReadOnly(true);
    dataCountEdit->setFixedWidth(120);
    dataCountEdit->setAlignment(Qt::AlignCenter);
    dataCountEdit->setStyleSheet(R"(
                                 QLineEdit {
                                 background-color: #F5F7FA;
                                 border: 1px solid #DCDFE6;
                                 border-radius: 4px;
                                 color: #4080FF;
                                 font-weight: bold;
                                 font-size: 16px;
                                 font-family: 'Consolas', 'Monospace';
                                 }
                                 )");

    connect(fileWorker, &FileWorker::sigFileCount, this, &MainWindow::updateWriteCount);
    countLayout->addWidget(countLabel);
    countLayout->addWidget(dataCountEdit);
    countLayout->addStretch();
    mainLayout->addLayout(countLayout);

    // 第三层：开始存储与停止存储 (水平并列)
    QHBoxLayout *storageBtnLayout = new QHBoxLayout();
    storageBtnLayout->setSpacing(10);

    startStorageBtn = new QPushButton("开始存储", parametersWidget);
    stopStorageBtn = new QPushButton("停止存储", parametersWidget);

    // 设置固定高度保持整齐
    startStorageBtn->setFixedHeight(32);
    stopStorageBtn->setFixedHeight(32);

    // 初始状态：停止按钮被禁用或按下
    startStorageBtn->setStyleSheet(R"(
                                   QPushButton { background-color: #67C23A; color: white; border-radius: 4px; font-weight: bold; border:none; }
                                   QPushButton:hover { background-color: #85CE61; }
                                   QPushButton:disabled { background-color: #F0F9EB; color: #C2E7B0; border: 1px solid #C2E7B0; }
                                   )");

    stopStorageBtn->setStyleSheet(R"(
                                  QPushButton { background-color: #F56C6C; color: white; border-radius: 4px; font-weight: bold; border:none; }
                                  QPushButton:hover { background-color: #F78989; }
                                  QPushButton:disabled { background-color: #FEF0F0; color: #FBC4C4; border: 1px solid #FBC4C4; }
                                  )");

    stopStorageBtn->setEnabled(false); // 初始默认为停止状态

    storageBtnLayout->addWidget(startStorageBtn);
    storageBtnLayout->addWidget(stopStorageBtn);
    mainLayout->addLayout(storageBtnLayout);



    exportDataBtn = new QPushButton("导出数据报告 (.csv)", parametersWidget);
    exportDataBtn->setFixedHeight(35);
    exportDataBtn->setStyleSheet(R"(
                                 QPushButton {
                                 background-color: #E6A23C;
                                 color: white;
                                 border-radius: 4px;
                                 font-size: 14px;
                                 font-weight: bold;
                                 border:none;
                                 }
                                 QPushButton:hover { background-color: #EBB563; }
                                 QPushButton:pressed { background-color: #CF9236; }
                                 )");


    mainLayout->addWidget(exportDataBtn);

    // 1. 开始存储：开启写入闸门
    connect(startStorageBtn, &QPushButton::clicked, this, [=](){
        QMetaObject::invokeMethod(fileWorker, "setStorageActive", Q_ARG(bool, true));

        startStorageBtn->setEnabled(false);
        stopStorageBtn->setEnabled(true);
        exportDataBtn->setEnabled(true);
        qDebug() << "开始记录数据...";
    });

    // 2. 停止存储：只是暂停写入，不结束当前文件
    connect(stopStorageBtn, &QPushButton::clicked, this, [=](){
        QMetaObject::invokeMethod(fileWorker, "setStorageActive", Q_ARG(bool, false));

        startStorageBtn->setEnabled(true);
        stopStorageBtn->setEnabled(false);
        // 停止时依然可以导出
        qDebug() << "暂停记录数据（文件仍打开）";
    });

    // 3. 导出报告：结束当前文件生命周期
    connect(exportDataBtn, &QPushButton::clicked, this, [=](){
        // 调用我们新增的彻底关闭逻辑
        QMetaObject::invokeMethod(fileWorker, "stopAndExport");

        // UI 状态重置
        startStorageBtn->setEnabled(true);
        stopStorageBtn->setEnabled(false);
        exportDataBtn->setEnabled(false); // 导出后，没开始新记录前不能再导

        QMessageBox::information(this, "导出成功", "当前记录已保存。点击“开始”将创建新记录文件。");
    });


    mainLayout->addSpacing(10);
    line3 = new QFrame(parametersWidget);
    line3->setFrameShape(QFrame::HLine);
    line3->setFrameShadow(QFrame::Sunken);
    line3->setStyleSheet("background-color: #EBEEF5;");
    mainLayout->addWidget(line3);

    parametersConfirmBtn = new QPushButton("应用阈值配置", parametersWidget);
    parametersConfirmBtn->setFixedHeight(35);
    parametersConfirmBtn->setStyleSheet(R"(
                                       QPushButton {
                                       background-color: #4080FF;
                                       color: white;
                                       border-radius: 4px;
                                       font-size: 14px;
                                       font-weight: bold;
                                       }
                                       QPushButton:hover { background-color: #66B1FF; }
                                       QPushButton:pressed { background-color: #3A8EE6; }
                                       )");
    connect(parametersConfirmBtn, &QPushButton::clicked, this, &MainWindow::onParametersConfirmBtn);

    mainLayout->addWidget(parametersConfirmBtn);
    mainLayout->addStretch(); // 将所有控件往上挤，避免下方留白不均匀
}

void MainWindow::sensorDataPreventWindow()
{
    sensorDataPreventWidget = new QWidget(contentContainer);
    // 8列布局需要较宽的容器，设为 600px 左右比较合适
    sensorDataPreventWidget->setMinimumWidth(600);
    sensorDataPreventWidget->setStyleSheet(R"(
        QWidget {
            background-color: #FFFFFF;
            border: 1px solid #DCDFE6;
            border-radius: 6px;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(sensorDataPreventWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);

    titleLabel = new QLabel("实时环境监测数据 (16路)", sensorDataPreventWidget);
    titleLabel->setStyleSheet("font-size: 18px; color: #333333; font-weight: bold; border:none;");
    titleLabel->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(titleLabel);

    dataGridContainer = new QWidget(sensorDataPreventWidget);
    dataGridContainer->setStyleSheet("border:none;");
    QGridLayout *gridLayout = new QGridLayout(dataGridContainer);
    // 关键：大幅缩小间距使布局紧凑
    gridLayout->setSpacing(4);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    QString cellStyle = R"(
        QLabel {
            border: 1px solid #EBEEF5;
            border-radius: 2px;
            background-color: #FDFDFD;
        }
    )";

    for (int i = 0; i < 16; ++i) {
        // 1. 创建超扁平单元格
        // 宽度 70px，高度 28px
        QLabel *dataCell = new QLabel(dataGridContainer);
        dataCell->setFixedSize(190, 38);
        dataCell->setStyleSheet(cellStyle);
        dataCell->setAlignment(Qt::AlignCenter);

        // 2. 优化 HTML 结构：去除 padding 确保不换行
        QString initialText = QString(
            "<table width='100%' cellpadding='0' cellspacing='0' style='border:none;'>"
            "<tr>"
            "<td align='center' style='color:#909399; font-weight:bold; font-size:15px;'>%1</td>"
            "<td align='center' style='color:#E6A23C; font-size:15px;'>T: ---- </td>"
            "<td align='center' style='color:#409EFF; font-size:15px;'>H: ---- </td>"
            "</tr>"
            "</table>"
        ).arg(i + 1, 2, 10, QChar('0'));

        dataCell->setText(initialText);
        sensorDataLabels[i] = dataCell;

        // 3. 计算行列：每行 8 个
        int row = i / 8; // 前8个在第0行，后8个在第1行
        int col = i % 8; // 0-7 循环
        gridLayout->addWidget(dataCell, row, col);
    }

    mainLayout->addWidget(dataGridContainer);

    line4 = new QFrame(parametersWidget);
    line4->setFrameShape(QFrame::HLine);
    line4->setFrameShadow(QFrame::Sunken);
    line4->setStyleSheet("background-color: #EBEEF5;");
    mainLayout->addWidget(line4);

    QWidget *chartHeader = new QWidget(sensorDataPreventWidget);
    chartHeader->setStyleSheet("border:none;");
    QHBoxLayout *headerLayout = new QHBoxLayout(chartHeader);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *chartTitle = new QLabel("传感器实时数据波形 (趋势图)", chartHeader);
    chartTitle->setStyleSheet("font-size: 18px; color: #333333; font-weight: bold; border:none;");

    QComboBox *chartChannelSelect = new QComboBox(chartHeader);
    for(int i=1; i<=16; ++i) chartChannelSelect->addItem(QString("查看通道 %1").arg(i, 2, 10, QChar('0')));
    chartChannelSelect->setFixedWidth(120);
    chartChannelSelect->setStyleSheet(R"(
                                      QComboBox { border: 1px solid #DCDFE6; border-radius: 4px; padding: 2px 5px; background: #F5F7FA; }
                                      )");

    headerLayout->addWidget(chartTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(chartChannelSelect);
    mainLayout->addWidget(chartHeader);

    mainLayout->addStretch();

    QWidget *customPlot = new QWidget(sensorDataPreventWidget);
    customPlot->setMinimumHeight(600); // 给波形图足够的垂直空间
    customPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    customPlot->setStyleSheet(R"(
                              QWidget {
                              background-color: #1E1E1E; /* 黑色背景更有科技感 */
                              border: 1px solid #333333;
                              border-radius: 4px;
                              }
                              )");
    mainLayout->addWidget(customPlot);
    mainLayout->addStretch();

    // 移除 Stretch，让数据显示在顶部；或者保留以适应大窗口

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
    // 转换并格式化为 16 进制显示
    QByteArray byteData = messages.toUtf8();
    QString hexStr = byteData.toHex().toUpper();
    QString formattedHex;
    for (int i = 0; i < hexStr.size(); i += 2)
    {
        formattedHex += hexStr.mid(i, 2) + " ";
    }
    // TODO: 追回到 textBrowser
}

void MainWindow::sendMessages()
{
    QString sendMessage_str = lineEdit->text();
    QByteArray sendMessage = sendMessage_str.toUtf8();
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
    QMessageBox::information(this, success ? "升级成功" : "升级失败", message);
}

void MainWindow::receSensorDataToPrevent(int index, float temp, float humity, QVector<uint8_t> time)
{
    updateSensorUI(index - 1, temp, humity);
}

void MainWindow::onParametersConfirmBtn()
{
    qDebug() << "参数设置";
    sendParametersData data;

    // 1. 读取哪16个通道开启
    quint16 channel = 0;
    for (int i = 0; i < 16; i++) {
        if (sensorCheckBoxes[i]->isChecked()) {
            channel |= 1 << i;
        }
    }
    data.channels = channel;
    qDebug() << channel;

    // 2. 读取四个阈值参数
    quint8 tempmin = (tempMinEdit->text().trimmed()).toUInt();
    quint8 tempmax = (tempMaxEdit->text().trimmed()).toUInt();
    quint8 humimin = (humiMinEdit->text().trimmed()).toUInt();
    quint8 humimax = (humiMaxEdit->text().trimmed()).toUInt();
    data.tempMin = tempmin;
    data.tempMax = tempmax;
    data.humiMin = humimin;
    data.humiMax = humimax;
    qDebug() << "温度阈值:" << tempmin << "-" << tempmax;
    qDebug() << "湿度阈值:" << humimin << "-" << humimax;

    // 3. 读取哪个采样速率
    quint8 rateindex = 0;
    for (int i = 0; i < 4; i++) {
        if (rateButtons[i]->isChecked()) {
            rateindex = i;
            break;
        }
    }
    data.rateValue = rateindex;
    qDebug() << "采样速率:" << rateindex;

    // 4. 数据处理与发送
    QByteArray byte;
    byte.resize(sizeof(sendParametersData));
    memcpy(byte.data(), &data, sizeof (sendParametersData));
    emit sigSendParametersData(byte);
}

void MainWindow::updateWriteCount(int num)
{
    dataCountEdit->setText(QString::number(num));
}

// 创建自定义标题栏
void MainWindow::createCustomTitleBar(int height, QString bgColor, QString textColor, QString hoverColor, QString titleText, QString borderColor, int borderWidth)
{
    QWidget *titleBar = new QWidget(this);
    titleBar->setFixedHeight(height);
    titleBar->setStyleSheet(QString("QWidget { background-color: %1; }").arg(bgColor));

    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(15, 0, 10, 0);
    titleLayout->setSpacing(15);

    // 添加标题栏图标
    QLabel *titleFlag = new QLabel(titleBar);
    QPixmap pixmap(":/new/res/res/flag.png");
    pixmap = pixmap.scaled(height - 10, height - 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    titleFlag->setPixmap(pixmap);
    titleFlag->setAlignment(Qt::AlignVCenter);

    QLabel *titleLabel = new QLabel(titleText, titleBar);
    titleLabel->setStyleSheet(QString("color: %1; font-size: 24px; font-weight: 500;").arg(textColor));

    QPushButton *minBtn = new QPushButton(titleBar);
    minBtn->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarMinButton));
    minBtn->setFixedSize(height - 10, height - 10);
    minBtn->setStyleSheet(QString(R"(
        QPushButton { border: none; border-radius: 4px; background-color: #696969;}
        QPushButton:hover { background-color: %1; }
    )")
                              .arg(hoverColor));

    QPushButton *closeBtn = new QPushButton(titleBar);
    closeBtn->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    closeBtn->setFixedSize(height - 10, height - 10);
    closeBtn->setStyleSheet(R"(
        QPushButton { border: none; border-radius: 4px; background-color: #696969; }
        QPushButton:hover { background-color: #FF4D4F; }
    )");

    titleLayout->addWidget(titleFlag);
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(minBtn);
    titleLayout->addWidget(closeBtn);

    // 创建主容器并设置边框
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setStyleSheet(QString(R"(
        QWidget {
            border: %1px solid %2; 
            border-radius: 1px; 
            background-color: #2A364A; 
        }
    )")
                                     .arg(borderWidth)
                                     .arg(borderColor));

    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);

    centralLayout->addWidget(titleBar);
    centralLayout->addWidget(ui->centralwidget);
    this->setCentralWidget(centralWidget);

    connect(minBtn, &QPushButton::clicked, this, &MainWindow::showMinimized);
    connect(closeBtn, &QPushButton::clicked, this, &MainWindow::close);

    titleBar->installEventFilter(this);
}

// 拖动功能实现
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            lastTitleBarPos = mouseEvent->globalPos() - this->frameGeometry().topLeft();
            return true;
        }
    }
    else if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->buttons() & Qt::LeftButton)
        {
            this->move(mouseEvent->globalPos() - lastTitleBarPos);
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
