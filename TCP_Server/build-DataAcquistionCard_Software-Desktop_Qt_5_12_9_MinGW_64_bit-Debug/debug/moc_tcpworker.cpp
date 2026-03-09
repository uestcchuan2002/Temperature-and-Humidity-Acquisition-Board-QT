/****************************************************************************
** Meta object code from reading C++ file 'tcpworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DataAcquistionCard_Software/tcpworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TcpWorker_t {
    QByteArrayData data[19];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TcpWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TcpWorker_t qt_meta_stringdata_TcpWorker = {
    {
QT_MOC_LITERAL(0, 0, 9), // "TcpWorker"
QT_MOC_LITERAL(1, 10, 12), // "sigConnected"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 15), // "sigDisconnected"
QT_MOC_LITERAL(4, 40, 7), // "sigRecv"
QT_MOC_LITERAL(5, 48, 8), // "messages"
QT_MOC_LITERAL(6, 57, 21), // "updateUpgradeProgress"
QT_MOC_LITERAL(7, 79, 4), // "port"
QT_MOC_LITERAL(8, 84, 19), // "updateUpgradeStatus"
QT_MOC_LITERAL(9, 104, 5), // "state"
QT_MOC_LITERAL(10, 110, 13), // "connectToHost"
QT_MOC_LITERAL(11, 124, 2), // "ip"
QT_MOC_LITERAL(12, 127, 18), // "disconnectFromHost"
QT_MOC_LITERAL(13, 146, 8), // "sendData"
QT_MOC_LITERAL(14, 155, 4), // "data"
QT_MOC_LITERAL(15, 160, 15), // "receiveMessages"
QT_MOC_LITERAL(16, 176, 12), // "startUpgrade"
QT_MOC_LITERAL(17, 189, 8), // "firmware"
QT_MOC_LITERAL(18, 198, 11) // "onReadyRead"

    },
    "TcpWorker\0sigConnected\0\0sigDisconnected\0"
    "sigRecv\0messages\0updateUpgradeProgress\0"
    "port\0updateUpgradeStatus\0state\0"
    "connectToHost\0ip\0disconnectFromHost\0"
    "sendData\0data\0receiveMessages\0"
    "startUpgrade\0firmware\0onReadyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    1,   71,    2, 0x06 /* Public */,
       6,    1,   74,    2, 0x06 /* Public */,
       8,    1,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    2,   80,    2, 0x0a /* Public */,
      12,    0,   85,    2, 0x0a /* Public */,
      13,    1,   86,    2, 0x0a /* Public */,
      15,    0,   89,    2, 0x0a /* Public */,
      16,    1,   90,    2, 0x0a /* Public */,
      18,    0,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::QString,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,   11,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,   17,
    QMetaType::Void,

       0        // eod
};

void TcpWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TcpWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigConnected(); break;
        case 1: _t->sigDisconnected(); break;
        case 2: _t->sigRecv((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->updateUpgradeProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->updateUpgradeStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->connectToHost((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 6: _t->disconnectFromHost(); break;
        case 7: _t->sendData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 8: _t->receiveMessages(); break;
        case 9: _t->startUpgrade((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 10: _t->onReadyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TcpWorker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpWorker::sigConnected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TcpWorker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpWorker::sigDisconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TcpWorker::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpWorker::sigRecv)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TcpWorker::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpWorker::updateUpgradeProgress)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TcpWorker::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpWorker::updateUpgradeStatus)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TcpWorker::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TcpWorker.data,
    qt_meta_data_TcpWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TcpWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TcpWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TcpWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void TcpWorker::sigConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TcpWorker::sigDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TcpWorker::sigRecv(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TcpWorker::updateUpgradeProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TcpWorker::updateUpgradeStatus(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
