// ---------------------------- //
#ifndef IVIEWSETTINGS
#define IVIEWSETTINGS
// ---------------------------- //
#include <QTime>
#include <QStringList>
// ---------------------------- //

class IViewSettings
{

public: // Methods

    virtual void setWorkingRate     ( int         rate       ) = 0;
    virtual void setWorkingRates    ( QStringList ratesList  ) = 0;
    virtual void setTheme           ( int         theme      ) = 0;
    virtual void setThemes          ( QStringList themesList ) = 0;
    virtual void setOpacityValue    ( int         value      ) = 0;
    virtual void setAutorun         ( bool        isSet      ) = 0;
    virtual void setTray            ( bool        isSet      ) = 0;
    virtual void setContextMenu     ( bool        isSet      ) = 0;
    virtual void setUpdates         ( bool        isSet      ) = 0;
    virtual void setUpdatesPath     ( QString     path       ) = 0;
    virtual void setLaunchStart     ( QTime       time       ) = 0;
    virtual void setLaunchEnd       ( QTime       time       ) = 0;
    virtual void setLaunchTime      ( QTime       time       ) = 0;
    virtual void setMaxTime         ( QTime       time       ) = 0;
    virtual void setBeforeTime      ( QTime       time       ) = 0;
    virtual void setAfterTime       ( QTime       time       ) = 0;
    virtual void setNotifyEndWorkDay( bool        isSet      ) = 0;
    virtual void setViewWidget      ( bool        isSet      ) = 0;
    virtual void setTopWidget       ( bool        isSet      ) = 0;
    virtual void setOpacityWidget   ( int         value      ) = 0;
    virtual void setOpacityText     ( int         value      ) = 0;

public: // Signals

    virtual void changedWorkingRate     ( int     ) = 0;
    virtual void changedTheme           ( int     ) = 0;
    virtual void changedOpacityValue    ( int     ) = 0;
    virtual void changedAutorun         ( bool    ) = 0;
    virtual void changedTray            ( bool    ) = 0;
    virtual void changedContextMenu     ( bool    ) = 0;
    virtual void changedUpdates         ( bool    ) = 0;
    virtual void changedUpdatePath      ( QString ) = 0;
    virtual void changedLaunchStart     ( QTime   ) = 0;
    virtual void changedLaunchEnd       ( QTime   ) = 0;
    virtual void changedLaunchTime      ( QTime   ) = 0;
    virtual void changedMaxTime         ( QTime   ) = 0;
    virtual void changedBeforeTime      ( QTime   ) = 0;
    virtual void changedAfterTime       ( QTime   ) = 0;
    virtual void changedNotifyEndWorkDay( bool    ) = 0;
    virtual void changedViewWidget      ( bool    ) = 0;
    virtual void changedTopWidget       ( bool    ) = 0;
    virtual void resetPositionWidget    (         ) = 0;
    virtual void changedOpacityWidget   ( int     ) = 0;
    virtual void changedOpacityText     ( int     ) = 0;

    virtual void resetUpdatePath() = 0;

    virtual void closeApp() = 0;
};

#endif // IVIEWSETTINGS

