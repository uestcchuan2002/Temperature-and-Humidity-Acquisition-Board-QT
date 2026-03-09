#include "firmwareupgradeworker.h"

FirmwareUpgradeWorker::FirmwareUpgradeWorker(QObject *parent) : QObject(parent)
{

}

void FirmwareUpgradeWorker::setFirmwareData(const QByteArray &data)
{
    m_firmwareData = data;
}

void FirmwareUpgradeWorker::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
}

void FirmwareUpgradeWorker::startUpgrade()
{
    if(!m_socket || m_firmwareData.isEmpty())
    {
        emit upgradeFinished(false, "无效参数");
        return;
    }

    m_stop = false;

    emit statusChanged("发送升级开始指令...");

    if(!sendStartPacket())
    {
        emit upgradeFinished(false, "发送START失败");
        return;
    }

    const int packetSize = 1024;
    int totalSize = m_firmwareData.size();

    for(int offset = 0; offset < totalSize; offset += packetSize)
    {
        if(m_stop)
        {
            emit upgradeFinished(false, "升级被终止");
            return;
        }

        QByteArray chunk = m_firmwareData.mid(offset, packetSize);

        if(!sendDataPacket(offset, chunk))
        {
            emit upgradeFinished(false, "发送数据失败");
            return;
        }

        int sentSize = offset + chunk.size();
        int progress = (sentSize * 100) / totalSize;

        if(progress > 100)
            progress = 100;

        emit progressChanged(progress);
    }

    emit statusChanged("发送结束指令...");

    if(!sendEndPacket())
    {
        emit upgradeFinished(false, "发送END失败");
        return;
    }

    emit progressChanged(100);
    emit upgradeFinished(true, "升级完成");
}

bool FirmwareUpgradeWorker::sendStartPacket()
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint8 cmd = 0xA0;
    quint32 size = m_firmwareData.size();

    stream << cmd;
    stream << size;

    m_socket->write(packet);

    return waitForAck();
}

bool FirmwareUpgradeWorker::sendDataPacket(quint32 offset, const QByteArray &chunk)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    quint8 cmd = 0xA1;
    quint16 length = chunk.size();

    stream << cmd;
    stream << offset;
    stream << length;

    packet.append(chunk);

    m_socket->write(packet);

    return waitForAck();
}

bool FirmwareUpgradeWorker::sendEndPacket()
{
    QByteArray packet;
    packet.append(0xA2);

    m_socket->write(packet);

    return waitForAck();
}

bool FirmwareUpgradeWorker::waitForAck(int timeoutMs)
{
    if(!m_socket->waitForReadyRead(timeoutMs))
        return false;

    QByteArray response = m_socket->readAll();

    if(response.contains(0x55))   // 假设0x55为ACK
        return true;

    return false;
}

void FirmwareUpgradeWorker::stopUpgrade()
{
    m_stop = true;
}
