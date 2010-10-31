/****************************************************************************
** Meta object code from reading C++ file 'cQMainWindow.h'
**
** Created: Sun 31. Oct 01:03:05 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "cQMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cQMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_cQMainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      30,   13,   13,   13, 0x0a,
      44,   13,   13,   13, 0x0a,
      59,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_cQMainWindow[] = {
    "cQMainWindow\0\0OnSetBgColour()\0"
    "OnCreateBox()\0OnMoveCamera()\0OnMoveBox0()\0"
};

const QMetaObject cQMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_cQMainWindow,
      qt_meta_data_cQMainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &cQMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *cQMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *cQMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_cQMainWindow))
        return static_cast<void*>(const_cast< cQMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int cQMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnSetBgColour(); break;
        case 1: OnCreateBox(); break;
        case 2: OnMoveCamera(); break;
        case 3: OnMoveBox0(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
