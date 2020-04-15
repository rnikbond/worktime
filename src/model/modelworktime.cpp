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

    isTickTimer   = true;

    DataBase      = NULL;

    selectedDate  = HelperWT::currentDate();
    workingRate   = HelperWT::UnknownWR;

    WorkMonth     = NULL;
    SelectedMonth = new MonthWorkTime( this );
    CurrentMonth  = new MonthWorkTime( this );

    WorkTimer = new QTimer( this );

    connect( WorkTimer, SIGNAL(timeout()), SLOT(workTimerTick()) );
}
// ------------------------------------------------------------------------------------ //

ModelWorkTime::~ModelWorkTime()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::~ModelWorkTime()";
#endif
}
// ------------------------------------------------------------------------------------ //

void ModelWorkTime::reload()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::reload()";
#endif

    if( workingRate != HelperWT::UnknownWR )
    {
        if( DataBase->isExists(selectedDate, workingRate) == false )
        {
            emit enabledWait( true );

            DataBase->insertYear( selectedDate, workingRate );
            DataBase->insertScheduleYear( selectedDate, workingRate );

            emit enabledWait( false );
        }


        if( isEqualMonth(selectedDate, HelperWT::currentDate()) )
        {
            load( CurrentMonth );

            WorkMonth = CurrentMonth;

            updateTimeEscape();
            workTimerTick();

            WorkTimer->start( 1000 );
        }
        else
        {
            load( SelectedMonth );

            WorkMonth = SelectedMonth;
        }
    }
}
// ------------------------------------------------------------------------------------ //

void ModelWorkTime::load( MonthWorkTime * Month )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::load()";
#endif

    if( Month == NULL )
    {
        qDebug() << "[ERROR] ModelWorkTime::load(MonthWorkTime * Month) = Month is NULL";
        return;
    }

    Month->clear();

    for( int day = 1; day <= selectedDate.daysInMonth(); day++ )
    {
        QDate Date( selectedDate.year(), selectedDate.month(), day );

        DayWorkTime * NewDay = new DayWorkTime( Date, Month );

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

        Month->addDay( NewDay );
    }

    Month->updateTimeNeedInMonth  ( selectedDate );
    Month->updateTimeNeedInWeek   ( selectedDate );
    Month->updateTimeWorkedInMonth( selectedDate );
    Month->updateTimeWorkedInWeek ( selectedDate );
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

    if( CurrentMonth->isLoaded() )
    {
        DayWorkTime * Today = CurrentMonth->dayAtDate( HelperWT::currentDate() );

        if( Today != NULL )
        {
            if( Today->isWorkedDay() )
            {
                if( isCorrectIntervals() )
                {
                    WTimeExt CurrentTime = HelperWT::currentTime();

                    if( CurrentTime.seconds() == 0 )
                        updateTimeEnd();

                    WTimeExt CurrentTimeExt = QTime::currentTime();
                    WTimeExt TimeEndExt     = timeEscape();

                    int interval = Today->countIntervals() - 1;

                    if( WTime(TimeEndExt) > MaxTime )
                        TimeEndExt = MaxTime;

                    if( WTimeExt(Today->timeEnd(interval)) > CurrentTimeExt + WTimeExt(AfterTime) )
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
            }
            else
            {
                emit updateReverseTimer( tr("Нерабочий день") );
            }
        }
        else
        {
            emit updateReverseTimer( tr("Нет данных о дне") );
            qWarning() << "[ERROR] ModelWorkTime::workTimerTick() = Today(" << HelperWT::currentDate() << ") is NULL";
        }
    }
    else
    {
        emit updateReverseTimer( tr("Нет данных о месяце") );
        qWarning() << "[ERROR] ModelWorkTime::workTimerTick() = CurrentMonth not loaded";
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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::checkTimeStart() = WorkMonth is NULL";
        return;
    }

    if( selectedDate == HelperWT::currentDate() )
    {
        int intervalID = WorkMonth->countIntervals( selectedDate ) - 1;

        if( intervalID == 0 )
        {
            WTime TimeStart = WorkMonth->timeStart( selectedDate, intervalID );

            if( TimeStart.isEmpty() )
            {
                TimeStart = HelperWT::timePower();

                TimeStart -= BeforeTime;

                setTimeStart( intervalID, TimeStart );
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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::checkLaunch() = WorkMonth is NULL";
        return;
    }

    if( workingRate == HelperWT::Hours_40_Lanch   || workingRate == HelperWT::Hours_35_NoLanch ||
        workingRate == HelperWT::Hours_30_NoLanch || workingRate == HelperWT::Hours_40_4_Days   )
    {
        DayWorkTime * Day = WorkMonth->dayAtDate( selectedDate );

        const QList<Interval*> & IntervalsList = Day->intervals();

        int LaunchMinutes = LaunchLengthTime.toMinutes();

        WTime LastTimeEnd;

        bool isSetTime = false;

        for( int i = 0; i < IntervalsList.count(); i++ )
        {
            Interval * Item = IntervalsList[i];

            WTime TimeStart = Item->timeStart();
            WTime TimeEnd   = Item->timeEnd();

            if( TimeStart > TimeEnd )
                continue;

            if( TimeStart.isEmpty() == false )
                isSetTime = true;

            if( TimeStart > LaunchStartTime && TimeStart < LaunchEndTime )
            {
                if( LastTimeEnd.isEmpty() == false && TimeStart >= LastTimeEnd )
                {
                    if( LastTimeEnd > LaunchStartTime )
                    {
                        LaunchMinutes -= (TimeStart - LastTimeEnd).toMinutes();
                    }
                    else
                    {
                        LaunchMinutes -= (TimeStart - LaunchStartTime).toMinutes();
                    }
                }
                else
                {
                    LaunchMinutes -= (TimeStart - LaunchStartTime).toMinutes();
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

    if( isTickTimer == false )
        return;

    if( CurrentMonth->isLoaded() )
    {
        if( selectedDate == HelperWT::currentDate() )
        {
            DayWorkTime * Today = CurrentMonth->dayAtDate( selectedDate );

            if( Today != NULL )
            {
                int interval = Today->countIntervals() - 1;

                WTime TimeEnd = Today->timeEnd( interval );
                WTime CurretnTime = QTime::currentTime() + AfterTime;

                if( CurretnTime > TimeEnd )
                {
                    CurrentMonth->setTimeEnd( selectedDate, interval, CurretnTime.toQTime(), true );
                    DataBase    ->setTimeEnd( selectedDate, workingRate, interval, CurretnTime );

                    emit refreshTimeEnd( interval, CurretnTime );

                    checkLaunch();
                }
            }
            else
            {
                qWarning() << "[ERROR] ModelWorkTime::checkLaunch() = Today(" << selectedDate << ") is NULL";
            }
        }

        if( isEqualMonth(selectedDate, HelperWT::currentDate()) )
            updateTimeEscape();
    }
    else
    {
        qWarning() << "[ERROR] ModelWorkTime::checkLaunch() = CurrentMonth not loaded";
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::updateTimeEscape
 *
 * Обновление времени ухода и связанной дополнительной информации
 */
void ModelWorkTime::updateTimeEscape()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::updateTimeEscape()";
#endif

    if( CurrentMonth->isLoaded() )
    {
        DayWorkTime * Today = CurrentMonth->dayAtDate( HelperWT::currentDate() );

        if( Today->isWorkedDay()  )
        {
            WTime TimeEscape = timeEscape();

            if( TimeEscape.isEmpty() == false )
            {
                QString InfoEscape;

                if( TimeEscape > MaxTime )
                {
                    InfoEscape = tr(" и останется %1").arg((TimeEscape - MaxTime).toString());
                    TimeEscape = MaxTime;
                }

                emit updateEscape( tr("Конец в ") + TimeEscape.toString() + InfoEscape );

            }
            else
            {
                emit updateEscape( "" );
            }
        }
        else
        {
            emit updateEscape( "" );
        }
    }
    else
    {
        qWarning() << "[ERROR] ModelWorkTime::updateTimeEscape() = WorkMonth is NULL";
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

/*!
 * \brief ModelWorkTime::setWorkingRate Изменение рабочей ставки
 * \param rate Индекс рабочей ставки
 */
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

            emit reloadMonth();
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setDate Изменение выборанной даты
 * \param date Дата
 */
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

    if( selectedDate == HelperWT::currentDate() )
        updateTimeEnd();

    checkTimeStart();

    if( isChangeMonth ) emit reloadMonth();
    if( isChangeWeek  ) emit reloadWeek ();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setTimeLaunchStart Изменение времени начала обеда
 * \param time Время начала обеда
 */
void ModelWorkTime::setTimeLaunchStart( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTimeLaunchStart( " << time.toString() << " )";
#endif

    LaunchStartTime = time;

    checkLaunch();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setTimeLaunchEnd Изменение конца начала обеда
 * \param time Время конца обеда
 */
void ModelWorkTime::setTimeLaunchEnd( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTimeLaunchEnd( " << time.toString() << " )";
#endif

    LaunchEndTime = time;

    checkLaunch();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setTimeLaunchLength Изменение времени длительности обеда
 * \param time Длительность обеда
 */
void ModelWorkTime::setTimeLaunchLength( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTimeLaunchLength( " << time.toString() << " )";
#endif

    LaunchLengthTime = time;

    checkLaunch();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setTimeMax Изменение максимального времени конца рабочего дня
 * \param time Максимальное время конца рабочего дня
 */
void ModelWorkTime::setTimeMax( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTimeMax( " << time.toString() << " )";
#endif

    MaxTime = time;

    if( isEqualMonth(selectedDate, HelperWT::currentDate()) )
        updateTimeEscape();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setTimeBefore Изменение времени, сколько отнимать от времени начала
 * \param time Время, отнимаемое от времени начала
 */
void ModelWorkTime::setTimeBefore( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTimeBefore( " << time.toString() << " )";
#endif

    BeforeTime = time;

    checkTimeStart();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::setTimeAfter Изменение времени, сколько добавлять ко времени конца
 * \param time Время, добавляемое ко времени конца
 */
void ModelWorkTime::setTimeAfter( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTimeAfter( " << time.toString() << " )";
#endif

    AfterTime = time;

    updateTimeEnd();
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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::typeDay() = WorkMonth is NULL";
        return -1;
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::intervals() = WorkMonth is NULL";
        return QStringList();
    }


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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::note() = WorkMonth is NULL";
        return QString();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::colorMonthDays() = WorkMonth is NULL";
        return QStringList();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::infoStatisticInDay() = WorkMonth is NULL";
        return QString();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeStatisticInDay() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeWorkedInDay() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeNeedInDay() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::infoStatisticToDay() = WorkMonth is NULL";
        return QString();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeStatisticToDay() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeWorkedToDay() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeNeedToDay() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::infoStatisticInWeek() = WorkMonth is NULL";
        return QString();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeStatisticInWeek() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeWorkedInWeek() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeNeedInWeek() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::infoStatisticInMonth() = WorkMonth is NULL";
        return QString();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeStatisticInMonth() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeWorkedInMonth() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeNeedInMonth() = WorkMonth is NULL";
        return WTime();
    }

    return WorkMonth->timeNeedInMonth();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief ModelWorkTime::todayTimeEscape
 * \return
 */
WTime ModelWorkTime::todayTimeEscape()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::todayTimeEscape()";
#endif

    return timeEscape();
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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeStart() = WorkMonth is NULL";
        return WTime();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::timeEnd() = WorkMonth is NULL";
        return WTime();
    }

    return WorkMonth->timeEnd( selectedDate, id );
}
// ------------------------------------------------------------------------------------ //

WTime ModelWorkTime::timeEscape()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::timeEscape()";
#endif

    WTime TimeEscape;

    if( CurrentMonth->isLoaded() )
    {
        DayWorkTime * Today = CurrentMonth->dayAtDate( HelperWT::currentDate() );

        if( Today == NULL )
        {
            qWarning() << "[ERROR] ModelWorkTime::timeEscape() = Today" << HelperWT::currentDate() << " is NULL";
            return WTime();
        }

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

                    UnwokredTime = CurrentMonth->timeNeedToDay  (Yesterday) - CurrentMonth->timeWorkedToDay(Yesterday);
                    RewokredTime = CurrentMonth->timeWorkedToDay(Yesterday) - CurrentMonth->timeNeedToDay  (Yesterday);

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
    else
    {
        qWarning() << "[ERROR] ModelWorkTime::timeEscape() = CurrentMonth not loaded";
    }

    return TimeEscape;
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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::addInterval() = WorkMonth is NULL";
        return;
    }

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
    }

    if( isEqualMonth(selectedDate, HelperWT::currentDate()) )
        updateTimeEscape();
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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::removeInterval() = WorkMonth is NULL";
        return;
    }

    WorkMonth->removeInterval( selectedDate, id );
    DataBase ->removeInterval( selectedDate, workingRate, id );

    // Если 1-й интервал пустой, а время было добавлено во втором интервале,
    // и из-за этого изменилось время, сколько нужно отработать, то нужно обновить
    // информацию об обеде
    checkLaunch();

    if( selectedDate == HelperWT::currentDate() )
        updateTimeEnd();

    if( isEqualMonth(selectedDate, HelperWT::currentDate()) )
    {
        workTimerTick();
        updateTimeEscape();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::renameInterval() = WorkMonth is NULL";
        return;
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::setTimeStart() = WorkMonth is NULL";
        return;
    }

    WorkMonth->setTimeStart( selectedDate, id, time.toQTime(), true );
    DataBase ->setTimeStart( selectedDate, workingRate, id, time );

    checkTimeStart();
    checkLaunch();

    emit refreshTimeStart( id, time );

    if( selectedDate == HelperWT::currentDate() )
        updateTimeEnd();

    if( isEqualMonth(selectedDate, HelperWT::currentDate()) )
    {
        workTimerTick();
        updateTimeEscape();
    }
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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::setTimeEnd() = WorkMonth is NULL";
        return;
    }

    WorkMonth->setTimeEnd( selectedDate, id, time.toQTime(), true );
    DataBase ->setTimeEnd( selectedDate, workingRate, id, time );

    checkLaunch();

    emit refreshTimeEnd( id, time );

    if( selectedDate == HelperWT::currentDate() )
        updateTimeEnd();

    if( isEqualMonth(selectedDate, HelperWT::currentDate()) )
    {
        workTimerTick();
        updateTimeEscape();
    }
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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::setTimeNeed() = WorkMonth is NULL";
        return;
    }

    WorkMonth->setTimeNeed( selectedDate, time.toQTime(), true );
    DataBase ->setTimeNeed( selectedDate, workingRate, time, DataBaseWT::User );

    if( isEqualMonth(selectedDate, HelperWT::currentDate()) )
    {
        workTimerTick();
        updateTimeEscape();
    }

    emit reloadStatisticTime();
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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::setTypeDay() = WorkMonth is NULL";
        return;
    }

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
            updateTimeEnd();

        if( isEqualMonth(selectedDate, HelperWT::currentDate()) )
        {
            workTimerTick();
            updateTimeEscape();
        }

        emit reloadMonth();
    }

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

    if( WorkMonth == NULL )
    {
        qWarning() << "[ERROR] ModelWorkTime::setNote() = WorkMonth is NULL";
        return;
    }

    WorkMonth->setNote( selectedDate, note );
    DataBase ->setNote( selectedDate, workingRate, note );
}
// ------------------------------------------------------------------------------------ //

void ModelWorkTime::setTimerState( bool state )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::setTimerState( " << state <<  " )";
#endif

    isTickTimer = state;

    if( isTickTimer )
    {
        workTimerTick();
        updateTimeEnd();

        WorkTimer->start( 1000 );
    }
    else
    {
        WorkTimer->stop();
    }
}
// ------------------------------------------------------------------------------------ //

bool ModelWorkTime::isEqualMonth( const QDate & date1, const QDate & date2 )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::isEqualMonth( " << date1 << ", " << date2 << " )";
#endif

    return date1.year() == date2.year() && date1.month() == date2.month();
}
// ------------------------------------------------------------------------------------------ //

bool ModelWorkTime::isCorrectIntervals()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call ModelWorkTime::isCorrectIntervals()";
#endif

    // TODO :: Перенести в класс DayWorkTime

    if( CurrentMonth->isLoaded() )
    {
        DayWorkTime * Today = CurrentMonth->dayAtDate( HelperWT::currentDate() );

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
        else
        {
            qWarning() << "[WARNING] ModelWorkTime::isCorrectIntervals() = Today(" << HelperWT::currentDate() << ") is NULL";
        }
    }
    else
    {
        qWarning() << "[WARNING] ModelWorkTime::isCorrectIntervals() = CurrentMonth not loaded";
    }

    return false;
}
// ------------------------------------------------------------------------------------------ //
