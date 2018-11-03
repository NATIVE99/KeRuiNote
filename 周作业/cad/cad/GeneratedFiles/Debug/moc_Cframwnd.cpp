/****************************************************************************
** Meta object code from reading C++ file 'Cframwnd.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Cframwnd.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Cframwnd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Cframwnd_t {
    QByteArrayData data[12];
    char stringdata0[178];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cframwnd_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cframwnd_t qt_meta_stringdata_Cframwnd = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Cframwnd"
QT_MOC_LITERAL(1, 9, 16), // "slots_tirgerLine"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 7), // "checked"
QT_MOC_LITERAL(4, 35, 16), // "slots_tirgerRect"
QT_MOC_LITERAL(5, 52, 19), // "slots_tirgerEillpse"
QT_MOC_LITERAL(6, 72, 17), // "slots_tirgerColor"
QT_MOC_LITERAL(7, 90, 18), // "slots_tirgerSelect"
QT_MOC_LITERAL(8, 109, 16), // "slots_tirgerUndo"
QT_MOC_LITERAL(9, 126, 17), // "slots_tirgerExect"
QT_MOC_LITERAL(10, 144, 16), // "slots_tirgerOpen"
QT_MOC_LITERAL(11, 161, 16) // "slots_tirgerSave"

    },
    "Cframwnd\0slots_tirgerLine\0\0checked\0"
    "slots_tirgerRect\0slots_tirgerEillpse\0"
    "slots_tirgerColor\0slots_tirgerSelect\0"
    "slots_tirgerUndo\0slots_tirgerExect\0"
    "slots_tirgerOpen\0slots_tirgerSave"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cframwnd[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x0a /* Public */,
       4,    1,   62,    2, 0x0a /* Public */,
       5,    1,   65,    2, 0x0a /* Public */,
       6,    1,   68,    2, 0x0a /* Public */,
       7,    1,   71,    2, 0x0a /* Public */,
       8,    1,   74,    2, 0x0a /* Public */,
       9,    1,   77,    2, 0x0a /* Public */,
      10,    1,   80,    2, 0x0a /* Public */,
      11,    1,   83,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void Cframwnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Cframwnd *_t = static_cast<Cframwnd *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slots_tirgerLine((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->slots_tirgerRect((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slots_tirgerEillpse((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->slots_tirgerColor((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->slots_tirgerSelect((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->slots_tirgerUndo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->slots_tirgerExect((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->slots_tirgerOpen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->slots_tirgerSave((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Cframwnd::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Cframwnd.data,
      qt_meta_data_Cframwnd,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Cframwnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cframwnd::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Cframwnd.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Cframwnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
