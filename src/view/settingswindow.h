// ---------------------------- //
#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H
// ---------------------------- //
#include <QWidget>
// ---------------------------- //
#include "iviewsettings.h"
// ---------------------------- //
namespace Ui { class SettingsWindow; }
// ---------------------------- //

class SettingsWindow : public QWidget, public IViewSettings
{
    Q_OBJECT

    int workingRate;

public:

    explicit SettingsWindow( QWidget * parent = 0 );
    ~SettingsWindow();

public: // From Interfaces

    void setWorkingRate  ( int         rate       );
    void setWorkingRates ( QStringList ratesList  );
    void setTheme        ( int         theme      );
    void setThemes       ( QStringList themesList );
    void setOpacityValue ( int         value      );
    void setAutorun      ( bool        isSet      );
    void setTray         ( bool        isSet      );
    void setContextMenu  ( bool        isSet      );
    void setUpdates      ( bool        isSet      );
    void setUpdatesPath  ( QString     path       );
    void setLaunchStart  ( QTime       time       );
    void setLaunchEnd    ( QTime       time       );
    void setLaunchTime   ( QTime       time       );
    void setMaxTime      ( QTime       time       );
    void setBeforeTime   ( QTime       time       );
    void setAfterTime    ( QTime       time       );
    void setViewWidget   ( bool        isSet      );
    void setTopWidget    ( bool        isSet      );
    void setOpacityWidget( int         value      );

private:

    Ui::SettingsWindow * gui;

    void connectGUI();
    void configureGUI();

    void updateTimeEnabled();
    void updateMenuEnabled();

protected:

    void showEvent ( QShowEvent  * ShowEvent  );
    void closeEvent( QCloseEvent * CloseEvent );

private slots:

    void selectMenuItem( int item );

    // ---- PAGES ---- //

    // Working rate PAGE
    void selectWorkingRate( int rate );
    void currentWorkingRate();
    void saveWorkingRate();

    // View PAGE
    void changeThemeCBox    ( int theme );
    void changeOpacitySlider( int value );

    // Auto PAGE
    void checkedAutorun    ();
    void checkedTray       ();
    void checkedContextMenu();
    void checkedUpdates    ();
    void clickSelectPath   ();
    void clickResetPath    ();

    // Time PAGE
    void changeLaunchStart( QTime time );
    void changeLaunchEnd  ( QTime time );
    void changeLaunchTime ( QTime time );
    void changeMaxTime    ( QTime time );
    void changeBeforeTime ( QTime time );
    void changeAfterTime  ( QTime time );

    // Widget PAGE
    void checkedViewWidget  ();
    void checkedTopWidget   ();
    void changeOpacityWidget( int value );

signals: // From Interfaces

    void changedWorkingRate  ( int     );
    void changedTheme        ( int     );
    void changedOpacityValue ( int     );
    void changedAutorun      ( bool    );
    void changedTray         ( bool    );
    void changedContextMenu  ( bool    );
    void changedUpdates      ( bool    );
    void changedUpdatePath   ( QString );
    void changedLaunchStart  ( QTime   );
    void changedLaunchEnd    ( QTime   );
    void changedLaunchTime   ( QTime   );
    void changedMaxTime      ( QTime   );
    void changedBeforeTime   ( QTime   );
    void changedAfterTime    ( QTime   );
    void changedViewWidget   ( bool    );
    void changedTopWidget    ( bool    );
    void changedOpacityWidget( int     );

    void resetUpdatePath();

    void closeApp();
};

#endif // SETTINGSWINDOW_H
