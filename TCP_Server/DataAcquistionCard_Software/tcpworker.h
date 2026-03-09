#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QThread>
#include <QObject>
#include <QTcpSocket>
#include <QEventLoop>
#include <QTimer>
#include <QElapsedTimer>
#include <QCoreApplication>
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

signals:
    void sigConnected();
    void sigDisconnected();
    void sigRecv(QString messages);
    void updateUpgradeProgress(int port);
    void updateUpgradeStatus(QString state);

private:
    QTcpSocket *socket;

private:
    bool sendStartPacket(int size);
    bool sendDataPacket(quint32 offset, QByteArray data);
    bool sendEndPacket();

    bool waitAck(int timeout = 3000);
    QByteArray recvBuffer;

    quint16 crc16_modbus(const QByteArray &data);
};

#endif // TCPWORKER_H
