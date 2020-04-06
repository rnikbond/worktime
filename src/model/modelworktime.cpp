// ---------------------------- //
#include <QDebug>
// ---------------------------- //
#include "helperwt.h"
// ---------------------------- //
#include "modelworktime.h"
// ---------------------------- //

ModelWorkTime::ModelWorkTime( QObject * parent ) : QObject( parent )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::ModelWorkTime( ... )";
#endif

    DataBase     = NULL;
    selectedDate = HelperWT::currentDate();
    workingRate  = HelperWT::UnknownWR;
    WorkMonth    = new MonthWorkTime( this );
}
// ------------------------------------------------------------------------------------ //

void ModelWorkTime::reload()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::reload()";
#endif

    if( workingRate != HelperWT::UnknownWR )
    {
        emit enabledWait( true );

        if( DataBase->isExists(selectedDate, workingRate) == false )
        {
            DataBase->insertYear( selectedDate, workingRate );
            DataBase->insertScheduleYear( selectedDate, workingRate );
        }

        WorkMonth->clear();

        for( int day = 1; day <= selectedDate.daysInMonth(); day++ )
        {
            QDate Date( selectedDate.year(), selectedDate.month(), day );

            DayWorkTime * NewDay = new DayWorkTime( Date, WorkMonth );

            WTime TimeNeed = DataBase->timeNeed(Date, workingRate);
            WTime TimeNeedSchedule = DataBase->timeNeedSchedule(Date, workingRate);

            NewDay->setTypeDay ( DayWorkTime::TypesDay(DataBase->typeDay(Date, workingRate)) );
            NewDay->setTimeNeed( TimeNeed );
            NewDay->setTimeNeedSchedule( TimeNeedSchedule );
            NewDay->changeNote ( DataBase->note(Date, workingRate) );

            QList<Interval*> IntervalsList;

            DataBase->intervals( Date, workingRate, IntervalsList );

            for( int interval = 0; interval < IntervalsList.count(); interval++ )
                NewDay->addInterval( IntervalsList[interval] );

            WorkMonth->addDay( NewDay );
        }

        emit enabledWait( false );
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setDataBase
 * \param DB Указатель на базу данных
 */
void ModelWorkTime::setDataBase( DataBaseWT * DB )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setDataBase( ... )";
#endif

    DataBase = DB;
}
// ------------------------------------------------------------------------------------ //

void ModelWorkTime::setWorkingRate( int rate )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setWorkingRate( " << rate << " )";
#endif

    if( rate != HelperWT::UnknownWR )
    {
        if( workingRate != rate )
        {
            workingRate = rate;
            reload();
        }
    }
}
// ------------------------------------------------------------------------------------ //

void ModelWorkTime::setDate( const QDate & date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setDate( " << date << " )";
#endif

    if( workingRate == HelperWT::UnknownWR )
        return;

    bool isChangeMonth = (selectedDate.year() != date.year()) || (selectedDate.month()      != date.month());
    bool isChangeWeek  = (selectedDate.year() != date.year()) || (selectedDate.weekNumber() != date.weekNumber());

    selectedDate = date;

    if( isChangeMonth )
        reload();

//    checkTimeStart();

//    if( isChangeMonth ) emit changedMonth()к;
//    if( isChangeWeek  ) emit changedWeek ();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::typeDay
 * \return Тип дня
 */
int ModelWorkTime::typeDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::colorMonthDays()";
#endif

    return WorkMonth->typeDay( selectedDate );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::intervals
 * \return Список названий интервалов
 */
QStringList ModelWorkTime::intervals()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::intervals()";
#endif

    return WorkMonth->titlesIntervals( selectedDate );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::note
 * \return Текст заметки
 */
QString ModelWorkTime::note()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::note()";
#endif

    return WorkMonth->note( selectedDate );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::colorMonthDays
 * \return Список названий цветов дней месяца, где 1-й элемент соответствует
 * 1-му дню месяца, и так до последнего дня месяца соответственно.
 */
QStringList ModelWorkTime::colorMonthDays()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::colorMonthDays()";
#endif

    QStringList Colors;

    for( int day = 1; day <= selectedDate.daysInMonth(); day++ )
        Colors << WorkMonth->colorDay(QDate(selectedDate.year(), selectedDate.month(), day)).name();

    return Colors;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::infoStatisticInDay
 * \return Строку: 'Недоработано', 'Переработано' или 'Статистика' за день
 */
QString ModelWorkTime::infoStatisticInDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::infoStatisticInDay()";
#endif

    WTime TimeNeed   = WorkMonth->timeNeedInDay  ( selectedDate );
    WTime TimeWorked = WorkMonth->timeWorkedInDay( selectedDate );

         if( TimeNeed > TimeWorked ) return tr("Недоработано");
    else if( TimeNeed < TimeWorked ) return tr("Переработано");
    else                             return tr("Статистика"  );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeStatisticInDay
 * \return Недоработанное или переработанное время за день
 */
WTime ModelWorkTime::timeStatisticInDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeStatisticInDay()";
#endif

    WTime TimeNeed   = WorkMonth->timeNeedInDay  ( selectedDate );
    WTime TimeWorked = WorkMonth->timeWorkedInDay( selectedDate );

    return TimeNeed > TimeWorked ? (TimeNeed - TimeWorked) : (TimeWorked - TimeNeed);
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeWorkedInDay
 * \return Время, отработанное за день
 */
WTime ModelWorkTime::timeWorkedInDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeWorkedInDay()";
#endif

    return WorkMonth->timeWorkedInDay( selectedDate );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeNeedInDay
 * \return Время, сколько нужно отработать за день
 */
WTime ModelWorkTime::timeNeedInDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeNeedInDay()";
#endif

    return WorkMonth->timeNeedInDay( selectedDate );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::infoStatisticToDay
 * \return Строку: 'Недоработано', 'Переработано' или 'Статистика' с 1-го дня месяца до выбранного дня
 */
QString ModelWorkTime::infoStatisticToDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::infoStatisticToDay()";
#endif

    WTime TimeNeed   = WorkMonth->timeNeedToDay  ( selectedDate );
    WTime TimeWorked = WorkMonth->timeWorkedToDay( selectedDate );


         if( TimeNeed > TimeWorked ) return tr("Недоработано");
    else if( TimeNeed < TimeWorked ) return tr("Переработано");
    else                             return tr("Статистика"  );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeStatisticToDay
 * \return Недоработанное или переработанное время с 1-го дня месяца до выбранного дня
 */
WTime ModelWorkTime::timeStatisticToDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeStatisticToDay()";
#endif

    WTime TimeNeed   = WorkMonth->timeNeedToDay  ( selectedDate );
    WTime TimeWorked = WorkMonth->timeWorkedToDay( selectedDate );

    return TimeNeed > TimeWorked ? (TimeNeed - TimeWorked) : (TimeWorked - TimeNeed);
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeWorkedToDay
 * \return Время, сколько отработано с 1-го дня месяца до выбранного дня
 */
WTime ModelWorkTime::timeWorkedToDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeWorkedToDay()";
#endif

    return WorkMonth->timeWorkedToDay( selectedDate );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeNeedToDay
 * \return Время, сколько нужно отработать с 1-го дня месяца до выбранного дня
 */
WTime ModelWorkTime::timeNeedToDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeNeedToDay()";
#endif

    return WorkMonth->timeNeedToDay( selectedDate );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::infoStatisticInWeek
 * \return Строку: 'Недоработано', 'Переработано' или 'Статистика' за неделю
 */
QString ModelWorkTime::infoStatisticInWeek()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::infoStatisticInWeek()";
#endif

    WTime TimeNeed   = WorkMonth->timeNeedInWeek  ( selectedDate );
    WTime TimeWorked = WorkMonth->timeWorkedInWeek( selectedDate );

         if( TimeNeed > TimeWorked ) return tr("Недоработано");
    else if( TimeNeed < TimeWorked ) return tr("Переработано");
    else                             return tr("Статистика"  );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeStatisticInWeek
 * \return Время, сколько недоработано или переработано за неделю
 */
WTime ModelWorkTime::timeStatisticInWeek()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeStatisticInWeek()";
#endif

    WTime TimeNeed   = WorkMonth->timeNeedInWeek  ( selectedDate );
    WTime TimeWorked = WorkMonth->timeWorkedInWeek( selectedDate );

    return TimeNeed > TimeWorked ? (TimeNeed - TimeWorked) : (TimeWorked - TimeNeed);
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeWorkedInWeek
 * \return Время, сколько отработано за неделю
 */
WTime ModelWorkTime::timeWorkedInWeek()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeWorkedInWeek()";
#endif

    return WorkMonth->timeWorkedInWeek( selectedDate );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeNeedInWeek
 * \return Время, сколько нужно отработать за неделю
 */
WTime ModelWorkTime::timeNeedInWeek()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeNeedInWeek()";
#endif

    return WorkMonth->timeNeedInWeek( selectedDate );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::infoStatisticInMonth
 * \return Строку: 'Недоработано', 'Переработано' или 'Статистика' за месяц
 */
QString ModelWorkTime::infoStatisticInMonth()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::infoStatisticInMonth()";
#endif

    WTime TimeNeed   = WorkMonth->timeNeedInMonth  ();
    WTime TimeWorked = WorkMonth->timeWorkedInMonth();

         if( TimeNeed > TimeWorked ) return tr("Недоработано");
    else if( TimeNeed < TimeWorked ) return tr("Переработано");
    else                             return tr("Статистика"  );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeStatisticInMonth
 * \return Время, сколько недоработано или переработано за месяц
 */
WTime ModelWorkTime::timeStatisticInMonth()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeStatisticInMonth()";
#endif

    WTime TimeNeed   = WorkMonth->timeNeedInMonth  ();
    WTime TimeWorked = WorkMonth->timeWorkedInMonth();

    return TimeNeed > TimeWorked ? (TimeNeed - TimeWorked) : (TimeWorked - TimeNeed);
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeWorkedInMonth
 * \return Время, сколько отработано за месяц
 */
WTime ModelWorkTime::timeWorkedInMonth()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeWorkedInMonth()";
#endif

    return WorkMonth->timeWorkedInMonth();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeNeedInMonth
 * \return Время, сколько нужно отработать за месяц
 */
WTime ModelWorkTime::timeNeedInMonth()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeNeedInMonth()";
#endif

    return WorkMonth->timeNeedInMonth();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeStart
 * \param id Идентификатор интервала в рамках дня
 * \return Время начала интевала с идентификатором \a id.
 */
WTime ModelWorkTime::timeStart( const int id )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeStart( " << id << " )";
#endif

    return WorkMonth->timeStart( selectedDate, id );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeStart
 * \param id Идентификатор интервала в рамках дня
 * \return Время конца интевала с идентификатором \a id.
 */
WTime ModelWorkTime::timeEnd( const int id )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeEnd( " << id << " )";
#endif

    return WorkMonth->timeEnd( selectedDate, id );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::addInterval
 * \param title Название нового интервала
 */
void ModelWorkTime::addInterval( const QString & title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::addInterval( " << title << " )";
#endif

    WorkMonth->addInterval( selectedDate, title );
    DataBase ->addInterval( selectedDate, workingRate, title );

//    if( SelectedDate == QDate::currentDate() )
//    {
//        int intervasCount = WorkMonth->countIntervals( SelectedDate );

//        if( intervasCount > 1 )
//        {
//            if( WorkMonth->timeStart(SelectedDate, intervasCount - 1).isEmpty() )
//            {
//                changeTimeStart( intervasCount - 1, WorkMonth->timeEnd(SelectedDate, intervasCount - 2) );
//            }
//        }

//        checkTimeStart();

//        updateLaunchTime();

//        TodayWorked->updateTimeEnd();
//    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::removeInterval
 * \param id Идентификатор интервала в рамках дня
 *
 * Удаление интервала и идентификатором \a id.
 */
void ModelWorkTime::removeInterval( const int id )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::removeInterval( " << id << " )";
#endif

    WorkMonth->removeInterval( selectedDate, id );
    DataBase ->removeInterval( selectedDate, workingRate, id );

    //updateLaunchTime();

    //updateTodayWorker();

    //emit updateTime();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::renameInterval
 * \param id Идентификатор интервала в рамках дня
 * \param title Новое название интервала
 */
void ModelWorkTime::renameInterval( const int id, const QString & title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::renameInterval( " << id << ", " << title << " )";
#endif

    WorkMonth->renameInterval( selectedDate, id, title );
    DataBase ->renameInterval( selectedDate, workingRate, id, title );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setTimeStart
 * \param id Идентификатор интервала в рамках дня, время начала которого нужно изменить
 * \param time Время начала интервала с идентификатором \a id.
 */
void ModelWorkTime::setTimeStart( const int id, const WTime & time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTimeStart( " << id << ", " << time.toString() << " )";
#endif

    WorkMonth->setTimeStart( selectedDate, id, time.toQTime(), true );
    DataBase ->setTimeStart( selectedDate, workingRate, id, time );

    //checkTimeStart();
    //updateLaunchTime();
   // updateTodayWorker();

    //emit updateTime();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setTimeEnd
 * \param id Идентификатор интервала в рамках дня, время конца которого нужно изменить
 * \param time Время конца интервала с идентификатором \a id.
 */
void ModelWorkTime::setTimeEnd( const int id, const WTime & time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTimeEnd(" << id << ", " << time.toString() << ")";
#endif

    WorkMonth->setTimeEnd( selectedDate, id, time.toQTime(), true );
    DataBase ->setTimeEnd( selectedDate, workingRate, id, time );

//    updateTodayWorker();

//    if( SelectedDate == QDate::currentDate() && id == WorkMonth->countIntervals(SelectedDate) - 1 )
//        TodayWorked->updateTimeEnd();

//    updateLaunchTime();

//    emit updateTime();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setTimeNeed
 * \param time Время, сколько нужно отработать за день
 */
void ModelWorkTime::setTimeNeed( const WTime & time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTimeNeed(" << time.toString() << ")";
#endif

    WorkMonth->setTimeNeed( selectedDate, time.toQTime(), true );
    DataBase ->setTimeNeed( selectedDate, workingRate, time, DataBaseWT::User );

//    updateTodayWorker();

//    emit updateTime();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setTypeDay
 * \param type Тип дня
 */
void ModelWorkTime::setTypeDay( int type )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTypeDay(" << type << ")";
#endif

    DayWorkTime::TypesDay oldType = DayWorkTime::TypesDay( WorkMonth->typeDay(selectedDate) );
    DayWorkTime::TypesDay newType = DayWorkTime::TypesDay( type );

    WorkMonth->setTypeDay( selectedDate, newType );
    DataBase ->setTypeDay( selectedDate, workingRate, newType );

    if( (oldType == DayWorkTime::WorkDay && newType != DayWorkTime::WorkDay) ||
        (oldType != DayWorkTime::WorkDay && newType == DayWorkTime::WorkDay) )
    {
        if( newType == DayWorkTime::WorkDay )
        {
            if( WorkMonth->countIntervals(selectedDate) == 0 )
            {
                addInterval( DayWorkTime::nameDay(DayWorkTime::WorkDay) );
            }
        }

        //updateTodayWorker();

        //emit updateTime();
    }

//    emit recolorDay( SelectedDate, WorkMonth->colorDay(SelectedDate) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setNote
 * \param note Текст заметки
 */
void ModelWorkTime::setNote( QString note )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setNote(" << note << ")";
#endif

    WorkMonth->setNote( selectedDate, note );
    DataBase ->setNote( selectedDate, workingRate, note );

//    updateTodayWorker();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::isLoadedMonth
 * \param date Проверка того, что данные за месяц выгружены из базы данных
 * \return TRUE, если данные за месяц \a date выгружены из базы данных
 */
bool ModelWorkTime::isLoadedMonth( const QDate & date )
{
#ifdef QT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::isLoadedMonth(" << date << ")";
#endif

    if( WorkMonth != NULL )
        return WorkMonth->isThisMonth( date );

    return false;
}
// ------------------------------------------------------------------------------------------ //
