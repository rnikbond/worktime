// ---------------------------- //
#include <QColor>
// ---------------------------- //
#include "iworktime.h"
// ---------------------------- //
#include "presenterworktime.h"
// ---------------------------- //

/*!
 * \brief PresenterWorkTime::PresenterWorkTime
 * \param View
 * \param parent
 *
 * В конструкторе происходит соединение сигналов от \a View.
 *
 * \sa PresenterWorkTime::connectView
 */
PresenterWorkTime::PresenterWorkTime( IWorkTime * View, QObject * parent ) : QObject( parent )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::PresenterWorkTime(...)";
#endif

    ViewWT = View;

    connectView( View );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshFull
 *
 * Полное обновление GUI
 */
void PresenterWorkTime::refreshFull()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshFull()";
#endif

    //ViewWT->setColorDays( Model->colorsDays() );

    refreshTimeMonth();
    refreshTimeWeek ();
    refreshDataDay  ();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshDataDay
 *
 * Обновление данных за день: Тип дня, список интервалов, заметка.
 */
void PresenterWorkTime::refreshDataDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshDataDay()";
#endif

//    ViewWT->setTypeDay  ( Model->typeDay  () );
//    ViewWT->setIntervals( Model->intervals() );
//    ViewWT->setNote     ( Model->note     () );

    refreshTimeDay();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshTimeDay
 *
 * Обновление времени за день: сколько отработано, статистика времени и сколько нужно.
 * Также обновляется время статистики 1-го дня месяца до выбранного дня.
 */
void PresenterWorkTime::refreshTimeDay()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshTimeDay()";
#endif

//    WTime StatisticTimeInDay = Model->timeStatisticInDay();
//    WTime StatisticTimeToDay = Model->timeStatisticToDay();

//    QString InfoInDay = Model->infoStatisticInDay();
//    QString InfoToDay = Model->infoStatisticToDay();

//    ViewWT->setTimeWorkedInDay   ( Model->timeWorkedInDay() );
//    ViewWT->setTimeStatisticInDay( StatisticTimeInDay, InfoInDay );
//    ViewWT->setTimeNeedInDay     ( Model->timeNeedInDay() );

//    ViewWT->setTimeWorkedToDay   ( Model->timeWorkedToDay() );
//    ViewWT->setTimeStatisticToDay( StatisticTimeToDay, InfoToDay );
//    ViewWT->setTimeNeedToDay     ( Model->timeNeedToDay() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshTimeWeek
 *
 * Обновление времени за неделю: сколько отработано, статистика времени и сколько нужно.
 */
void PresenterWorkTime::refreshTimeWeek()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshTimeWeek()";
#endif

//    WTime StatisticTime = Model->timeStatisticInWeek();
//    QString Info = Model->infoStatisticInWeek();

//    ViewWT->setTimeWorkedInWeek   ( Model->timeWorkedInWeek() );
//    ViewWT->setTimeStatisticInWeek( StatisticTime, Info );
//    ViewWT->setTimeNeedInWeek     ( Model->timeNeedInWeek() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshTimeMonth
 *
 * Обновление времени за месяц: сколько отработано, статистика времени и сколько нужно.
 */
void PresenterWorkTime::refreshTimeMonth()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshTimeMonth()";
#endif

//    WTime StatisticTime = Model->timeStatisticInMonth();
//    QString Info = Model->infoStatisticInMonth();

//    ViewWT->setTimeWorkedInMonth   ( Model->timeWorkedInMonth() );
//    ViewWT->setTimeStatisticInMonth( StatisticTime, Info );
//    ViewWT->setTimeNeedInMonth     ( Model->timeNeedInMonth() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshTimeStart
 * \param id Идентификатор интервала дня
 * \param time Время начала интервала
 */
void PresenterWorkTime::refreshTimeStart( int id, WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshTimeStart( " << id, ", " << time.toString() << " )";
#endif

    ViewWT->setTimeStart( time, id );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshTimeEnd
 * \param id Идентификатор интервала дня
 * \param time Время конца интервала
 */
void PresenterWorkTime::refreshTimeEnd( int id, WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshTimeEnd( " << id, ", " << time.toString() << " )";
#endif

    ViewWT->updateTimeEnd( id, time );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshTimeReverse
 * \param time Время таймера обратного отсчета
 */
void PresenterWorkTime::refreshTimeReverse( WTimeExt time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshTimeReverse( " << time.toString() << " )";
#endif

    ViewWT->setTimeReverseTimer( time );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshTimeEscape
 * \param time Время ухода
 * \param info Информация об оставшемся времени
 */
void PresenterWorkTime::refreshTimeEscape( WTime time, QString info )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshTimeEscape( " << time.toString() << ", " << info << " )";
#endif

    ViewWT->setTimeEscape( time );
    ViewWT->setInfoEscape( info );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshColorDay
 * \param date День, цвет которого нужно изменить
 * \param color Цвет дня
 */
void PresenterWorkTime::refreshColorDay( QDate date, QColor color )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshColorDay( " << date.toString("dd.MM.yyyy") << ", " << color.name() << " )";
#endif

    ViewWT->setColorDay( date, color );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshColorLight
 * \param color Цвет подсветки
 */
void PresenterWorkTime::refreshColorLight( QColor color )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshColorLight( " << color.name() << " )";
#endif

    ViewWT->setColorLight( color );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userSelectDate
 * \param date Выбранная пользователем дата
 */
void PresenterWorkTime::userSelectDate( const QDate & date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userSelectDate( " << date << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userSelectInterval
 * \param id Индетификатор интервала дня
 */
void PresenterWorkTime::userSelectInterval( int id )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userSelectInterval( " << id << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userRemoveInterval
 * \param id Индетификатор интервала дня
 */
void PresenterWorkTime::userRemoveInterval( int id )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userRemoveInterval( " << id << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userRenameInterval
 * \param id Индетификатор интервала дня
 * \param title Новое название интервала
 */
void PresenterWorkTime::userRenameInterval( int id, QString title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userRenameInterval( " << id << ", " << title << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userChangeTimeStart
 * \param id Индетификатор интервала дня
 * \param time Время начала интервала
 */
void PresenterWorkTime::userChangeTimeStart( int id, WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userRenameInterval( " << id << ", " << time.toString() << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userChangeTimeEnd
 * \param id Индетификатор интервала дня
 * \param time Время конца интервала
 */
void PresenterWorkTime::userChangeTimeEnd( int id, WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userChangeTimeEnd( " << id << ", " << time.toString() << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userChangeTimeNeed
 * \param time Время, сколько нужнот отработать за день
 */
void PresenterWorkTime::userChangeTimeNeed( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userChangeTimeNeed( " << time.toString() << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userAddInterval
 * \param title Название нового интервала
 */
void PresenterWorkTime::userAddInterval( QString title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userAddInterval( " << title << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userChangeTypeDay
 * \param type Тип дня
 */
void PresenterWorkTime::userChangeTypeDay( int type )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userChangeTypeDay( " << type << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userChangeNote
 * \param note Текст заметки
 */
void PresenterWorkTime::userChangeNote( QString note )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userChangeNote( " << note << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::connectView
 * \param View Указатель на View
 *
 * Соединение сигналов от View со слотами.
 */
void PresenterWorkTime::connectView( IWorkTime * View )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::PresenterWorkTime(...)";
#endif

    QObject * ViewObj = dynamic_cast<QObject*>( View );

    QObject::connect( ViewObj, SIGNAL(userSelectDate     (QDate       )), SLOT(userSelectDate     (QDate       )) );
    QObject::connect( ViewObj, SIGNAL(userSelectInterval (int         )), SLOT(userSelectInterval (int         )) );
    QObject::connect( ViewObj, SIGNAL(userRemoveInterval (int         )), SLOT(userRemoveInterval (int         )) );
    QObject::connect( ViewObj, SIGNAL(userRenameInterval (int, QString)), SLOT(userRenameInterval (int, QString)) );
    QObject::connect( ViewObj, SIGNAL(userChangeTimeStart(int, WTime  )), SLOT(userChangeTimeStart(int, WTime  )) );
    QObject::connect( ViewObj, SIGNAL(userChangeTimeEnd  (int, WTime  )), SLOT(userChangeTimeEnd  (int, WTime  )) );
    QObject::connect( ViewObj, SIGNAL(userChangeTimeNeed ( WTime      )), SLOT(userChangeTimeNeed (WTime       )) );
    QObject::connect( ViewObj, SIGNAL(userAddInterval    (QString     )), SLOT(userAddInterval    (QString     )) );
    QObject::connect( ViewObj, SIGNAL(userChangeTypeDay  (int         )), SLOT(userChangeTypeDay  (int         )) );
    QObject::connect( ViewObj, SIGNAL(userChangeNote     (QString     )), SLOT(userChangeNote     (QString     )) );
}
// ------------------------------------------------------------------------------------ //
