// ---------------------------- //
#include <QMenu>
// ---------------------------- //
#include "helperwt.h"
#include "dayworktime.h"
// ---------------------------- //
#include "worktimewindow.h"
#include "ui_worktimewindow.h"
// ---------------------------- //

/*!
 * \brief WorkTimeWindow::WorkTimeWindow
 * \param parent Указатель на родительский объект
 *
 * В конструкторе класса вызывается метод \sa WorkTimeWindow::configuringGUI()
 * для графической настройки.
 */
WorkTimeWindow::WorkTimeWindow( QWidget * parent ) : QWidget( parent ), gui( new Ui::WorkTimeWindow )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::WorkTimeWindow(...)";
#endif

    configuringGUI();
    connectSingnalSlot();
}
// ------------------------------------------------------------------------------------ //

WorkTimeWindow::~WorkTimeWindow()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::~WorkTimeWindow()";
#endif

    delete gui;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::changeTimeStart
 * \param time Изменное пользователем время начала интервала
 *
 * Вызывается при измении пользователем времени начала интервала.
 * Происходит испускание сигнала WorkTimeWindow::userChangeTimeStart.
 */
void WorkTimeWindow::changeTimeStart( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::changeTimeStart( " << time << " )";
#endif

    if( gui->IntervalsList->currentRow() >= 0 )
        emit userChangeTimeStart( gui->IntervalsList->currentRow(), WTime(time) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::changeTimeEnd
 * \param time Изменное пользователем время конца интервала
 *
 * Вызывается при измении пользователем времени конца интервала.
 * Происходит испускание сигнала WorkTimeWindow::userChangeTimeEnd.
 */
void WorkTimeWindow::changeTimeEnd( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::changeTimeEnd( " << time << " )";
#endif

    if( gui->IntervalsList->currentRow() >= 0 )
        emit userChangeTimeEnd( gui->IntervalsList->currentRow(), WTime(time) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::changeTimeNeed
 * \param time Изменное пользователем время, сколько нужно отработать за выбранный день
 *
 * Вызывается при измении пользователем времени конца интервала,
 * сколько нужно отработать за выбранный день.
 * Происходит испускание сигнала WorkTimeWindow::userChangeTimeNeed.
 */
void WorkTimeWindow::changeTimeNeed( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::changeTimeNeed( " << time << " )";
#endif

    emit userChangeTimeNeed( WTime(time) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::finishChangeTimeStart
 * Вызывается при окончании редактирования времени начала интервала.
 *
 * Испускается сигнал WorkTimeWindow::userChangeTimeStart.
 */
void WorkTimeWindow::finishChangeTimeStart()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::finishChangeTimeStart()";
#endif

    if( gui->IntervalsList->currentRow() >= 0 )
        emit userChangeTimeStart( gui->IntervalsList->currentRow(), WTime(gui->TimeStartValue->time())  );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::finishChangeTimeEnd
 * Вызывается при окончании редактирования времени конца интервала.
 *
 * Испускается сигнал WorkTimeWindow::userChangeTimeEnd.
 */
void WorkTimeWindow::finishChangeTimeEnd()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::finishChangeTimeEnd()";
#endif

    if( gui->IntervalsList->currentRow() >= 0 )
        emit userChangeTimeEnd( gui->IntervalsList->currentRow(), WTime(gui->TimeEndValue->time()) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::changeNote
 *
 * Редактирование заметки.
 *
 * Испускается сигнал WorkTimeWindow::userChangeNote.
 */
void WorkTimeWindow::changeNote()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::changeNote()";
#endif

    emit userChangeNote( gui->NoteEdit->toPlainText() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::todayClick
 * Выбор сегодняшнего дня в календаре
 */
void WorkTimeWindow::todayClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::todayClick()";
#endif

    gui->WorkCalendar->setSelectedDate( HelperWT::currentDate() );
    selectDate();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::changeTypeDay
 * \param type Тип дня, выбранный пользователем
 *
 * Испускается сигнал \a WorkTimeWindow::userChangeTypeDay.
 *
 * Если пользователь выбрал рабочий день, поля ввода времени начала и конца интервала,
 * а также список интервалов активитуется. Если не рабочий день - блокируются.
 */
void WorkTimeWindow::changeTypeDay( int type )
{
    emit userChangeTypeDay( type );

    gui->WorkCalendar->setFocus();

    if( DayWorkTime::TypesDay(type) != DayWorkTime::WorkDay )
    {
        gui->IntervalsList ->setEnabled( false );
        gui->TimeStartValue->setEnabled( false );
        gui->TimeEndValue  ->setEnabled( false );

        gui->IntervalsList->setCurrentRow( -1 );
    }
    else
    {
        gui->IntervalsList ->setEnabled( true );
        gui->TimeStartValue->setEnabled( true );
        gui->TimeEndValue  ->setEnabled( true );

        gui->IntervalsList->setCurrentRow( gui->IntervalsList->count() - 1 );
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::configuringGUI
 *
 * Выбор дня в календаре
 *
 * При выборе дня испускается сигнал \a  userSelectDate(QDate) и
 * вызывается метод \sa WorkTimeWindow::updateTitles() для смены названий заголовков.
 */
void WorkTimeWindow::selectDate()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::selectDate()";
#endif

    emit userSelectDate( gui->WorkCalendar->selectedDate() );

    updateTitles();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTypesDay
 * \param list Список названий типов дней
 *
 * Выпадающий список "Тип дня" заполняется вариантами из списка \a list.
 */
void WorkTimeWindow::setTypesDay( QStringList list )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTypesDay( " << list << ")";
#endif

    gui->TypeDayCBox->blockSignals( true );

    gui->TypeDayCBox->clear();
    gui->TypeDayCBox->addItems( list );

    gui->TypeDayCBox->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTypeDay
 * \param type Тип дня (индекс)
 *
 * Если тип дня отличный от рабочего, список интервалов и поля
 * для ввода времени начала и времени конца блокируются.
 */
void WorkTimeWindow::setTypeDay( int type )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTypeDay( " << type << ")";
#endif

    gui->TypeDayCBox->blockSignals( true );
    gui->TypeDayCBox->setCurrentIndex( type );
    gui->TypeDayCBox->blockSignals( false );


    if( DayWorkTime::TypesDay(type) != DayWorkTime::WorkDay )
    {
        gui->IntervalsList ->setEnabled( false );
        gui->TimeStartValue->setEnabled( false );
        gui->TimeEndValue  ->setEnabled( false );

        gui->IntervalsList->setCurrentRow( -1 );
    }
    else
    {
        gui->IntervalsList ->setEnabled( true );
        gui->TimeStartValue->setEnabled( true );
        gui->TimeEndValue  ->setEnabled( true );

        gui->IntervalsList->setCurrentRow( gui->IntervalsList->count() - 1 );
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeStart
 * \param time Время начала интервала
 * \param id Идентификатор интервала дня
 */
void WorkTimeWindow::setTimeStart( WTime time, int id )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeStart( " << time.toString() << ", " << id << ")";
#endif

    if( gui->IntervalsList->isEnabled() && gui->IntervalsList->currentRow() != id )
        gui->IntervalsList->setCurrentRow( id );

    gui->TimeStartValue->blockSignals( true );
    gui->TimeStartValue->setTime( time.toQTime() );
    gui->TimeStartValue->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeEnd
 * \param time Время конца интервала
 * \param id Идентификатор интервала дня
 */
void WorkTimeWindow::setTimeEnd( WTime time, int id )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeEnd( " << time.toString() << ", " << id << ")";
#endif

    if( gui->IntervalsList->isEnabled() && gui->IntervalsList->currentRow() != id )
        gui->IntervalsList->setCurrentRow( id );

    gui->TimeEndValue->blockSignals( true );
    gui->TimeEndValue->setTime( time.toQTime() );
    gui->TimeEndValue->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setIntervals
 * \param list Список названий интевалов
 *
 * Список интевалов запоняется интервалами из списка \a list.
 *
 * Если нет ни одного интервала и тип дня - Рабочий, интервал будет добавлен автоматически.
 * После заполнения списка интервалами автоматически выбирается последний.
 */
void WorkTimeWindow::setIntervals( QStringList list )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setIntervals( " << list << ")";
#endif

    gui->IntervalsList->blockSignals( true );

    gui->IntervalsList->clear();

    for( int interval = 0; interval < list.count(); interval++ )
    {
        QListWidgetItem * Interval = new QListWidgetItem( list.at(interval), gui->IntervalsList );
        Interval->setFlags( Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable );

        gui->IntervalsList->addItem( Interval );
    }

    if( gui->IntervalsList->count() == 0 && gui->TypeDayCBox->currentIndex() == DayWorkTime::WorkDay )
        addInterval();

    selectInterval( gui->IntervalsList->count() - 1 );

    gui->IntervalsList->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setNote
 * \param note Текст заметки
 */
void WorkTimeWindow::setNote( QString note )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setNote( " << note << ")";
#endif

    gui->NoteEdit->blockSignals( true );
    gui->NoteEdit->setText( note );
    gui->NoteEdit->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeWorkedInDay
 * \param time Время, сколько отработано за день
 */
void WorkTimeWindow::setTimeWorkedInDay( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeWorkedInDay( " << time.toString() << ")";
#endif

    gui->WorkedDayValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeStatisticInDay
 * \param time Время, сколько недоработано или переработано за день
 * \param Title Заголовок: недоработано или переработано за день
 */
void WorkTimeWindow::setTimeStatisticInDay( WTime time, QString title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeStatisticInDay( " << time.toString() << ", " << Title << ")";
#endif

    gui->StatisticDayLabel->setText( title );
    gui->StatisticDayValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeNeedInDay
 * \param time Время, сколько нужно отработать за день
 */
void WorkTimeWindow::setTimeNeedInDay( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeNeedInDay( " << time.toString() << ")";
#endif

    gui->NeedDayValue->blockSignals( true );
    gui->NeedDayValue->setTime( time.toQTime() );
    gui->NeedDayValue->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeWorkedInWeek
 * \param time Время, сколько отработано за неделю
 */
void WorkTimeWindow::setTimeWorkedInWeek( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeWorkedInWeek( " << time.toString() << ")";
#endif

    gui->WorkedWeekValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeStatisticInWeek
 * \param time Время, сколько недоработано или переработано за неделю
 * \param Title Заголовок: недоработано или переработано за неделю
 */
void WorkTimeWindow::setTimeStatisticInWeek( WTime time, QString title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeStatisticInWeek( " << time.toString() << ", " << Title << ")";
#endif

    gui->StatisticWeekLabel->setText( title );
    gui->StatisticWeekValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeNeedInWeek
 * \param time Время, сколько нужно отработать за неделю
 */
void WorkTimeWindow::setTimeNeedInWeek( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeNeedInWeek( " << time.toString() << ")";
#endif

    gui->NeedWeekValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeWorkedInMonth
 * \param time Время, сколько отработано за месяц
 */
void WorkTimeWindow::setTimeWorkedInMonth( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeWorkedInMonth( " << time.toString() << ")";
#endif

    gui->WorkedMonthValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeStatisticInMonth
 * \param time Время, сколько недоработано или переработано за месяц
 * \param Title Заголовок: недоработано или переработано за месяц
 */
void WorkTimeWindow::setTimeStatisticInMonth( WTime time, QString title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeStatisticInMonth( " << time.toString() << ", " << Title << ")";
#endif

    gui->StatisticMonthLabel->setText( title );
    gui->StatisticMonthValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeNeedInMonth
 * \param time Время, сколько нужно отработать за месяц
 */
void WorkTimeWindow::setTimeNeedInMonth( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeNeedInMonth( " << time.toString() << ")";
#endif

    gui->NeedMonthValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeWorkedToDay
 * \param time Время, сколько отработано c 1-го числа месяца до выбранного дня
 */
void WorkTimeWindow::setTimeWorkedToDay( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeWorkedToDay( " << time.toString() << ")";
#endif

    gui->WorkedToDayValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeStatisticToDay
 * \param time Время, сколько недоработано или переработано c 1-го числа месяца до выбранного дня
 * \param Title Заголовок: недоработано или переработано c 1-го числа месяца до выбранного дня
 */
void WorkTimeWindow::setTimeStatisticToDay( WTime time, QString title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeStatisticToDay( " << time.toString() << ", " << Title << ")";
#endif

    gui->StatisticToDayLabel->setText( title );
    gui->StatisticToDayValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeNeedToDay
 * \param time Время, сколько нужно отработать c 1-го числа месяца до выбранного дня
 */
void WorkTimeWindow::setTimeNeedToDay( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeNeedToDay( " << time.toString() << ")";
#endif

    gui->NeedToDayValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeEscape
 * \param time Время, во сколько сегодня нужно закончить работать
 */
void WorkTimeWindow::setTimeEscape( WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeEscape( " << time.toString() << ")";
#endif

    gui->EscapeValue->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setInfoEscape
 * \param info Подсказка со временем, сколько останется недоработано
 *
 * В случае, если время ухода будет больше, чем максимальное время конца
 *  рабочего дня, будет отображена подсказка, сколько времени останется отработать.
 */
void WorkTimeWindow::setInfoEscape( QString info )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setInfoEscape( " << info << ")";
#endif

    if( info.isEmpty() )
    {
        gui->EsapeInfo->clear();
        gui->EsapeInfo->hide();
    }
    else
    {
        gui->EsapeInfo->setText( info );
        gui->EsapeInfo->show();
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeReverseTimer
 * \param time Таймер обратного отсчета рабочего времени
 */
void WorkTimeWindow::setTimeReverseTimer( WTimeExt time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeReverseTimer( " << time.toString() << ")";
#endif

    gui->ReverseTimer->setText( time.toString() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setColorDay
 * \param date День, цвет которого в календаре нужно изменить
 * \param color Цвет дня
 */
void WorkTimeWindow::setColorDay( QDate date, QColor color )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setColorDay( " << date.toString("dd.MM.yyyy") << ", " << color.name() << ")";
#endif

    QTextCharFormat TextCharFormat = gui->WorkCalendar->dateTextFormat( date );

    TextCharFormat.setForeground( color );

    gui->WorkCalendar->setDateTextFormat( date, TextCharFormat );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setColorDays
 * \param Colors Список названий цветов дял всего месяца
 */
void WorkTimeWindow::setColorDays( QStringList Colors )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setColorDays( " << Colors << ")";
#endif

    const QDate SelectedDate = gui->WorkCalendar->selectedDate();

    for( int day = 1; day <= Colors.count(); day++ )
    {
        QDate Date( SelectedDate.year(), SelectedDate.month(), day );

        QTextCharFormat TextCharFormat = gui->WorkCalendar->dateTextFormat( Date );

        TextCharFormat.setForeground( QColor(Colors.at(day - 1)) );

        gui->WorkCalendar->setDateTextFormat( Date, TextCharFormat );
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setColorLight
 * \param color Цвет подсветки
 *
 * Устанавливается цвет подсветки, означающий, есть ли недоработки.
 */
void WorkTimeWindow::setColorLight( QColor color )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setColorLight( " << color.name() << ")";
#endif

    gui->ReverseTimer->setStyleSheet( QString("border-radius   : 1;    " \
                                              "color           : white;" \
                                              "background-color: %1;"
                                              ).arg( color.name()) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::updateTimeEnd
 * \param id   Идентификатор интервала дня
 * \param time Время конца интервала
 *
 * Обновляется время конца рабочего дня по таймеру.
 */
void WorkTimeWindow::updateTimeEnd( int id, WTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::updateTimeEnd( " << id << ", " << time.toString() << ")";
#endif

    if( gui->WorkCalendar->selectedDate() == QDate::currentDate() )
    {
        if( gui->TimeEndValue->hasFocus() == false )
        {
            if( gui->IntervalsList->currentRow() == id )
            {
                gui->TimeEndValue->setTime( time.toQTime() );
            }
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::selectInterval
 * \param id Идентификатор интервала дня
 *
 * Если \a id >= 0, поля ввода времени начала и конца интервала разблокируются.
 * Иначе поля ввода времени начала и конца блокируются и устанавливается время "00:00".
 */
void WorkTimeWindow::selectInterval( const int id )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::selectInterval(" << id << ")";
#endif

    gui->TimeStartValue->blockSignals( true );
    gui->TimeEndValue  ->blockSignals( true );

    if( id >= 0 )
    {
        gui->TimeStartValue->setEnabled( true );
        gui->TimeEndValue  ->setEnabled( true );

        emit userSelectInterval( id );
    }
    else
    {
        gui->TimeStartValue->setEnabled( false );
        gui->TimeEndValue  ->setEnabled( false );

        gui->TimeStartValue->setTime( QTime(0, 0) );
        gui->TimeEndValue  ->setTime( QTime(0, 0) );
    }

    gui->TimeStartValue->blockSignals( false );
    gui->TimeEndValue  ->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::contextMenuIntervals
 * \param MenuPoint Координата курсора
 *
 * Вызывается контестное меню при клике правой кнопкой мыши  в списке интервалов.
 */
void WorkTimeWindow::contextMenuIntervals( QPoint MenuPoint )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::contextMenuIntervals( QPoint(" << MenuPoint << ") )";
#endif

    QMenu IntervalsContextMenu;

    IntervalsContextMenu.addAction( tr("Добавить интервал"), this, SLOT(addInterval()) );

    if( gui->IntervalsList->currentRow() >= 0 )
    {
        IntervalsContextMenu.addAction( tr("Очистить время"), this, SLOT(clearTime()) );

        if( gui->IntervalsList->count() > 1 )
            IntervalsContextMenu.addAction( tr("Удалить интервал" ), this, SLOT(removeInterval()) );
    }

    IntervalsContextMenu.exec( gui->IntervalsList->mapToGlobal(MenuPoint) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::renameInterval
 * \param RenamedInterval Указатель на интервал в списке интервалов
 *
 * Если при переименовывании интервала название интервала пустое, испускается
 * сигнал \a WorkTimeWindow::userRenameInterval.
 * Если не пустое, испускается сигнал WorkTimeWindow::userSelectDate.
 */
void WorkTimeWindow::renameInterval( QListWidgetItem * RenamedInterval )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::renameInterval( " << RenamedInterval->text() << ")";
#endif

    int intervalID = gui->IntervalsList->currentRow();

    if( intervalID != -1 )
    {
        if( RenamedInterval->text().isEmpty() == false )
        {
            emit userRenameInterval( intervalID, RenamedInterval->text() );
        }
        else
        {
            emit userSelectDate( gui->WorkCalendar->selectedDate() );
            gui->IntervalsList->setCurrentRow( intervalID );
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::clearTime
 *
 * Очистка времени интервала
 *
 * Испускаются сигналы \a WorkTimeWindow::userChangeTimeStart и \a WorkTimeWindow::userChangeTimeEnd
 */
void WorkTimeWindow::clearTime()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::clearTime()";
#endif

    gui->TimeStartValue->blockSignals( true );
    gui->TimeEndValue  ->blockSignals( true );

    gui->TimeStartValue->setTime( QTime(0, 0) );
    gui->TimeEndValue  ->setTime( QTime(0, 0) );

    emit userChangeTimeStart( gui->IntervalsList->currentRow(), WTime() );
    emit userChangeTimeEnd  ( gui->IntervalsList->currentRow(), WTime() );

    gui->TimeStartValue->blockSignals( false );
    gui->TimeEndValue  ->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::addInterval
 * Добавление нового интервала.
 *
 * Испускается сигнал \sa WorkTimeWindow::userAddInterval.
 */
void WorkTimeWindow::addInterval()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::addInterval()";
#endif

    gui->IntervalsList->blockSignals( true );

    QListWidgetItem* NewIntervalItem = new QListWidgetItem( DayWorkTime::nameDay(DayWorkTime::WorkDay), gui->IntervalsList );
    NewIntervalItem->setFlags( Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable );

    emit userAddInterval( NewIntervalItem->text() );

    gui->IntervalsList->blockSignals( false );

    gui->IntervalsList->setCurrentRow( gui->IntervalsList->count() - 1 );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::removeInterval
 *
 * Удаление интервала
 */
void WorkTimeWindow::removeInterval()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::removeInterval()";
#endif

    int IntervalRow = gui->IntervalsList->currentRow();

    emit userRemoveInterval( IntervalRow );

    gui->IntervalsList->takeItem( IntervalRow );

    gui->IntervalsList->setCurrentRow( gui->IntervalsList->count() - 1 );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::MenuClick
 * Скрытие/Отображение меню и статистики за день/неделю/месяц
 *
 * При отображении меню для каленаря устанавливается короткий формат дней недели.
 * При скрытом меню для каленаря устанавливается длинный формат дней недели.
 */
void WorkTimeWindow::MenuClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::MenuClick()";
#endif

    bool visibleState = !gui->MenuGBox->isVisible();

    gui->MenuGBox     ->setVisible( visibleState );
    gui->StatisticGBox->setVisible( visibleState );

    if( visibleState )
    {
        gui->WorkCalendar->setHorizontalHeaderFormat( QCalendarWidget::ShortDayNames );
    }
    else
    {
        gui->WorkCalendar->setHorizontalHeaderFormat( QCalendarWidget::LongDayNames );
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::SalaryClick
 * Вызов окна "Зарплата"
 */
void WorkTimeWindow::SalaryClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::SalaryClick()";
#endif

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::TableTimeClick
 * Вызов окна "Таблица времени"
 */
void WorkTimeWindow::TableTimeClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::TableTimeClick()";
#endif

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::ScheduleClick
 * Вызов окна "Расписание"
 */
void WorkTimeWindow::ScheduleClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::ScheduleClick()";
#endif

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::SeveralDaysClick
 * Выозов окна "Несколько дней"
 */
void WorkTimeWindow::SeveralDaysClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::SeveralDaysClick()";
#endif

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::ChangesClick
 * Вызов окна "Изменения"
 */
void WorkTimeWindow::ChangesClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::ChangesClick()";
#endif

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::SettingsClick
 * Вызов окна "Настройки"
 */
void WorkTimeWindow::SettingsClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::SettingsClick()";
#endif

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::UpdateClick
 * Обновление программы WorkTime.
 * При обновлении с сервера будет скопировано обновление с названием "new_WorkTime.exe" и сохранено
 * в том же каталоге, что и программа.
 * После успешного копирования текущая программа будет переименована в "old_WorkTime.exe".
 * После этого новая программа будет переименована в "WorkTime.exe".
 * Если все вышеописанные пункты были успешно выполнены, будет запущен процесс с новой программой,
 * а текущая программа будет закрыта при помощи метода \a qApp->quit().
 */
void WorkTimeWindow::UpdateClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::UpdateClick()";
#endif

}
// ------------------------------------------------------------------------------------ //

 /*!
  * \brief WorkTimeWindow::updateTitles
  *
  * Смена заголовка группы объектов: Статистика до выбранного дня.
  * Смена заголовка группы объектов: Статистика за день/неделю/месяц.
  */
void WorkTimeWindow::updateTitles()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::updateTitles()";
#endif

    const QDate& Date = gui->WorkCalendar->selectedDate();

    gui->StatisticToolBox->setItemText( DayPage, tr("%1 %2 (%3)").arg(Date.day())
                                                                 .arg(HelperWT::nameMonthInGenitiveCase(Date, false))
                                                                 .arg(QLocale().dayName(Date.dayOfWeek())) );

    gui->StatisticToolBox->setItemText( WeekPage , tr("Неделя №%1").arg(Date.weekNumber()) );
    gui->StatisticToolBox->setItemText( MonthPage, tr("%1").arg(QLocale().monthName(Date.month())) );

    gui->StatisticToDayGBox->setTitle( tr("С 1 по %2 %3").arg(Date.day()).arg(HelperWT::nameMonthInGenitiveCase(Date, false)) );
}
// ------------------------------------------------------------------------------------ //

 /*!
  * \brief WorkTimeWindow::connectSingnalSlot
  * Соединение сигналов со слотами
  */
void WorkTimeWindow::connectSingnalSlot()
{
#ifdef WT_INFO_CALL_FUNC
     qDebug() << "#call WorkTimeWindow::connectSingnalSlot()";
#endif

    // -------------------------------- CALENDAR -------------------------------- //
    connect( gui->WorkCalendar, SIGNAL(selectionChanged(    )), SLOT(selectDate()) );
    connect( gui->TodayButton , SIGNAL(clicked         (bool)), SLOT(todayClick()) );

    // -------------------------------- MENU -------------------------------- //
    connect( gui->MenuButton       , SIGNAL(clicked(bool)), SLOT(MenuClick       ()) );
    connect( gui->SalaryButton     , SIGNAL(clicked(bool)), SLOT(SalaryClick     ()) );
    connect( gui->TableTimeButton  , SIGNAL(clicked(bool)), SLOT(TableTimeClick  ()) );
    connect( gui->ScheduleButton   , SIGNAL(clicked(bool)), SLOT(ScheduleClick   ()) );
    connect( gui->SeveralDaysButton, SIGNAL(clicked(bool)), SLOT(SeveralDaysClick()) );
    connect( gui->ChangesButton    , SIGNAL(clicked(bool)), SLOT(ChangesClick    ()) );
    connect( gui->SettingsButton   , SIGNAL(clicked(bool)), SLOT(SettingsClick   ()) );
    connect( gui->UpdateButton     , SIGNAL(clicked(bool)), SLOT(UpdateClick     ()) );

    // -------------------------------- TYPE DAY -------------------------------- //
    connect( gui->TypeDayCBox, SIGNAL(currentIndexChanged(int)), SLOT(changeTypeDay(int)) );

    // -------------------------------- INTERVALS LIST -------------------------------- //
    connect( gui->IntervalsList, SIGNAL(currentRowChanged         (int             )), SLOT(selectInterval      (int             )) );
    connect( gui->IntervalsList, SIGNAL(customContextMenuRequested(QPoint          )), SLOT(contextMenuIntervals(QPoint          )) );
    connect( gui->IntervalsList, SIGNAL(itemChanged               (QListWidgetItem*)), SLOT(renameInterval      (QListWidgetItem*)) );

    // -------------------------------- CHANGE TIME -------------------------------- //
    connect( gui->TimeStartValue, SIGNAL(timeChanged    (QTime)), SLOT(changeTimeStart      (QTime)) );
    connect( gui->TimeEndValue  , SIGNAL(timeChanged    (QTime)), SLOT(changeTimeEnd        (QTime)) );
    connect( gui->NeedDayValue  , SIGNAL(timeChanged    (QTime)), SLOT(changeTimeNeed       (QTime)) );
    connect( gui->TimeStartValue, SIGNAL(editingFinished(     )), SLOT(finishChangeTimeStart(     )) );
    connect( gui->TimeEndValue  , SIGNAL(editingFinished(     )), SLOT(finishChangeTimeEnd  (     )) );

    // -------------------------------- NOTE -------------------------------- //
    connect( gui->NoteEdit, SIGNAL(textChanged()), SLOT(changeNote()) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::configuringGUI
 *
 * Устанавливается заголовок основного окна программы.
 * Устаналивается стиль рамки окна: Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint.
 * Для календаря убирается вертикальный заголовок и устаналивается длинные названия
 * дней недели. Также устанавливается цвет выходных дней.
 */
void WorkTimeWindow::configuringGUI()
{
#ifdef WT_INFO_CALL_FUNC
     qDebug() << "#call WorkTimeWindow::initialize()";
#endif

     gui->setupUi( this );

     setWindowTitle( tr("WorkTime") );
     setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint );

     gui->WorkCalendar->setSelectionMode         ( QCalendarWidget::SingleSelection  );
     gui->WorkCalendar->setHorizontalHeaderFormat( QCalendarWidget::LongDayNames     );
     gui->WorkCalendar->setVerticalHeaderFormat  ( QCalendarWidget::NoVerticalHeader );

     QTextCharFormat WeekendFomat = gui->WorkCalendar->weekdayTextFormat( Qt::Saturday );

     //WeekendFomat.setForeground( QBrush( DayWorkTime::colorDay(DayWorkTime::Weekend)) );

     gui->WorkCalendar->setWeekdayTextFormat( Qt::Saturday, WeekendFomat );
     gui->WorkCalendar->setWeekdayTextFormat( Qt::Sunday  , WeekendFomat );
}
// ------------------------------------------------------------------------------------ //
