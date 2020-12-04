// ------------------------------------ //
#include "wtime.h"
// ------------------------------------ //
#include "wtimeext.h"
// ------------------------------------ //

//WTimeExt operator = ( WTimeExt& Time1, const WTime& Time2 )
//{
//    Time1.addHour   ( Time2.hours  () );
//    Time1.addMinutes( Time2.minutes() );
//    Time1.addSeconds( Time2.seconds() );

//    return Time;
//}
// ------------------------------------------------------------------------ //

WTimeExt& WTimeExt::operator = (const WTime &Time )
{
    setHour   ( Time.hours() );
    setMinutes( Time.minutes() );
    setSeconds( 0 );

    return *this;
}
// ------------------------------------------------------------------------ //

const WTimeExt operator + ( const WTimeExt& Time1, const WTimeExt& Time2 )
{
    return WTimeExt( Time1.hours  () + Time2.hours  (),
                     Time1.minutes() + Time2.minutes(),
                     Time1.seconds() + Time2.seconds() );
}
// ------------------------------------------------------------------------ //


const WTimeExt operator - (const WTimeExt &Time1, const WTimeExt &Time2)
{
    return WTimeExt( Time1.hours  () - Time2.hours  (),
                     Time1.minutes() - Time2.minutes(),
                     Time1.seconds() - Time2.seconds() );
}
// ------------------------------------------------------------------------ //

WTimeExt& operator += ( WTimeExt& Time, const WTimeExt& TimeAdd )
{
    Time.addHour   ( TimeAdd.hours  () );
    Time.addMinutes( TimeAdd.minutes() );
    Time.addSeconds( TimeAdd.seconds() );

    return Time;
}
// ------------------------------------------------------------------------ //

WTimeExt& operator -= ( WTimeExt& Time, const WTimeExt& TimeAdd )
{
    Time.addHour   ( -TimeAdd.hours  () );
    Time.addMinutes( -TimeAdd.minutes() );
    Time.addSeconds( -TimeAdd.seconds() );

    return Time;
}
// ------------------------------------------------------------------------ //

bool operator == ( const WTimeExt& Time1, const WTimeExt& Time2 )
{
    return Time1.hours() == Time2.hours() && Time1.minutes() == Time2.minutes() && Time1.seconds() == Time2.seconds();
}
// ------------------------------------------------------------------------ //

bool operator != ( const WTimeExt& Time1, const WTimeExt& Time2 )
{
    return !( Time1 == Time2 );
}
// ------------------------------------------------------------------------ //

bool operator < ( const WTimeExt& Time1, const WTimeExt& Time2 )
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
            if( Time1.seconds() < Time2.seconds() )
            {
                return true;
            }
            else if( Time1.seconds() > Time2.seconds() )
            {
                return false;
            }
            else
            {
                return false;
            }
        }
    }
}
// ------------------------------------------------------------------------ //

bool operator > ( const WTimeExt& Time1, const WTimeExt& Time2 )
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
            if( Time1.seconds() > Time2.seconds() )
            {
                return true;
            }
            else if( Time1.seconds() < Time2.seconds() )
            {
                return false;
            }
            else
            {
                return false;
            }
        }
    }
}
// ------------------------------------------------------------------------ //

bool operator < ( const WTimeExt& Time1, const QTime& Time2 )
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
            if( Time1.seconds() < Time2.second() )
            {
                return true;
            }
            else if( Time1.seconds() > Time2.second() )
            {
                return false;
            }
            else
            {
                return false;
            }
        }
    }
}
// ------------------------------------------------------------------------ //

bool operator > ( const WTimeExt& Time1, const QTime& Time2 )
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
            if( Time1.seconds() > Time2.second() )
            {
                return true;
            }
            else if( Time1.seconds() < Time2.second() )
            {
                return false;
            }
            else
            {
                return false;
            }
        }
    }
}
// ------------------------------------------------------------------------ //

WTimeExt::WTimeExt()
{
    Hours   = 0;
    Minutes = 0;
    Seconds = 0;

    isRecheck = true;
}
// ------------------------------------------------------------------------ //

WTimeExt::WTimeExt(int h, int m , int s)
{
    setTime( h, m, s );
    isRecheck = true;
}
// ------------------------------------------------------------------------ //

WTimeExt::WTimeExt( QTime Time )
{
    setTime( Time.hour(), Time.minute(), Time.second() );
    isRecheck = true;
}
// ------------------------------------------------------------------------ //

WTimeExt::WTimeExt( WTime Time )
{
    setTime( Time.hours(), Time.minutes(), 0 );
    isRecheck = true;
}
// ------------------------------------------------------------------------ //

WTimeExt::WTimeExt( QString StringTime )
{
    QTime Time = QTime::fromString( StringTime, "hh:mm:ss" );

    setTime( Time.hour(), Time.minute(), Time.second() );
    isRecheck = true;
}
// ------------------------------------------------------------------------ //

void WTimeExt::setRecheck( bool recheck )
{
    isRecheck = recheck;
}
// ------------------------------------------------------------------------ //

void WTimeExt::clear()
{
    Hours   = 0;
    Minutes = 0;
    Seconds = 0;
}
// ------------------------------------------------------------------------ //

void WTimeExt::setTime( int h, int m, int s )
{
    Hours   = h;
    Minutes = m;
    Seconds = s;

    if( isRecheck )
        checkTime();
}
// ------------------------------------------------------------------------ //

QTime WTimeExt::toQTime() const
{
    return QTime( Hours, Minutes, Seconds );
}
// ------------------------------------------------------------------------ //

WTime WTimeExt::toWTime() const
{
    return WTime( Hours, Minutes );
}
// ------------------------------------------------------------------------ //

QString WTimeExt::toString() const
{
    return QString::number(Hours  ).rightJustified( 2, '0') + ":" +
           QString::number(Minutes).rightJustified( 2, '0') + ":" +
           QString::number(Seconds).rightJustified( 2, '0');
}
// ------------------------------------------------------------------------ //

bool WTimeExt::isEmpty() const
{
    return Hours == 0 && Minutes == 0 && Seconds == 0;
}
// ------------------------------------------------------------------------ //

int WTimeExt::hours() const
{
    return Hours;
}
// ------------------------------------------------------------------------ //

int WTimeExt::minutes() const
{
    return Minutes;
}
// ------------------------------------------------------------------------ //

int WTimeExt::seconds() const
{
    return Seconds;
}
// ------------------------------------------------------------------------ //

void WTimeExt::setHour( int h )
{
    Hours = h;

    if( isRecheck )
        checkTime();
}
// ------------------------------------------------------------------------ //

void WTimeExt::setMinutes( int m )
{
    Minutes = m;

    if( isRecheck )
        checkTime();
}
// ------------------------------------------------------------------------ //

void WTimeExt::setSeconds( int s )
{
    Seconds = s;

    if( isRecheck )
        checkTime();
}
// ------------------------------------------------------------------------ //

void WTimeExt::addHour( int h )
{
    Hours += h;

    if( isRecheck )
        checkTime();
}
// ------------------------------------------------------------------------ //

void WTimeExt::addMinutes( int m )
{
    Minutes += m;

    if( isRecheck )
        checkTime();
}
// ------------------------------------------------------------------------ //

void WTimeExt::addSeconds( int s )
{
    Seconds += s;

    if( isRecheck )
        checkTime();
}
// ------------------------------------------------------------------------ //

void WTimeExt::checkTime()
{
    NormalizeTime( Hours, Minutes, Seconds );
}
// ------------------------------------------------------------------------ //

void WTimeExt::NormalizeTime( int& hours, int& minutes, int& seconds )
{
    if( hours < 0 )
    {
        hours   = 0;
        minutes = 0;
        seconds = 0;
        return;
    }

    //! Нормализация минут !//

    while( seconds < 0 )
    {
        if( minutes == 0 && hours == 0 )
        {
            seconds = 0;
            return;
        }

        minutes--;
        seconds += 60;
    }

    while( seconds >= 60 )
    {
        minutes++;
        seconds -= 60;
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
