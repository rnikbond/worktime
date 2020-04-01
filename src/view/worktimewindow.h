// ---------------------------- //
#ifndef WORKTIMEWINDOW_H
#define WORKTIMEWINDOW_H
// ---------------------------- //
#include <QWidget>
// ---------------------------- //
#include "wtime.h"
#include "wtimeext.h"
#include "iworktime.h"
// ---------------------------- //
namespace Ui { class WorkTimeWindow; }
// ---------------------------- //

class WorkTimeWindow : public QWidget, public IWorkTime
{
    Q_OBJECT

    enum
    {
        DayPage  , ///< Страница "День"
        WeekPage , ///< Страница "Неделя"
        MonthPage  ///< Страница "Месяц"
    };

public:

    explicit WorkTimeWindow( QWidget * parent = 0 );
    ~WorkTimeWindow();

private:

    void configuringGUI();
    void connectSingnalSlot();
    void updateTitles();

    Ui::WorkTimeWindow * gui;

private slots:

    // Calendar slots
    void selectDate();
    void todayClick();

    // Change time slots
    void changeTimeStart( QTime time );
    void changeTimeEnd  ( QTime time );
    void changeTimeNeed ( QTime time );
    void finishChangeTimeStart();
    void finishChangeTimeEnd  ();

    // Menu slots
    void MenuClick       ();
    void SalaryClick     ();
    void TableTimeClick  ();
    void ScheduleClick   ();
    void SeveralDaysClick();
    void ChangesClick    ();
    void SettingsClick   ();
    void UpdateClick     ();

signals:

    void userSelectDate( const QDate & date );
};
// ------------------------------------------------------------------------------------ //

#endif // WORKTIMEWINDOW_H
