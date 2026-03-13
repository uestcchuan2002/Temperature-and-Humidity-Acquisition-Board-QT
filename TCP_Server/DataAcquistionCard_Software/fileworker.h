#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QDateTime>
#include <QDebug>

class FileWorker : public QObject
{
    Q_OBJECT
public:
    explicit FileWorker(QObject *parent = nullptr); // 声明构造函数
    virtual ~FileWorker();

public slots:
    // 这里的参数列表必须与信号发射处完全一致
    void onSaveSensorData(uint8_t id, float temp, float humi, QVector<uint8_t> time);
    void setStorageActive(bool active);
    void stopAndExport();

signals:
    void sigFileCount(int num);

private:
    bool m_isStoring;
    QFile m_file;
    QTextStream m_stream;
    void initFile();
    int count_write = 0;
};

#endif // FILEWORKER_H
