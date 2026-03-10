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
    m_currentState = Collecting;    // 状态修改为：采集
    emit sigConnected();
}

void TcpWorker::disconnectFromHost()
{
    socket->disconnectFromHost();
    m_currentState = Idle;          // 状态修改为：空闲
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

    m_currentState= Upgrading;  // 状态修改为：升级中

    /* 1 发送 START */
    if(!sendStartPacket(total))
    {
        qDebug() << "START失败";
        m_currentState = Collecting;
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
            m_currentState = Collecting;
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
        m_currentState = Collecting;
        return;
    }
    emit updateUpgradeStatus("升级状态：升级成功");
    m_currentState = Collecting;
    qDebug() << "OTA升级完成";
}

void TcpWorker::onReadyRead()
{
    QByteArray data = socket->readAll();
    qDebug() << "recv:" << data.toHex();

    if (m_currentState == Upgrading) {
        ackBuffer.append(data);
    } else if (m_currentState == Collecting) {
        sensorBuffer.append(data);
        parseSensorData();
    }
}

typedef struct {
    uint8_t device_id;                              // 设备ID
    float temperature;                              // 温度值
    float humidity;                                 // 湿度值
    uint8_t time[6];                                // 时间数组（时/分/秒/年/月/日，可根据实际定义调整）
} __attribute__((packed)) SensorData_t;


void TcpWorker::parseSensorData()
{
    const int payloadSize = sizeof (SensorData_t);
    const int totalFrameLen = 20; // 3字节头 + Payload + 2字节CRC

    while (sensorBuffer.size() >= totalFrameLen) {
        // 1. 查找包头 55 AA B1
        if ((unsigned char)sensorBuffer[0] != 0x55 ||
                (unsigned char)sensorBuffer[0] != 0x55 ||
                (unsigned char)sensorBuffer[0] != 0x55)
        {
            sensorBuffer.remove(0, 1);
            continue;
        }

        // 2. CRC校验
        uint16_t recvCrc = (quint8)sensorBuffer[18] | ((quint8)sensorBuffer[19] << 8);

        // 计算前 (totalFrameLen - 2) 字节的 CRC
        uint16_t calcCrc = crc16_modbus(sensorBuffer.left(18));

        if (recvCrc != calcCrc) {
            qDebug() << "CRC Error! Raw Data:" << sensorBuffer.left(totalFrameLen).toHex();
            qDebug() << "Expected Index 18-19. Recv CRC:" << hex << recvCrc << "Calc:" << calcCrc;
            sensorBuffer.remove(0, 2); // 丢弃错包头
            continue;
        }

        // 3. 解析 Payload (处理对齐问题)
        QByteArray payload = sensorBuffer.mid(3, payloadSize);
        uint8_t device_id = (uint8_t)payload[0];
        float temp, humi;
        memcpy(&temp, payload.constData() + 1, 4);
        memcpy(&humi, payload.constData() + 5, 4);

        // 解析时间 (偏移 12)
        QVector<uint8_t> timeData;
        for(int i=0; i<6; ++i) timeData.append((uint8_t)payload[9 + i]);

        qDebug() << QString("ID:%1 Temp:%2 Humi:%3 Time:20%4-%5-%6-%7-%8-%9")
                    .arg(device_id).arg(temp).arg(humi)
                    .arg(timeData[0]).arg(timeData[1]).arg(timeData[2]).arg(timeData[3]).arg(timeData[4]).arg(timeData[5]);
        // 5. 移除已处理的完整帧
        sensorBuffer.remove(0, totalFrameLen);
    }
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
        if(ackBuffer.size() >= 3) {
            // 滑动窗口查找55 AA B0
            int index = -1;
            for (int i = 0; i <= ackBuffer.size() - 3; ++i) {
                if ((unsigned char)ackBuffer[i] == 0x55 &&
                    (unsigned char)ackBuffer[i+1] == 0xAA &&
                    (unsigned char)ackBuffer[i+2] == 0xB0) {
                    index = i;
                    break;
                }
            }

            if (index != -1) {
                // 找到了应答，移除该帧及其之前的所有数据（防止缓冲区无限堆积）
                ackBuffer.remove(0, index + 3);
                return true;
            }
        }

        // 2. 阻塞等待新数据到达
        int remaining = timeout - timer.elapsed();
        if (remaining > 0) {
            // 注意：waitForReadyRead 在有数据时会立即返回 true
            if (socket->waitForReadyRead(qMin(50, remaining))) {
                // 读取新到达的所有字节并追加到缓冲区
                ackBuffer.append(socket->readAll());
            }
        } else {
            break;  //彻底超时
        }
    }

    qDebug() << "ACK Timeout! Final buffer hex:" << ackBuffer.toHex();
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
