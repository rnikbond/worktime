// ---------------------------- //
#ifndef IVIEWSETTINGS
#define IVIEWSETTINGS
// ---------------------------- //
class QStringList;
// ---------------------------- //

class IViewSettings
{

public: // Methods

    virtual void setWorkingRate( int rate ) = 0;
    virtual void setWorkingRates( QStringList ratesList ) = 0;

public: // Signals

    virtual void changedWorkingRate( int ) = 0;

};

#endif // IVIEWSETTINGS

