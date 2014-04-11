/****************************************************************************
** Meta object code from reading C++ file 'cameraprocess.h'
**
** Created: Fri Apr 11 16:18:19 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "cameraprocess.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cameraprocess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CameraProcess[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   15,   14,   14, 0x05,
      66,   14,   14,   14, 0x05,
      77,   14,   14,   14, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CameraProcess[] = {
    "CameraProcess\0\0data,width,height\0"
    "sendData(unsigned char*,int,int)\0"
    "prevPage()\0nextPage()\0"
};

const QMetaObject CameraProcess::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CameraProcess,
      qt_meta_data_CameraProcess, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CameraProcess::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CameraProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CameraProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CameraProcess))
        return static_cast<void*>(const_cast< CameraProcess*>(this));
    return QThread::qt_metacast(_clname);
}

int CameraProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendData((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: prevPage(); break;
        case 2: nextPage(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CameraProcess::sendData(unsigned char * _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CameraProcess::prevPage()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CameraProcess::nextPage()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
