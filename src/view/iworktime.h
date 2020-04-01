// ---------------------------- //
#ifndef IWORKTIME
#define IWORKTIME
// ---------------------------- //
#include "wtime.h"
#include "wtimeext.h"
// ---------------------------- //
class QDate      ;
class QColor     ;
class QString    ;
class QStringList;
// ---------------------------- //

class IWorkTime
{

public: // Members

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

    virtual void setTimeEscape      ( WTime time    ) = 0;
    virtual void setInfoEscape      ( QString info  ) = 0;
    virtual void setTimeReverseTimer( WTimeExt time ) = 0;

    virtual void setColorDay ( QDate date, QColor color ) = 0;
    virtual void setColorDays( QStringList Colors       ) = 0;

    virtual void setColorLight( QColor color ) = 0;

    virtual void updateTimeEnd( int id, WTime time ) = 0;

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
};

#endif // IWORKTIME

