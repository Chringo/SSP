/****************************************************************************
** Meta object code from reading C++ file 'AttributesHandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AttributesHandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AttributesHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Ui__AttributesHandler_t {
    QByteArrayData data[14];
    char stringdata0[245];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Ui__AttributesHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Ui__AttributesHandler_t qt_meta_stringdata_Ui__AttributesHandler = {
    {
QT_MOC_LITERAL(0, 0, 21), // "Ui::AttributesHandler"
QT_MOC_LITERAL(1, 22, 22), // "on_translate_X_changed"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 3), // "val"
QT_MOC_LITERAL(4, 50, 22), // "on_translate_Y_changed"
QT_MOC_LITERAL(5, 73, 22), // "on_translate_Z_changed"
QT_MOC_LITERAL(6, 96, 21), // "on_rotation_X_changed"
QT_MOC_LITERAL(7, 118, 21), // "on_rotation_Y_changed"
QT_MOC_LITERAL(8, 140, 21), // "on_rotation_Z_changed"
QT_MOC_LITERAL(9, 162, 18), // "on_scale_X_changed"
QT_MOC_LITERAL(10, 181, 18), // "on_scale_Y_changed"
QT_MOC_LITERAL(11, 200, 18), // "on_scale_Z_changed"
QT_MOC_LITERAL(12, 219, 19), // "on_isStatic_changed"
QT_MOC_LITERAL(13, 239, 5) // "state"

    },
    "Ui::AttributesHandler\0on_translate_X_changed\0"
    "\0val\0on_translate_Y_changed\0"
    "on_translate_Z_changed\0on_rotation_X_changed\0"
    "on_rotation_Y_changed\0on_rotation_Z_changed\0"
    "on_scale_X_changed\0on_scale_Y_changed\0"
    "on_scale_Z_changed\0on_isStatic_changed\0"
    "state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Ui__AttributesHandler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x0a /* Public */,
       4,    1,   67,    2, 0x0a /* Public */,
       5,    1,   70,    2, 0x0a /* Public */,
       6,    1,   73,    2, 0x0a /* Public */,
       7,    1,   76,    2, 0x0a /* Public */,
       8,    1,   79,    2, 0x0a /* Public */,
       9,    1,   82,    2, 0x0a /* Public */,
      10,    1,   85,    2, 0x0a /* Public */,
      11,    1,   88,    2, 0x0a /* Public */,
      12,    1,   91,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Int,   13,

       0        // eod
};

void Ui::AttributesHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AttributesHandler *_t = static_cast<AttributesHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_translate_X_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->on_translate_Y_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->on_translate_Z_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->on_rotation_X_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->on_rotation_Y_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->on_rotation_Z_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->on_scale_X_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->on_scale_Y_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->on_scale_Z_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->on_isStatic_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Ui::AttributesHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Ui__AttributesHandler.data,
      qt_meta_data_Ui__AttributesHandler,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Ui::AttributesHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Ui::AttributesHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Ui__AttributesHandler.stringdata0))
        return static_cast<void*>(const_cast< AttributesHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int Ui::AttributesHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
