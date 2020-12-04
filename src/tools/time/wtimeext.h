// ------------------------------------ //
#ifndef WTIMEEXT_H
#define WTIMEEXT_H
// ------------------------------------ //
#include <QTime>
// ------------------------------------ //
class WTime;
// ------------------------------------ //

class WTimeExt
{
    int Hours  ;
    int Minutes;
    int Seconds;

    bool isRecheck;

public:

    WTimeExt();
    WTimeExt( QTime Time );
    WTimeExt( WTime Time );
    WTimeExt( QString StringTime );
    WTimeExt( int h, int m, int s );

    void checkTime();

    void setRecheck( bool recheck );

    void clear();

    void setTime( int h, int m, int s );

    void setHour   ( int h );
    void setMinutes( int m );
    void setSeconds( int s );

    void addHour   ( int h );
    void addMinutes( int m );
    void addSeconds( int s );

    bool isEmpty() const;

    int hours  () const;
    int minutes() const;
    int seconds() const;

    QTime   toQTime () const;
    WTime   toWTime () const;
    QString toString() const;

    friend const WTimeExt operator + ( const WTimeExt& Time1, const WTimeExt& Time2 );
    friend const WTimeExt operator - ( const WTimeExt& Time1, const WTimeExt& Time2 );

    friend WTimeExt& operator += ( WTimeExt& Time , const WTimeExt& TimeAdd );
    friend WTimeExt& operator -= ( WTimeExt& Time , const WTimeExt& TimeAdd );

    friend bool operator == ( const WTimeExt &Time1, const WTimeExt &Time2 );
    friend bool operator != ( const WTimeExt &Time1, const WTimeExt &Time2 );
    friend bool operator <  ( const WTimeExt &Time1, const WTimeExt &Time2 );
    friend bool operator >  ( const WTimeExt &Time1, const WTimeExt &Time2 );
    friend bool operator <= ( const WTimeExt &Time1, const WTimeExt &Time2 );
    friend bool operator >= ( const WTimeExt &Time1, const WTimeExt &Time2 );

    friend bool operator <  ( const WTimeExt &Time1, const QTime &Time2 );
    friend bool operator >  ( const WTimeExt &Time1, const QTime &Time2 );

    WTimeExt& operator =(const WTime &Time );

private:

    void NormalizeTime( int& hours, int& minutes, int& seconds );
};

#endif // WTIMEEXT_H
