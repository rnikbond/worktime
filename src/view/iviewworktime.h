// ---------------------------- //
#ifndef IVIEWWORKTIME
#define IVIEWWORKTIME
// ---------------------------- //
#include <QRect>
#include <QDate>
#include <QColor>
#include <QString>
#include <QStringList>
// ---------------------------- //
#include "wtime.h"
#include "wtimeext.h"
// ---------------------------- //

class IViewWorkTime
{

public: // Members

    virtual void setSelectedDate( const QDate & date ) = 0;

    virtual void setTypesDay( QStringList list ) = 0;

    virtual void setTypeDay   ( int         type         ) = 0;
    virtual void setTimeStart ( WTime       time, int id ) = 0;
    virtual void setTimeEnd   ( WTime       time, int id ) = 0;
    virtual void setIntervals ( QStringList list         ) = 0;
    virtual void setNote      ( QString     note         ) = 0;

    virtual void setTimeWorkedInDay   ( WTime time                ) = 0;
    virtual void setTimeStatisticInDay( WTime time, QString Title ) = 0;
    virtual void setTimeNeedInDay     ( WTime time                ) = 0;

    virtual void setTimeWorkedInWeek   ( WTime time                ) = 0;
    virtual void setTimeStatisticInWeek( WTime time, QString Title ) = 0;
    virtual void setTimeNeedInWeek     ( WTime time                ) = 0;

    virtual void setTimeWorkedInMonth   ( WTime time                ) = 0;
    virtual void setTimeStatisticInMonth( WTime time, QString Title ) = 0;
    virtual void setTimeNeedInMonth     ( WTime time                ) = 0;

    virtual void setTimeWorkedToDay   ( WTime time                ) = 0;
    virtual void setTimeStatisticToDay( WTime time, QString Title ) = 0;
    virtual void setTimeNeedToDay     ( WTime time                ) = 0;

    virtual void setInfoEscape      ( QString info ) = 0;
    virtual void setTimeReverseTimer( QString time ) = 0;

    virtual void setColorDay ( QDate date, QColor color ) = 0;
    virtual void setColorDays( QStringList Colors       ) = 0;

    virtual void setColorLight( QColor color ) = 0;

    virtual void updateTimeEnd( int id, WTime time ) = 0;

    virtual void setShownMenu   ( bool isShown ) = 0;
    virtual void setSelectedPage( int  page    ) = 0;

    virtual void setSalaryExists     ( bool isExists ) = 0;
    virtual void setTableTimeExists  ( bool isExists ) = 0;
    virtual void setScheduleExists   ( bool isExists ) = 0;
    virtual void setSeveralDaysExists( bool isExists ) = 0;
    virtual void setChangesExists    ( bool isExists ) = 0;
    virtual void setSettingsExists   ( bool isExists ) = 0;

public: // Signals

    virtual void userSelectDate    ( const QDate   & date                ) = 0;
    virtual void userSelectInterval( const int id                        ) = 0;
    virtual void userRemoveInterval( const int id                        ) = 0;
    virtual void userRenameInterval( const int id, const QString & title ) = 0;

    virtual void userAddInterval    ( const QString & title ) = 0;
    virtual void userChangeTypeDay  ( int     type          ) = 0;
    virtual void userChangeTimeStart( int id, WTime time    ) = 0;
    virtual void userChangeTimeEnd  ( int id, WTime time    ) = 0;
    virtual void userChangeTimeNeed ( WTime   time          ) = 0;
    virtual void userChangeNote     ( QString note          ) = 0;

    virtual void userChangeVisibleMenu( bool ) = 0;
    virtual void userSelectPage       ( int  ) = 0;

    virtual void closeWindow() = 0;
    virtual void changedGeometry( QRect geometry ) = 0;

    virtual void showSalary     () = 0;
    virtual void showTableTime  () = 0;
    virtual void showSchedule   () = 0;
    virtual void showSeveralDays() = 0;
    virtual void showChanges    () = 0;
    virtual void showSettings   () = 0;

    virtual void runTimer( bool isRun, bool isNewInterval ) = 0;
};

#endif // IVIEWWORKTIME

