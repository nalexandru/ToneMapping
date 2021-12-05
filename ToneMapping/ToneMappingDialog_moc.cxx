/****************************************************************************
** Meta object code from reading C++ file 'ToneMappingDialog.h'
**
** Created: Sun Dec 5 05:40:05 2021
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ToneMappingDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ToneMappingDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ToneMappingDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      31,   18,   18,   18, 0x0a,
      47,   43,   18,   18, 0x0a,
      70,   43,   18,   18, 0x0a,
      96,   90,   18,   18, 0x0a,
     121,  116,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ToneMappingDialog[] = {
    "ToneMappingDialog\0\0loadImage()\0"
    "saveImage()\0val\0exposureChanged(float)\0"
    "gammaChanged(float)\0state\0mtStateChanged(int)\0"
    "mode\0modeChanged(int)\0"
};

void ToneMappingDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ToneMappingDialog *_t = static_cast<ToneMappingDialog *>(_o);
        switch (_id) {
        case 0: _t->loadImage(); break;
        case 1: _t->saveImage(); break;
        case 2: _t->exposureChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->gammaChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->mtStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->modeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ToneMappingDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ToneMappingDialog::staticMetaObject = {
    { &DzBasicDialog::staticMetaObject, qt_meta_stringdata_ToneMappingDialog,
      qt_meta_data_ToneMappingDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ToneMappingDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ToneMappingDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ToneMappingDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToneMappingDialog))
        return static_cast<void*>(const_cast< ToneMappingDialog*>(this));
    return DzBasicDialog::qt_metacast(_clname);
}

int ToneMappingDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DzBasicDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
