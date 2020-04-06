// --------------------------- //
#ifndef DAYWORKTIME_H
#define DAYWORKTIME_H
// --------------------------- //
#include <QObject>
#include <QColor>
// --------------------------- //
#include "wtime.h"
#include "interval.h"
// --------------------------- //

class DayWorkTime : public QObject
{
    Q_OBJECT

public:

    enum TypesDay
    {
        UnknownDay = -1,
        WorkDay    =  0,
        Weekend    =  1,
        Hospital   =  2,
        Secondment =  3,
        Vacation   =  4
    };

    static QStringList namesDay();
    static QString nameDay( TypesDay Type );
    static QColor colorDay( TypesDay Type );

protected:

    QList<Interval*> IntervalsList;
    WTime TimeNeed;
    WTime TimeNeedSchedule;

    TypesDay TypeDay;

    QDate Date;

    QString Note;

public:

    explicit DayWorkTime( const QDate date, QObject *parent = NULL );

    QString intervalTitle( int id );
    const QList<Interval*> & intervals();

    void setTypeDay( TypesDay type );
    void setTimeStart( const WTime time, int interval );
    void setTimeEnd  ( const WTime time, int interval );
    void setTimeNeed ( const WTime time );
    void setTimeNeedSchedule ( const WTime time );

    void addInterval( const QString title );
    void addInterval( Interval* Item );
    void removeInterval( const int interval );
    void renameInterval( const int interval, const QString newTitle );
    int  countIntervals() const;

    void changeNote(const QString note );
    QString note();

    bool isWorkedDay();
    TypesDay typeDay();

    QStringList titlesIntervals();

    QColor colorDay() const;

    QDate date();

    WTime timeNeed();
    WTime timeNeedSchedule();
    WTime timeWorked();
    WTime timeWorked(int interval);
    WTime timeStart( const int interval );
    WTime timeEnd  ( const int interval );
};

#endif // DAYWORKTIME_H
