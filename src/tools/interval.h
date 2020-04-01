// --------------------------- //
#ifndef INTERVAL_H
#define INTERVAL_H
// --------------------------- //
#include <QObject>
// --------------------------- //
#include "wtime.h"
// --------------------------- //

class Interval : public QObject
{
    Q_OBJECT

    QString Title;

    WTime TimeStart;
    WTime TimeEnd;

public:

    Q_DISABLE_COPY(Interval)

    Interval( QObject* parent = NULL );
    Interval( QString title, QObject* parent = NULL );

    void setTitle    ( const QString title );
    void setTimeStart( const WTime time );
    void setTimeEnd  ( const WTime time );

    QString title();
    WTime timeStart();
    WTime timeEnd();
    WTime timeWorked();
};

#endif // INTERVAL_H
