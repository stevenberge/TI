/****************************************************************************
** Meta object code from reading C++ file 'textedit.h'
**
** Created: Fri Apr 11 16:29:30 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "textedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'textedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TextEdit[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
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
     123,    9,    9,    9, 0x08,
     136,    9,    9,    9, 0x08,
     157,    9,    9,    9, 0x08,
     168,    9,    9,    9, 0x08,
     179,    9,    9,    9, 0x08,
     190,    9,    9,    9, 0x08,
     204,    9,    9,    9, 0x08,
     218,    9,    9,    9, 0x08,
     229,    9,    9,    9, 0x08,
     240,    9,    9,    9, 0x08,
     256,    9,    9,    9, 0x08,
     271,  269,    9,    9, 0x08,
     289,    9,    9,    9, 0x08,
     301,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TextEdit[] = {
    "TextEdit\0\0e\0mouseMoveSig(QMouseEvent*)\0"
    "mousePressSig(QMouseEvent*)\0"
    "mouseReleaseSig(QMouseEvent*)\0"
    "onMouseMove(QMouseEvent*)\0loadCanvas()\0"
    "pageChanged(QString)\0nextPage()\0"
    "prevPage()\0hideText()\0iniFontSize()\0"
    "dividePages()\0fileOpen()\0textBold()\0"
    "textUnderline()\0textItalic()\0p\0"
    "textSize(QString)\0textColor()\0"
    "cursorPositionChanged()\0"
};

const QMetaObject TextEdit::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TextEdit,
      qt_meta_data_TextEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TextEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TextEdit))
        return static_cast<void*>(const_cast< TextEdit*>(this));
    return QWidget::qt_metacast(_clname);
}

int TextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mouseMoveSig((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 1: mousePressSig((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: mouseReleaseSig((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: onMouseMove((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: loadCanvas(); break;
        case 5: pageChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: nextPage(); break;
        case 7: prevPage(); break;
        case 8: hideText(); break;
        case 9: iniFontSize(); break;
        case 10: dividePages(); break;
        case 11: fileOpen(); break;
        case 12: textBold(); break;
        case 13: textUnderline(); break;
        case 14: textItalic(); break;
        case 15: textSize((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: textColor(); break;
        case 17: cursorPositionChanged(); break;
        default: ;
        }
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void TextEdit::mouseMoveSig(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TextEdit::mousePressSig(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TextEdit::mouseReleaseSig(QMouseEvent * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
