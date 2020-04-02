// ---------------------------- //
#ifndef MODELWORKTIME_H
#define MODELWORKTIME_H
// ---------------------------- //
#include <QObject>
// ---------------------------- //
#include "databasewt.h"
#include "imodelworktime.h"
// ---------------------------- //

class ModelWorkTime : public QObject, public IModelWorkTime
{
    Q_OBJECT

    QDate selectedDate;

    DataBaseWT * DataBase;

    int workingRate;

public:

    explicit ModelWorkTime( QObject * parent = 0 );

private:

    void reload();

public: // From Interfaces

    void setWorkingRate( int rate );
    void setDate( const QDate & date );

    int         typeDay();
    QStringList intervals();
    QString     note();
    QStringList colorMonthDays();

    QString infoStatisticInDay();
    WTime   timeStatisticInDay();
    WTime   timeWorkedInDay();
    WTime   timeNeedInDay();

    QString infoStatisticToDay();
    WTime   timeStatisticToDay();
    WTime   timeWorkedToDay();
    WTime   timeNeedToDay();

    QString infoStatisticInWeek();
    WTime   timeStatisticInWeek();
    WTime   timeWorkedInWeek();
    WTime   timeNeedInWeek();

    QString infoStatisticInMonth();
    WTime   timeStatisticInMonth();
    WTime   timeWorkedInMonth();
    WTime   timeNeedInMonth();

    WTime timeStart( const int id );
    WTime timeEnd  ( const int id );

    void addInterval  ( const QString & title );
    void removeInterval( const int id );
    void renameInterval( const int id, const QString & title );

    void setTimeStart( const int id, const WTime & time );
    void setTimeEnd  ( const int id, const WTime & time );
    void setTimeNeed ( const WTime & time );

    void setTypeDay( int type );

    void setNote( QString note );

signals: // Interface signals

    void enabledWait( bool );
};

#endif // MODELWORKTIME_H
