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

    DataBaseWT    * DataBase ;
    MonthWorkTime * WorkMonth;
    MonthWorkTime * CurrentMonth;
    MonthWorkTime * SelectedMonth;

    int workingRate;

    QTimer * WorkTimer;

    WTime LaunchLengthTime;
    WTime LaunchStartTime ;
    WTime LaunchEndTime   ;
    WTime MaxTime         ;
    WTime BeforeTime      ;
    WTime AfterTime       ;

    bool isTickTimer;

public:

    explicit ModelWorkTime( QObject * parent = 0 );
    ~ModelWorkTime();

    void update();

private:

    void reload();
    void load( MonthWorkTime * Month );

    void checkTimeStart();
    void checkLaunch();

    WTime timeEscape();

    void updateTimeEnd();
    void updateTimeEscape();

    bool isEqualMonth( const QDate & date1, const QDate & date2 );
    bool isCorrectIntervals();

private slots:

    void workTimerTick();

public: // From Interfaces

    void setDataBase( DataBaseWT * DB );

    void setWorkingRate( int rate );
    void setDate( const QDate & date );

    void setTimeLaunchStart ( WTime time );
    void setTimeLaunchEnd   ( WTime time );
    void setTimeLaunchLength( WTime time );
    void setTimeMax         ( WTime time );
    void setTimeBefore      ( WTime time );
    void setTimeAfter       ( WTime time );

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

    WTime todayTimeEscape();

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

    void setTimerState( bool state, bool isNewInterval );

signals: // Interface signals

    void enabledWait( bool );

    void reloadMonth();
    void reloadWeek ();
    void reloadDay  ();

    void reloadStatisticTime();
    void refreshTimeStart( int id, WTime time );
    void refreshTimeEnd  ( int id, WTime time );

    void recolorDay( const QDate & date, const QColor & color );

    void updateEscape( QString info );

    void updateReverseTimer( QString time );
};

#endif // MODELWORKTIME_H
