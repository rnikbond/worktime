// ---------------------------- //
#include <QTextCharFormat>
// ---------------------------- //
#include "helperwt.h"
#include "databasewt.h"
#include "monthworktime.h"
// ---------------------------- //
#include "severaldayswindow.h"
#include "ui_severaldayswindow.h"
// ---------------------------- //

SeveralDaysWindow::SeveralDaysWindow( QWidget * parent ) : QWidget( parent ), gui( new Ui::SeveralDaysWindow )
{
    gui->setupUi( this );

    workingRate = HelperWT::UnknownWR;

    DataBase = NULL;

    Variant = NoneSelection;

    QDate current = HelperWT::currentDate();

    gui->DateStartEdit->setMinimumDate( QDate(current.year(),  1,  1) );
    gui->DateStartEdit->setMaximumDate( QDate(current.year(), 12, 30) );
    gui->DateEndEdit  ->setMinimumDate( QDate(current.year(),  1,  2) );
    gui->DateEndEdit  ->setMaximumDate( QDate(current.year(), 12, 31) );

    gui->TypeDaysCBox->addItems( DayWorkTime::namesDay() );

    WorkMonth = new MonthWorkTime( this );

    connect( gui->DaysCalendar    , SIGNAL(clicked(QDate)), SLOT(selectDate  (    )) );
    connect( gui->ResetButton     , SIGNAL(clicked(bool )), SLOT(clear       (    )) );
    connect( gui->SaveButton      , SIGNAL(clicked(bool )), SLOT(saveClick   (    )) );
    connect( gui->StartDateRButton, SIGNAL(clicked(bool )), SLOT(startChecked(bool)) );
    connect( gui->EndDateRButton  , SIGNAL(clicked(bool )), SLOT(endChecked  (bool)) );
}
// ------------------------------------------------------------------------------------ //

SeveralDaysWindow::~SeveralDaysWindow()
{
    delete gui;
}
// ------------------------------------------------------------------------------------ //

void SeveralDaysWindow::startChecked( bool )
{
    gui->TypeDaysCBox->setEnabled( false );
    gui->SaveButton  ->setEnabled( false );
}
// ------------------------------------------------------------------------------------ //

void SeveralDaysWindow::endChecked( bool )
{
}
// ------------------------------------------------------------------------------------ //

void SeveralDaysWindow::clear()
{
    SelectedDate = HelperWT::currentDate();

    gui->DateStartEdit->setDate( gui->DateStartEdit->minimumDate() );
    gui->DateEndEdit  ->setDate( gui->DateEndEdit  ->minimumDate() );
    gui->DaysCalendar->setMinimumDate( gui->DateStartEdit->minimumDate() );
    gui->DaysCalendar->setMaximumDate( gui->DateStartEdit->maximumDate() );

    gui->DateStartEdit->setEnabled( false );
    gui->DateEndEdit  ->setEnabled( false );
    gui->TypeDaysCBox ->setEnabled( false );
    gui->SaveButton   ->setEnabled( false );

    gui->TypeDaysCBox->setCurrentIndex( 0 );

    gui->StepInfoLabel->setText( tr("Выберете дату начала") );

    gui->DaysCalendar->setSelectedDate( SelectedDate );

    gui->TypeDaysCBox->setCurrentIndex( 0 );

    gui->StartDateRButton->setChecked( true );
}
// ------------------------------------------------------------------------------------ //

void SeveralDaysWindow::selectDate()
{
    SelectedDate = gui->DaysCalendar->selectedDate();

    if( gui->StartDateRButton->isChecked() )
    {
        gui->StepInfoLabel->setText( tr("Отлично. Теперь выберете дату конца интервала") );

        loadMonth();

        gui->DaysCalendar->setMinimumDate( SelectedDate );
        gui->DaysCalendar->setMaximumDate( QDate(SelectedDate.year(), SelectedDate.month(), SelectedDate.daysInMonth()) );

        gui->DateStartEdit->setDate( SelectedDate );

        gui->EndDateRButton->setChecked( true );
        endChecked( true );
    }
    else if( gui->EndDateRButton->isChecked() )
    {
        gui->StepInfoLabel->setText( tr("Вы выбрали интервал с %1 по %2.\nТеперь выберете, что это за интервал и затем сохраните.")
                                     .arg(gui->DateStartEdit->date().toString("dd.MM.yyyy"))
                                     .arg(SelectedDate.toString("dd.MM.yyyy")) );

        gui->DateEndEdit->setDate( SelectedDate );

        gui->TypeDaysCBox->setEnabled( true );
        gui->SaveButton  ->setEnabled( true );
    }
}
// ------------------------------------------------------------------------------------ //

void SeveralDaysWindow::saveClick()
{
    QDate Start = gui->DateStartEdit->date();
    QDate End   = gui->DateEndEdit->date();

    if( End > Start && Start.month() == End.month() )
    {

        for( int day = Start.day(); day <= End.day(); day++ )
             DataBase->setTypeDay( QDate(Start.year(), Start.month(), day), workingRate, gui->TypeDaysCBox->currentIndex() );

        clear();

        emit savedChanges();
    }
}
// ------------------------------------------------------------------------------------ //

void SeveralDaysWindow::loadMonth()
{
    WorkMonth->clear();

    if( workingRate == -1 )
        return;

    if( DataBase == NULL )
        return;

    for( int day = 1; day <= SelectedDate.daysInMonth(); day++ )
    {
        QDate Date( SelectedDate.year(), SelectedDate.month(), day );

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
    }

    WorkMonth->fullUpdate( SelectedDate );

    for( int day = 1; day <= SelectedDate.daysInMonth(); day++ )
    {
        QDate Date( SelectedDate.year(), SelectedDate.month(), day );

        QTextCharFormat TextCharFormat = gui->DaysCalendar->dateTextFormat( Date );

        TextCharFormat.setForeground( WorkMonth->colorDay(Date) );

        gui->DaysCalendar->setDateTextFormat( Date, TextCharFormat );
    }
}
// ------------------------------------------------------------------------------------ //

void SeveralDaysWindow::setWorkingRate( int rate )
{
    workingRate = rate;
}
// ------------------------------------------------------------------------------------ //

void SeveralDaysWindow::setDataBase( DataBaseWT * DB )
{
    DataBase = DB;
}
// ------------------------------------------------------------------------------------ //

void SeveralDaysWindow::showEvent( QShowEvent * ShowEvent )
{
    clear();

    QWidget::showEvent( ShowEvent );
}
// ------------------------------------------------------------------------------------ //
