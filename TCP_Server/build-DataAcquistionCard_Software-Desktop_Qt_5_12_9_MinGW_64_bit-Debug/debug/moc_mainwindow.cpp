/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DataAcquistionCard_Software/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[34];
    char stringdata0[378];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 10), // "sigConnect"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 2), // "ip"
QT_MOC_LITERAL(4, 26, 4), // "port"
QT_MOC_LITERAL(5, 31, 13), // "sigDisconnect"
QT_MOC_LITERAL(6, 45, 7), // "sigSend"
QT_MOC_LITERAL(7, 53, 4), // "data"
QT_MOC_LITERAL(8, 58, 9), // "sigUpdate"
QT_MOC_LITERAL(9, 68, 21), // "sigSendParametersData"
QT_MOC_LITERAL(10, 90, 9), // "toConnect"
QT_MOC_LITERAL(11, 100, 12), // "toDisConnect"
QT_MOC_LITERAL(12, 113, 9), // "connected"
QT_MOC_LITERAL(13, 123, 12), // "disconnected"
QT_MOC_LITERAL(14, 136, 15), // "receiveMessages"
QT_MOC_LITERAL(15, 152, 8), // "messages"
QT_MOC_LITERAL(16, 161, 12), // "sendMessages"
QT_MOC_LITERAL(17, 174, 12), // "startUpgrade"
QT_MOC_LITERAL(18, 187, 17), // "onUpgradeProgress"
QT_MOC_LITERAL(19, 205, 7), // "percent"
QT_MOC_LITERAL(20, 213, 15), // "onUpgradeStatus"
QT_MOC_LITERAL(21, 229, 6), // "status"
QT_MOC_LITERAL(22, 236, 17), // "onUpgradeFinished"
QT_MOC_LITERAL(23, 254, 7), // "success"
QT_MOC_LITERAL(24, 262, 7), // "message"
QT_MOC_LITERAL(25, 270, 23), // "receSensorDataToPrevent"
QT_MOC_LITERAL(26, 294, 5), // "index"
QT_MOC_LITERAL(27, 300, 4), // "temp"
QT_MOC_LITERAL(28, 305, 6), // "humity"
QT_MOC_LITERAL(29, 312, 16), // "QVector<uint8_t>"
QT_MOC_LITERAL(30, 329, 4), // "time"
QT_MOC_LITERAL(31, 334, 22), // "onParametersConfirmBtn"
QT_MOC_LITERAL(32, 357, 16), // "updateWriteCount"
QT_MOC_LITERAL(33, 374, 3) // "num"

    },
    "MainWindow\0sigConnect\0\0ip\0port\0"
    "sigDisconnect\0sigSend\0data\0sigUpdate\0"
    "sigSendParametersData\0toConnect\0"
    "toDisConnect\0connected\0disconnected\0"
    "receiveMessages\0messages\0sendMessages\0"
    "startUpgrade\0onUpgradeProgress\0percent\0"
    "onUpgradeStatus\0status\0onUpgradeFinished\0"
    "success\0message\0receSensorDataToPrevent\0"
    "index\0temp\0humity\0QVector<uint8_t>\0"
    "time\0onParametersConfirmBtn\0"
    "updateWriteCount\0num"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  104,    2, 0x06 /* Public */,
       5,    0,  109,    2, 0x06 /* Public */,
       6,    1,  110,    2, 0x06 /* Public */,
       8,    1,  113,    2, 0x06 /* Public */,
       9,    1,  116,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,  119,    2, 0x08 /* Private */,
      11,    0,  120,    2, 0x08 /* Private */,
      12,    0,  121,    2, 0x08 /* Private */,
      13,    0,  122,    2, 0x08 /* Private */,
      14,    1,  123,    2, 0x08 /* Private */,
      16,    0,  126,    2, 0x08 /* Private */,
      17,    0,  127,    2, 0x08 /* Private */,
      18,    1,  128,    2, 0x08 /* Private */,
      20,    1,  131,    2, 0x08 /* Private */,
      22,    2,  134,    2, 0x08 /* Private */,
      25,    4,  139,    2, 0x08 /* Private */,
      31,    0,  148,    2, 0x08 /* Private */,
      32,    1,  149,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UShort,    3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    7,
    QMetaType::Void, QMetaType::QByteArray,    7,
    QMetaType::Void, QMetaType::QByteArray,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   23,   24,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float, 0x80000000 | 29,   26,   27,   28,   30,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   33,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigConnect((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 1: _t->sigDisconnect(); break;
        case 2: _t->sigSend((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->sigUpdate((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->sigSendParametersData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->toConnect(); break;
        case 6: _t->toDisConnect(); break;
        case 7: _t->connected(); break;
        case 8: _t->disconnected(); break;
        case 9: _t->receiveMessages((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->sendMessages(); break;
        case 11: _t->startUpgrade(); break;
        case 12: _t->onUpgradeProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->onUpgradeStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->onUpgradeFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 15: _t->receSensorDataToPrevent((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< QVector<uint8_t>(*)>(_a[4]))); break;
        case 16: _t->onParametersConfirmBtn(); break;
        case 17: _t->updateWriteCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(QString , quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sigConnect)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sigDisconnect)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sigSend)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sigUpdate)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sigSendParametersData)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::sigConnect(QString _t1, quint16 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::sigDisconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::sigSend(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::sigUpdate(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::sigSendParametersData(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
