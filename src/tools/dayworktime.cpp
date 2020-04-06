// --------------------------- //
#include <QDebug>
// --------------------------- //
#include "dayworktime.h"
// --------------------------- //

DayWorkTime::DayWorkTime( const QDate date, QObject *parent ) : QObject( parent )
{
    Date    = date;
    TypeDay = WorkDay;
}
// --------------------------------------------------------------------------------- //

QString DayWorkTime::intervalTitle( int id )
{
    if( id < IntervalsList.count() )
        return IntervalsList.at( id )->title();

    return "";
}
// --------------------------------------------------------------------------------- //

const QList<Interval*> & DayWorkTime::intervals()
{
    return IntervalsList;
}
// --------------------------------------------------------------------------------- //

void DayWorkTime::addInterval(const QString title)
{
    IntervalsList.append( new Interval( title, this ) );
}
// --------------------------------------------------------------------------------- //

void DayWorkTime::addInterval( Interval* Item )
{
	Item->setParent( this );
    IntervalsList.append( Item );
}
// --------------------------------------------------------------------------------- //

void DayWorkTime::removeInterval( const int interval )
{
    if( interval >= 0 && interval < IntervalsList.count() )
    {
        Interval* Item = IntervalsList.takeAt( interval );

        // TODO :: Проверить, точно ли удаляется объект
        delete Item;
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: DayWorkTime::removeInterval(" << interval << ") - interval not in range intervals list";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void DayWorkTime::renameInterval( const int interval, const QString newTitle )
{
    if( interval >= 0 && interval < IntervalsList.count() )
    {
        Interval* Item = IntervalsList[ interval ];

        Item->setTitle( newTitle );
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: DayWorkTime::renameInterval(" << interval << ") - interval not in range intervals list";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void DayWorkTime::setTimeStart( const WTime time, int interval )
{
    if( interval >= 0 && interval < IntervalsList.count() )
    {
        Interval* Item = IntervalsList[ interval ];

        if( Item != NULL )
        {
            Item->setTimeStart( time );
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: DayWorkTime::setTimeStart(" << time.toString() << ", " << interval << ") - interval is NULL";
#endif
        }
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: DayWorkTime::setTimeStart(" << time.toString() << ", " << interval << ") - interval not in range intervals list";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void DayWorkTime::setTimeEnd( const WTime time, int interval )
{
    if( interval >= 0 && interval < IntervalsList.count() )
    {
        Interval* Item = IntervalsList[ interval ];

        if( Item != NULL )
        {
            Item->setTimeEnd( time );
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: DayWorkTime::setTimeEnd(" << time.toString() << ", " << interval << ") - interval is NULL";
#endif
        }
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: DayWorkTime::setTimeEnd(" << time.toString() << ", " << interval << ") - interval not in range intervals list";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void DayWorkTime::setTimeNeed( const WTime time )
{
    TimeNeed = time;
}
// --------------------------------------------------------------------------------- //

void DayWorkTime::setTimeNeedSchedule ( const WTime time )
{
    TimeNeedSchedule = time;
}
// --------------------------------------------------------------------------------- //

void DayWorkTime::setTypeDay( TypesDay type )
{
    TypeDay = type;
}
// --------------------------------------------------------------------------------- //

QDate DayWorkTime::date()
{
    return Date;
}
// --------------------------------------------------------------------------------- //

WTime DayWorkTime::timeNeed()
{
    return TypeDay == WorkDay ? TimeNeed : WTime();
}
// --------------------------------------------------------------------------------- //

WTime DayWorkTime::timeNeedSchedule()
{
     return TypeDay == WorkDay ? TimeNeedSchedule : WTime();
}
// --------------------------------------------------------------------------------- //

WTime DayWorkTime::timeWorked()
{
    WTime TimeWorked;

    if( TypeDay == WorkDay )
    {
        foreach( Interval* Item, IntervalsList )
        {
            if( Item != NULL )
            {
                TimeWorked += Item->timeWorked();
            }
            else
            {
#ifdef WT_DEBUG
                qCritical() << " #Error :: DayWorkTime::timeWorked() - interval is NULL";
 #endif
            }
        }
    }

    return TimeWorked;
}
// --------------------------------------------------------------------------------- //

WTime DayWorkTime::timeWorked( int interval )
{
    WTime TimeWorked;

    if( TypeDay == WorkDay )
    {
        if( interval <= IntervalsList.count() - 1 )
        {
            TimeWorked = IntervalsList[interval]->timeWorked();
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: DayWorkTime::timeWorked(" << interval << ") interval not found";
#endif
        }
    }

    return TimeWorked;
}
// --------------------------------------------------------------------------------- //

WTime DayWorkTime::timeStart( const int interval )
{
    if( interval >= 0 && interval < IntervalsList.count() )
    {
        Interval* Item = IntervalsList[ interval ];

        if( Item != NULL )
        {
            return Item->timeStart();
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: DayWorkTime::timeStart(" << interval << ") - interval is NULL";
#endif
        }
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: DayWorkTime::timeStart(" << interval << ") - interval not in range intervals list";
#endif
    }

    return WTime();
}
// --------------------------------------------------------------------------------- //

WTime DayWorkTime::timeEnd( const int interval )
{
    if( interval >= 0 && interval < IntervalsList.count() )
    {
        Interval* Item = IntervalsList[ interval ];

        if( Item != NULL )
        {
            return Item->timeEnd();
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: DayWorkTime::timeEnd(" << interval << ") - interval is NULL";
#endif
        }
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: DayWorkTime::timeEnd(" << interval << ") - interval not in range intervals list";
#endif
    }

    return WTime();
}
// --------------------------------------------------------------------------------- //

int DayWorkTime::countIntervals() const
{
    return IntervalsList.count();
}
// --------------------------------------------------------------------------------- //

void DayWorkTime::changeNote( const QString note )
{
    Note = note;
}
// --------------------------------------------------------------------------------- //

QString DayWorkTime::note()
{
    return Note;
}
// --------------------------------------------------------------------------------- //

bool DayWorkTime::isWorkedDay()
{
    return TypeDay == WorkDay;
}
// --------------------------------------------------------------------------------- //

DayWorkTime::TypesDay DayWorkTime::typeDay()
{
    return TypeDay;
}
// --------------------------------------------------------------------------------- //

QStringList DayWorkTime::namesDay()
{
    static QStringList TypesDayList( QStringList() << nameDay( WorkDay    )
                                                   << nameDay( Weekend    )
                                                   << nameDay( Hospital   )
                                                   << nameDay( Secondment )
                                                   << nameDay( Vacation   ) );

    return TypesDayList;
}
// --------------------------------------------------------------------------------- //

QStringList DayWorkTime::titlesIntervals()
{
    QStringList Titles;

    foreach (Interval* Item, IntervalsList) { Titles << Item->title(); }

    return Titles;
}
// --------------------------------------------------------------------------------- //

QColor DayWorkTime::colorDay() const
{
    return colorDay( TypeDay );
}
// --------------------------------------------------------------------------------- //

QColor DayWorkTime::colorDay( TypesDay Type )
{
    switch( Type )
    {
        case WorkDay   : return QColor( "#A9A9A9" );
        case Weekend   : return QColor( "#E32636" );
        case Hospital  : return QColor( "#4682B4" );
        case Secondment: return QColor( "#66CDAA" );
        case Vacation  : return QColor( "#FFD700" );
        default        : return QColor( "#000000" );
    }

    return Qt::black;
}
// --------------------------------------------------------------------------------- //

QString DayWorkTime::nameDay( TypesDay Type )
{
    switch( Type )
    {
        case WorkDay   : return tr("Рабочий"     );
        case Weekend   : return tr("Выходной"    );
        case Hospital  : return tr("Больничный"  );
        case Secondment: return tr("Командировка");
        case Vacation  : return tr("Отпуск"      );
        default        : return ""                ;
    }

    return "";
}
// --------------------------------------------------------------------------------- //

