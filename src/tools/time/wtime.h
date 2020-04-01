// ------------------------------------ //
#ifndef WTIME_H
#define WTIME_H
// ------------------------------------ //
#include <QTime>
// ------------------------------------ //
#include "wtimeext.h"
// ------------------------------------ //

class WTime
{
    int Hours  ;
    int Minutes;

public:

    WTime();
    WTime( QTime Time );
    WTime( WTimeExt Time );
    WTime( QString StringTime );
    WTime( int h, int m );

    void clear();

    void setTime( int h, int m );

    void setHour   ( int h );
    void setMinutes( int m );

    void addHour   ( int h );
    void addMinutes( int m );

    bool isEmpty() const;

    int hours  () const;
    int minutes() const;

    QTime   toQTime    () const;
    QString toString   () const;
    int     toMinutes  () const;

    friend const WTime operator + ( const WTime& Time1, const WTime& Time2 );
    friend const WTime operator - ( const WTime& Time1, const WTime& Time2 );

    friend WTime& operator += ( WTime& Time, const WTime& TimeAdd );
    friend WTime& operator -= ( WTime& Time, const WTime& TimeAdd );

    friend bool operator == ( const WTime &Time1, const WTime &Time2 );
    friend bool operator != ( const WTime &Time1, const WTime &Time2 );
    friend bool operator <  ( const WTime &Time1, const WTime &Time2 );
    friend bool operator >  ( const WTime &Time1, const WTime &Time2 );
    friend bool operator <  ( const WTime &Time1, const QTime &Time2 );
    friend bool operator >  ( const WTime &Time1, const QTime &Time2 );
    friend bool operator <= ( const WTime &Time1, const WTime &Time2 );
    friend bool operator >= ( const WTime &Time1, const WTime &Time2 );

private:

	void checkTime();
    void NormalizeTime( int& hours, int& minutes );
};

#endif // WTIME_H
