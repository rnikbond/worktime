// ---------------------------- //
#ifndef SALARYWINDOW_H
#define SALARYWINDOW_H
// ---------------------------- //
#include <QDate>
#include <QWidget>
// ---------------------------- //
namespace Ui { class SalaryWindow; }
// ---------------------------- //
class QCPBars;
class DataBaseWT;
// ---------------------------- //

struct Salary
{
    float value;
    QDate date;

    Salary()
    {
        date  = QDate( 1996, 04, 15 );
        value = 0.0;
    }

    Salary( QDate dateSalary, float valueSalary )
    {
        date  = dateSalary ;
        value = valueSalary;
    }

    QString toString() const
    {
        return date.toString("dd.MM.yyyy") + " - " + QString::number( value );
    }
};

class SalaryWindow : public QWidget
{
    Q_OBJECT

    enum Months
    {
        January    = 0 ,
        February   = 1 ,
        March      = 2 ,
        April      = 3 ,
        May        = 4 ,
        June       = 5 ,
        July       = 6 ,
        August     = 7 ,
        September  = 8 ,
        October    = 9 ,
        November   = 10,
        December   = 11,
        CountMonth = 12
    };

    int workingRate;

    DataBaseWT * DataBase ;

    QDate SelectedDate;

    QList<Salary> SalaryMonth[CountMonth];

    QCPBars *fossil;

public:

    explicit SalaryWindow( QWidget * parent = 0 );
    ~SalaryWindow();

    void setWorkingRate( int rate );
    void setDataBase( DataBaseWT * DB );

private:

    Ui::SalaryWindow * gui;

    void reloadSalary();
    void reloadCalendar();
    void reloadFull();

    void updateStatistic();
    void updateChart();

protected:

    void showEvent( QShowEvent * ShowEvent );
    void resizeEvent( QResizeEvent *ResizeEvent );

private slots:

    void selectDate  ();
    void changedPageCalendar(int year, int month );

    void selectSalary();

    void addSalary   ();
    void changeSalary();
    void removeSalary();

signals:

    void updateSalary();
};

#endif // SALARYWINDOW_H
