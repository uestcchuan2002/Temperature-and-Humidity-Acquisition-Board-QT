#include "fileworker.h"

// 报错处修正：确保作用域 FileWorker:: 被正确添加
FileWorker::FileWorker(QObject *parent)
    : QObject(parent), m_isStoring(false)
{
    // 构造函数逻辑
}

FileWorker::~FileWorker()
{
    if (m_file.isOpen()) {
        m_file.close();
    }
}

void FileWorker::stopAndExport() {
    m_isStoring = false; // 先停止写入
    count_write = 0;
    emit sigFileCount(count_write);

    if (m_file.isOpen()) {
        m_stream.flush();
        m_file.close(); // 关键：关闭文件，这样下次 initFile 会创建新文件
        qDebug() << "文件已导出并关闭：" << m_file.fileName();
    }
}

void FileWorker::setStorageActive(bool active) {
    m_isStoring = active;
    if (active && !m_file.isOpen()) {
        initFile(); // 如果点了开始且文件没打开，就创个新的
    }
}

void FileWorker::initFile()
{
    if (m_file.isOpen()) return;

    count_write = 0;
    emit sigFileCount(count_write);

    QString fileName = QString("SensorData_%1.csv")
            .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));

    m_file.setFileName(fileName);
    if (m_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_stream.setDevice(&m_file);
        if (m_file.size() == 0) {
            m_stream << "Timestamp,DeviceID,Temp,Humi\n"; // 写入表头
        }
    }
}

void FileWorker::onSaveSensorData(uint8_t id, float temp, float humi, QVector<uint8_t> time)
{
    if (!m_isStoring) return;

    if (!m_file.isOpen()) initFile();

    // 格式化时间 20xx-xx-xx ...
    QString timeStr = QString("20%1-%2-%3 %4:%5:%6")
            .arg(time[0], 2, 10, QChar('0'))
            .arg(time[1], 2, 10, QChar('0'))
            .arg(time[2], 2, 10, QChar('0'))
            .arg(time[3], 2, 10, QChar('0'))
            .arg(time[4], 2, 10, QChar('0'))
            .arg(time[5], 2, 10, QChar('0'));

    m_stream << timeStr << "," << id << "," << temp << "," << humi << "\n";
    ++count_write;
    emit sigFileCount(count_write);

    // 如果数据量大，建议每隔几秒或几百条手动 flush 一次
    // m_stream.flush();
}
