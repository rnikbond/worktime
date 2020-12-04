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

    QPoint OldPos;
    bool isMoveWindow;

    bool isRunTimer;

public:

    explicit WorkTimeWindow( QWidget * parent = 0 );
    ~WorkTimeWindow();

    void setInfoUser( QString text );

    void setSalaryExists     ( bool isExists );
    void setTableTimeExists  ( bool isExists );
    void setScheduleExists   ( bool isExists );
    void setSeveralDaysExists( bool isExists );
    void setChangesExists    ( bool isExists );
    void setSettingsExists   ( bool isExists );
    void setCalcTimeExists   ( bool isExists );
    void setNotifyExists     ( bool isExists );
    void setUpdatesExists    ( bool isExists );

public: // Interface functions

    void setSelectedDate( const QDate & date );

    // Инициализация общих элементов
    void setTypesDay( QStringList list );

    void setTypeDay   ( int         type         );
    void setTimeStart ( WTime       time, int id );
    void setTimeEnd   ( WTime       time, int id );
    void setIntervals ( QStringList list         );
    void setNote      ( QString     note         );
    void setTypeData  ( int type                 );

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

    void setInfoEscape      ( QString info );
    void setTimeReverseTimer( QString time );

    void setColorDay ( QDate date, QColor color );
    void setColorDays( QStringList Colors       );

    void setColorLight( QColor color );

    void updateTimeEnd( int id, WTime time );

    void setShownMenu   ( bool isShown );
    void setSelectedPage( int  page    );

private:

    void configuringGUI();
    void connectSingnalSlot();
    void updateTitles();

    Ui::WorkTimeWindow * gui;

private slots:

    // Window
    void selectPage( int page );

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
    void convertToHtml();
    void convertToSimpleText();
    void contextMenuNote( QPoint );

    void checkedUserData( bool isChecked );

    // Menu slots
    void MenuClick       ();
    void SalaryClick     ();
    void TableTimeClick  ();
    void ScheduleClick   ();
    void SeveralDaysClick();
    void CalcTimeClick   ();
    void NotifyClick     ();
    void ChangesClick    ();
    void SettingsClick   ();
    void UpdateClick     ();

    void stopTimeClick();
    void runTimeClick();

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
    void userChangeTypeData ( int type                            );

    void userAddInterval( const QString & title );

    void userChangeVisibleMenu( bool );
    void userSelectPage       ( int  );

    void closeWindow();
    void changedGeometry( QRect geometry );

    void showSalary     ();
    void showTableTime  ();
    void showSchedule   ();
    void showSeveralDays();
    void showCalcTime   ();
    void showNotify     ();
    void showChanges    ();
    void showSettings   ();

    void userDropUpdate( QString );

    void runTimer( bool isRun, bool isNewInterval );

protected:

    void showEvent     ( QShowEvent      * ShowEvent  );
    void hideEvent     ( QHideEvent      * HideEvent  );
    void closeEvent    ( QCloseEvent     * CloseEvent );
    void dragEnterEvent( QDragEnterEvent * event      );
    void dropEvent     ( QDropEvent      * event      );
};
// ------------------------------------------------------------------------------------ //

#endif // WORKTIMEWINDOW_H
