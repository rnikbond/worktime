// ---------------------------- //
#ifndef SEVERALDAYSWINDOW_H
#define SEVERALDAYSWINDOW_H
// ---------------------------- //
#include <QDate>
#include <QWidget>
// ---------------------------- //
namespace Ui { class SeveralDaysWindow; }
// ---------------------------- //
class DataBaseWT   ;
class MonthWorkTime;
// ---------------------------- //

class SeveralDaysWindow : public QWidget
{
    Q_OBJECT

    enum SelectionVariant
    {
        NoneSelection,
        StartSelect  ,
        EndSelect    ,
        TypeDaySelect
    };

    SelectionVariant Variant;

    int workingRate;

    DataBaseWT    * DataBase ;
    MonthWorkTime * WorkMonth;

    QDate SelectedDate;

    QColor BackgroundWeekDay;
    QColor BackgroundWorkDay;
    QColor BackgroundSelectedDay;

public:

    explicit SeveralDaysWindow( QWidget * parent = 0 );
    ~SeveralDaysWindow();

    void setWorkingRate( int rate );
    void setDataBase( DataBaseWT * DB );

private:

    Ui::SeveralDaysWindow * gui;

    void loadMonth();

protected:

    void showEvent( QShowEvent * ShowEvent );

private slots:

    void clear();
    void selectDate();
    void saveClick();
    void startChecked( bool );
    void endChecked( bool );

signals:

    void savedChanges();
};

#endif // SEVERALDAYSWINDOW_H
