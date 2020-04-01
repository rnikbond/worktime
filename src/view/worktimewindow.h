// ---------------------------- //
#ifndef WORKTIMEWINDOW_H
#define WORKTIMEWINDOW_H
// ---------------------------- //
#include <QWidget>
// ---------------------------- //
#include "iworktime.h"
// ---------------------------- //
namespace Ui { class WorkTimeWindow; }
// ---------------------------- //

class WorkTimeWindow : public QWidget, public IWorkTime
{
    Q_OBJECT

    enum
    {
        DayPage  ,
        WeekPage ,
        MonthPage
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
