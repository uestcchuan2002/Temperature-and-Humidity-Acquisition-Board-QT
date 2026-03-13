#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QThread>
#include <QObject>
#include <QTcpSocket>
#include <QEventLoop>
#include <QTimer>
#include <QElapsedTimer>
#include <QCoreApplication>


enum WorkerState {
    Idle,       // 空闲
    Upgrading,  // 升级中
    Collecting  // 传感器数据采集
};


#pragma pack(1)

struct sendParametersData {
    quint16 channels;
    quint8 tempMin;
    quint8 tempMax;
    quint8 humiMin;
    quint8 humiMax;
    quint8 rateValue;
};
#pragma pack()



class TcpWorker : public QObject
{
    Q_OBJECT
public:
    explicit TcpWorker(QObject *parent = nullptr);

public slots:
    void connectToHost(QString ip, quint16 port);
    void disconnectFromHost();
    void sendData(QByteArray data);
    void receiveMessages();
    void startUpgrade(QByteArray firmware);
    void onReadyRead();
    void onSendParametersData(QByteArray data);

signals:
    void sigConnected();
    void sigDisconnected();
    void sigRecv(QString messages);
    void updateUpgradeProgress(int port);
    void updateUpgradeStatus(QString state);
    void sendSensorDataToPrevent(int index, float temp, float humity, QVector<uint8_t> time);

private:
    QTcpSocket *socket;
    WorkerState m_currentState = Idle;
    QByteArray ackBuffer;       // 专门存 OTA 应答 (B0)
    QByteArray sensorBuffer;    // 专门存 传感器数据 (D0)

private:
    bool sendStartPacket(int size);
    bool sendDataPacket(quint32 offset, QByteArray data);
    bool sendEndPacket();

    bool waitAck(int timeout = 3000);
    void parseSensorData();

    quint16 crc16_modbus(const QByteArray &data);
};

#endif // TCPWORKER_H
