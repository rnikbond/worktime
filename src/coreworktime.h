// ---------------------------- //
#ifndef COREWORKTIME_H
#define COREWORKTIME_H
// ---------------------------- //
#include <QLabel>
#include <QMovie>
#include <QObject>
#include <QSystemTrayIcon>
// ---------------------------- //
#include "databasewt.h"
#include "salarywindow.h"
#include "desktopwidget.h"
#include "modelworktime.h"
#include "changeswindow.h"
#include "worktimewindow.h"
#include "settingswindow.h"
#include "tablesdatabase.h"
#include "tabletimewindow.h"
#include "severaldayswindow.h"
#include "presenterworktime.h"
// ---------------------------- //
#define VERSION_MAJOR    1
#define VERSION_MINOR    1
#define VERSION_SUBMINOR 1
// ---------------------------- //
class CoreWorkTime : public QObject
{
    Q_OBJECT

    int VersionMajor   ;
    int VersionMinor   ;
    int VersionSubminor;

    int workingRate;

    int themeIndex;
    int opacityValue;
    bool isAutorun;
    bool isTray;
    bool isContextMenu;
    bool isCheckUpdates;
    QString updatePath;
    QTime LaunchStartTime;
    QTime LaunchEndTime;
    QTime LaunchLengthTime;
    QTime MaxTime;
    QTime BeforeTime;
    QTime AfterTime;
    bool isViewWidget;
    bool isTopWidget;
    int opacityWidget;
    QRect WorkTimeGeometry;
    QRect WidgetDesktopGeometry;
    bool isShownMenu;
    int selectedPage;

    QLabel * WaitLabel;

    QThread           * DataBaseThread;
    DataBaseWT        * DataBase;

    ModelWorkTime     * ModelWT;
    WorkTimeWindow    * WorkTime;
    PresenterWorkTime * PresenterWT;

    SettingsWindow    * Settings;
    SalaryWindow      * SalaryWidget;
    DesktopWidget     * DesktopWindow;
    TableTimeWindow   * TableTimeWidget;
    SeveralDaysWindow * SeveralDaysWidget;
    ChangesWindow     * ChangesWidget;

    TablesDataBase * TablesWindow;

    QSystemTrayIcon * TrayWorkTime;

public:

    explicit CoreWorkTime( QObject * parent = 0 );
    ~CoreWorkTime();

    void start();

private:

    void connectSalary();
    void connectWidget();
    void connectWorkTime();
    void connectSettings();
    void connectSeveralDays();
    void connectModel();

    void initialize();
    void initializeWidget();

    void updateSettings();

    void createLoaded();

    void createObjects();

    void createTray();

    void removeOld();
    bool isAlreadyRunning();

    void readConfig();
    void writeConfig();

    void normalizeGeometryWorkTime();

    void showWindow( QWidget * Window, bool isCenterDisplay );

    QStringList themePathes();
    QStringList themeNames();

    void applyTheme( int theme );

    float toOpacity( int value );

private slots:

    void wait(bool isWait );

    // Slots from WorkTime window
    void closeWorkTimeWindow();
    void showWorkTime();
    void switchVisibleWorkTime();
    void changeVisibleMenu( bool isShown );
    void changePage( int page );
    void changeGeometryWidget( QRect geometry );
    void changeGeometryWorkTime( QRect geometry );

    // Slots from Settings window
    void changedWorkingRate  ( int     );
    void changeTheme        ( int     );
    void changeOpacityValue ( int     );
    void changeAutorun      ( bool    );
    void changeTray         ( bool    );
    void changeContextMenu  ( bool    );
    void changeUpdates      ( bool    );
    void changeUpdatePath   ( QString );
    void changeLaunchStart  ( QTime   );
    void changeLaunchEnd    ( QTime   );
    void changeLaunchTime   ( QTime   );
    void changeMaxTime      ( QTime   );
    void changeBeforeTime   ( QTime   );
    void changeAfterTime    ( QTime   );
    void changeViewWidget   ( bool    );
    void changeTopWidget    ( bool    );
    void changeOpacityWidget( int     );

    void resetUpdatePath();

    void showSalary();
    void showSettings();
    void showTableTime();
    void showSeveralDays();
    void showChanges();

    void reloadAll();

    void closeApp();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void infoLog ( const QString & logText );
    void errorLog( const QString & logText );
};

#endif // COREWORKTIME_H
