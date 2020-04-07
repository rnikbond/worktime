// ---------------------------- //
#ifndef MODELWORKTIME_H
#define MODELWORKTIME_H
// ---------------------------- //
#include <QObject>
// ---------------------------- //
#include "databasewt.h"
#include "monthworktime.h"
#include "imodelworktime.h"
// ---------------------------- //

class ModelWorkTime : public QObject, public IModelWorkTime
{
    Q_OBJECT

    QDate selectedDate;

    DayWorkTime   * Today;

    DataBaseWT    * DataBase ;
    MonthWorkTime * WorkMonth;

    int workingRate;

    QTimer * WorkTimer;

public:

    explicit ModelWorkTime( QObject * parent = 0 );

private:

    void reload();
    bool isLoadedMonth( const QDate & date );

    void checkTimeStart();
    void checkLaunch();

    bool isCorrectIntervals();

    WTime timeEscape();

    void updateTimeEscape();


public: // From Interfaces

    void setDataBase( DataBaseWT * DB );

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

private slots:

    void updateTimeEnd();
    void workTimerTick();

signals: // Interface signals

    void enabledWait( bool );

    void reloadMonth();
    void reloadWeek ();
    void reloadDay  ();

    void recolorDay( const QDate & date, const QColor & color );

    void updateEscape( QString info );

    void updateReverseTimer( QString time );
};

#endif // MODELWORKTIME_H
