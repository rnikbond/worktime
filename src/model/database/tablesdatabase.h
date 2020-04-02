// ------------------------------ //
#ifndef TABLESDATABASE_H
#define TABLESDATABASE_H
// ------------------------------ //
#include <QTimer>
#include <QWidget>
#include <QSqlTableModel>
// ------------------------------ //
class DataBaseWT;
// ------------------------------ //
namespace Ui { class TablesDataBase; }
// ------------------------------ //

class TablesDataBase : public QWidget
{
    Q_OBJECT

    QTimer* UpdateTablesTimer;

    QSqlTableModel* WorkDaysModel;
    QSqlTableModel* WorkingRatesModel;
    QSqlTableModel* ScheduleTimeModel;
    QSqlTableModel* TimeIntervalsModel;
    QSqlTableModel* NotesModel;
    QSqlTableModel* SalariesModel;

public:
    explicit TablesDataBase( DataBaseWT * DataBase, QWidget * parent = 0 );
    ~TablesDataBase();

private:
    Ui::TablesDataBase *gui;

    void setupTableView( DataBaseWT * DataBase );

public slots:

    void udpateFromDataBase();
    void changeTimeStart();
    void changeTimeEnd();
    void removeInterval();
    void removeSalary();
};

#endif // TABLESDATABASE_H
