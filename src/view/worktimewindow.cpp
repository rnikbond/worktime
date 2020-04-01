// ---------------------------- //
// ---------------------------- //
// ---------------------------- //
#include "worktimewindow.h"
#include "ui_worktimewindow.h"
// ---------------------------- //

WorkTimeWindow::WorkTimeWindow( QWidget * parent ) : QWidget( parent ), gui( new Ui::WorkTimeWindow )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#call WorkTimeWindow::WorkTimeWindow(...)";
#endif

    configuringGUI();
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
}
// ------------------------------------------------------------------------------------ //

