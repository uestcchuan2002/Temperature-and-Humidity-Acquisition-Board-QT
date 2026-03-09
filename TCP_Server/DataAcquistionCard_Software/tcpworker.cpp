#include "tcpworker.h"

TcpWorker::TcpWorker(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::readyRead,
            this, &TcpWorker::onReadyRead);
}

void TcpWorker::connectToHost(QString ip, quint16 port)
{
    socket->connectToHost(ip, port);
    emit sigConnected();
}

void TcpWorker::disconnectFromHost()
{
    socket->disconnectFromHost();
    emit sigDisconnected();
}

void TcpWorker::sendData(QByteArray data)
{
    socket->write(data);
}

void TcpWorker::receiveMessages()
{
   ;
}

void TcpWorker::startUpgrade(QByteArray firmware)
{
    int total = firmware.size();

    qDebug() << "固件大小:" << total << "bytes";
    emit updateUpgradeStatus("升级状态：初始化中...");
    const int packetSize = 512;

    /* 1 发送 START */
    if(!sendStartPacket(total))
    {
        qDebug() << "START失败";
        return;
    }
    emit updateUpgradeStatus("升级状态：进行中...");
    qDebug() << "START成功";

    /* 2 分包发送 DATA */
    for (int offset = 0; offset < total; offset += packetSize)
    {
        int remain = total - offset;
        int sendSize = qMin(packetSize, remain);

        QByteArray chunk = firmware.mid(offset, sendSize);
        if (!sendDataPacket(offset, chunk))
        {
            qDebug() << "DATA发送失败 offset:" << offset;
            return;
        }

        int progress = (offset + sendSize) * 100 / total;
        qDebug() << "升级进度:" << progress << "%";
        emit updateUpgradeProgress(progress);

        QThread::msleep(2);
    }

    /* 3 发送 END */
    if(!sendEndPacket())
    {
        qDebug() << "END发送失败";
        return;
    }
    emit updateUpgradeStatus("升级状态：升级成功");
    qDebug() << "OTA升级完成";
}

void TcpWorker::onReadyRead()
{
    QByteArray data = socket->readAll();

    recvBuffer.append(data);

    qDebug() << "recv:" << data.toHex();
}

/*
    55 AA A0 00 04 size(4B) CRC
*/
bool TcpWorker::sendStartPacket(int size)
{
    QByteArray packet;

    packet.append(0x55);                /* 帧头 */
    packet.append(0xAA);                /* 帧头 */

    packet.append(0xA0);                /* 命令为发送指令:0xa0 */

    quint16 len = 4;

    packet.append((char*)&len,2);       /* 数据长度 */
    packet.append((char*)&size,4);      /* 存储固件大小 */
    quint16 crc = crc16_modbus(packet); /* crc16计算 */
    packet.append((char*)&crc, 2);

    socket->write(packet);              /* 发送start数据 */
    socket->flush();                    /* 确保数据立即送出内核缓存区 */

    qDebug() << packet.toHex();
    return waitAck(20000);
}

bool TcpWorker::sendDataPacket(quint32 offset, QByteArray data)
{
    QByteArray packet;

    quint8 cmd = 0xA1;
    quint16 len = 4 + data.size();   // offset + data

    // HEADER
    packet.append(0x55);
    packet.append(0xAA);

    // CMD
    packet.append(cmd);

    // LEN
    packet.append((char*)&len,2);

    // PAYLOAD
    packet.append((char*)&offset,4);
    packet.append(data);

    // CRC
    quint16 crc = crc16_modbus(packet);
    packet.append((char*)&crc,2);

    qint64 written = socket->write(packet);

    if(written != packet.size())
    {
        qDebug() << "send failed";
        return false;
    }

    qDebug() << "DATA offset:" << offset << "len:" << data.size();
    socket->flush();    /* 确保数据立即送出内核缓存区 */

    // 等待ACK
    if(!waitAck(5000)) {
        qDebug() << "ACK timeout";
        return false;
    }

    return true;
}

bool TcpWorker::sendEndPacket()
{
    QByteArray packet;

    packet.append(0x55);
    packet.append(0xAA);

    quint8 cmd = 0xA2;
    packet.append(cmd);

    quint16 len = 0;
    packet.append((char*)&len,2);

    quint16 crc = crc16_modbus(packet);
    packet.append((char*)&crc,2);

    socket->write(packet);

    qDebug() << "Send END:" << packet.toHex();

    return waitAck(3000);
}

bool TcpWorker::waitAck(int timeout)
{
    QElapsedTimer timer;
    timer.start();

    /* 在独立线程中，我们循环等待直到超时或收到正确应答 */
    while(timer.elapsed() < timeout) {
        // 1. 检测缓存区是否已有数据，即使没有新数据到达，也要先检查之前读取但未处理的数据
        if(recvBuffer.size() >= 3) {
            // 滑动窗口查找55 AA B0
            int index = -1;
            for (int i = 0; i <= recvBuffer.size() - 3; ++i) {
                if ((unsigned char)recvBuffer[i] == 0x55 &&
                    (unsigned char)recvBuffer[i+1] == 0xAA &&
                    (unsigned char)recvBuffer[i+2] == 0xB0) {
                    index = i;
                    break;
                }
            }

            if (index != -1) {
                // 找到了应答，移除该帧及其之前的所有数据（防止缓冲区无限堆积）
                recvBuffer.remove(0, index + 3);
                return true;
            }
        }

        // 2. 阻塞等待新数据到达
        int remaining = timeout - timer.elapsed();
        if (remaining > 0) {
            // 注意：waitForReadyRead 在有数据时会立即返回 true
            if (socket->waitForReadyRead(qMin(50, remaining))) {
                // 读取新到达的所有字节并追加到缓冲区
                recvBuffer.append(socket->readAll());
            }
        } else {
            break;  //彻底超时
        }
    }

    qDebug() << "ACK Timeout! Final buffer hex:" << recvBuffer.toHex();
    return false;

}


quint16 TcpWorker::crc16_modbus(const QByteArray &data)
{
    quint16 crc = 0xFFFF;

    for (int i = 0; i < data.size(); i++)
    {
        crc ^= (quint8)data[i];

        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}
