QT += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorkTime
TEMPLATE = app

#DEFINES += WT_INFO_CALL_FUNC

INCLUDEPATH += src
INCLUDEPATH += src/model
INCLUDEPATH += src/view
INCLUDEPATH += src/presenter
INCLUDEPATH += src/interface
INCLUDEPATH += src/tools
INCLUDEPATH += src/tools/time
INCLUDEPATH += src/model/database

HEADERS += src/model/imodelworktime.h          \
           src/model/database/databasewt.h     \
           src/model/database/tablesdatabase.h \
           src/model/modelworktime.h           \
           src/view/iviewworktime.h            \
           src/view/worktimewindow.h           \
           src/presenter/presenterworktime.h   \
           src/tools/helperwt.h                \
           src/tools/time/wtime.h              \
           src/tools/time/wtimeext.h           \
           src/tools/dayworktime.h             \
           src/tools/interval.h                \
           src/tools/monthworktime.h           \
           src/tools/network.h                 \
           src/coreworktime.h                  \
           src/view/settingswindow.h           \
           src/view/iviewsettings.h            \
           src/interface/ilog.h                \
           src/view/desktopwidget.h            \
           src/interface/idesktopwidget.h      \
           src/view/tabletimewindow.h          \
           src/view/severaldayswindow.h        \
           src/view/salarywindow.h             \
           src/view/changeswindow.h            \
           src/tools/qzipreader_p.h            \
           src/tools/qzipwriter_p.h


SOURCES += src/main.cpp                          \
           src/model/modelworktime.cpp           \
           src/model/database/databasewt.cpp     \
           src/model/database/tablesdatabase.cpp \
           src/view/worktimewindow.cpp           \
           src/presenter/presenterworktime.cpp   \
           src/tools/helperwt.cpp                \
           src/tools/time/wtime.cpp              \
           src/tools/time/wtimeext.cpp           \
           src/tools/dayworktime.cpp             \
           src/tools/interval.cpp                \
           src/tools/monthworktime.cpp           \
           src/tools/network.cpp                 \
           src/coreworktime.cpp                  \
           src/view/settingswindow.cpp           \
           src/view/desktopwidget.cpp            \
           src/view/tabletimewindow.cpp          \
           src/view/severaldayswindow.cpp        \
           src/view/salarywindow.cpp             \
           src/view/changeswindow.cpp            \
           src/tools/qzip.cpp

FORMS   += src/gui/worktimewindow.ui            \
           src/model/database/tablesdatabase.ui \
           src/view/settingswindow.ui           \
           src/view/desktopwidget.ui            \
           src/view/tabletimewindow.ui          \
           src/view/severaldayswindow.ui        \
           src/view/salarywindow.ui             \
           src/view/changeswindow.ui

RESOURCES += resources/resource.qrc
RC_FILE   += resources/application.rc

UI_DIR      = .autocreate/ui
MOC_DIR     = .autocreate/moc
OBJECTS_DIR = .autocreate/obj

QMAKE_LFLAGS_RELEASE += -static -static-libgcc
