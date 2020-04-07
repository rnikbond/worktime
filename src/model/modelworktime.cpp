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
    Today        = NULL;
    selectedDate = HelperWT::currentDate();
    workingRate  = HelperWT::UnknownWR;
    WorkMonth    = new MonthWorkTime( this );

    WorkTimer = new QTimer( this );

    connect( WorkTimer, SIGNAL(timeout()), SLOT(workTimerTick()) );
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

            if( Date == HelperWT::currentDate() && Today == NULL )
            {
                Today = NewDay;

                WorkTimer->start( 1000 );

                updateTimeEscape();
            }
        }

        WorkMonth->updateTimeNeedInMonth  ( selectedDate );
        WorkMonth->updateTimeNeedInWeek   ( selectedDate );
        WorkMonth->updateTimeWorkedInMonth( selectedDate );
        WorkMonth->updateTimeWorkedInWeek ( selectedDate );

        emit enabledWait( false );
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::checkTimeStart
 *
 * Проверка времени начала.
 */
void ModelWorkTime::checkTimeStart()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::checkTimeStart()";
#endif

    if( selectedDate == HelperWT::currentDate() )
    {
        int intervalID = WorkMonth->countIntervals( selectedDate ) - 1;

        if( intervalID == 0 )
        {
            WTime TimeStart = WorkMonth->timeStart( selectedDate, intervalID );

            if( TimeStart.isEmpty() )
            {
                TimeStart = HelperWT::timePower();

                //TimeStart += TimeSubstructStart;

                WorkMonth->setTimeStart( selectedDate, intervalID, TimeStart.toQTime(), true );
                DataBase ->setTimeStart( selectedDate, workingRate, intervalID, TimeStart );

                checkLaunch();

                emit reloadDay();
            }
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::checkLaunch
 *
 * Проверка времени обеда
 */
void ModelWorkTime::checkLaunch()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::checkLaunch()";
#endif

    WTime TimeLaunch( 00, 45 );
    WTime TimeLaunchStart( 12, 00 );
    WTime TimeLaunchEnd( 16, 00 );

    if( workingRate == HelperWT::Hours_40_Lanch   || workingRate == HelperWT::Hours_35_NoLanch ||
        workingRate == HelperWT::Hours_30_NoLanch || workingRate == HelperWT::Hours_40_4_Days   )
    {
        DayWorkTime * Day = WorkMonth->dayAtDate( selectedDate );

        const QList<Interval*> & IntervalsList = Day->intervals();

        int LaunchMinutes = TimeLaunch.toMinutes();

        WTime LastTimeEnd;

        bool isSetTime = false;

        for( int i = 0; i < IntervalsList.count(); i++ )
        {
            Interval * Item = IntervalsList[i];

            WTime TimeStart = Item->timeStart();
            WTime TimeEnd   = Item->timeEnd();

            if( TimeStart.isEmpty() == false )
                isSetTime = true;

            if( TimeStart > TimeLaunchStart && TimeStart < TimeLaunchEnd )
            {
                if( LastTimeEnd.isEmpty() == false && TimeStart >= LastTimeEnd )
                {
                    LaunchMinutes -= (TimeStart - LastTimeEnd).toMinutes();
                }
                else
                {
                    LaunchMinutes -= (TimeStart - TimeLaunchStart).toMinutes();
                }
            }

            LastTimeEnd = TimeEnd;
        }

        if( LaunchMinutes < 0 )
            LaunchMinutes = 0;

        WTime TimeNeed = Day->timeNeedSchedule();

        if( isSetTime )
            TimeNeed.addMinutes( LaunchMinutes );

        setTimeNeed( TimeNeed );
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::updateTimeEnd
 *
 * Обновление времени конца рабочего дня
 */
void ModelWorkTime::updateTimeEnd()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::updateTimeEnd()";
#endif

    WTime TimeSubstructEnd;

    if( selectedDate == HelperWT::currentDate() )
    {
        if( Today != NULL )
        {
            int interval = Today->countIntervals() - 1;

            WTime TimeEnd = Today->timeEnd( interval );
            WTime CurretnTime = QTime::currentTime() + TimeSubstructEnd;

            if( CurretnTime > TimeEnd )
            {
                WorkMonth->setTimeEnd( selectedDate, interval, CurretnTime.toQTime(), true );
                DataBase ->setTimeEnd( selectedDate, workingRate, interval, CurretnTime );

                checkLaunch();
            }

            emit reloadDay();
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::workTimerTick
 */
void ModelWorkTime::workTimerTick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::workTimerTick()";
#endif

    WTime TimeMaxEscape( 21, 00 );
    WTime TimeSubstructEnd;

    if( Today != NULL )
    {
        if( isCorrectIntervals() )
        {
            if( Today->isWorkedDay() )
            {
                WTimeExt CurrentTime = HelperWT::currentTime();

                if( CurrentTime.seconds() == 0 )
                    updateTimeEnd();

                WTimeExt CurrentTimeExt = QTime::currentTime();
                WTimeExt TimeEndExt     = timeEscape();
                int interval = Today->countIntervals() - 1;

                if( WTime(TimeEndExt) > TimeMaxEscape )
                    TimeEndExt = TimeMaxEscape;

                if( WTimeExt(Today->timeEnd(interval)) > CurrentTimeExt + WTimeExt(TimeSubstructEnd) )
                    TimeEndExt = Today->timeEnd( interval );

                if( CurrentTimeExt < TimeEndExt  )
                {
                    emit updateReverseTimer( (TimeEndExt - CurrentTimeExt).toString() );
                }
                else
                {
                    emit updateReverseTimer( (CurrentTimeExt - TimeEndExt).toString() );
                }
            }
            else
            {
                emit updateReverseTimer( tr("НЕРАБОЧИЙ ДЕНЬ") );
            }
        }
        else
        {
            emit updateReverseTimer( tr("ОШИБКА ИНТЕРВАЛОВ") );
        }
    }
}
// ------------------------------------------------------------------------------------ //

bool ModelWorkTime::isCorrectIntervals()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::isCorrectIntervals()";
#endif

    if( Today != NULL )
    {
        int countIntervals = Today->countIntervals();

        if( countIntervals > 0 )
        {
            bool isCorrectIntervals = true;

            for( int i = 0; i < countIntervals; i++ )
            {
                if( i == 0 )
                {
                    if( Today->timeStart(i) > Today->timeEnd(i) )
                    {
                        isCorrectIntervals = false;
                        break;
                    }
                }
                else
                {
                    if( Today->timeStart(i) < Today->timeEnd(i - 1) )
                    {
                        isCorrectIntervals = false;
                        break;
                    }
                }
            }

            return isCorrectIntervals;
        }
    }

    return false;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::timeEscape
 * \return Время, во сколько сегодня нужно уйти
 */
WTime ModelWorkTime::timeEscape()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeEscape()";
#endif

    WTime TimeEscape;

    if( Today != NULL )
    {
        if( Today->isWorkedDay() )
        {
            if( isCorrectIntervals() )
            {
                WTime TimeStart;
                WTime TodayWorked;

                WTime RemainTime;

                WTime UnwokredTime;
                WTime RewokredTime;

                if( Today->date().day() == 1 )
                {
                    RemainTime = Today->timeNeed();
                }
                else
                {
                    QDate Yesterday( Today->date().year(), Today->date().month(), Today->date().day() - 1 );

                    UnwokredTime = WorkMonth->timeNeedToDay(Yesterday) - WorkMonth->timeWorkedToDay(Yesterday);
                    RewokredTime = WorkMonth->timeWorkedToDay(Yesterday) - WorkMonth->timeNeedToDay(Yesterday);

                    if( RewokredTime > UnwokredTime )
                    {
                        // Если есть переработки, но их не хватает, чтоб отработать сегодняшний день
                        if( Today->timeNeed() > RewokredTime )
                        {
                            RemainTime = Today->timeNeed() - RewokredTime;
                        }
                    }
                    else // Есть недоработки
                    {
                         RemainTime = UnwokredTime + Today->timeNeed();
                    }
                }

                int lastInterval = Today->countIntervals() - 1;

                for( int interval = 0; interval < lastInterval; interval++ )
                    TodayWorked += Today->timeWorked( interval );

                if( lastInterval >= 0 )
                    TimeStart = Today->timeStart( lastInterval );

                RemainTime -= TodayWorked;

                TimeEscape = TimeStart + RemainTime;
            }
        }
    }

    return TimeEscape;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief updateTimeEscape
 *
 * Обновление времени, во сколько нужно уйти
 */
void ModelWorkTime::updateTimeEscape()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::updateTimeEscape()";
#endif

    WTime TimeMaxEscape( 21, 00 );

    if( Today != NULL )
    {
        if( Today->isWorkedDay()  )
        {
            WTime TimeEscape = timeEscape();

            if( TimeEscape.isEmpty() == false )
            {
                QString InfoEscape;

                if( TimeEscape > TimeMaxEscape )
                {
                    InfoEscape = tr(" и останется %1").arg((TimeEscape - TimeMaxEscape).toString());
                    TimeEscape = TimeMaxEscape;
                }

                updateEscape( tr("Конец в ") + TimeEscape.toString() + InfoEscape );

            }
            else
            {
                updateEscape( "" );
            }
        }
        else
        {
            updateEscape( "" );
        }
    }
    else
    {
        updateEscape( tr("Ошибка #1") );
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

            workTimerTick();

            emit reloadMonth();
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

    if( selectedDate == HelperWT::currentDate() )
        updateTimeEnd();

    if( isChangeMonth )
        reload();

    checkTimeStart();

    if( isChangeMonth ) emit reloadMonth();
    if( isChangeWeek  ) emit reloadWeek ();
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

    if( selectedDate == HelperWT::currentDate() )
    {
        int intervasCount = WorkMonth->countIntervals( selectedDate );

        if( intervasCount == 1 )
        {
            checkTimeStart();
        }
        else if( intervasCount > 1 )
        {
            if( WorkMonth->timeStart(selectedDate, intervasCount - 1).isEmpty() )
            {
                setTimeStart( intervasCount - 1, WorkMonth->timeEnd(selectedDate, intervasCount - 2) );
            }
        }

        updateTimeEnd();
        updateTimeEscape();

        emit reloadMonth();
    }

    workTimerTick();
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

    if( selectedDate == HelperWT::currentDate() )
    {
        updateTimeEnd();
        updateTimeEscape();
    }

    workTimerTick();

    emit reloadMonth();
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

    checkTimeStart();
    checkLaunch();

    if( selectedDate == HelperWT::currentDate() )
        updateTimeEnd();

    updateTimeEscape();
    workTimerTick();

    emit reloadMonth();
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

    checkLaunch();

    if( selectedDate == HelperWT::currentDate() )
        updateTimeEnd();

    updateTimeEscape();
    workTimerTick();

    emit reloadMonth();
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

    updateTimeEscape();
    workTimerTick();

    emit reloadMonth();
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

        if( selectedDate == HelperWT::currentDate() )
        {
            updateTimeEnd();
        }

        emit reloadMonth();
    }

    updateTimeEscape();
    workTimerTick();

    emit recolorDay( selectedDate, WorkMonth->colorDay(selectedDate) );
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
