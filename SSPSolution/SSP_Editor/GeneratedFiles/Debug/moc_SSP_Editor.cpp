/****************************************************************************
** Meta object code from reading C++ file 'SSP_Editor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../SSP_Editor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SSP_Editor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SSP_Editor_t {
    QByteArrayData data[8];
    char stringdata0[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SSP_Editor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SSP_Editor_t qt_meta_stringdata_SSP_Editor = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SSP_Editor"
QT_MOC_LITERAL(1, 11, 25), // "on_treeView_doubleClicked"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 29), // "on_treeView_selection_Changed"
QT_MOC_LITERAL(4, 68, 19), // "on_NewScene_clicked"
QT_MOC_LITERAL(5, 88, 20), // "on_LoadScene_clicked"
QT_MOC_LITERAL(6, 109, 20), // "on_SaveScene_clicked"
QT_MOC_LITERAL(7, 130, 19) // "on_BuildBPF_clicked"

    },
    "SSP_Editor\0on_treeView_doubleClicked\0"
    "\0on_treeView_selection_Changed\0"
    "on_NewScene_clicked\0on_LoadScene_clicked\0"
    "on_SaveScene_clicked\0on_BuildBPF_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SSP_Editor[] = {

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
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SSP_Editor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SSP_Editor *_t = static_cast<SSP_Editor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_treeView_doubleClicked(); break;
        case 1: _t->on_treeView_selection_Changed(); break;
        case 2: _t->on_NewScene_clicked(); break;
        case 3: _t->on_LoadScene_clicked(); break;
        case 4: _t->on_SaveScene_clicked(); break;
        case 5: _t->on_BuildBPF_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject SSP_Editor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_SSP_Editor.data,
      qt_meta_data_SSP_Editor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SSP_Editor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SSP_Editor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SSP_Editor.stringdata0))
        return static_cast<void*>(const_cast< SSP_Editor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int SSP_Editor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
