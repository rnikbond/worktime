// ---------------------------- //
#ifndef SALARYWINDOW_H
#define SALARYWINDOW_H
// ---------------------------- //
#include <QDate>
#include <QWidget>
// ---------------------------- //
namespace Ui { class SalaryWindow; }
// ---------------------------- //
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

    int workingRate;

    DataBaseWT * DataBase ;

    QDate SelectedDate;

    QList<Salary> SalaryMonth;

public:

    explicit SalaryWindow( QWidget * parent = 0 );
    ~SalaryWindow();

    void setWorkingRate( int rate );
    void setDataBase( DataBaseWT * DB );

private:

    Ui::SalaryWindow * gui;

    void reloadSalary();

protected:

    void showEvent( QShowEvent * ShowEvent );

private slots:

    void selectDate  ();
    void selectSalary();

    void addSalary   ();
    void changeSalary();
    void removeSalary();

signals:

    void updateSalary();
};

#endif // SALARYWINDOW_H
