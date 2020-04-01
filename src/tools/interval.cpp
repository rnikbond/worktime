// --------------------------- //
#include "interval.h"
// --------------------------- //

Interval::Interval( QObject* parent ) : QObject( parent )
{
    Title = "";
}
// --------------------------------------------------------------------------------- //

Interval::Interval( QString title, QObject* parent ) : QObject( parent )
{
    Title = title;
}
// --------------------------------------------------------------------------------- //

void Interval::setTitle(const QString title )
{
    Title = title;
}
// --------------------------------------------------------------------------------- //

void Interval::setTimeStart( const WTime time )
{
    TimeStart = time;
}
// --------------------------------------------------------------------------------- //

void Interval::setTimeEnd( const WTime time )
{
    TimeEnd = time;
}
// --------------------------------------------------------------------------------- //

QString Interval::title()
{
    return Title;
}
// --------------------------------------------------------------------------------- //

WTime Interval::timeStart()
{
    return TimeStart;
}
// --------------------------------------------------------------------------------- //

WTime Interval::timeEnd()
{
    return TimeEnd;
}
// --------------------------------------------------------------------------------- //

WTime Interval::timeWorked()
{
    return ( TimeEnd - TimeStart );
}
// --------------------------------------------------------------------------------- //
