// ---------------------------- //
#ifndef TABLETIMEWINDOW_H
#define TABLETIMEWINDOW_H
// ---------------------------- //
#include <QWidget>
// ---------------------------- //
namespace Ui { class TableTimeWindow; }
// ---------------------------- //
class DataBaseWT   ;
class MonthWorkTime;
// ---------------------------- //

class TableTimeWindow : public QWidget
{
    Q_OBJECT

    int workingRate;

    DataBaseWT    * DataBase ;
    MonthWorkTime * WorkMonth;

public:

    explicit TableTimeWindow( QWidget * parent = 0 );
    ~TableTimeWindow();

    void setWorkingRate( int rate );
    void setDataBase( DataBaseWT * DB );

    void loadMonth();
    void updateTable();

private:

    Ui::TableTimeWindow * gui;

protected:

    void showEvent( QShowEvent * ShowEvent );

private slots:

    void currentMonthClick();
    void changedMonth();
    void saveToPdfClick();
    void printClick();
    void addCustomText();
};

#endif // TABLETIMEWINDOW_H
