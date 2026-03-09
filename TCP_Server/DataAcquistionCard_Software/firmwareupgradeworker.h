#ifndef FIRMWAREUPGRADEWORKER_H
#define FIRMWAREUPGRADEWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>

class FirmwareUpgradeWorker : public QObject
{
    Q_OBJECT
public:
    explicit FirmwareUpgradeWorker(QObject *parent = nullptr);

    void setFirmwareData(const QByteArray &data);
    void setSocket(QTcpSocket *socket);

signals:
    void progressChanged(int percent);
    void statusChanged(QString status);
    void upgradeFinished(bool success, QString message);

public slots:
    void startUpgrade();
    void stopUpgrade();

private:
    bool sendStartPacket();
    bool sendDataPacket(quint32 offset, const QByteArray &chunk);
    bool sendEndPacket();
    bool waitForAck(int timeoutMs = 3000);

private:
    QByteArray m_firmwareData;
    QTcpSocket *m_socket = nullptr;
    bool m_stop = false;
};

#endif // FIRMWAREUPGRADEWORKER_H
