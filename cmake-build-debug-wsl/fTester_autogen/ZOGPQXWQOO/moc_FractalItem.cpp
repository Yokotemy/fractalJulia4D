/****************************************************************************
** Meta object code from reading C++ file 'FractalItem.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/UIBridge/FractalItem.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FractalItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11FractalItemE_t {};
} // unnamed namespace

template <> constexpr inline auto FractalItem::qt_create_metaobjectdata<qt_meta_tag_ZN11FractalItemE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "FractalItem",
        "renderTimeChanged",
        "",
        "renderScaleChanged",
        "dimensionChanged",
        "useGPUChanged",
        "isRunningChanged",
        "animAxisChanged",
        "cpuFrameReady",
        "updateFrame",
        "onCpuFrameReady",
        "setKeyState",
        "key",
        "isPressed",
        "rotateCamera",
        "dx",
        "dy",
        "resetBuffers",
        "setCx",
        "v",
        "setCy",
        "setCz",
        "setCw",
        "renderTime",
        "useGPU",
        "isRunning",
        "renderScale",
        "dimension",
        "animAxis"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'renderTimeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'renderScaleChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dimensionChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'useGPUChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isRunningChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'animAxisChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cpuFrameReady'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateFrame'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCpuFrameReady'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Method 'setKeyState'
        QtMocHelpers::MethodData<void(const QString &, bool)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::Bool, 13 },
        }}),
        // Method 'rotateCamera'
        QtMocHelpers::MethodData<void(float, float)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 15 }, { QMetaType::Float, 16 },
        }}),
        // Method 'resetBuffers'
        QtMocHelpers::MethodData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'setCx'
        QtMocHelpers::MethodData<void(float)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 19 },
        }}),
        // Method 'setCy'
        QtMocHelpers::MethodData<void(float)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 19 },
        }}),
        // Method 'setCz'
        QtMocHelpers::MethodData<void(float)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 19 },
        }}),
        // Method 'setCw'
        QtMocHelpers::MethodData<void(float)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 19 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'renderTime'
        QtMocHelpers::PropertyData<QString>(23, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'useGPU'
        QtMocHelpers::PropertyData<bool>(24, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'isRunning'
        QtMocHelpers::PropertyData<bool>(25, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
        // property 'renderScale'
        QtMocHelpers::PropertyData<float>(26, QMetaType::Float, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'dimension'
        QtMocHelpers::PropertyData<int>(27, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'animAxis'
        QtMocHelpers::PropertyData<int>(28, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 5),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FractalItem, qt_meta_tag_ZN11FractalItemE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject FractalItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QQuickPaintedItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11FractalItemE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11FractalItemE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11FractalItemE_t>.metaTypes,
    nullptr
} };

void FractalItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FractalItem *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->renderTimeChanged(); break;
        case 1: _t->renderScaleChanged(); break;
        case 2: _t->dimensionChanged(); break;
        case 3: _t->useGPUChanged(); break;
        case 4: _t->isRunningChanged(); break;
        case 5: _t->animAxisChanged(); break;
        case 6: _t->cpuFrameReady(); break;
        case 7: _t->updateFrame(); break;
        case 8: _t->onCpuFrameReady(); break;
        case 9: _t->setKeyState((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 10: _t->rotateCamera((*reinterpret_cast<std::add_pointer_t<float>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<float>>(_a[2]))); break;
        case 11: _t->resetBuffers(); break;
        case 12: _t->setCx((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 13: _t->setCy((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 14: _t->setCz((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 15: _t->setCw((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FractalItem::*)()>(_a, &FractalItem::renderTimeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (FractalItem::*)()>(_a, &FractalItem::renderScaleChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (FractalItem::*)()>(_a, &FractalItem::dimensionChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (FractalItem::*)()>(_a, &FractalItem::useGPUChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (FractalItem::*)()>(_a, &FractalItem::isRunningChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (FractalItem::*)()>(_a, &FractalItem::animAxisChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (FractalItem::*)()>(_a, &FractalItem::cpuFrameReady, 6))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->renderTime(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->useGPU(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isRunning(); break;
        case 3: *reinterpret_cast<float*>(_v) = _t->renderScale(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->dimension(); break;
        case 5: *reinterpret_cast<int*>(_v) = _t->animAxis(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setUseGPU(*reinterpret_cast<bool*>(_v)); break;
        case 2: _t->setIsRunning(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setRenderScale(*reinterpret_cast<float*>(_v)); break;
        case 4: _t->setDimension(*reinterpret_cast<int*>(_v)); break;
        case 5: _t->setAnimAxis(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *FractalItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FractalItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11FractalItemE_t>.strings))
        return static_cast<void*>(this);
    return QQuickPaintedItem::qt_metacast(_clname);
}

int FractalItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickPaintedItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void FractalItem::renderTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FractalItem::renderScaleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FractalItem::dimensionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void FractalItem::useGPUChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void FractalItem::isRunningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void FractalItem::animAxisChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void FractalItem::cpuFrameReady()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
