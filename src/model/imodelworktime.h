// ---------------------------- //
#ifndef IMODELWORKTIME
#define IMODELWORKTIME
// ---------------------------- //
#include <QDate>
#include <QColor>
// ---------------------------- //
#include "wtime.h"
// ---------------------------- //

class IModelWorkTime
{

public:

    virtual void setWorkingRate( int rate ) = 0;
    virtual void setDate( const QDate & date ) = 0;

    virtual int         typeDay()        = 0;
    virtual QStringList intervals()      = 0;
    virtual QString     note()           = 0;
    virtual QStringList colorMonthDays() = 0;

    virtual QString infoStatisticInDay() = 0;
    virtual WTime   timeStatisticInDay() = 0;
    virtual WTime   timeWorkedInDay()    = 0;
    virtual WTime   timeNeedInDay()      = 0;

    virtual QString infoStatisticToDay() = 0;
    virtual WTime   timeStatisticToDay() = 0;
    virtual WTime   timeWorkedToDay()    = 0;
    virtual WTime   timeNeedToDay()      = 0;

    virtual QString infoStatisticInWeek() = 0;
    virtual WTime   timeStatisticInWeek() = 0;
    virtual WTime   timeWorkedInWeek()    = 0;
    virtual WTime   timeNeedInWeek()      = 0;

    virtual QString infoStatisticInMonth() = 0;
    virtual WTime   timeStatisticInMonth() = 0;
    virtual WTime   timeWorkedInMonth()    = 0;
    virtual WTime   timeNeedInMonth()      = 0;

    virtual WTime timeStart( const int id ) = 0;
    virtual WTime timeEnd  ( const int id ) = 0;

    virtual void addInterval  ( const QString & title ) = 0;
    virtual void removeInterval( const int id ) = 0;
    virtual void renameInterval( const int id, const QString & title ) = 0;

    virtual void setTimeStart( const int id, const WTime & time ) = 0;
    virtual void setTimeEnd  ( const int id, const WTime & time ) = 0;
    virtual void setTimeNeed ( const WTime & time ) = 0;

    virtual void setTypeDay( int type ) = 0;

    virtual void setNote( QString note ) = 0;

public: // Signals

    virtual void enabledWait( bool ) = 0;
};

#endif // IMODELWORKTIME

