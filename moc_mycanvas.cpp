/****************************************************************************
** Meta object code from reading C++ file 'mycanvas.h'
**
** Created: Fri Apr 11 16:29:31 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mycanvas.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mycanvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyCanvas[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   10,    9,    9, 0x05,
      39,   10,    9,    9, 0x05,
      67,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      97,   10,    9,    9, 0x08,
     117,   10,    9,    9, 0x08,
     137,   10,    9,    9, 0x08,
     157,   10,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MyCanvas[] = {
    "MyCanvas\0\0e\0mouseMoveSig(QMouseEvent*)\0"
    "mousePressSig(QMouseEvent*)\0"
    "mouseReleaseSig(QMouseEvent*)\0"
    "slot1(QMouseEvent*)\0slot2(QMouseEvent*)\0"
    "slot3(QMouseEvent*)\0mouseMoveSlot(QMouseEvent*)\0"
};

const QMetaObject MyCanvas::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MyCanvas,
      qt_meta_data_MyCanvas, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyCanvas::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyCanvas))
        return static_cast<void*>(const_cast< MyCanvas*>(this));
    return QWidget::qt_metacast(_clname);
}

int MyCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mouseMoveSig((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 1: mousePressSig((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: mouseReleaseSig((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: slot1((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: slot2((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: slot3((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: mouseMoveSlot((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MyCanvas::mouseMoveSig(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyCanvas::mousePressSig(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MyCanvas::mouseReleaseSig(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
