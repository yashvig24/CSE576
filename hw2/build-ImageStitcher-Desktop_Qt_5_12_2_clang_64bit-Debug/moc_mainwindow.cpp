/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../code/mainwindow.h"
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
    QByteArrayData data[34];
    char stringdata0[310];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 9), // "OpenImage"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 9), // "SaveImage"
QT_MOC_LITERAL(4, 32, 17), // "HarrisCornerImage"
QT_MOC_LITERAL(5, 50, 11), // "MatchImages"
QT_MOC_LITERAL(6, 62, 6), // "RANSAC"
QT_MOC_LITERAL(7, 69, 12), // "StitchImages"
QT_MOC_LITERAL(8, 82, 12), // "PaddingImage"
QT_MOC_LITERAL(9, 95, 7), // "double*"
QT_MOC_LITERAL(10, 103, 5), // "image"
QT_MOC_LITERAL(11, 109, 10), // "imageWidth"
QT_MOC_LITERAL(12, 120, 11), // "imageHeight"
QT_MOC_LITERAL(13, 132, 11), // "kernelWidth"
QT_MOC_LITERAL(14, 144, 12), // "kernelHeight"
QT_MOC_LITERAL(15, 157, 11), // "Convolution"
QT_MOC_LITERAL(16, 169, 6), // "kernel"
QT_MOC_LITERAL(17, 176, 3), // "add"
QT_MOC_LITERAL(18, 180, 17), // "FirstDerivImage_x"
QT_MOC_LITERAL(19, 198, 1), // "w"
QT_MOC_LITERAL(20, 200, 1), // "h"
QT_MOC_LITERAL(21, 202, 5), // "sigma"
QT_MOC_LITERAL(22, 208, 17), // "FirstDerivImage_y"
QT_MOC_LITERAL(23, 226, 20), // "ConvertDouble2QImage"
QT_MOC_LITERAL(24, 247, 5), // "Image"
QT_MOC_LITERAL(25, 253, 7), // "QImage*"
QT_MOC_LITERAL(26, 261, 13), // "restrictColor"
QT_MOC_LITERAL(27, 275, 4), // "QRgb"
QT_MOC_LITERAL(28, 280, 3), // "red"
QT_MOC_LITERAL(29, 284, 5), // "green"
QT_MOC_LITERAL(30, 290, 4), // "blue"
QT_MOC_LITERAL(31, 295, 8), // "ImageMul"
QT_MOC_LITERAL(32, 304, 2), // "Ix"
QT_MOC_LITERAL(33, 307, 2) // "Iy"

    },
    "MainWindow\0OpenImage\0\0SaveImage\0"
    "HarrisCornerImage\0MatchImages\0RANSAC\0"
    "StitchImages\0PaddingImage\0double*\0"
    "image\0imageWidth\0imageHeight\0kernelWidth\0"
    "kernelHeight\0Convolution\0kernel\0add\0"
    "FirstDerivImage_x\0w\0h\0sigma\0"
    "FirstDerivImage_y\0ConvertDouble2QImage\0"
    "Image\0QImage*\0restrictColor\0QRgb\0red\0"
    "green\0blue\0ImageMul\0Ix\0Iy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    5,   90,    2, 0x08 /* Private */,
      15,    7,  101,    2, 0x08 /* Private */,
      15,    6,  116,    2, 0x28 /* Private | MethodCloned */,
      18,    4,  129,    2, 0x08 /* Private */,
      22,    4,  138,    2, 0x08 /* Private */,
      23,    4,  147,    2, 0x08 /* Private */,
      26,    3,  156,    2, 0x08 /* Private */,
      31,    4,  163,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 9, 0x80000000 | 9, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   10,   11,   12,   13,   14,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Int, QMetaType::Int, 0x80000000 | 9, QMetaType::Int, QMetaType::Int, QMetaType::Bool,   10,   11,   12,   16,   13,   14,   17,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Int, QMetaType::Int, 0x80000000 | 9, QMetaType::Int, QMetaType::Int,   10,   11,   12,   16,   13,   14,
    0x80000000 | 9, 0x80000000 | 9, QMetaType::Int, QMetaType::Int, QMetaType::Double,   10,   19,   20,   21,
    0x80000000 | 9, 0x80000000 | 9, QMetaType::Int, QMetaType::Int, QMetaType::Double,   10,   19,   20,   21,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 25, QMetaType::Int, QMetaType::Int,   24,   10,   11,   12,
    0x80000000 | 27, QMetaType::Double, QMetaType::Double, QMetaType::Double,   28,   29,   30,
    0x80000000 | 9, 0x80000000 | 9, 0x80000000 | 9, QMetaType::Int, QMetaType::Int,   32,   33,   19,   20,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OpenImage(); break;
        case 1: _t->SaveImage(); break;
        case 2: _t->HarrisCornerImage(); break;
        case 3: _t->MatchImages(); break;
        case 4: _t->RANSAC(); break;
        case 5: _t->StitchImages(); break;
        case 6: { double* _r = _t->PaddingImage((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< double**>(_a[0]) = std::move(_r); }  break;
        case 7: _t->Convolution((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< double*(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7]))); break;
        case 8: _t->Convolution((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< double*(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 9: { double* _r = _t->FirstDerivImage_x((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< double**>(_a[0]) = std::move(_r); }  break;
        case 10: { double* _r = _t->FirstDerivImage_y((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< double**>(_a[0]) = std::move(_r); }  break;
        case 11: _t->ConvertDouble2QImage((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< QImage*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 12: { QRgb _r = _t->restrictColor((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QRgb*>(_a[0]) = std::move(_r); }  break;
        case 13: { double* _r = _t->ImageMul((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< double*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< double**>(_a[0]) = std::move(_r); }  break;
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
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
