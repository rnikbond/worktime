// ---------------------------- //
#ifndef WORKTIMEWINDOW_H
#define WORKTIMEWINDOW_H
// ---------------------------- //
#include <QWidget>
// ---------------------------- //
#include "wtime.h"
#include "wtimeext.h"
#include "iviewworktime.h"
// ---------------------------- //
namespace Ui { class WorkTimeWindow; }
// ---------------------------- //
class QListWidgetItem;
// ---------------------------- //

class WorkTimeWindow : public QWidget, public IViewWorkTime
{
    Q_OBJECT

    enum
    {
        DayPage  , ///< Страница "День"
        WeekPage , ///< Страница "Неделя"
        MonthPage  ///< Страница "Месяц"
    };

    bool   isMoveWindow;
    QPoint WindowPosition;

public:

    explicit WorkTimeWindow( QWidget * parent = 0 );
    ~WorkTimeWindow();

    void setSalaryExists     ( bool isExists );
    void setTableTimeExists  ( bool isExists );
    void setScheduleExists   ( bool isExists );
    void setSeveralDaysExists( bool isExists );
    void setChangesExists    ( bool isExists );
    void setSettingsExists   ( bool isExists );

public: // Interface functions

    // Инициализация общих элементов
    void setTypesDay( QStringList list );

    void setTypeDay   ( int         type         );
    void setTimeStart ( WTime       time, int id );
    void setTimeEnd   ( WTime       time, int id );
    void setIntervals ( QStringList list         );
    void setNote      ( QString     note         );

    void setTimeWorkedInDay   ( WTime time                );
    void setTimeStatisticInDay( WTime time, QString title );
    void setTimeNeedInDay     ( WTime time                );

    void setTimeWorkedInWeek   ( WTime time                );
    void setTimeStatisticInWeek( WTime time, QString title );
    void setTimeNeedInWeek     ( WTime time                );

    void setTimeWorkedInMonth   ( WTime time                );
    void setTimeStatisticInMonth( WTime time, QString title );
    void setTimeNeedInMonth     ( WTime time                );

    void setTimeWorkedToDay   ( WTime time                );
    void setTimeStatisticToDay( WTime time, QString title );
    void setTimeNeedToDay     ( WTime time                );

    void setTimeEscape      ( WTime time    );
    void setInfoEscape      ( QString info  );
    void setTimeReverseTimer( WTimeExt time );

    void setColorDay ( QDate date, QColor color );
    void setColorDays( QStringList Colors       );

    void setColorLight( QColor color );

    void updateTimeEnd( int id, WTime time );

private:

    void configuringGUI();
    void connectSingnalSlot();
    void updateTitles();

    Ui::WorkTimeWindow * gui;

private slots:

    // Window

    void closeWorkTime();

    // Calendar slots
    void selectDate();
    void todayClick();

    void changeTypeDay( int type );

    // Intervals list
    void contextMenuIntervals( QPoint MenuPoint );
    void renameInterval( QListWidgetItem * RenamedInterval );
    void clearTime();
    void addInterval();
    void removeInterval();
    void selectInterval( const int id );

    // Change time slots
    void changeTimeStart( QTime time );
    void changeTimeEnd  ( QTime time );
    void changeTimeNeed ( QTime time );
    void finishChangeTimeStart();
    void finishChangeTimeEnd  ();

    // Change note
    void changeNote();

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

    void userSelectDate     ( const QDate & date                  );
    void userSelectInterval ( const int id                        );
    void userRenameInterval ( const int id, const QString & title );
    void userRemoveInterval ( const int id                        );
    void userChangeTypeDay  ( int     type                        );
    void userChangeTimeStart( int id, WTime time                  );
    void userChangeTimeEnd  ( int id, WTime time                  );
    void userChangeTimeNeed ( WTime   time                        );
    void userChangeNote     ( QString note                        );

    void userAddInterval( const QString & title );

    void showSalary     ();
    void showTableTime  ();
    void showSchedule   ();
    void showSeveralDays();
    void showChanges    ();
    void showSettings   ();

protected:

    void mousePressEvent  ( QMouseEvent * MouseEvent );
    void mouseReleaseEvent( QMouseEvent * MouseEvent );
    void mouseMoveEvent   ( QMouseEvent * MouseEvent );
};
// ------------------------------------------------------------------------------------ //

#endif // WORKTIMEWINDOW_H
