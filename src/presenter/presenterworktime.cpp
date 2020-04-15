// ---------------------------- //
#include <QColor>
#include <QDebug>
// ---------------------------- //
#include "iviewworktime.h"
#include "imodelworktime.h"
#include "idesktopwidget.h"
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
PresenterWorkTime::PresenterWorkTime( QObject * parent ) : QObject( parent )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::PresenterWorkTime(...)";
#endif

    ViewWT  = NULL;
    ModelWT = NULL;
}
// ------------------------------------------------------------------------------------ //

void PresenterWorkTime::setView( IViewWorkTime * View  )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::setView(...)";
#endif

    ViewWT  = View;

    connectView( View );
}
// ------------------------------------------------------------------------------------ //

void PresenterWorkTime::setModel( IModelWorkTime * Model )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::setModel(...)";
#endif

    ModelWT = Model;

    connectModel( Model );
}
// ------------------------------------------------------------------------------------ //

void PresenterWorkTime::setWidget( IDesktopWidget * Widget )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::setWidget(...)";
#endif

    Desktop = Widget;
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

    ViewWT->setColorDays( ModelWT->colorMonthDays() );

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

    ViewWT->setTypeDay  ( ModelWT->typeDay  () );
    ViewWT->setIntervals( ModelWT->intervals() );
    ViewWT->setNote     ( ModelWT->note     () );

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

    WTime StatisticTimeInDay = ModelWT->timeStatisticInDay();
    WTime StatisticTimeToDay = ModelWT->timeStatisticToDay();

    QString InfoInDay = ModelWT->infoStatisticInDay();
    QString InfoToDay = ModelWT->infoStatisticToDay();

    ViewWT->setTimeWorkedInDay   ( ModelWT->timeWorkedInDay() );
    ViewWT->setTimeStatisticInDay( StatisticTimeInDay, InfoInDay );
    ViewWT->setTimeNeedInDay     ( ModelWT->timeNeedInDay() );

    ViewWT->setTimeWorkedToDay   ( ModelWT->timeWorkedToDay() );
    ViewWT->setTimeStatisticToDay( StatisticTimeToDay, InfoToDay );
    ViewWT->setTimeNeedToDay     ( ModelWT->timeNeedToDay() );
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

    WTime StatisticTime = ModelWT->timeStatisticInWeek();
    QString Info = ModelWT->infoStatisticInWeek();

    ViewWT->setTimeWorkedInWeek   ( ModelWT->timeWorkedInWeek() );
    ViewWT->setTimeStatisticInWeek( StatisticTime, Info );
    ViewWT->setTimeNeedInWeek     ( ModelWT->timeNeedInWeek() );
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

    WTime StatisticTime = ModelWT->timeStatisticInMonth();
    QString Info = ModelWT->infoStatisticInMonth();

    ViewWT->setTimeWorkedInMonth   ( ModelWT->timeWorkedInMonth() );
    ViewWT->setTimeStatisticInMonth( StatisticTime, Info );
    ViewWT->setTimeNeedInMonth     ( ModelWT->timeNeedInMonth() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshStatisticTime
 */
void PresenterWorkTime::refreshStatisticTime()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshStatisticTime()";
#endif

    refreshTimeDay  ();
    refreshTimeWeek ();
    refreshTimeMonth();
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
    qDebug() << "#Call PresenterWorkTime::refreshTimeStart( " << id << ", " << time.toString() << " )";
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
    qDebug() << "#Call PresenterWorkTime::refreshTimeEnd( " << id << ", " << time.toString() << " )";
#endif

    ViewWT->updateTimeEnd( id, time );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshTimeReverse
 * \param time Время таймера обратного отсчета
 */
void PresenterWorkTime::refreshTimeReverse( QString time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshTimeReverse( " << time << " )";
#endif

    ViewWT->setTimeReverseTimer( time );
    Desktop->setReverseInfo( time );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::refreshInfoEscape
 * \param time Время ухода
 * \param info Информация об оставшемся времени
 */
void PresenterWorkTime::refreshInfoEscape( QString info )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::refreshInfoEscape( " << info << " )";
#endif

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

    ModelWT->setDate( date );

    refreshDataDay();
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

    ViewWT->setTimeStart( ModelWT->timeStart(id), id );
    ViewWT->setTimeEnd  ( ModelWT->timeEnd  (id), id );
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

    ModelWT->removeInterval( id );
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

    ModelWT->renameInterval( id, title );
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

    ModelWT->setTimeStart( id, time );
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

    ModelWT->setTimeEnd( id, time );
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

    ModelWT->setTimeNeed( time );
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

    ModelWT->addInterval( title );
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

    ModelWT->setTypeDay( type );

    refreshDataDay();
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

    ModelWT->setNote( note );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::userChnageTimerState
 * \param state
 */
void PresenterWorkTime::userChangeTimerState( bool state )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::userChnageTimerState( " << state << " )";
#endif

    ModelWT->setTimerState( state );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::connectModel
 *
 * Соединение сигналов от Model
 */
void PresenterWorkTime::connectModel( IModelWorkTime * Model )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::connectModel()";
#endif

    QObject * ModelObj = dynamic_cast<QObject*>( Model );

    connect( ModelObj, SIGNAL(reloadStatisticTime(          )), SLOT(refreshStatisticTime(          )) );
    connect( ModelObj, SIGNAL(refreshTimeStart   (int, WTime)), SLOT(refreshTimeStart    (int, WTime)) );
    connect( ModelObj, SIGNAL(refreshTimeEnd     (int, WTime)), SLOT(refreshTimeEnd      (int, WTime)) );
    connect( ModelObj, SIGNAL(reloadMonth        (          )), SLOT(refreshFull         (          )) );
    connect( ModelObj, SIGNAL(reloadWeek         (          )), SLOT(refreshTimeWeek     (          )) );
    connect( ModelObj, SIGNAL(reloadDay          (          )), SLOT(refreshDataDay      (          )) );
    connect( ModelObj, SIGNAL(updateEscape       (QString   )), SLOT(refreshInfoEscape   (QString   )) );
    connect( ModelObj, SIGNAL(updateReverseTimer (QString   )), SLOT(refreshTimeReverse  (QString   )) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief PresenterWorkTime::connectView
 * \param View Указатель на View
 *
 * Соединение сигналов от View со слотами.
 */
void PresenterWorkTime::connectView( IViewWorkTime * View )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call PresenterWorkTime::PresenterWorkTime(...)";
#endif

    QObject * ViewObj = dynamic_cast<QObject*>( View );

    QObject::connect( ViewObj, SIGNAL(userSelectDate     (QDate       )), SLOT(userSelectDate      (QDate       )) );
    QObject::connect( ViewObj, SIGNAL(userSelectInterval (int         )), SLOT(userSelectInterval  (int         )) );
    QObject::connect( ViewObj, SIGNAL(userRemoveInterval (int         )), SLOT(userRemoveInterval  (int         )) );
    QObject::connect( ViewObj, SIGNAL(userRenameInterval (int, QString)), SLOT(userRenameInterval  (int, QString)) );
    QObject::connect( ViewObj, SIGNAL(userChangeTimeStart(int, WTime  )), SLOT(userChangeTimeStart (int, WTime  )) );
    QObject::connect( ViewObj, SIGNAL(userChangeTimeEnd  (int, WTime  )), SLOT(userChangeTimeEnd   (int, WTime  )) );
    QObject::connect( ViewObj, SIGNAL(userChangeTimeNeed ( WTime      )), SLOT(userChangeTimeNeed  (WTime       )) );
    QObject::connect( ViewObj, SIGNAL(userAddInterval    (QString     )), SLOT(userAddInterval     (QString     )) );
    QObject::connect( ViewObj, SIGNAL(userChangeTypeDay  (int         )), SLOT(userChangeTypeDay   (int         )) );
    QObject::connect( ViewObj, SIGNAL(userChangeNote     (QString     )), SLOT(userChangeNote      (QString     )) );
    QObject::connect( ViewObj, SIGNAL(runTimer           (bool        )), SLOT(userChangeTimerState(bool        )) );
}
// ------------------------------------------------------------------------------------ //
