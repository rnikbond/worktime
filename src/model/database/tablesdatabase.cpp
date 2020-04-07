// ------------------------------ //
#include "helperwt.h"
#include "databasewt.h"
// ------------------------------ //
#include "tablesdatabase.h"
#include "ui_tablesdatabase.h"
// ------------------------------ //

TablesDataBase::TablesDataBase( DataBaseWT * DB, QWidget *parent ) : QWidget( parent ), gui( new Ui::TablesDataBase )
{
    gui->setupUi(this);

    DataBase = DB;

    setupTableView();

    UpdateTablesTimer = new QTimer( this );

    connect( gui->UpdateButton, SIGNAL(clicked(bool)), SLOT(udpateFromDataBase()) );
    connect( UpdateTablesTimer, SIGNAL(timeout(    )), SLOT(udpateFromDataBase()) );

    connect( gui->ChangeTimeStartButton, SIGNAL(clicked(bool)), SLOT(changeTimeStart()) );
    connect( gui->ChangeTimeEndButton, SIGNAL(clicked(bool)), SLOT(changeTimeEnd()) );
    connect( gui->RemoveButton, SIGNAL(clicked(bool)), SLOT(removeInterval()) );
    connect( gui->RemoveSalaryButton, SIGNAL(clicked(bool)), SLOT(removeSalary()) );

    UpdateTablesTimer->start( 500 );

    gui->DateEdit->setDate( HelperWT::currentDate() );
}
// --------------------------------------------------------------------------------- //

TablesDataBase::~TablesDataBase()
{
    delete gui;
}
// --------------------------------------------------------------------------------- //

void TablesDataBase::changeTimeStart()
{
    QTime Time = gui->TimeNeed->time();
    int Interval = gui->IntervalEdit->text().toInt();
    QDate Date = gui->DateEdit->date();

    qDebug() << "changeTimeStart ::: " << Date << "\tInterval: " << Interval << "\t Time: " << Time;

    //DataBase->changeTimeStart( Date, 0, Interval, WTime(Time) );
    udpateFromDataBase();
}
// --------------------------------------------------------------------------------- //

void TablesDataBase::changeTimeEnd()
{
//    QTime Time = gui->TimeNeed->time();
//    int Interval = gui->IntervalEdit->text().toInt();
//    QDate Date = gui->DateEdit->date();

//    qDebug() << "changeTimeEnd ::: " << Date << "\tInterval: " << Interval << "\t Time: " << Time;

//    DataBase->changeTimeEnd( Date, 0, Interval, WTime(Time) );
//    udpateFromDataBase();
}
// --------------------------------------------------------------------------------- //

void TablesDataBase::removeInterval()
{
//    int Interval = gui->IntervalEdit->text().toInt();
//    QDate Date = gui->DateEdit->date();

//    qDebug() << "removeInterval ::: " << Date << "\tInterval: " << Interval;

//    DataBase->removeInterval( Date, 0, Interval );
//    udpateFromDataBase();
}
// --------------------------------------------------------------------------------- //

void TablesDataBase::removeSalary()
{
//    int ID = gui->IntervalEdit->text().toInt();

//    qDebug() << "removeSalary ::: " << ID;

//    DataBase->removeSalary( ID );
//    udpateFromDataBase();
}
// --------------------------------------------------------------------------------- //

void TablesDataBase::udpateFromDataBase()
{
    WorkDaysModel     ->select();
    WorkingRatesModel ->select();
    ScheduleTimeModel ->select();
    TimeIntervalsModel->select();
    NotesModel        ->select();
    SalariesModel     ->select();
}
// ------------------------------------------------------------------------------------------ //

void TablesDataBase::setupTableView()
{
    // -----------------------------------------
    WorkDaysModel = new QSqlTableModel( this, DataBase->sqlDatabase() );
    WorkDaysModel->setTable( "WorkDays" );
    WorkDaysModel->setSort( 0, Qt::AscendingOrder);

    gui->WorkDaysTable->setModel( WorkDaysModel );
    gui->WorkDaysTable->resizeColumnsToContents();
    gui->WorkDaysTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    gui->WorkDaysTable->horizontalHeader()->setStretchLastSection(true);

    // -----------------------------------------

    WorkingRatesModel = new QSqlTableModel( this, DataBase->sqlDatabase() );
    WorkingRatesModel->setTable( "WorkingRates" );
    WorkingRatesModel->setSort( 0, Qt::AscendingOrder);

    gui->WorkingRatesTable->setModel( WorkingRatesModel );
    gui->WorkingRatesTable->resizeColumnsToContents();
    gui->WorkingRatesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    gui->WorkingRatesTable->horizontalHeader()->setStretchLastSection(true);

    // -----------------------------------------

    ScheduleTimeModel = new QSqlTableModel( this, DataBase->sqlDatabase() );
    ScheduleTimeModel->setTable( "ScheduleTime" );
    ScheduleTimeModel->setSort( 0, Qt::AscendingOrder);

    gui->ScheduleTimeTable->setModel( ScheduleTimeModel );
    gui->ScheduleTimeTable->resizeColumnsToContents();
    gui->ScheduleTimeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    gui->ScheduleTimeTable->horizontalHeader()->setStretchLastSection(true);

    // -----------------------------------------

    TimeIntervalsModel = new QSqlTableModel( this, DataBase->sqlDatabase() );
    TimeIntervalsModel->setTable( "TimeIntervals" );
    TimeIntervalsModel->setSort( 0, Qt::AscendingOrder);

    gui->TimeIntervalsTable->setModel( TimeIntervalsModel );
    gui->TimeIntervalsTable->resizeColumnsToContents();
    gui->TimeIntervalsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    gui->TimeIntervalsTable->horizontalHeader()->setStretchLastSection(true);

    // -----------------------------------------

    NotesModel = new QSqlTableModel( this, DataBase->sqlDatabase() );
    NotesModel->setTable( "Notes" );
    NotesModel->setSort( 0, Qt::AscendingOrder);

    gui->NotesTable->setModel( NotesModel );
    gui->NotesTable->resizeColumnsToContents();
    gui->NotesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    gui->NotesTable->horizontalHeader()->setStretchLastSection(true);

    // -----------------------------------------

    SalariesModel = new QSqlTableModel( this );
    SalariesModel->setTable( "Salaries" );
    SalariesModel->setSort( 0, Qt::AscendingOrder);

    gui->SalariesTable->setModel( SalariesModel );
    gui->SalariesTable->resizeColumnsToContents();
    gui->SalariesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    gui->SalariesTable->horizontalHeader()->setStretchLastSection(true);

    // -----------------------------------------
}
