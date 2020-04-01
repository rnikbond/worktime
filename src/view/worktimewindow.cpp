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
 * \brief WorkTimeWindow::todayClick
 * Выбор сегодняшнего дня в календаре
 */
void WorkTimeWindow::todayClick()
{
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
    connect( gui->WorkCalendar, SIGNAL(selectionChanged(    )), SLOT(selectDate()) );
    connect( gui->TodayButton , SIGNAL(clicked         (bool)), SLOT(todayClick()) );
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
