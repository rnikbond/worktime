// ---------------------------- //
#ifndef COREWORKTIME_H
#define COREWORKTIME_H
// ---------------------------- //
#include <QLabel>
#include <QMovie>
#include <QObject>
// ---------------------------- //
#include "databasewt.h"
#include "modelworktime.h"
#include "worktimewindow.h"
#include "settingswindow.h"
#include "tablesdatabase.h"
#include "presenterworktime.h"
// ---------------------------- //

class CoreWorkTime : public QObject
{
    Q_OBJECT

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
    QMovie * WaitMovie;

    QThread           * DataBaseThread;
    DataBaseWT        * DataBase;

    ModelWorkTime     * ModelWT;
    WorkTimeWindow    * WorkTime;
    PresenterWorkTime * PresenterWT;

    SettingsWindow    * Settings;

    TablesDataBase * TablesWindow;

public:

    explicit CoreWorkTime( QObject * parent = 0 );
    ~CoreWorkTime();

    void start();

private:

    void connectWorkTime();
    void connectSettings();
    void connectModel();

    void createLoaded();

    void createObjects();

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

    void showSettings();

    void closeApp();

    void infoLog ( const QString & logText );
    void errorLog( const QString & logText );
};

#endif // COREWORKTIME_H
