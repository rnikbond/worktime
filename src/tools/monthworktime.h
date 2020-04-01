// --------------------------- //
#ifndef MONTHWORKTIME_H
#define MONTHWORKTIME_H
// --------------------------- //
#include <QObject>
// --------------------------- //
#include "dayworktime.h"
// --------------------------- //

class MonthWorkTime : public QObject
{
    Q_OBJECT

    QStringList WorkingRatesList;

    QList<DayWorkTime*> DaysList;

    int updatedWeekNeed;
    int updatedWeekWorked;

    WTime NeedInWeekTime;
    WTime WorkedInWeekTime;

    WTime NeedInMonthTime;
    WTime WorkedInMonthTime;

    int WorkingTimeID;

public:

    explicit MonthWorkTime( QObject *parent = NULL );

    void clear( bool isFull = false );
    void fullUpdate( const QDate & Date );

    int countDays();

    bool isThisMonth( const QDate& date );

    void setTimeStart( const QDate& date, const int interval, const QTime& time, bool isUpdate );
    void setTimeEnd  ( const QDate& date, const int interval, const QTime& time,  bool isUpdate );
    void setTimeNeed ( const QDate& date, const QTime& time, bool isUpdate );

    void addDay( const QDate date );
    void addDay( DayWorkTime* Day );
    void setTypeDay( const QDate date, const DayWorkTime::TypesDay type );

    void addInterval( const QDate& date, const QString& title );
    void removeInterval(const QDate& date, const int interval );
    void renameInterval(const QDate& date, const int interval, const QString& newTitle );

    void setNote( const QDate& date, const QString& note );
    QString note( const QDate& date );

    void changeTypeDay( const QDate& date, DayWorkTime::TypesDay type );
    DayWorkTime::TypesDay typeDay( const QDate& date );
    QColor colorDay( const QDate& date );

    int countIntervals( const QDate& date );
    int lastInterval( const QDate& date );

    QStringList titlesIntervals( const QDate& date );

    WTime timeStart( const QDate& date, const int interval);
    WTime timeEnd  ( const QDate& date, const int interval);

    WTime timeNeedInDay    ( const QDate& date );
    WTime timeWorkedInDay  ( const QDate& date );

    WTime timeNeedInWeek    ( const QDate& date );
    WTime timeWorkedInWeek  ( const QDate& date );

    WTime timeNeedInMonth    ();
    WTime timeWorkedInMonth  ();

    WTime timeNeedToDay    ( const QDate& date );
    WTime timeWorkedToDay  ( const QDate& date );

    DayWorkTime* dayAtDate( const QDate& date );

    void updateTimeNeedInWeek   ( const QDate& date );
    void updateTimeWorkedInWeek ( const QDate& date );
    void updateTimeNeedInMonth  ( const QDate& date );
    void updateTimeWorkedInMonth( const QDate& date );

};

#endif // MONTHWORKTIME_H
