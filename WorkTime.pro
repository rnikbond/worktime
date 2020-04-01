QT += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkTime
TEMPLATE = app

#DEFINES += WT_DEBUG
#DEFINES += WT_INFO_CALL_FUNC

INCLUDEPATH += src
INCLUDEPATH += src/view
INCLUDEPATH += src/tools

HEADERS += src/view/worktimewindow.h \
           src/view/iworktime.h \
           src/tools/helperwt.h

SOURCES += src/main.cpp \
           src/view/worktimewindow.cpp \
           src/tools/helperwt.cpp

FORMS   += src/gui/worktimewindow.ui

#RESOURCES += resources/resource.qrc
#RC_FILE   += resources/application.rc

UI_DIR      = .autocreate/ui
MOC_DIR     = .autocreate/moc
OBJECTS_DIR = .autocreate/obj

QMAKE_LFLAGS_RELEASE += -static -static-libgcc
