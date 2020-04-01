// ---------------------------- //
// ---------------------------- //
#include "helperwt.h"
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
 * Происходит испускание сигнала
 */
void WorkTimeWindow::changeTimeStart( QTime time )
{

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::changeTimeEnd
 * \param time Изменное пользователем время конца интервала
 *
 * Вызывается при измении пользователем времени конца интервала.
 * Происходит испускание сигнала
 */
void WorkTimeWindow::changeTimeEnd( QTime time )
{

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::changeTimeNeed
 * \param time Изменное пользователем время, сколько нужно отработать за выбранный день
 *
 * Вызывается при измении пользователем времени конца интервала,
 * сколько нужно отработать за выбранный день.
 * Происходит испускание сигнала
 */
void WorkTimeWindow::changeTimeNeed( QTime time )
{

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::finishChangeTimeStart
 * Вызывается при окончании редактирования времени начала интервала.
 */
void WorkTimeWindow::finishChangeTimeStart()
{

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief WorkTimeWindow::finishChangeTimeEnd
 * Вызывается при окончании редактирования времени конца интервала.
 */
void WorkTimeWindow::finishChangeTimeEnd()
{

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
#ifdef QT_INFO_CALL_FUNC
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

    // -------------------------------- CHANGE TIME -------------------------------- //
    connect( gui->TimeStartValue, SIGNAL(timeChanged    (QTime)), SLOT(changeTimeStart      (QTime)) );
    connect( gui->TimeEndValue  , SIGNAL(timeChanged    (QTime)), SLOT(changeTimeEnd        (QTime)) );
    connect( gui->NeedDayValue  , SIGNAL(timeChanged    (QTime)), SLOT(changeTimeNeed       (QTime)) );
    connect( gui->TimeStartValue, SIGNAL(editingFinished(     )), SLOT(finishChangeTimeStart(     )) );
    connect( gui->TimeEndValue  , SIGNAL(editingFinished(     )), SLOT(finishChangeTimeEnd  (     )) );
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
