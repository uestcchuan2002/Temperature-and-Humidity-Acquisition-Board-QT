/****************************************************************************
** Meta object code from reading C++ file 'firmwareupgradeworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DataAcquistionCard_Software/firmwareupgradeworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'firmwareupgradeworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FirmwareUpgradeWorker_t {
    QByteArrayData data[9];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FirmwareUpgradeWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FirmwareUpgradeWorker_t qt_meta_stringdata_FirmwareUpgradeWorker = {
    {
QT_MOC_LITERAL(0, 0, 21), // "FirmwareUpgradeWorker"
QT_MOC_LITERAL(1, 22, 15), // "progressChanged"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 7), // "percent"
QT_MOC_LITERAL(4, 47, 13), // "statusChanged"
QT_MOC_LITERAL(5, 61, 6), // "status"
QT_MOC_LITERAL(6, 68, 15), // "upgradeFinished"
QT_MOC_LITERAL(7, 84, 7), // "success"
QT_MOC_LITERAL(8, 92, 7) // "message"

    },
    "FirmwareUpgradeWorker\0progressChanged\0"
    "\0percent\0statusChanged\0status\0"
    "upgradeFinished\0success\0message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FirmwareUpgradeWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,
       6,    2,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    7,    8,

       0        // eod
};

void FirmwareUpgradeWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FirmwareUpgradeWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->progressChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->statusChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->upgradeFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FirmwareUpgradeWorker::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwareUpgradeWorker::progressChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FirmwareUpgradeWorker::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwareUpgradeWorker::statusChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FirmwareUpgradeWorker::*)(bool , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwareUpgradeWorker::upgradeFinished)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FirmwareUpgradeWorker::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_FirmwareUpgradeWorker.data,
    qt_meta_data_FirmwareUpgradeWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FirmwareUpgradeWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FirmwareUpgradeWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FirmwareUpgradeWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FirmwareUpgradeWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void FirmwareUpgradeWorker::progressChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FirmwareUpgradeWorker::statusChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FirmwareUpgradeWorker::upgradeFinished(bool _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
