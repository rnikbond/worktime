// ---------------------------- //
#ifndef PRESENTERWORKTIME_H
#define PRESENTERWORKTIME_H
// ---------------------------- //
#include <QObject>
// ---------------------------- //
#include "wtime.h"
#include "wtimeext.h"
// ---------------------------- //
class IWorkTime;
// ---------------------------- //

class PresenterWorkTime : public QObject
{
    Q_OBJECT

    IWorkTime * ViewWT;

public:

    explicit PresenterWorkTime( IWorkTime * View, QObject * parent = 0 );

private:

    void connectView( IWorkTime * View );

    void refreshFull     ();
    void refreshDataDay  ();
    void refreshTimeDay  ();
    void refreshTimeWeek ();
    void refreshTimeMonth();
    void refreshTimeStart     ( int id, WTime time       );
    void refreshTimeEnd       ( int id, WTime time       );
    void refreshTimeReverse   ( WTimeExt time            );
    void refreshTimeEscape    ( WTime time, QString info );
    void refreshColorDay      ( QDate date, QColor color );
    void refreshColorLight    ( QColor color             );

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
};

#endif // PRESENTERWORKTIME_H
