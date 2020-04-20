// ------------------------------------ //
#ifndef DATABASEWT_H
#define DATABASEWT_H
// ------------------------------------ //
#include <QtSql>
#include <QObject>
// ------------------------------------ //
#include "wtime.h"
#include "interval.h"
#include "ilog.h"

// ------------------------------------ //

class DataBaseWT : public QObject, public ILog
{
    Q_OBJECT

public:

    enum TypeTimeNeed
    {
        User    ,
        Schedule
    };

private:

    QSqlDatabase DataBase;

public:

    DataBaseWT( QObject* parent = NULL );
    ~DataBaseWT();

    bool isOpenDataBase  ();
    bool isExistsDataBase();

    bool openDataBase    ();
    void closeDataBase   ();

    QSqlDatabase & sqlDatabase();

public:

    bool isExists( const QDate &Date, const int RateID  );
    int dateID( const QDate &Date, const int RateID );
    int lastWorkDaysID();

    WTime timeNeed( const QDate & date, const int rateID );
    WTime timeNeedSchedule( const QDate& date, const int rateID );

    QString note( const QDate & date, const int rateID );

    void intervals( const QDate & date, const int rateID, QList<Interval*> & intervals );

    int typeDay( const QDate & date, const int rateID );

    int lastIntervalID();
    int intervalFromDayID( const int DateID, const int id );
    void addInterval( const QDate & date, const int rateID, const QString & title );
    void removeInterval( const QDate& Date, const int RateID, const int id );
    void renameInterval( const QDate & date, const int rateID, const int id, const QString & title );

    void setTimeStart(const QDate & date, const int rateID, const int id , const WTime & time );
    void setTimeEnd ( const QDate & date, const int rateID, const int id , const WTime & time );
    void setTimeNeed( const QDate & date, const int rateID , WTime time, TypeTimeNeed type );

    void setTypeDay( const QDate & date, const int rateID, const int type );

    int noteId( const int DateID );
    int lastNoteID();
    void setNote( const QDate & date, const int rateID, const QString & note );

    int scheduleId( const int DateID );
    int lastScheduleID();

    void insertYear( const QDate & date, int rateID );
    void insertScheduleYear(  const QDate & date, int rateID  );

    int  lastSalaryID();
    int  salaryFromDayID( const int DateID, const int id );
    void getMonthSalaries( const int RateID, const QDate & Date, QList<QDate> & Dates, QList<float> & Salaries );
    void addSalary( const int RateID, const QDate & Date, const float salary );
    void removeSalary( const int id );
    void removeSalary( const int RateID, const QDate & Date, const int id );
    void changeSalary( const int RateID, const QDate & Date, const int id, float value );

    void insertWorkingRates( const QStringList & ratesList );

private:

    bool configureDataBase();

    QString getStringFromDate( const QDate& Date );
    QString getStringFromTime( const QTime& Time );

    QTime getTimeFromString( const QString& TimeString );
    QDate getDateFromString( const QString& DateString );

signals: // From ILog

    void infoLog ( const QString & );
    void errorLog( const QString & );
};

#endif // DATABASEWT_H
