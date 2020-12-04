// ---------------------------- //
#include <QTextCharFormat>
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

    fossil = new QCPBars( gui->Chart->xAxis, gui->Chart->yAxis );
    gui->Chart->addPlottable( fossil );
    gui->Chart->setInteractions( QCP::iRangeZoom );

    QPen pen;
    pen.setWidth( 1 );
    pen.setColor( Qt::green );
    fossil->setPen( pen );
    fossil->setBrush( QColor("#008080") );

    connect( gui->AddButton     , SIGNAL(clicked           (bool   )), SLOT(addSalary          (        )) );
    connect( gui->SalaryEdit    , SIGNAL(returnPressed     (       )), SLOT(addSalary          (        )) );
    connect( gui->ChangeButton  , SIGNAL(clicked           (bool   )), SLOT(changeSalary       (        )) );
    connect( gui->RemoveButton  , SIGNAL(clicked           (bool   )), SLOT(removeSalary       (        )) );
    connect( gui->SalaryCalendar, SIGNAL(selectionChanged  (       )), SLOT(selectDate         (        )) );
    connect( gui->SalaryCalendar, SIGNAL(currentPageChanged(int,int)), SLOT(changedPageCalendar(int, int)) );
    connect( gui->SalaryList    , SIGNAL(currentRowChanged (int    )), SLOT(selectSalary       (        )) );

    gui->SalaryEdit->setStyleSheet( "border: 1px inset green; font-size: 10pt; color: green;" );

    gui->SalaryAverageValue->setStyleSheet( "border: 1px groove #008080;" );
    gui->SalaryYearValue   ->setStyleSheet( "border: 1px inset  #008585;" );
    gui->SalaryMonthValue  ->setStyleSheet( "border: 1px outset #009090;" );
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

void SalaryWindow::changedPageCalendar(int year, int month )
{
    QDate NewDate(year, month, 1);
    int selectedDay = gui->SalaryCalendar->selectedDate().day();

    if( NewDate.daysInMonth() >= selectedDay )
        NewDate.setDate( year, month, selectedDay );

    gui->SalaryCalendar->setSelectedDate( NewDate );
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::selectDate()
{
    QDate NewDate = gui->SalaryCalendar->selectedDate();

    bool isChangeMonth = NewDate.year() != SelectedDate.year() || NewDate.month() != SelectedDate.month();

    SelectedDate = NewDate;

    if( isChangeMonth )
        reloadFull();

    gui->SalaryList->clear();

    const QList<Salary>& MonthSalary = SalaryMonth[SelectedDate.month() - 1];

    for( int i = 0; i < MonthSalary.count(); i++ )
    {
        if( MonthSalary.at(i).date == SelectedDate )
        {
            QListWidgetItem * SalaryItem = new QListWidgetItem( MonthSalary.at(i).toString(), gui->SalaryList );
            SalaryItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

            gui->SalaryList->addItem( SalaryItem );
        }
    }

    gui->SalaryDayLabel->setText( tr("Зарплата %1 %2").arg(SelectedDate.day()).arg(HelperWT::nameMonthInGenitiveCase(SelectedDate, false)) );

    gui->SalaryList->setCurrentRow( gui->SalaryList->count() - 1 );

    selectSalary();
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::reloadFull()
{
    reloadSalary   ();
    reloadCalendar ();
    updateStatistic();
    updateChart    ();

    gui->SalaryAverageLabel->setText( tr("Среднняя ЗП:") );
    gui->SalaryYearLabel   ->setText( tr("В %1 году:").arg(SelectedDate.year()) );
    gui->SalaryMonthLabel  ->setText( tr("В %1:").arg(HelperWT::nameMonthInDativeCase(SelectedDate, true)) );
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::reloadSalary()
{
    QList<QDate> Dates;
    QList<float> Salaries;

    // Загрузка зарплаты за год
    for( int month = January; month < CountMonth; month++ )
    {
        SalaryMonth[month].clear();

        QDate Date( SelectedDate.year(), month + 1, 1 );

        DataBase->getMonthSalaries( workingRate, Date, Dates, Salaries );

        for( int day = 0; day < Dates.count(); day++ )
        {
            SalaryMonth[month].append( Salary( Dates.at(day), Salaries.at(day) ) );
        }

        Dates.clear();
        Salaries.clear();
    }
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::reloadCalendar()
{
    const QList<Salary>& MonthSalary = SalaryMonth[SelectedDate.month() - 1];

    for( int day = 1; day <= SelectedDate.daysInMonth(); day++ )
    {
        bool isExistsSalary = false;

        QDate Date( SelectedDate.year(), SelectedDate.month(), day );

        for( int i = 0; i < MonthSalary.count(); i++ )
        {
            if( MonthSalary.at(i).date == Date )
            {
                isExistsSalary = true;
                break;
            }
        }

        QTextCharFormat TextCharFormat = gui->SalaryCalendar->dateTextFormat( Date );

        TextCharFormat.setForeground( isExistsSalary ? Qt::green : Qt::white );

        gui->SalaryCalendar->setDateTextFormat( Date, TextCharFormat );
    }
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::updateStatistic()
{
    int countMonth    = 0  ;
    int salaryMonth   = 0.0;
    int salaryYear    = 0.0;
    int salaryAverage = 0.0;

    // Загрузка зарплаты за год
    for( int month = January; month < CountMonth; month++ )
    {
        float salary = 0.0;

        const QList<Salary>& MonthSalary = SalaryMonth[month];

        for( int day = 0; day < MonthSalary.count(); day++ )
            salary += MonthSalary.at(day).value;

        salaryYear += salary;

        if( salary > 0.0 )
            countMonth++;

        if( month + 1 == SelectedDate.month() )
            salaryMonth = salary;
    }

    if( countMonth > 0 )
        salaryAverage = salaryYear / countMonth;

    QString SalaryAverageText = QString::number(salaryAverage);
    QString SalaryYearText    = QString::number(salaryYear   );
    QString SalaryMonthText   = QString::number(salaryMonth  );

    for( int i = SalaryAverageText.length() - 3; i > 0; i -= 3 )
        SalaryAverageText.insert( i, '.' );

    for( int i = SalaryYearText.length() - 3; i > 0; i -= 3 )
        SalaryYearText.insert( i, '.' );

    for( int i = SalaryMonthText.length() - 3; i > 0; i -= 3 )
        SalaryMonthText.insert( i, '.' );


    gui->SalaryAverageValue->setText( SalaryAverageText );
    gui->SalaryYearValue   ->setText( SalaryYearText    );
    gui->SalaryMonthValue  ->setText( SalaryMonthText   );
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::updateChart()
{
    QVector<double> chartDates, chartSalary;

    QVector<QString> monthLabels;

    double maxSalary = 0.0;

    for( int month = 1; month <= 12; month++ )
    {
        double salary = 0.0;

        QList<QDate> Dates;
        QList<float> Salaries;
        QList<Salary> MonthSalary;

        QDate Date( SelectedDate.year(), month, 1 );

        DataBase->getMonthSalaries( workingRate, Date, Dates, Salaries );

        for( int i = 0; i < Dates.count(); i++ )
            MonthSalary.append( Salary( Dates.at(i), Salaries.at(i) ) );

        monthLabels.append(  QDate::longMonthName(month) );
        chartDates.append( month );

        for( int day = 1; day <= Date.daysInMonth(); day++ )
        {
            QDate DayMonth( SelectedDate.year(), month, day );

            for( int i = 0; i < MonthSalary.count(); i++ )
            {
                if( MonthSalary.at(i).date == DayMonth )
                {
                    salary += MonthSalary.at(i).value;
                }
            }
        }

        chartSalary.append( salary );

        if( salary > maxSalary )
            maxSalary = salary;
    }

    gui->Chart->xAxis->setAutoTicks( false );
    gui->Chart->xAxis->setAutoTickLabels( false );
    gui->Chart->xAxis->setTickVector( chartDates );
    gui->Chart->xAxis->setTickVectorLabels( monthLabels );
    gui->Chart->xAxis->setTickLabelRotation( 50 );
    gui->Chart->xAxis->setSubTickCount( 0 );
    gui->Chart->xAxis->setTickLength( 0, 4 );
    gui->Chart->xAxis->grid()->setVisible( false );
    gui->Chart->xAxis->setRange( 1, SelectedDate.daysInMonth() );
    gui->Chart->xAxis->setTickLabelColor( Qt::lightGray );
    gui->Chart->xAxis->setBasePen( QPen(Qt::lightGray) );
    gui->Chart->xAxis->setTickPen( QPen(Qt::lightGray) );

    gui->Chart->yAxis->setAutoTicks( true );
    gui->Chart->yAxis->setAutoTickLabels( true );
    gui->Chart->yAxis->setLabel( tr("Зарплата") );
    gui->Chart->yAxis->setLabelColor( Qt::lightGray );
    gui->Chart->yAxis->grid()->setVisible( true );
    gui->Chart->yAxis->setTickLength( 0, 4 );
    gui->Chart->yAxis->setSubTickCount( 0 );
    gui->Chart->yAxis->setRange( 0, maxSalary + 10000 );
    gui->Chart->yAxis->setTickLabelColor( Qt::lightGray );
    gui->Chart->yAxis->setBasePen( QPen(Qt::lightGray) );
    gui->Chart->yAxis->setTickPen( QPen(Qt::lightGray) );
    gui->Chart->yAxis->setPadding( 10 );

    QPen gridPen;
    gridPen.setStyle( Qt::SolidLine );
    gridPen.setColor( Qt::lightGray );
    gui->Chart->yAxis->grid()->setPen( gridPen );
    gridPen.setStyle( Qt::DotLine );
    gui->Chart->yAxis->grid()->setSubGridPen( gridPen );

    fossil->setData( chartDates, chartSalary );

    gui->Chart->addGraph();

    QPen graphPen;
    graphPen.setColor( QColor("#DC143C") );
    graphPen.setWidth( 2 );
    graphPen.setStyle( Qt::DotLine );

    gui->Chart->graph(0)->setPen( graphPen );
    gui->Chart->graph(0)->setData( chartDates, chartSalary );
    gui->Chart->graph(0)->setSelectable( true );

    gui->Chart->setBackground( palette().background() );

    gui->Chart->rescaleAxes();
    gui->Chart->replot();
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::selectSalary()
{
    gui->SalaryEdit->clear();

    if( gui->SalaryList->currentRow() >= 0 )
    {
        QString DateSalary = gui->SalaryList->currentItem()->text();

        DateSalary = DateSalary.replace(" ", "");

        QStringList SlipSalary = DateSalary.split( "-" );
        gui->SalaryEdit->setText( SlipSalary.at(1) );
    }
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::addSalary()
{
    float value = gui->SalaryEdit->text().toFloat();

     if( value > 0 )
     {
        DataBase->addSalary( workingRate, SelectedDate, value );
        reloadFull();
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
        reloadFull();
     }
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::removeSalary()
{
    int salaryID = gui->SalaryList->currentRow();

    if( salaryID >= 0 )
    {
        DataBase->removeSalary( workingRate, SelectedDate, salaryID );
        reloadFull();
    }
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::resizeEvent( QResizeEvent *ResizeEvent )
{
    gui->Chart->replot();
    QWidget::resizeEvent( ResizeEvent );
}
// ------------------------------------------------------------------------------------ //

void SalaryWindow::showEvent( QShowEvent * ShowEvent )
{
    SelectedDate = HelperWT::currentDate();

    reloadFull();

    gui->SalaryDayLabel->setText( tr("Зарплата %1 %2").arg(SelectedDate.day()).arg(HelperWT::nameMonthInGenitiveCase(SelectedDate, false)) );

    QWidget::showEvent( ShowEvent );
}
// ------------------------------------------------------------------------------------ //
