TEMPLATE = app
CONFIG  += console c++11
CONFIG  -= app_bundle

HEADERS += \
    mathematica.h \
    sfmrecon.h

SOURCES += main.cpp \
    mathematica.cpp \
    sfmrecon.cpp

  #-- miko libraries
unix:!macx
{
  MIKO_CGAL       = $$PWD/../../MikoLibraries/MikoCgal
  MIKO_CGAL_LIB   = $$MIKO_CGAL/build/target
  MIKO_CGAL_INC   = $$MIKO_CGAL/library

  MIKO_TOOLS      = $$PWD/../../MikoLibraries/MikoTools
  MIKO_TOOLS_LIB  = $$MIKO_TOOLS/build/target
  MIKO_TOOLS_INC  = $$MIKO_TOOLS/library

  LIBS          += -L$$MIKO_CGAL_LIB   -lMikoCgal
  DEPENDPATH    +=   $$MIKO_CGAL_INC
  INCLUDEPATH   +=   $$MIKO_CGAL_INC
  QMAKE_LFLAGS  += -Wl,--rpath=$$MIKO_CGAL_LIB

  LIBS          += -L$$MIKO_TOOLS_LIB  -lMikoTools
  INCLUDEPATH   +=   $$MIKO_TOOLS_INC
  DEPENDPATH    +=   $$MIKO_TOOLS_INC
  QMAKE_LFLAGS  += -Wl,--rpath=$$MIKO_TOOLS_LIB
}

  #-- theia sfm
LIBS += -ltheia
LIBS += -lglog
LIBS += -lgflags

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/theia
INCLUDEPATH += /usr/local/include/theia/libraries
INCLUDEPATH += /usr/local/include/theia/libraries/vlfeat
INCLUDEPATH += /usr/local/include/theia/libraries/statx
INCLUDEPATH += /usr/local/include/theia/libraries/optimo

  #-- cgal
LIBS += -lCGAL
LIBS += -lgmp

CONFIG    += link_pkgconfig
PKGCONFIG += eigen3

  #-- mathematica
MATHEMATICA               = /usr/local/Wolfram/Mathematica/11.1
MATHEMATICA_LIB           = $$MATHEMATICA/SystemFiles/Libraries/Linux-x86-64
MATHEMATICA_MATHLINK_LIB  = $$MATHEMATICA/SystemFiles/Links/MathLink/DeveloperKit/Linux-x86-64/CompilerAdditions

LIBS          += -L$$MATHEMATICA_LIB -lML64i4
DEPENDPATH    +=   $$MATHAMTICA_LIB
INCLUDEPATH   +=   $$MATHAMTICA_LIB
INCLUDEPATH   +=   $$MATHEMATICA_MATHLINK_LIB
QMAKE_LFLAGS  += -Wl,--rpath=$$MATHAMTICA_LIB
QMAKE_LFLAGS  += -Wl,--rpath=$$MATHEMATICA_MATHLINK_LIB

LIBS += -lm
LIBS += -lpthread
LIBS += -lrt
LIBS += -lstdc++
LIBS += -ldl
LIBS += -luuid
