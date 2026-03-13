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
#include <QProgressBar>
#include <QFile>
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QThread>
#include <QCheckBox>
#include <QFrame>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>

#include "tcpworker.h"
#include "fileworker.h"

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
    void sigSendParametersData(QByteArray data);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;

    QPoint lastTitleBarPos;


    QThread *tcpThread = nullptr;       // TCP线程
    TcpWorker *tcpWorker = nullptr;     // TCP实例

    QThread *fileThread = nullptr;      // 文件线程
    FileWorker *fileWorker = nullptr;   // 文件实例

    QWidget *contentContainer;

    QWidget *contolArea;
    QWidget *tcpConnectControlArea;
    QWidget *firmwareUpdateWidget;

    QWidget *parameterAndSensorArea;
    QWidget *parametersWidget;
    QWidget *sensorDataPreventWidget;


    QWidget *tcpConnectControlAreaFirst;
    QWidget *tcpConnectControlAreaSecond;
    QLabel *ipLabel;
    QLineEdit *ipEdit;
    QLabel *portLabel;
    QLineEdit *portEdit;
    QPushButton *connectBtn;
    QPushButton *disconnectBtn;
    QLabel *tcpConncetState;
    QLineEdit *lineEdit;
    QPushButton *messageSendBtn;

    QLabel *otaLabel;
    QPushButton *selectFileBtn;
    QLineEdit *firmwarePathEdit;
    QPushButton *updateBtn;
    QLabel *updateStateLabel;
    QProgressBar *updateProgressBar;
    QByteArray m_firmwareData;
    QString    m_firmwarePath;

    QLabel *sensorOpenLabel;
    QCheckBox* sensorCheckBoxes[16];
    QFrame *line;
    QLabel *thresholdTitle;
    QLineEdit *tempMinEdit, *tempMaxEdit;
    QLineEdit *humiMinEdit, *humiMaxEdit;
    QPushButton *parametersConfirmBtn;
    QFrame *line1;
    QLabel *rateTitle;
    QButtonGroup *rateGroup;
    QRadioButton *rateButtons[4];
    QFrame *line2;
    QFrame *line3;
    QLineEdit *dataCountEdit;
    QPushButton *startStorageBtn;
    QPushButton *stopStorageBtn;
    QPushButton *exportDataBtn;

    QLabel *titleLabel;
    QWidget *dataGridContainer;
    QLabel *sensorDataLabels[16];
    QFrame *line4;

    void createCustomTitleBar(int height = 40,
                                  QString bgColor = "#2c3e50",
                                  QString textColor = "#ffffff",
                                  QString hoverColor = "#34495e",
                                  QString titleText = "?????",
                                  QString borderColor = "#165DFF",
                                  int borderWidth = 1);

    void initContentArea(void);

    void tcpConnectWindow(void);
    void firmwareUpdateWindow(void);
    void parametersWindow(void);
    void sensorDataPreventWindow(void);



    void updateSensorUI(int index, float temp, float humi) {
        if (index < 0 || index >= 16) return;

        QString text = QString(
                    "<table width='100%' style='border:none;'>"
                    "<tr>"
                    "<td align='center' style='color:#909399; font-weight:bold; font-size:15px;'>%1</td>"
                    "<td align='center' style='color:#E6A23C; font-size:15px;'>T: %2 </td>"
                    "<td align='center' style='color:#409EFF; font-size:15px;'>H: %3 </td>"
                    "</tr>"
                    "</table>"
                    ).arg(index + 1, 2, 10, QChar('0'))
                .arg(temp, 0, 'f', 1)
                .arg(humi, 0, 'f', 1);

        sensorDataLabels[index]->setText(text);
    }

private slots:
    void toConnect();
    void toDisConnect();
    void connected();
    void disconnected();
    void receiveMessages(QString messages);
    void sendMessages();
    void startUpgrade();
    void onUpgradeProgress(int percent);
    void onUpgradeStatus(QString status);
    void onUpgradeFinished(bool success, QString message);

    void receSensorDataToPrevent(int index, float temp, float humity, QVector<uint8_t> time);
    void onParametersConfirmBtn(void);
    void updateWriteCount(int num);
};
#endif // MAINWINDOW_H
