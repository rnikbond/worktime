QT += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkTime
TEMPLATE = app

#DEFINES += WT_DEBUG
#DEFINES += WT_INFO_CALL_FUNC

INCLUDEPATH += src
INCLUDEPATH += src/view
INCLUDEPATH += src/presenter
INCLUDEPATH += src/tools
INCLUDEPATH += src/tools/time

HEADERS += src/view/worktimewindow.h         \
           src/view/iworktime.h              \
           src/presenter/presenterworktime.h \
           src/tools/helperwt.h              \
           src/tools/time/wtime.h            \
           src/tools/time/wtimeext.h         \
           src/tools/dayworktime.h           \
           src/tools/interval.h              \
           src/tools/monthworktime.h         \
           src/tools/network.h               \
           src/coreworktime.h

SOURCES += src/main.cpp                        \
           src/view/worktimewindow.cpp         \
           src/presenter/presenterworktime.cpp \
           src/tools/helperwt.cpp              \
           src/tools/time/wtime.cpp            \
           src/tools/time/wtimeext.cpp         \
           src/tools/dayworktime.cpp           \
           src/tools/interval.cpp              \
           src/tools/monthworktime.cpp         \
           src/tools/network.cpp               \
           src/coreworktime.cpp

FORMS   += src/gui/worktimewindow.ui

#RESOURCES += resources/resource.qrc
#RC_FILE   += resources/application.rc

UI_DIR      = .autocreate/ui
MOC_DIR     = .autocreate/moc
OBJECTS_DIR = .autocreate/obj

QMAKE_LFLAGS_RELEASE += -static -static-libgcc
