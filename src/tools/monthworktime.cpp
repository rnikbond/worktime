// --------------------------- //
#include <QDebug>
// --------------------------- //
#include "helperwt.h"
// --------------------------- //
#include "monthworktime.h"
// --------------------------- //

MonthWorkTime::MonthWorkTime( QObject *parent) : QObject( parent )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::MonthWorkTime()";
#endif
    updatedWeekNeed   = 0;
    updatedWeekWorked = 0;
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::clear()
{    
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::clear()";
#endif

    int countDays = DaysList.count();

    for( int day = 0; day < countDays; day++ )
        delete DaysList.takeAt(0);

    DaysList.clear();
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::fullUpdate( const QDate & Date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::fullUpdate(" << Date << ")";
#endif

    updateTimeNeedInMonth  ( Date );
    updateTimeNeedInWeek   ( Date );
    updateTimeWorkedInMonth( Date );
    updateTimeWorkedInWeek ( Date );
}
// --------------------------------------------------------------------------------- //

int MonthWorkTime::countDays()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::countDays()";
#endif

    return DaysList.count();
}
// --------------------------------------------------------------------------------- //

int MonthWorkTime::year()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::year()";
#endif

    if( DaysList.at(0) != NULL )
        return DaysList.at(0)->date().year();

    return 0;
}
// --------------------------------------------------------------------------------- //

int MonthWorkTime::month()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::month()";
#endif

    if( DaysList.at(0) != NULL )
        return DaysList.at(0)->date().month();

    return 0;
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::setTypeData( const QDate& date, DayWorkTime::TypesData type )
{
    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Day->setTypeData( type );
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::setTypeData("
                    << date << ", " << type << ") - day not found";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::setTimeStart( const QDate& date, const int interval, const QTime& time,  bool isUpdate )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::setTimeStart(" << date << ", " << time << ", " << interval << ", " << isUpdate << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Day->setTimeStart( time, interval );

        if( isUpdate )
        {
            updateTimeWorkedInWeek ( date );
            updateTimeWorkedInMonth( date );
        }
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::setTimeStart("
                    << date << ", " << time << ", " << interval << ", " << isUpdate << ") - day not found";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::setTimeEnd( const QDate& date, const int interval, const QTime& time, bool isUpdate )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::setTimeEnd(" << date << ", " << interval << ", " << time  << ", " << isUpdate << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Day->setTimeEnd( time, interval );

        if( isUpdate )
        {
            updateTimeWorkedInWeek ( date );
            updateTimeWorkedInMonth( date );
        }
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::setTimeEnd("
                    << date << ", " << time << ", " << interval << ", " << isUpdate << ") - day not found";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::setTimeNeed( const QDate& date, const QTime& time, bool isUpdate )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::setTimeNeed(" << date << ", " << time << ", " << isUpdate << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Day->setTimeNeed( time );

        if( isUpdate )
        {
            updateTimeNeedInWeek ( date );
            updateTimeNeedInMonth( date );
        }
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::setTimeNeed(" << date << ", " << time << ", " << isUpdate << ") - day not found";
#endif
    }
}
// --------------------------------------------------------------------------------- //

int MonthWorkTime::countIntervals( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::countIntervals(" << date << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        return Day->countIntervals();
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::countIntervals(" << date << ") - day not found";
#endif
    }

    return 0;
}
// --------------------------------------------------------------------------------- //

int MonthWorkTime::lastInterval( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::lastInterval(" << date << ")";
#endif

    return ( countIntervals(date) - 1 );
}
// --------------------------------------------------------------------------------- //

QString MonthWorkTime::titleInterval( const QDate& date, const int interval )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::titleInterval(" << date << ", " << interval << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        return Day->titleInterval( interval );
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::titleInterval(" << date << ", " << interval << ") - day not found";
#endif
    }

    return "";
}
// --------------------------------------------------------------------------------- //

DayWorkTime::TypesData MonthWorkTime::typeData( const QDate& date )
{
    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        return Day->typeData();
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::typeData(" << date << ") - day not found";
#endif
    }

    return DayWorkTime::ScheduleData;
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeStart( const QDate& date, const int interval)
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeStart(" << date << ", " << interval << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        // TODO :: Тут падало, если сначала ставили выходной день, а затем начинали работать в этот день.
        // После этого прога начинала ставить время начала - он устанавливалось,
        // Затем была проверка времени. Проверка запрашивала у дня время начала, и если оно нулевое, то
        // заново шла установка времени. Т.к. по условию время возвращалось только если день - рабочий,
        // то из-за этого условия в выходной день возвращалось нулевое время и так по кругу.
//        if( Day->typeDay() == DayWorkTime::WorkDay || Day->typeDay() == DayWorkTime::Secondment )
//            return Day->timeStart( interval );
//        else
//            return WTime();

        return Day->timeStart( interval );
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: timeStart::timeStart(" << date << ", " << interval << ") - day not found";
#endif
    }

    return WTime();
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeEnd( const QDate& date, const int interval )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeEnd(" << date << ", " << interval << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        if( Day->typeDay() == DayWorkTime::WorkDay || Day->typeDay() == DayWorkTime::Secondment )
            return Day->timeEnd( interval );
        else
            return WTime();
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::timeEnd(" << date << ", " << interval << ") - day not found";
#endif
    }

    return WTime();
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeNeedInDay( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeNeedInDay(" << date << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        if( Day->typeDay() == DayWorkTime::WorkDay || Day->typeDay() == DayWorkTime::Secondment )
            return Day->timeNeed();
        else
            return WTime();
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::timeNeedInDay(" << date << ") - day not found";
#endif
    }

    return WTime();
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeNeedScheduleInDay( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeNeedScheduleInDay(" << date << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        return Day->timeNeedSchedule();
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::timeNeedScheduleInDay(" << date << ") - day not found";
#endif
    }

    return WTime();
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeWorkedInDay( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeWorkedInDay(" << date << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        if( Day->typeDay() == DayWorkTime::WorkDay || Day->typeDay() == DayWorkTime::Secondment )
            return Day->timeWorked();
        else
            return WTime();
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::timeWorkedInDay(" << date << ") - day not found";
#endif
    }

    return WTime();
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeNeedInWeek( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeNeedInWeek(" << date << ")";
#endif

    if( date.weekNumber() != updatedWeekNeed )
        updateTimeNeedInWeek( date );

    return NeedInWeekTime;
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeWorkedInWeek( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeWorkedInWeek(" << date << ")";
#endif

    if( date.weekNumber() != updatedWeekWorked )
        updateTimeWorkedInWeek( date );

    return WorkedInWeekTime;
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeNeedInMonth()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeNeedInMonth()";
#endif

    return NeedInMonthTime;
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeWorkedInMonth()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeWorkedInMonth()";
#endif

    return WorkedInMonthTime;
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeNeedToDay( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeNeedToDay(" << date << ")";
#endif

    WTime Time;

    for( int day = 1; day <= date.day(); day++ )
    {
        QDate ToDate( date.year(), date.month(), day );

        DayWorkTime* Day = dayAtDate( ToDate );

        if( Day != NULL )
        {
            Time += Day->timeNeed();
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: MonthWorkTime::timeNeedToDay(" << date << ") - Date " << ToDate << " not found";
#endif
        }
    }

    return Time;
}
// --------------------------------------------------------------------------------- //

WTime MonthWorkTime::timeWorkedToDay( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::timeWorkedToDay(" << date << ")";
#endif

    WTime Time;

    for( int day = 1; day <= date.day(); day++ )
    {
        QDate ToDate( date.year(), date.month(), day );

        DayWorkTime* Day = dayAtDate( ToDate );

        if( Day != NULL )
        {
            if( Day->date().day() <= date.day() )
            {
                Time += Day->timeWorked();
            }
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: MonthWorkTime::timeWorkedToDay(" << date << ") - Date " << ToDate << " not found";
#endif
        }
    }

    return Time;
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::updateTimeNeedInWeek( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::updateTimeNeedInWeek(" << date << ")";
#endif

    NeedInWeekTime.clear();

    for( int day = 1; day <= date.daysInMonth(); day++ )
    {
        QDate WeekDate( date.year(), date.month(), day );

        DayWorkTime* Day = dayAtDate( WeekDate );

        if( Day != NULL )
        {
            if( Day->date().weekNumber() ==  date.weekNumber() )
                NeedInWeekTime += Day->timeNeed();
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: MonthWorkTime::updateTimeNeedInWeek(" << date << ") - Date " << WeekDate << " not found";
#endif
        }
    }

    updatedWeekNeed = date.weekNumber();
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::updateTimeWorkedInWeek( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::updateTimeWorkedInWeek(" << date << ")";
#endif

    WorkedInWeekTime.clear();

    for( int day = 1; day <= date.daysInMonth(); day++ )
    {
        QDate WeekDate( date.year(), date.month(), day );

        DayWorkTime* Day = dayAtDate( WeekDate );

        if( Day != NULL )
        {
            if( Day->date().weekNumber() ==  date.weekNumber() )
                WorkedInWeekTime += Day->timeWorked();
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: MonthWorkTime::updateTimeWorkedInWeek(" << date << ") - Date " << WeekDate << " not found";
#endif
        }
    }

    updatedWeekWorked = date.weekNumber();
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::updateTimeNeedInMonth( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::updateTimeNeedInMonth(" << date << ")";
#endif

    NeedInMonthTime.clear();

    for( int day = 1; day <= date.daysInMonth(); day++ )
    {
        QDate MonthDate( date.year(), date.month(), day );

        DayWorkTime* Day = dayAtDate( MonthDate );

        if( Day != NULL )
        {
            NeedInMonthTime += Day->timeNeed();
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: MonthWorkTime::updateTimeNeedInMonth(" << date << ") - Date " << MonthDate << " not found";
#endif
        }
    }
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::updateTimeWorkedInMonth( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::updateTimeWorkedInMonth(" << date << ")";
#endif

    WorkedInMonthTime.clear();

    for( int day = 1; day <= date.daysInMonth(); day++ )
    {
        QDate MonthDate( date.year(), date.month(), day );

        DayWorkTime* Day = dayAtDate( MonthDate );

        if( Day != NULL )
        {
            WorkedInMonthTime += Day->timeWorked();
        }
        else
        {
#ifdef WT_DEBUG
            qCritical() << " #Error :: MonthWorkTime::updateTimeWorkedInMonth(" << MonthDate << ") - day not found";
#endif
        }
    }
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::addDay( const QDate date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::addDay(" << date << ")";
#endif

    DaysList.append( new DayWorkTime( date, this ) );
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::addDay( DayWorkTime* Day )
{
    DaysList.append( Day );
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::setTypeDay( const QDate date, const DayWorkTime::TypesDay newType )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::setTypeDay(" << date << ", " << newType << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        DayWorkTime::TypesDay oldType = DayWorkTime::TypesDay( typeDay(date) );

        Day->setTypeDay( newType );

        if( (oldType == DayWorkTime::WorkDay && newType != DayWorkTime::WorkDay) ||
            (oldType != DayWorkTime::WorkDay && newType == DayWorkTime::WorkDay) )
        {
            fullUpdate( date );
        }
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::setTypeDay(" << date << ", " << newType << ") - Date not found";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::addInterval( const QDate& date, const QString & title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::addInterval(" << date << ", " << title << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Day->addInterval( title );
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::addInterval(" << date << ", " << title << ") - Date not found";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::removeInterval( const QDate& date, const int interval )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::removeInterval(" << date << ", " << interval << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Day->removeInterval( interval );

        updateTimeWorkedInWeek ( date );
        updateTimeWorkedInMonth( date );
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::removeInterval(" << date << ", " << interval << ") - Date not found";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::renameInterval(const QDate& date, const int interval, const QString& newTitle )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::renameInterval(" << date << ", " << interval << ", " << newTitle << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Day->renameInterval( interval, newTitle );
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::renameInterval(" << date << ", " << interval << ", " << newTitle << ") - Date not found";
#endif
    }
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::setNote( const QDate& date, const QString& note )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::setNote(" << date << ", " << note << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Day->changeNote( note );
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::setNote(" << date << ", " << note << ") - Date not found";
#endif
    }
}
// --------------------------------------------------------------------------------- //

QString MonthWorkTime::note( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::note(" << date << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        return Day->note();
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::note(" << date << ") - Date not found";
#endif
    }

    return "";
}
// --------------------------------------------------------------------------------- //

void MonthWorkTime::changeTypeDay( const QDate& date, DayWorkTime::TypesDay type )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::changeTypeDay(" << date << ", " << type << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Day->setTypeDay( type );

        updateTimeNeedInWeek   ( date );
        updateTimeNeedInMonth  ( date );
        updateTimeWorkedInWeek ( date );
        updateTimeWorkedInMonth( date );
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::changeTypeDay(" << date << ", " << type  << ") - Date not found";
#endif
    }
}
// --------------------------------------------------------------------------------- //

DayWorkTime::TypesDay MonthWorkTime::typeDay( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::typeDay(" << date << ")";
#endif

    DayWorkTime::TypesDay Type = DayWorkTime::UnknownDay;

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Type = Day->typeDay();
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::typeDay(" << date << ") - Date not found";
#endif
    }

    return Type;
}
// --------------------------------------------------------------------------------- //

QColor MonthWorkTime::colorDay( const QDate & date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::colorDay(" << date << ")";
#endif

    QColor Color = Qt::black;

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        Color = Day->colorDay();
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::colorDay(" << date << ") - Date not found";
#endif
    }

    return Color;
}
// --------------------------------------------------------------------------------- //

QStringList MonthWorkTime::titlesIntervals( const QDate& date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call MonthWorkTime::titlesIntervals(" << date << ")";
#endif

    DayWorkTime* Day = dayAtDate( date );

    if( Day != NULL )
    {
        return Day->titlesIntervals();
    }
    else
    {
#ifdef WT_DEBUG
        qCritical() << " #Error :: MonthWorkTime::titlesIntervals(" << date << ") - Date not found";
#endif
    }

    return QStringList();
}
// --------------------------------------------------------------------------------- //

bool MonthWorkTime::isLoaded()
{
    return DaysList.count() > 0;
}
// --------------------------------------------------------------------------------- //

bool MonthWorkTime::isThisMonth( const QDate& date )
{
    return ( dayAtDate(date) != NULL );
}
// --------------------------------------------------------------------------------- //

DayWorkTime* MonthWorkTime::dayAtDate( const QDate& date )
{
    foreach( DayWorkTime* Day, DaysList )
        if( Day != NULL && Day->date() == date )
            return Day;

    return NULL;
}
// --------------------------------------------------------------------------------- //
