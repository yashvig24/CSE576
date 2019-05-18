/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[37];
    char stringdata0[306];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 12), // "LoadDatabase"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 9), // "OpenImage"
QT_MOC_LITERAL(4, 35, 13), // "QueryDatabase"
QT_MOC_LITERAL(5, 49, 5), // "Reset"
QT_MOC_LITERAL(6, 55, 10), // "DownSample"
QT_MOC_LITERAL(7, 66, 8), // "img_gray"
QT_MOC_LITERAL(8, 75, 4), // "int*"
QT_MOC_LITERAL(9, 80, 8), // "img_down"
QT_MOC_LITERAL(10, 89, 1), // "w"
QT_MOC_LITERAL(11, 91, 1), // "h"
QT_MOC_LITERAL(12, 93, 8), // "InitGLCM"
QT_MOC_LITERAL(13, 102, 9), // "double***"
QT_MOC_LITERAL(14, 112, 11), // "num_regions"
QT_MOC_LITERAL(15, 124, 4), // "size"
QT_MOC_LITERAL(16, 129, 10), // "updateGLCM"
QT_MOC_LITERAL(17, 140, 4), // "glcm"
QT_MOC_LITERAL(18, 145, 11), // "regionIndex"
QT_MOC_LITERAL(19, 157, 1), // "r"
QT_MOC_LITERAL(20, 159, 1), // "c"
QT_MOC_LITERAL(21, 161, 3), // "d_y"
QT_MOC_LITERAL(22, 165, 3), // "d_x"
QT_MOC_LITERAL(23, 169, 8), // "normGLCM"
QT_MOC_LITERAL(24, 178, 11), // "analyzeGLCM"
QT_MOC_LITERAL(25, 190, 7), // "double*"
QT_MOC_LITERAL(26, 198, 6), // "energy"
QT_MOC_LITERAL(27, 205, 7), // "entropy"
QT_MOC_LITERAL(28, 213, 8), // "contrast"
QT_MOC_LITERAL(29, 222, 10), // "CleanNoise"
QT_MOC_LITERAL(30, 233, 4), // "nimg"
QT_MOC_LITERAL(31, 238, 13), // "region_thresh"
QT_MOC_LITERAL(32, 252, 15), // "computeCentroid"
QT_MOC_LITERAL(33, 268, 8), // "double**"
QT_MOC_LITERAL(34, 277, 8), // "centroid"
QT_MOC_LITERAL(35, 286, 11), // "computeBBox"
QT_MOC_LITERAL(36, 298, 7) // "boundry"

    },
    "MainWindow\0LoadDatabase\0\0OpenImage\0"
    "QueryDatabase\0Reset\0DownSample\0img_gray\0"
    "int*\0img_down\0w\0h\0InitGLCM\0double***\0"
    "num_regions\0size\0updateGLCM\0glcm\0"
    "regionIndex\0r\0c\0d_y\0d_x\0normGLCM\0"
    "analyzeGLCM\0double*\0energy\0entropy\0"
    "contrast\0CleanNoise\0nimg\0region_thresh\0"
    "computeCentroid\0double**\0centroid\0"
    "computeBBox\0boundry"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08 /* Private */,
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    4,   78,    2, 0x08 /* Private */,
      12,    2,   87,    2, 0x08 /* Private */,
      16,    9,   92,    2, 0x08 /* Private */,
      23,    3,  111,    2, 0x08 /* Private */,
      24,    5,  118,    2, 0x08 /* Private */,
      29,    5,  129,    2, 0x08 /* Private */,
      32,    5,  140,    2, 0x08 /* Private */,
      35,    5,  151,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage, 0x80000000 | 8, QMetaType::Int, QMetaType::Int,    7,    9,   10,   11,
    0x80000000 | 13, QMetaType::Int, QMetaType::Int,   14,   15,
    QMetaType::Void, 0x80000000 | 13, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, 0x80000000 | 8,   17,   18,   19,   20,   21,   22,   10,   11,    9,
    QMetaType::Void, 0x80000000 | 13, QMetaType::Int, QMetaType::Int,   17,   14,   15,
    QMetaType::Void, 0x80000000 | 13, QMetaType::Int, 0x80000000 | 25, 0x80000000 | 25, 0x80000000 | 25,   17,   14,   26,   27,   28,
    QMetaType::Int, 0x80000000 | 8, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   30,   10,   11,   14,   31,
    QMetaType::Void, 0x80000000 | 33, QMetaType::Int, 0x80000000 | 8, QMetaType::Int, QMetaType::Int,   34,   14,   30,   10,   11,
    QMetaType::Void, 0x80000000 | 33, 0x80000000 | 8, QMetaType::Int, QMetaType::Int, QMetaType::Int,   36,   30,   14,   10,   11,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->LoadDatabase(); break;
        case 1: _t->OpenImage(); break;
        case 2: _t->QueryDatabase(); break;
        case 3: _t->Reset(); break;
        case 4: _t->DownSample((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 5: { double*** _r = _t->InitGLCM((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< double****>(_a[0]) = std::move(_r); }  break;
        case 6: _t->updateGLCM((*reinterpret_cast< double***(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8])),(*reinterpret_cast< int*(*)>(_a[9]))); break;
        case 7: _t->normGLCM((*reinterpret_cast< double***(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->analyzeGLCM((*reinterpret_cast< double***(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double*(*)>(_a[3])),(*reinterpret_cast< double*(*)>(_a[4])),(*reinterpret_cast< double*(*)>(_a[5]))); break;
        case 9: { int _r = _t->CleanNoise((*reinterpret_cast< int*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->computeCentroid((*reinterpret_cast< double**(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int*(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 11: _t->computeBBox((*reinterpret_cast< double**(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
