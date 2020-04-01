QT += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkTime
TEMPLATE = app

#DEFINES += WT_DEBUG
#DEFINES += WT_INFO_CALL_FUNC

INCLUDEPATH += src

HEADERS += 
SOURCES += src/main.cpp
FORMS   += 

#RESOURCES += resources/resource.qrc
#RC_FILE   += resources/application.rc

UI_DIR      = .autocreate/ui
MOC_DIR     = .autocreate/moc
OBJECTS_DIR = .autocreate/obj

QMAKE_LFLAGS_RELEASE += -static -static-libgcc
