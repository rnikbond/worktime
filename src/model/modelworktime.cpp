// ---------------------------- //
#include <qDebug>
#include <QThread>
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

    workingRate = HelperWT::UnknownWR;

    DataBase = DataBaseWT::instance();
    DataBase->openDataBase();

    QThread * BDThread = new QThread();

    DataBase->moveToThread( BDThread );

    BDThread->start();
}
// ------------------------------------------------------------------------------------ //

void ModelWorkTime::reload()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::loadMonth()";
#endif

    emit enabledWait( true );

    emit enabledWait( false );
}
// ------------------------------------------------------------------------------------ //

void ModelWorkTime::setWorkingRate( int rate )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setWorkingRate( " << rate << " )";
#endif

    if( workingRate != rate )
    {
        workingRate = rate;
        reload();
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

//    bool isChangeMonth = (selectedDate.year() != date.year()) || (selectedDate.month() != date.month());
//    bool isChangeWeek  = (selectedDate.year() != date.year()) || (selectedDate.weekNumber() != date.weekNumber());

    selectedDate = date;

    reload();

//    checkTimeStart();

//    if( isChangeMonth ) emit changedMonth();
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

    //return WorkMonth->typeDay( SelectedDate );
    return 0;
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

    //return WorkMonth->titlesIntervals( SelectedDate );
    return QStringList();
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

    //return WorkMonth->note( SelectedDate );
    return QString();
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

    //for( int day = 1; day <= selectedDate.daysInMonth(); day++ )
    //    Colors << WorkMonth->colorDay(QDate(selectedDate.year(), selectedDate.month(), day)).name();

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

//    WTime TimeNeed   = WorkMonth->timeNeedInDay  ( selectedDate );
//    WTime TimeWorked = WorkMonth->timeWorkedInDay( selectedDate );

//         if( TimeNeed > TimeWorked ) return tr("Недоработано");
//    else if( TimeNeed < TimeWorked ) return tr("Переработано");
//    else                             return tr("Статистика"  );

    return QString();
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

    //WTime TimeNeed   = WorkMonth->timeNeedInDay  ( selectedDate );
    //WTime TimeWorked = WorkMonth->timeWorkedInDay( selectedDate );

    //return TimeNeed > TimeWorked ? (TimeNeed - TimeWorked) : (TimeWorked - TimeNeed);

    return WTime();
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

    //return WorkMonth->timeWorkedInDay( selectedDate );

    return WTime();
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

    //return WorkMonth->timeNeedInDay( selectedDate );

    return WTime();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::infoStatisticToDay
 * \return Строку: 'Недоработано', 'Переработано' или 'Статистика' с 1-го дня месяца до выбранного дня
 */
QString ModelWorkTime::infoStatisticToDay()
{
    //WTime TimeNeed   = WorkMonth->timeNeedToDay  ( selectedDate );
    //WTime TimeWorked = WorkMonth->timeWorkedToDay( selectedDate );

    //return TimeNeed > TimeWorked ? (TimeNeed - TimeWorked) : (TimeWorked - TimeNeed);

    return QString();
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

    //WTime TimeNeed   = WorkMonth->timeNeedToDay  ( selectedDate );
    //WTime TimeWorked = WorkMonth->timeWorkedToDay( selectedDate );

    //return TimeNeed > TimeWorked ? (TimeNeed - TimeWorked) : (TimeWorked - TimeNeed);

    return WTime();
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

    //return WorkMonth->timeWorkedToDay( selectedDate );

    return WTime();
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

    //return WorkMonth->timeNeedToDay( selectedDate );

    return WTime();
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

    //    WTime TimeNeed   = WorkMonth->timeNeedInWeek  ( selectedDate );
    //    WTime TimeWorked = WorkMonth->timeWorkedInWeek( selectedDate );

    //         if( TimeNeed > TimeWorked ) return tr("Недоработано");
    //    else if( TimeNeed < TimeWorked ) return tr("Переработано");
    //    else                             return tr("Статистика"  );

    return QString();
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

    //WTime TimeNeed   = WorkMonth->timeNeedInWeek  ( selectedDate );
    //WTime TimeWorked = WorkMonth->timeWorkedInWeek( selectedDate );

    //return TimeNeed > TimeWorked ? (TimeNeed - TimeWorked) : (TimeWorked - TimeNeed);

    return WTime();
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

    //return WorkMonth->timeWorkedInWeek( selectedDate );

    return WTime();
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

    // return WorkMonth->timeNeedInWeek( selectedDate );

    return WTime();
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

    //    WTime TimeNeed   = WorkMonth->timeNeedInMonth  ( selectedDate );
    //    WTime TimeWorked = WorkMonth->timeWorkedInMonth( selectedDate );

    //         if( TimeNeed > TimeWorked ) return tr("Недоработано");
    //    else if( TimeNeed < TimeWorked ) return tr("Переработано");
    //    else                             return tr("Статистика"  );

    return QString();
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

//    WTime TimeNeed   = WorkMonth->timeNeedInMonth  ();
//    WTime TimeWorked = WorkMonth->timeWorkedInMonth();

//    return TimeNeed > TimeWorked ? (TimeNeed - TimeWorked) : (TimeWorked - TimeNeed);
    return WTime();
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

    //return WorkMonth->timeWorkedInMonth();
    return WTime();
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

    //return WorkMonth->timeNeedInMonth();
    return WTime();
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

    //return WorkMonth->timeStart( selectedDate, id );
    return WTime();
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

    //return WorkMonth->timeEnd( selectedDate, id );
    return WTime();
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

//    WorkMonth->addInterval( SelectedDate, title );
//    DataBase ->addInterval( SelectedDate, WorkingRate, title );

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

   // WorkMonth->removeInterval( selectedDate, id );
   // DataBase ->removeInterval( selectedDate, workingRate, id );

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

    //WorkMonth->renameInterval( SelectedDate, id, title );
    //DataBase ->renameInterval( SelectedDate, WorkingRate, id, title );
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

    //WorkMonth->setTimeStart   ( SelectedDate, id, time.toQTime(), true );
    //DataBase ->changeTimeStart( SelectedDate, WorkingRate, id, time );

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

//    WorkMonth->setTimeEnd   ( SelectedDate, id, time.toQTime(), true );
//    DataBase ->changeTimeEnd( SelectedDate, WorkingRate, id, time );

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

//    WorkMonth->setTimeNeed( SelectedDate, time.toQTime(), true );
//    DataBase ->changeTimeNeed( SelectedDate, WorkingRate, time );

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

//    DayWorkTime::TypesDay oldType = DayWorkTime::TypesDay( WorkMonth->typeDay(SelectedDate) );
//    DayWorkTime::TypesDay newType = DayWorkTime::TypesDay( type );

//    WorkMonth->setTypeDay   ( SelectedDate, newType );
//    DataBase ->changeTypeDay( SelectedDate, WorkingRate, newType );

//    if( (oldType == DayWorkTime::WorkDay && newType != DayWorkTime::WorkDay) ||
//        (oldType != DayWorkTime::WorkDay && newType == DayWorkTime::WorkDay) )
//    {
//        if( newType == DayWorkTime::WorkDay )
//        {
//            if( WorkMonth->countIntervals(SelectedDate) == 0 )
//            {
//                addInterval( DayWorkTime::nameDay(DayWorkTime::WorkDay) );
//            }
//        }

//        updateTodayWorker();

//        emit updateTime();
//    }

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

//    WorkMonth->setNote   ( SelectedDate, note );
//    DataBase ->changeNote( SelectedDate, WorkingRate, note );

//    updateTodayWorker();
}
// ------------------------------------------------------------------------------------ //

