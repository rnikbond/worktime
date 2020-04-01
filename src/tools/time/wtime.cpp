// ------------------------------------ //
#include "wtime.h"
// ------------------------------------ //

const WTime operator + ( const WTime& Time1, const WTime& Time2 )
{
    return WTime( Time1.hours  () + Time2.hours  (),
                  Time1.minutes() + Time2.minutes() );
}
// ------------------------------------------------------------------------ //


const WTime operator - (const WTime &Time1, const WTime &Time2)
{
    return WTime( Time1.hours  () - Time2.hours  (),
                  Time1.minutes() - Time2.minutes() );
}
// ------------------------------------------------------------------------ //

WTime& operator += ( WTime& Time, const WTime& TimeAdd )
{
    Time.addHour   ( TimeAdd.hours  () );
    Time.addMinutes( TimeAdd.minutes() );

    return Time;
}
// ------------------------------------------------------------------------ //

WTime& operator -= ( WTime& Time, const WTime& TimeAdd )
{
    Time.addHour   ( -TimeAdd.hours  () );
    Time.addMinutes( -TimeAdd.minutes() );

    return Time;
}
// ------------------------------------------------------------------------ //

bool operator == ( const WTime& Time1, const WTime& Time2 )
{
    return Time1.hours() == Time2.hours() && Time1.minutes() == Time2.minutes();
}
// ------------------------------------------------------------------------ //

bool operator != ( const WTime& Time1, const WTime& Time2 )
{
    return !( Time1 == Time2 );
}
// ------------------------------------------------------------------------ //

bool operator < ( const WTime& Time1, const QTime& Time2 )
{
    if( Time1.hours() < Time2.hour() )
    {
        return true;
    }
    else if( Time1.hours() > Time2.hour() )
    {
        return false;
    }
    else
    {
        if( Time1.minutes() < Time2.minute() )
        {
            return true;
        }
        else if( Time1.minutes() > Time2.minute() )
        {
            return false;
        }
        else
        {
            return false;
        }
    }
}
// ------------------------------------------------------------------------ //

bool operator > ( const WTime& Time1, const QTime& Time2 )
{
    if( Time1.hours() > Time2.hour() )
    {
        return true;
    }
    else if( Time1.hours() < Time2.hour() )
    {
        return false;
    }
    else
    {
        if( Time1.minutes() > Time2.minute() )
        {
            return true;
        }
        else if( Time1.minutes() < Time2.minute() )
        {
            return false;
        }
        else
        {
            return false;
        }
    }
}
// ------------------------------------------------------------------------ //

bool operator < ( const WTime& Time1, const WTime& Time2 )
{
    if( Time1.hours() < Time2.hours() )
    {
        return true;
    }
    else if( Time1.hours() > Time2.hours() )
    {
        return false;
    }
    else
    {
        if( Time1.minutes() < Time2.minutes() )
        {
            return true;
        }
        else if( Time1.minutes() > Time2.minutes() )
        {
            return false;
        }
        else
        {
            return false;
        }
    }
}
// ------------------------------------------------------------------------ //

bool operator > ( const WTime& Time1, const WTime& Time2 )
{
    if( Time1.hours() > Time2.hours() )
    {
        return true;
    }
    else if( Time1.hours() < Time2.hours() )
    {
        return false;
    }
    else
    {
        if( Time1.minutes() > Time2.minutes() )
        {
            return true;
        }
        else if( Time1.minutes() < Time2.minutes() )
        {
            return false;
        }
        else
        {
            return false;
        }
    }
}
// ------------------------------------------------------------------------ //

bool operator <= ( const WTime &Time1, const WTime &Time2 )
{
    return Time1 < Time2 || Time1 == Time2;
}
// ------------------------------------------------------------------------ //

bool operator >= ( const WTime &Time1, const WTime &Time2 )
{
    return Time1 > Time2 || Time1 == Time2;
}
// ------------------------------------------------------------------------ //

WTime::WTime()
{
	Hours   = 0;
	Minutes = 0;
}
// ------------------------------------------------------------------------ //

WTime::WTime(int h, int m )
{
    setTime( h, m );
}
// ------------------------------------------------------------------------ //

WTime::WTime( QTime Time )
{
    setTime( Time.hour(), Time.minute() );
}
// ------------------------------------------------------------------------ //

WTime::WTime( WTimeExt Time )
{
    setTime( Time.hours(), Time.minutes() );
}
// ------------------------------------------------------------------------ //

WTime::WTime( QString StringTime )
{
    QTime Time = QTime::fromString( StringTime, "hh:mm" );

    setTime( Time.hour(), Time.minute() );
}
// ------------------------------------------------------------------------ //

void WTime::clear()
{
    Hours   = 0;
    Minutes = 0;
}
// ------------------------------------------------------------------------ //

void WTime::setTime( int h, int m )
{
    Hours   = h;
    Minutes = m;

	checkTime();
}
// ------------------------------------------------------------------------ //

QTime WTime::toQTime() const
{
    return QTime( Hours, Minutes );
}
// ------------------------------------------------------------------------ //

QString WTime::toString() const
{
	return QString::number(Hours  ).rightJustified( 2, '0') + ":" +
           QString::number(Minutes).rightJustified( 2, '0');
}
// ------------------------------------------------------------------------ //

int WTime::toMinutes() const
{
    return Hours * 60 + Minutes;
}
// ------------------------------------------------------------------------ //

bool WTime::isEmpty() const
{
    return Hours == 0 && Minutes == 0;
}
// ------------------------------------------------------------------------ //

int WTime::hours() const
{
	return Hours;
}
// ------------------------------------------------------------------------ //

int WTime::minutes() const
{
	return Minutes;
}
// ------------------------------------------------------------------------ //

void WTime::setHour( int h )
{
	Hours = h;
	checkTime();
}
// ------------------------------------------------------------------------ //

void WTime::setMinutes( int m )
{
	Minutes = m;
	checkTime();
}
// ------------------------------------------------------------------------ //

void WTime::addHour( int h )
{
    Hours += h;
    checkTime();
}
// ------------------------------------------------------------------------ //

void WTime::addMinutes( int m )
{
    Minutes += m;
    checkTime();
}
// ------------------------------------------------------------------------ //

void WTime::checkTime()
{
    NormalizeTime( Hours, Minutes );
}
// ------------------------------------------------------------------------ //

void WTime::NormalizeTime( int& hours, int& minutes )
{
    if( hours < 0 )
    {
        hours   = 0;
        minutes = 0;
        return;
    }

    //! Нормализация минут !//

    while( minutes < 0 )
    {
        if( hours == 0 )
        {
            minutes = 0;
            return;
        }

        hours--;
        minutes += 60;
    }

    while( minutes >= 60 )
    {
        hours++;
        minutes -= 60;
    }
}
// ------------------------------------------------------------------------ //
