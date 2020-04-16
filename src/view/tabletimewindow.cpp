// ---------------------------- //
#include <QPrinter>
#include <QFileDialog>
#include <QPrintDialog>
#include <QTextDocument>
// ---------------------------- //
#include "helperwt.h"
#include "databasewt.h"
#include "monthworktime.h"
// ---------------------------- //
#include "tabletimewindow.h"
#include "ui_tabletimewindow.h"
// ---------------------------- //

TableTimeWindow::TableTimeWindow( QWidget * parent ) : QWidget( parent ), gui( new Ui::TableTimeWindow )
{
    gui->setupUi( this );

    DataBase = NULL;

    workingRate = -1;

    WorkMonth = new MonthWorkTime( this );

    for( int month = 1; month <= 12; month++ )
        gui->MonthCBox->insertItem( month - 1, QLocale().monthName(month) );

    connect( gui->MonthCBox         , SIGNAL(currentIndexChanged(int )), SLOT(changedMonth     ()) );
    connect( gui->SavePdfButton     , SIGNAL(clicked            (bool)), SLOT(saveToPdfClick   ()) );
    connect( gui->PrintButton       , SIGNAL(clicked            (bool)), SLOT(printClick       ()) );
    connect( gui->CurrentMonthButton, SIGNAL(clicked            (bool)), SLOT(currentMonthClick()) );
}
// ------------------------------------------------------------------------------------ //

TableTimeWindow::~TableTimeWindow()
{
    delete gui;
}
// ------------------------------------------------------------------------------------ //

void TableTimeWindow::updateTable()
{
    if( workingRate == -1 )
        return;

    int month = gui->MonthCBox->currentIndex() + 1;

    QString TableTag = "<table border=1 "      \
                              "width=100% "    \
                              "height=100% "   \
                              "cellspacing=-1 " \
                              "cellpadding=2 " \
                              "style=\"border-style:solid;"\
                                      "vertical-align:middle;"\
                                      "margin:5;"\
                                      "font-size:8pt;"\
                                      "border-color:#383838;"\
                                      "border-collapse:collapse;"\
                              "\">";

    QString MonthHtmlTime = TableTag;

    MonthHtmlTime += "<tr align=center>";

    MonthHtmlTime += tr("<th colspan=8>За %1 %2</th>").arg(QLocale().monthName( month )).arg(HelperWT::currentDate().year());

    MonthHtmlTime +=  "</tr>";

    MonthHtmlTime += tr("<tr align=center>"       \
                            "<th>День</th>"       \
                            "<th>Дата</th>"       \
                            "<th>Тип дня</th>"    \
                            "<th>Интервал</th>"   \
                            "<th>Начало</th>"     \
                            "<th>Конец</th>"      \
                            "<th>Отработано</th>" \
                            "<th>Нужно</th>"      \
                            "<th>Итог</th>"       \
                        "</tr>" );

    for( int day = 0; day < WorkMonth->countDays(); day++ )
    {
        QDate Date( HelperWT::currentDate().year(), month, day + 1 );

        DayWorkTime * Day = WorkMonth->dayAtDate( Date );

        if( Day != NULL )
        {
            int countIntervals = Day->countIntervals();

            QString DayHtmlTable;

            if( countIntervals > 0 )
            {
                for( int i = 0; i < countIntervals; i++)
                {
                    if( i == 0 )
                    {
                        WTime TimeNeed   = Day->timeNeed();
                        WTime TimeWorked = Day->timeWorked();

                        DayHtmlTable += "<tr>";
                        DayHtmlTable += QString("<td align=center rowspan=%1>").arg(countIntervals) + QDate::shortDayName(Date.dayOfWeek()) + "</td>";
                        DayHtmlTable += QString("<td align=center rowspan=%1>").arg(countIntervals) + Date.toString("dd.MM.yyyy") + "</td>";

                        DayHtmlTable += QString("<td align=center rowspan=%1>").arg(countIntervals) + DayWorkTime::nameDay(Day->typeDay()) + "</td>";
                        DayHtmlTable += "<td align=center >" + Day->intervalTitle( i )      + "</td>";
                        DayHtmlTable += "<td align=center >" + Day->timeStart(i).toString() + "</td>";
                        DayHtmlTable += "<td align=center >" + Day->timeEnd(i).toString()   + "</td>";

                        DayHtmlTable += QString("<td align=center rowspan=%1>").arg(countIntervals) + TimeWorked.toString() + "</td>";
                        DayHtmlTable += QString("<td align=center rowspan=%1>").arg(countIntervals) + TimeNeed.toString() + "</td>";

                        QString TimeString = "  " + WTime().toString();

                             if( TimeNeed   > TimeWorked ) TimeString = "- " + (TimeNeed - TimeWorked).toString();
                        else if( TimeWorked > TimeNeed   ) TimeString = "+ " + (TimeWorked - TimeNeed).toString();

                        DayHtmlTable += QString("<td align=center rowspan=%1>").arg(countIntervals) + TimeString + "</td>";
                        DayHtmlTable += "</tr>";
                    }
                    else
                    {
                        DayHtmlTable += "<tr>" \
                                            "<td align=center >" + Day->intervalTitle( i )      + "</td>" \
                                            "<td align=center >" + Day->timeStart(i).toString() + "</td>" \
                                            "<td align=center >" + Day->timeEnd(i).toString()   + "</td>" \
                                        "</tr>";
                    }
                }
            }
            else
            {
                //"<td align=center style='color: #008080;'>"
                DayHtmlTable += "<tr>" \
                                    "<td align=center>"  + QDate::shortDayName(Date.dayOfWeek()) + "</td>" \
                                    "<td align=center>"  + Date.toString("dd.MM.yyyy")            + "</td>"\
                                    "<td align=center>"  + DayWorkTime::nameDay(Day->typeDay())   + "</td>"\
                                    "<td colspan=3></td>"\
                                    "<td align=center >" + Day->timeWorked().toString() + "</td>" \
                                    "<td align=center >" + Day->timeWorked().toString() + "</td>" \
                                    "<td align=center >" + Day->timeNeed  ().toString() + "</td>" \
                                "</tr>";
            }

            MonthHtmlTime += DayHtmlTable;
        }
    }

    MonthHtmlTime += "</table>";

    MonthHtmlTime += "<br/>";

    MonthHtmlTime += TableTag;

    MonthHtmlTime += tr( "<tr>"\
                            "<th>Отработано</th>" \
                            "<th>Нужно</th>"      \
                            "<th>Итог</th>"       \
                         "</tr>");

    WTime TimeNeed   = WorkMonth->timeNeedInMonth  ();
    WTime TimeWorked = WorkMonth->timeWorkedInMonth();

    MonthHtmlTime += "<tr align=center>";
    MonthHtmlTime += "<td align=center>" + TimeWorked.toString() + "</td>";
    MonthHtmlTime += "<td align=center>" + TimeNeed.toString() + "</td>";

    QString TimeString = WTime().toString();

         if( TimeNeed > TimeWorked ) TimeString = "- " + (TimeNeed - TimeWorked).toString();
    else if( TimeWorked > TimeNeed ) TimeString = "+ " + (TimeWorked - TimeNeed).toString();

    MonthHtmlTime += "<td align=center>" + TimeString + "</td>";

    MonthHtmlTime += "</tr>";

    MonthHtmlTime += "</table>";

    MonthHtmlTime += tr("<h4 align=right>Таблица рабочего времени создана в программе WorkTime %1</h4>")
                     .arg(HelperWT::currentDate().toString("dd.MM.yyyy"));

    gui->TableTimeValue->setText( MonthHtmlTime );
}
// ------------------------------------------------------------------------------------ //

void TableTimeWindow::setWorkingRate( int rate )
{
    workingRate = rate;
}
// ------------------------------------------------------------------------------------ //

void TableTimeWindow::setDataBase( DataBaseWT * DB )
{
    DataBase = DB;
}
// ------------------------------------------------------------------------------------ //

void TableTimeWindow::loadMonth()
{
    WorkMonth->clear();

    if( workingRate == -1 )
        return;

    if( DataBase == NULL )
        return;

    QDate SelectedDate( HelperWT::currentDate().year(), gui->MonthCBox->currentIndex() + 1, 1 );

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
}
// ------------------------------------------------------------------------------------ //

void TableTimeWindow::changedMonth()
{
    loadMonth();
    updateTable();
}
// ------------------------------------------------------------------------------------ //

void TableTimeWindow::currentMonthClick()
{
    gui->MonthCBox->blockSignals( true );
    gui->MonthCBox->setCurrentIndex( HelperWT::currentDate().month() - 1 );
    gui->MonthCBox->blockSignals( false );

    changedMonth();

}
// ------------------------------------------------------------------------------------ //

void TableTimeWindow::printClick()
{
    QPrinter Printer;
    Printer.setFullPage( true );
    Printer.setPageSize( QPrinter::A4 );
    Printer.setOrientation( QPrinter::Portrait );

    QPrintDialog PrintDialog( &Printer, this );

    if( PrintDialog.exec() )
    {
        QTextDocument TextDocument;
        TextDocument.setHtml( gui->TableTimeValue->text() );
        TextDocument.setDefaultFont( QFont( "Arial", 6 ) );
        TextDocument.print( &Printer );
        close();
    }
}
// ------------------------------------------------------------------------------------ //

void TableTimeWindow::saveToPdfClick()
{
    QString FileName = QFileDialog::getSaveFileName( this,
                                                     tr("Сохранение времени в PDF"),
                                                     QDir::homePath() + tr("/Desktop/Время работы за %1 %2.pdf")
                                                     .arg(gui->MonthCBox->currentText())
                                                     .arg(HelperWT::currentDate().year()),
                                                     "*.pdf" );



    if( FileName.isEmpty() == false )
    {
        if( QFileInfo(FileName).suffix().isEmpty() )
            FileName.append( ".pdf" );

        QPrinter Printer( QPrinter::PrinterResolution );
        Printer.setOutputFormat( QPrinter::PdfFormat );
        Printer.setPaperSize( QPrinter::A4 );
        Printer.setOutputFileName( FileName );

        QTextDocument TextDocument;
        TextDocument.setHtml( gui->TableTimeValue->text() );
        TextDocument.setDefaultFont( QFont( "Arial", 6 ) );
        TextDocument.print( &Printer );

        close();
    }
}
// ------------------------------------------------------------------------------------ //


void TableTimeWindow::showEvent( QShowEvent * ShowEvent )
{
    currentMonthClick();
    QWidget::showEvent( ShowEvent );
}
// ------------------------------------------------------------------------------------ //
