CONFIG += qt qthread
QT += opengl
TARGET = soas
TEMPLATE = app
SOURCES += \
    src/main.cpp \
    src/game/scene.cpp \
    src/geom/camera.cpp \
    src/geom/spheremesh.cpp \
    src/geom/cubemesh.cpp \
    src/geom/conemesh.cpp \
    src/geom/mesh.cpp \
    src/geom/cylindermesh.cpp \
    src/geom/intersectshape.cpp \
    src/math/matrix.cpp \
    src/game/state.cpp \
    src/game/drawengine.cpp \
    src/game/glwidget.cpp

HEADERS += \
    src/game/scene.h \
    src/geom/hit.h \
    src/geom/mesh.h \
    src/geom/ray.h \
    src/geom/box.h \
    src/geom/cubemesh.h \
    src/geom/camera.h \
    src/geom/spheremesh.h \
    src/geom/cylindermesh.h \
    src/geom/intersectshape.h \
    src/geom/interval.h \
    src/geom/conemesh.h \
    src/math/algebra.h \
    src/math/matrix.h \
    src/math/common.h \
    src/math/vector.h \
    src/game/state.h \
    src/game/drawengine.h \
    src/game/glwidget.h

INCLUDEPATH += src/ui src/math src/geom src/game 
DEPENDPATH += src/ui src/math src/geom src/game 
DEFINES += TIXML_USE_STL
OTHER_FILES +=

RCC_DIR = "build/rcc"
UI_DIR = "build/ui"
MOC_DIR = "build/moc"
OBJECTS_DIR = "build/obj"

RESOURCES = application.qrc

# Dont add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat\
  -Wchar-subscripts -Wformat -Wmain -Wmissing-braces -Wparentheses -Wreorder\
  -Wreturn-type -Wsequence-point -Wsign-compare -Wstrict-aliasing\
  -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunused-label\
  -Wunused-variable -Wvolatile-register-var -Wno-extra

# QMAKE_CXX_FLAGS_WARN_ON += -Wunknown-pragmas -Wunused-function -Wmain

macx {
    QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds -Wc++0x-compat
}
