// ---------------------------- //
// ---------------------------- //
#include "helperwt.h"
#include "databasewt.h"
// ---------------------------- //
#include "salarywindow.h"
#include "ui_salarywindow.h"
// ---------------------------- //

SalaryWindow::SalaryWindow( QWidget * parent ) : QWidget( parent ), gui( new Ui::SalaryWindow )
{
    gui->setupUi( this );

    gui->SalaryList->setSelectionMode( QAbstractItemView::SingleSelection );

    connect( gui->AddButton     , SIGNAL(clicked          (bool)), SLOT(addSalary   ()) );
    connect( gui->SalaryEdit    , SIGNAL(returnPressed    (    )), SLOT(addSalary   ()) );
    connect( gui->ChangeButton  , SIGNAL(clicked          (bool)), SLOT(changeSalary()) );
    connect( gui->RemoveButton  , SIGNAL(clicked          (bool)), SLOT(removeSalary()) );
    connect( gui->SalaryCalendar, SIGNAL(selectionChanged (    )), SLOT(selectDate  ()) );
    connect( gui->SalaryList    , SIGNAL(currentRowChanged(int )), SLOT(selectSalary()) );

}
// ------------------------------------------------------------------------------------ //

SalaryWindow::~SalaryWindow()
{
    delete gui;
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::setWorkingRate( int rate )
{
    workingRate = rate;
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::setDataBase( DataBaseWT * DB )
{
    DataBase = DB;
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::selectDate()
{
    QDate NewDate = gui->SalaryCalendar->selectedDate();

    bool isChangeMonth = NewDate.year() != SelectedDate.year() || NewDate.month() != SelectedDate.month();

    SelectedDate = NewDate;

    if( isChangeMonth )
        reloadSalary();

    gui->SalaryList->clear();

    for( int i = 0; i < SalaryMonth.count(); i++ )
    {
        if( SalaryMonth.at(i).date == SelectedDate )
        {
            QListWidgetItem * SalaryItem = new QListWidgetItem( SalaryMonth.at(i).toString(), gui->SalaryList );
            SalaryItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

            gui->SalaryList->addItem( SalaryItem );
        }
    }

    gui->SalaryList->setCurrentRow( gui->SalaryList->count() - 1 );
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::reloadSalary()
{
    SalaryMonth.clear();

    QList<QDate> Dates;
    QList<float> Salaries;

    DataBase->getMonthSalaries( workingRate, SelectedDate, Dates, Salaries );

    for( int i = 0; i < Dates.count(); i++ )
        SalaryMonth.append( Salary( Dates.at(i), Salaries.at(i) ) );
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::selectSalary()
{
    if( gui->SalaryList->currentRow() >= 0 )
    {
        QString DateSalary = gui->SalaryList->currentItem()->text();

        DateSalary = DateSalary.replace(" ", "");

        QStringList SlipSalary = DateSalary.split( "-" );

        QDate Date = QDate::fromString( SlipSalary.at(0), "dd.MM.yyyy" );

        float SummSalary = 0.0;

        for( int i = 0; i < SalaryMonth.count(); i++ )
        {
            SummSalary += SalaryMonth.at(i).value;

            if( SalaryMonth.at(i).date == Date )
                gui->SalaryEdit->setText( QString::number(SalaryMonth.at(i).value) );
        }

        gui->MonthValue->setText( QString::number(SummSalary) );

        gui->SalaryCalendar->setSelectedDate( Date );

        SelectedDate = Date;
    }
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::addSalary()
{
    float value = gui->SalaryEdit->text().toFloat();

     if( value > 0 )
     {
        DataBase->addSalary( workingRate, SelectedDate, value );
        reloadSalary();
     }
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::changeSalary()
{
    float value = gui->SalaryEdit->text().toFloat();
    int salaryID = gui->SalaryList->currentRow();

     if( salaryID >= 0 && value > 0 )
     {
        DataBase->changeSalary( workingRate, SelectedDate, salaryID, value );
        reloadSalary();
     }
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::removeSalary()
{
    int salaryID = gui->SalaryList->currentRow();

    if( salaryID >= 0 )
    {
        DataBase->removeSalary( workingRate, SelectedDate, salaryID );
        reloadSalary();
    }
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::showEvent( QShowEvent * ShowEvent )
{
    SelectedDate = HelperWT::currentDate();

    reloadSalary();

    QWidget::showEvent( ShowEvent );
}
// ------------------------------------------------------------------------------------ //
