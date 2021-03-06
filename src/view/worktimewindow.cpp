// ---------------------------- //
#include <QMenu>
#include <QDebug>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDesktopWidget>
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

    isRunTimer = true;

    gui->InfoUserEdit->hide();
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
 * \brief WorkTimeWindow::setInfoUser
 * \param text Информация для пользователя
 */
void WorkTimeWindow::setInfoUser( QString text )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setInfoUser( " << text << " )";
#endif

    QString oldInfo = gui->InfoUserEdit->text();

    gui->InfoUserEdit->setText( oldInfo.isEmpty() ? text : oldInfo + "|" + text);
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
 * \brief WorkTimeWindow::contextMenuNote
 *
 * Отображение контекстного меню в редакторе заметки
 */
void WorkTimeWindow::contextMenuNote( QPoint MenuPoint )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "call contextMenuNote(" << MenuPoint << ")";
#endif

    QMenu NoteMenu;

    NoteMenu.addAction( tr("В HTML"         ), this, SLOT(convertToHtml      ()) );
    NoteMenu.addAction( tr("В обычный текст"), this, SLOT(convertToSimpleText()) );

    NoteMenu.exec( gui->NoteEdit->mapToGlobal(MenuPoint) );
}
// ------------------------------------------------------------------------------------ //

void WorkTimeWindow::checkedUserData( bool isChecked )
{
    gui->NeedDayValue->setReadOnly( !isChecked );
    emit userChangeTypeData( isChecked );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::convertToHtml
 *
 * Конвертирование текста в HTML
 */
void WorkTimeWindow::convertToHtml()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::convertToHtml()";
#endif

    gui->NoteEdit->blockSignals( true );

    QString TextHTML = gui->NoteEdit->toPlainText();
    gui->NoteEdit->clear();
    gui->NoteEdit->appendHtml( TextHTML );

    gui->NoteEdit->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::convertToHtml
 *
 * Конвертирование текста из HTML в обычный текст
 */
void WorkTimeWindow::convertToSimpleText()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::convertToSimpleText()";
#endif

    gui->NoteEdit->blockSignals( true );

    QString TextHTML = gui->NoteEdit->toPlainText();
    gui->NoteEdit->setPlainText( TextHTML );

    gui->NoteEdit->blockSignals( false );
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
 * \brief WorkTimeWindow::selectPage
 * \param page Индекс выбранной страницы
 *
 * Испускается сигнал WorkTimeWindow::userSelectPage.
 */
void WorkTimeWindow::selectPage( int page )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::selectPage( " << page << " )";
#endif

    emit userSelectPage( page );
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
 * \brief WorkTimeWindow::setSelectedDate
 * \param date Дата, которая должна быть выбрана
 */
void WorkTimeWindow::setSelectedDate( const QDate & date )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setSelectedDate( " << date << ")";
#endif

    gui->WorkCalendar->setSelectedDate( date );
    selectDate();
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
    gui->NoteEdit->clear();
    gui->NoteEdit->appendHtml( note );
    gui->NoteEdit->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

void WorkTimeWindow::setTypeData( int type )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTypeData( " << type << ")";
#endif

    gui->UserDataCheck->blockSignals( true );
    gui->UserDataCheck->setChecked( type );
    gui->UserDataCheck->blockSignals( false );

    gui->NeedDayValue->setReadOnly( !type );
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
    qDebug() << "#call WorkTimeWindow::setTimeStatisticInDay( " << time.toString() << ", " << title << ")";
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
    qDebug() << "#call WorkTimeWindow::setTimeStatisticInWeek( " << time.toString() << ", " << title << ")";
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
    qDebug() << "#call WorkTimeWindow::setTimeStatisticInMonth( " << time.toString() << ", " << title << ")";
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
    qDebug() << "#call WorkTimeWindow::setTimeStatisticToDay( " << time.toString() << ", " << title << ")";
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

    setWindowTitle( info );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTimeReverseTimer
 * \param time Таймер обратного отсчета рабочего времени
 */
void WorkTimeWindow::setTimeReverseTimer( QString time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::setTimeReverseTimer( " << time << ")";
#endif

    gui->ReverseTimer->setText( time );
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

    if( gui->WorkCalendar->selectedDate() == HelperWT::currentDate() )
    {
        if( gui->TimeEndValue->hasFocus() == false )
        {
            if( gui->IntervalsList->currentRow() == id )
            {
                gui->TimeEndValue->blockSignals( true );
                gui->TimeEndValue->setTime( time.toQTime() );
                gui->TimeEndValue->blockSignals( false );
            }
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setShownMenu
 * \param isShown Признак отображения меню
 */
void WorkTimeWindow::setShownMenu( bool isShown )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::setShownMenu(" << isShown << ")";
#endif

    gui->MenuGBox     ->setVisible( isShown );
    gui->StatisticGBox->setVisible( isShown );

    if( isShown )
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
 * \brief WorkTimeWindow::setSelectedPage
 * \param page Индекс выбранной страницы
 */
void WorkTimeWindow::setSelectedPage( int page )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::setSelectedPage(" << page << ")";
#endif

    gui->StatisticToolBox->setCurrentIndex( page );
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

    //gui->IntervalsList->takeItem( IntervalRow );

    emit userRemoveInterval( IntervalRow );

    gui->IntervalsList->setCurrentRow( gui->IntervalsList->count() - 1 );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::MenuClick
 * Скрытие/Отображение меню и статистики за день/неделю/месяц
 *
 * При отображении меню для каленаря устанавливается короткий формат дней недели.
 * При скрытом меню для каленаря устанавливается длинный формат дней недели.
 *
 * Испускается сигнал WorkTimeWindow::userChangeVisibleMenu.
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

    emit userChangeVisibleMenu( visibleState );
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

    emit showSalary();
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

    emit showTableTime();
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

    emit showSchedule();
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

    emit showSeveralDays();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::CalcTimeClick
 * Выозов окна "Калькулятор времени"
 */
void WorkTimeWindow::CalcTimeClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::CalcTimeClick()";
#endif
    emit showCalcTime();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::NotifyClick
 * Вызов окна "Оповещения"
 */
void WorkTimeWindow::NotifyClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::CalcTimeClick()";
#endif

    emit showNotify();
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

    emit showChanges();
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

    emit showSettings();
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


    emit userDropUpdate( "" );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::runTimeClick
 *
 * Запуск таймера
 */
void WorkTimeWindow::runTimeClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::runTimeClick()";
#endif

    isRunTimer = true;

    gui->RunTimeButton ->hide();
    gui->StopTimeButton->show();

    emit runTimer( isRunTimer, true );

    gui->StopTimeButton->setFocus();
}
// ------------------------------------------------------------------------------------ //


/*!
 * \brief WorkTimeWindow::stopTimeClick
 *
 * Остановка таймера
 */
void WorkTimeWindow::stopTimeClick()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::stopTimeClick()";
#endif

    isRunTimer = false;

    gui->RunTimeButton ->show();
    gui->StopTimeButton->hide();

    emit runTimer( isRunTimer, false );

    gui->RunTimeButton->setFocus();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setSalaryExists
 * \param isExists Признак наличия окна "Зарплата".
 *
 * Если \a isExists == TRUE, кнопка для открытия окна "Зарплата" будет отображена.
 * Иначе она будет скрыта с формы.
 */
void WorkTimeWindow::setSalaryExists( bool isExists )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::setSalaryExists( " << isExists << " )";
#endif

    gui->SalaryButton->setVisible( isExists );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setTableTimeExists
 * \param isExists Признак наличия окна "Таблица времени".
 *
 * Если \a isExists == TRUE, кнопка для открытия окна "Таблица времени" будет отображена.
 * Иначе она будет скрыта с формы.
 */
void WorkTimeWindow::setTableTimeExists( bool isExists )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::setTableTimeExists( " << isExists << " )";
#endif

    gui->TableTimeButton->setVisible( isExists );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setScheduleExists
 * \param isExists Признак наличия окна "Расписание".
 *
 * Если \a isExists == TRUE, кнопка для открытия окна "Расписание" будет отображена.
 * Иначе она будет скрыта с формы.
 */
void WorkTimeWindow::setScheduleExists( bool isExists )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::setScheduleExists( " << isExists << " )";
#endif

    gui->ScheduleButton->setVisible( isExists );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setSeveralDaysExists
 * \param isExists Признак наличия окна "Несколько дней".
 *
 * Если \a isExists == TRUE, кнопка для открытия окна "Несколько дней" будет отображена.
 * Иначе она будет скрыта с формы.
 */
void WorkTimeWindow::setSeveralDaysExists( bool isExists )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::setSeveralDaysExists( " << isExists << " )";
#endif

    gui->SeveralDaysButton->setVisible( isExists );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setChangesExists
 * \param isExists Признак наличия окна "Изменения".
 *
 * Если \a isExists == TRUE, кнопка для открытия окна "Изменения" будет отображена.
 * Иначе она будет скрыта с формы.
 */
void WorkTimeWindow::setChangesExists( bool isExists )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::setChangesExists( " << isExists << " )";
#endif

    gui->ChangesButton->setVisible( isExists );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setSettingsWidget
 * \param isExists Признак наличия окна "Настройки".
 *
 * Если \a isExists == TRUE, кнопка для открытия окна "Настройки" будет отображена.
 * Иначе она будет скрыта с формы.
 */
void WorkTimeWindow::setSettingsExists( bool isExists )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::setSettingsWidget( " << isExists << " )";
#endif

    gui->SettingsButton->setVisible( isExists );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setCalcTimeExists
 * \param isExists == TRUE, кнопка для открытия окна "Калькулятор времени" будет отображена.
 * Иначе она будет скрыта с формы.
 */
void WorkTimeWindow::setCalcTimeExists( bool isExists )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::setCalcTimeExists( " << isExists << " )";
#endif

    gui->CalcTimeButton->setVisible( isExists );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setNotifyExists
 * \param isExists == TRUE, кнопка для открытия окна "Оповещения" будет отображена.
 * Иначе она будет скрыта с формы.
 */
void WorkTimeWindow::setNotifyExists( bool isExists )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call WorkTimeWindow::setNotifyExists( " << isExists << " )";
#endif

    gui->NotifyButton->setVisible( isExists );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::setUpdatesExists
 * \param isExists == TRUE, кнопка для открытия окна "Обновления" будет отображена.
 * Иначе она будет скрыта с формы.
 */
void WorkTimeWindow::setUpdatesExists( bool isExists )
{
    gui->UpdateButton->setVisible( isExists );
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

    connect( gui->StopTimeButton, SIGNAL(clicked(bool)), SLOT(stopTimeClick()) );
    connect( gui->RunTimeButton , SIGNAL(clicked(bool)), SLOT(runTimeClick ()) );

    // -------------------------------- CALENDAR -------------------------------- //
    connect( gui->WorkCalendar, SIGNAL(selectionChanged(    )), SLOT(selectDate()) );
    connect( gui->TodayButton , SIGNAL(clicked         (bool)), SLOT(todayClick()) );

    // -------------------------------- MENU -------------------------------- //
    connect( gui->MenuButton       , SIGNAL(clicked(bool)), SLOT(MenuClick       ()) );
    connect( gui->SalaryButton     , SIGNAL(clicked(bool)), SLOT(SalaryClick     ()) );
    connect( gui->TableTimeButton  , SIGNAL(clicked(bool)), SLOT(TableTimeClick  ()) );
    connect( gui->ScheduleButton   , SIGNAL(clicked(bool)), SLOT(ScheduleClick   ()) );
    connect( gui->SeveralDaysButton, SIGNAL(clicked(bool)), SLOT(SeveralDaysClick()) );
    connect( gui->CalcTimeButton   , SIGNAL(clicked(bool)), SLOT(CalcTimeClick   ()) );
    connect( gui->NotifyButton     , SIGNAL(clicked(bool)), SLOT(NotifyClick     ()) );
    connect( gui->ChangesButton    , SIGNAL(clicked(bool)), SLOT(ChangesClick    ()) );
    connect( gui->SettingsButton   , SIGNAL(clicked(bool)), SLOT(SettingsClick   ()) );
    connect( gui->UpdateButton     , SIGNAL(clicked(bool)), SLOT(UpdateClick     ()) );

    // -------------------------------- TYPE DAY -------------------------------- //
    connect( gui->TypeDayCBox, SIGNAL(currentIndexChanged(int)), SLOT(changeTypeDay(int)) );

    // -------------------------------- INTERVALS LIST -------------------------------- //
    connect( gui->IntervalsList, SIGNAL(currentRowChanged         (int             )), SLOT(selectInterval      (int             )) );
    connect( gui->IntervalsList, SIGNAL(customContextMenuRequested(QPoint          )), SLOT(contextMenuIntervals(QPoint          )) );
    connect( gui->IntervalsList, SIGNAL(itemChanged               (QListWidgetItem*)), SLOT(renameInterval      (QListWidgetItem*)) );

    connect( gui->UserDataCheck, SIGNAL(clicked(bool)), SLOT(checkedUserData(bool)) );

    // -------------------------------- CHANGE TIME -------------------------------- //
    connect( gui->TimeStartValue, SIGNAL(timeChanged    (QTime)), SLOT(changeTimeStart      (QTime)) );
    connect( gui->TimeEndValue  , SIGNAL(timeChanged    (QTime)), SLOT(changeTimeEnd        (QTime)) );
    connect( gui->NeedDayValue  , SIGNAL(timeChanged    (QTime)), SLOT(changeTimeNeed       (QTime)) );
    connect( gui->TimeStartValue, SIGNAL(editingFinished(     )), SLOT(finishChangeTimeStart(     )) );
    connect( gui->TimeEndValue  , SIGNAL(editingFinished(     )), SLOT(finishChangeTimeEnd  (     )) );

    // -------------------------------- NOTE -------------------------------- //
    connect( gui->NoteEdit, SIGNAL(textChanged()), SLOT(changeNote()) );
    connect( gui->NoteEdit, SIGNAL(customContextMenuRequested(QPoint)), SLOT(contextMenuNote(QPoint)) );

    // -------------------------------- STATISTIC -------------------------------- //
    connect( gui->StatisticToolBox, SIGNAL(currentChanged(int)), SLOT(selectPage(int)) );
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

    setAcceptDrops( true );
    setWindowIcon ( QIcon(":/icons/logo/worktime.png") );
    setWindowFlags( Qt::Window | Qt::WindowCloseButtonHint );

    setMouseTracking( true );

    gui->WorkCalendar->setSelectionMode         ( QCalendarWidget::SingleSelection  );
    gui->WorkCalendar->setHorizontalHeaderFormat( QCalendarWidget::LongDayNames     );
    gui->WorkCalendar->setVerticalHeaderFormat  ( QCalendarWidget::NoVerticalHeader );

    QTextCharFormat WeekendFomat = gui->WorkCalendar->weekdayTextFormat( Qt::Saturday );

    WeekendFomat.setForeground( QBrush( DayWorkTime::colorDay(DayWorkTime::Weekend)) );

    gui->WorkCalendar->setWeekdayTextFormat( Qt::Saturday, WeekendFomat );
    gui->WorkCalendar->setWeekdayTextFormat( Qt::Sunday  , WeekendFomat );

    gui->IntervalsList->setSelectionMode    ( QAbstractItemView::SingleSelection );
    gui->IntervalsList->setContextMenuPolicy( Qt::CustomContextMenu              );

    gui->TypeDayCBox->addItems( DayWorkTime::namesDay() );

    gui->NoteEdit->setAcceptDrops( false );
    gui->NoteEdit->setContextMenuPolicy( Qt::CustomContextMenu );

    setSalaryExists     ( false );
    setTableTimeExists  ( false );
    setScheduleExists   ( false );
    setSeveralDaysExists( false );
    setChangesExists    ( false );
    setSettingsExists   ( false );
    setCalcTimeExists   ( false );
    setNotifyExists     ( false );

    gui->UpdateButton->hide();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::showEvent
 * \param ShowEvent
 */
void WorkTimeWindow::showEvent( QShowEvent * ShowEvent )
{
    isRunTimer = true;

    gui->RunTimeButton ->hide();
    gui->StopTimeButton->show();

    emit runTimer( isRunTimer, false );

    gui->WorkCalendar->setFocus();

    QWidget::showEvent( ShowEvent );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::hideEvent
 * \param HideEvent
 */
void WorkTimeWindow::hideEvent( QHideEvent * HideEvent )
{
    gui->WorkCalendar->setSelectedDate( HelperWT::currentDate() );
    selectDate();

    QWidget::hideEvent( HideEvent );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::closeEvent
 * \param HideEvent Указатель на событие скрытия окна
 *
 * Испускается сигнал WorkTimeWindow::changedGeometry.
 * Испускается сигнал WorkTimeWindow::closeWindow.
 */
void WorkTimeWindow::closeEvent( QCloseEvent * CloseEvent )
{
    emit changedGeometry( geometry() );
    emit closeWindow();

    CloseEvent->ignore();
}
// ------------------------------------------------------------------------------------ //

void WorkTimeWindow::dragEnterEvent( QDragEnterEvent* DragEnterEvent )
{
    DragEnterEvent->accept();
}
// ------------------------------------------------------------------------------------ //

void WorkTimeWindow::dropEvent( QDropEvent * DropEvent )
{
    QString filePath = DropEvent->mimeData()->urls()[0].toLocalFile();

    if( QFile(filePath).exists() && QFileInfo(filePath).isExecutable() )
    {
        emit userDropUpdate( filePath );
    }
    else
    {
        QMessageBox MsgBox( QMessageBox::Warning, "", QObject::tr("Некорректный файл для обновления") );
        MsgBox.exec();
    }
}
// ------------------------------------------------------------------------------------ //
