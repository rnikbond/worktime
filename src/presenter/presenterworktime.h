// ---------------------------- //
#ifndef PRESENTERWORKTIME_H
#define PRESENTERWORKTIME_H
// ---------------------------- //
#include <QColor>
#include <QObject>
// ---------------------------- //
#include "wtime.h"
#include "wtimeext.h"
// ---------------------------- //
class IViewWorkTime ;
class IModelWorkTime;
class IDesktopWidget;
// ---------------------------- //

class PresenterWorkTime : public QObject
{
    Q_OBJECT

    IViewWorkTime  * ViewWT ;
    IModelWorkTime * ModelWT;
    IDesktopWidget * Desktop;

public:

    explicit PresenterWorkTime( QObject * parent = 0 );

    void setView  ( IViewWorkTime  * View   );
    void setModel ( IModelWorkTime * Model  );
    void setWidget( IDesktopWidget * Widget );

private:

    void connectModel( IModelWorkTime * Model );
    void connectView ( IViewWorkTime  * View  );

private slots:

    void refreshFull         ();
    void refreshDataDay      ();
    void refreshTimeDay      ();
    void refreshTimeWeek     ();
    void refreshTimeMonth    ();
    void refreshStatisticTime();
    void refreshTimeStart    ( int id, WTime time       );
    void refreshTimeEnd      ( int id, WTime time       );
    void refreshTimeReverse  ( QString time             );
    void refreshInfoEscape   ( QString info );
    void refreshColorDay     ( QDate date, QColor color );
    void refreshColorLight   ( QColor color             );

private slots:

    // Slots for View
    void userSelectDate( const QDate & date );
    void userSelectInterval ( int id );
    void userRemoveInterval ( int id );
    void userRenameInterval ( int id, QString title );
    void userChangeTimeStart( int id, WTime time );
    void userChangeTimeEnd  ( int id, WTime time );
    void userChangeTimeNeed ( WTime time         );
    void userAddInterval    ( QString title      );
    void userChangeTypeDay  ( int type           );
    void userChangeNote     ( QString note       );
    void userChangeTypeData ( int type           );

    void userChangeTimerState( bool state , bool isNewInterval );
};

#endif // PRESENTERWORKTIME_H
