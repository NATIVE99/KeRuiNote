/****************************************************************************
** Meta object code from reading C++ file 'RemoteControl.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../RemoteControl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RemoteControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RemoteControl_t {
    QByteArrayData data[14];
    char stringdata0[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RemoteControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RemoteControl_t qt_meta_stringdata_RemoteControl = {
    {
QT_MOC_LITERAL(0, 0, 13), // "RemoteControl"
QT_MOC_LITERAL(1, 14, 13), // "slot_ShowMenu"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3), // "pos"
QT_MOC_LITERAL(4, 33, 12), // "slot_ShowCmd"
QT_MOC_LITERAL(5, 46, 14), // "slot_ShowGreen"
QT_MOC_LITERAL(6, 61, 17), // "HandleClinentInfo"
QT_MOC_LITERAL(7, 79, 15), // "CClientSession*"
QT_MOC_LITERAL(8, 95, 3), // "pCs"
QT_MOC_LITERAL(9, 99, 13), // "slot_RecvData"
QT_MOC_LITERAL(10, 113, 1), // "s"
QT_MOC_LITERAL(11, 115, 11), // "nDialogType"
QT_MOC_LITERAL(12, 127, 4), // "data"
QT_MOC_LITERAL(13, 132, 17) // "slot_CreateDialog"

    },
    "RemoteControl\0slot_ShowMenu\0\0pos\0"
    "slot_ShowCmd\0slot_ShowGreen\0"
    "HandleClinentInfo\0CClientSession*\0pCs\0"
    "slot_RecvData\0s\0nDialogType\0data\0"
    "slot_CreateDialog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RemoteControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       4,    0,   47,    2, 0x0a /* Public */,
       5,    0,   48,    2, 0x0a /* Public */,
       6,    1,   49,    2, 0x0a /* Public */,
       9,    3,   52,    2, 0x0a /* Public */,
      13,    2,   59,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QPoint,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QByteArray,   10,   11,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,

       0        // eod
};

void RemoteControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RemoteControl *_t = static_cast<RemoteControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slot_ShowMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 1: _t->slot_ShowCmd(); break;
        case 2: _t->slot_ShowGreen(); break;
        case 3: _t->HandleClinentInfo((*reinterpret_cast< CClientSession*(*)>(_a[1]))); break;
        case 4: _t->slot_RecvData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 5: _t->slot_CreateDialog((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject RemoteControl::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_RemoteControl.data,
      qt_meta_data_RemoteControl,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RemoteControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RemoteControl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RemoteControl.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int RemoteControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
