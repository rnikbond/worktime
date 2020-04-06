// ------------------------------------ //
#include <QFile>
// ------------------------------------ //
#include "helperwt.h"
// ------------------------------------ //
#include "databasewt.h"
// ------------------------------------ //

DataBaseWT::DataBaseWT( QObject * parent ) : QObject( parent )
{
    DataBase = QSqlDatabase::addDatabase( "QSQLITE" );
    DataBase.setDatabaseName( HelperWT::pathToDataBase() );

    openDataBase();
    configureDataBase();
}
// ------------------------------------------------------------------------------------ //

DataBaseWT::~DataBaseWT()
{
    closeDataBase();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::isExists
 * \param date Дата, наличие которой в базе данных нужно проверить
 * \param rateID Идентификатор рабочей ставки
 * \return TRUE, если дата \a Date есть в базе данных. Иначе FALSE.
 */
bool DataBaseWT::isExists( const QDate & date, const int rateID  )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::isExists(" << date << ", " << rateID << ");
#endif

    return dateID( date, rateID ) >= 0;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::dateID
 * \param date Дата, идентификатор которой будет возвращен
 * \param rateID Идентификатор рабочей ставки
 * \return Идентификатор даты. Если дата не найдена, возвращается -1.
 */
int DataBaseWT::dateID( const QDate & date, const int rateID )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::dateID(" << date << ", " << rateID << ");
#endif

    int DateID = -1;

    QSqlQuery GetDateIdQuery( DataBase );

    GetDateIdQuery.prepare( "SELECT ID FROM WorkDays WHERE Year=:Year AND Month=:Month AND Day=:Day AND ID_WorkingRate=:ID_WR" );
    GetDateIdQuery.bindValue(":Year" , date.year () );
    GetDateIdQuery.bindValue(":Month", date.month() );
    GetDateIdQuery.bindValue(":Day"  , date.day  () );
    GetDateIdQuery.bindValue(":ID_WR", rateID       );

    if( GetDateIdQuery.exec() )
    {
        if( GetDateIdQuery.last() )
            DateID = GetDateIdQuery.value(0).toInt();
    }

    return DateID;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::lastWorkDaysID
 * \return Послений идентификатор, добавленный в таблицу \a WorkDays.
 */
int DataBaseWT::lastWorkDaysID()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::lastWorkDaysID()";
#endif

    QSqlQuery GetLastIdQuery( DataBase );
    GetLastIdQuery.prepare( "SELECT ID FROM WorkDays ORDER BY ID DESC LIMIT 1" );

    if( GetLastIdQuery.exec() )
    {
        if( GetLastIdQuery.last() )
            return GetLastIdQuery.value(0).toInt();
    }
    else
    {
        qWarning() << "DataBaseWT::lastWorkDaysID() :: " << GetLastIdQuery.lastError().text();
    }

    return -1;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::timeNeed
 * \param date Дата, для которой нужно получить время, сколько нужно отработать
 * \param rateID Идентификатор рабочей ставки
 * \return Время, сколько нужно отработать за \a date
 */
WTime DataBaseWT::timeNeed( const QDate & date, const int rateID )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::timeNeed( " << date << ", " << rateID << " )";
#endif

    WTime TimeNeed( 0, 0 );

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] timeNeed( " << date << ", " << rateID << " ) :: Day not found";
    }
    else
    {
        QSqlQuery GetTimeNeedQuery( DataBase );

        GetTimeNeedQuery.prepare( "SELECT UserTimeNeed From ScheduleTime WHERE ID_WorkDay=:ID_WD" );
        GetTimeNeedQuery.bindValue( ":ID_WD", DateID );

        if( GetTimeNeedQuery.exec() )
        {
            if( GetTimeNeedQuery.last() )
                TimeNeed = getTimeFromString( GetTimeNeedQuery.value(0).toString() );
        }
        else
        {
            qCritical() << "[2] timeNeed( " << date << ", " << rateID << " ) :: " << GetTimeNeedQuery.lastError().text();
        }
    }

    return TimeNeed;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::timeNeed
 * \param date Дата, для которой нужно получить время, сколько нужно отработать
 * \param rateID Идентификатор рабочей ставки
 * \return Время, сколько нужно отработать за \a date по расписанию
 */
WTime DataBaseWT::timeNeedSchedule( const QDate & date, const int rateID )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::timeNeedSchedule( " << date << ", " << rateID << " )";
#endif

    WTime TimeNeed( 0, 0 );

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] timeNeedSchedule( " << date << ", " << rateID << " ) :: Day not found";
    }
    else
    {
        QSqlQuery GetTimeNeedQuery( DataBase );

        GetTimeNeedQuery.prepare( "SELECT TimeNeed From ScheduleTime WHERE ID_WorkDay=:ID_WD" );
        GetTimeNeedQuery.bindValue( ":ID_WD", DateID );

        if( GetTimeNeedQuery.exec() )
        {
            if( GetTimeNeedQuery.last() )
                TimeNeed = getTimeFromString( GetTimeNeedQuery.value(0).toString() );
        }
        else
        {
            qCritical() << "[2] timeNeedSchedule( " << date << ", " << rateID << " ) :: " << GetTimeNeedQuery.lastError().text();
        }
    }

    return TimeNeed;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::note
 * \param date Дата, для которой нужно получить заметку
 * \param rateID Идентификатор рабочей ставки
 * \return Текст заметки для даты \a date
 */
QString DataBaseWT::note( const QDate & date, const int rateID )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::note( " << date << ", " << rateID << " )";
#endif

    QString Text = "";

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] note(" << date << ", " << rateID << ") :: uncorrect date ID==-1";
    }
    else
    {
        QSqlQuery GetNoteQuery( DataBase );

        GetNoteQuery.prepare( "SELECT Content FROM Notes WHERE ID_WorkDay=:ID_WD" );

        GetNoteQuery.bindValue( ":ID_WD", DateID );

        if( GetNoteQuery.exec() )
        {
            if( GetNoteQuery.last() )
                Text = GetNoteQuery.value(0).toString();
        }
        else
        {
            qCritical() << "[2] note(" << date << ", " << rateID << ") :: " << GetNoteQuery.lastError().text();
        }
    }

    return Text;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::intervals
 * \param date Дата, для которой нужно получить список указателей на интервалы
 * \param rateID Идентификатор рабочей ставки
 * \param intervals Ссылка на список интервалов, которой необходимо заполнить
 */
void DataBaseWT::intervals( const QDate & date, const int rateID, QList<Interval*> & intervals )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::intervals( " << date << ", " << rateID << ", QList )";
#endif

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] intervals(" << date   << ", "
                                        << rateID << ") :: uncorrect date ID==-1";
    }
    else
    {
        QSqlQuery GetIntervalsQuery( DataBase );

        GetIntervalsQuery.prepare( "SELECT Caption, TimeStart, TimeEnd FROM TimeIntervals WHERE ID_WorkDay=:ID_WD" );

        GetIntervalsQuery.bindValue( ":ID_WD", DateID );

        if( GetIntervalsQuery.exec() )
        {
            while( GetIntervalsQuery.next() )
            {
                Interval * IntervalDay = new Interval( GetIntervalsQuery.value(0).toString() );
                IntervalDay->setTimeStart( WTime(GetIntervalsQuery.value(1).toString()) );
                IntervalDay->setTimeEnd  ( WTime(GetIntervalsQuery.value(2).toString()) );

                intervals.append( IntervalDay );
            }
        }
        else
        {
            qCritical() << "[2] intervals(" << date   << ", "
                                            << rateID << ") :: " << GetIntervalsQuery.lastError().text();
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::typeDay
 * \param date Дата, для которой нужно получить тип дня
 * \param rateID Идентификатор рабочей ставки
 * \return Тип дня
 */
int DataBaseWT::typeDay( const QDate & date, const int rateID )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::typeDay( " << date << ", " << rateID << " )";
#endif

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] typeDay(" << date << ", " << rateID << ") :: date not found";
    }
    else
    {
        QSqlQuery GetTypeWorkDayQuery( DataBase );
        GetTypeWorkDayQuery.prepare( "SELECT UserTypeWorkDay FROM ScheduleTime WHERE ID=:ID" );
        GetTypeWorkDayQuery.bindValue( ":ID", DateID );

        if( GetTypeWorkDayQuery.exec() )
        {
            if( GetTypeWorkDayQuery.last() )
                return GetTypeWorkDayQuery.value(0).toInt();
        }
        else
        {
            qCritical() << "[2] typeDay(" << date << ", " << rateID << ") :: " << GetTypeWorkDayQuery.lastError().text();
        }
    }

    return false;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::lastIntervalID
 * \return Последний идентификатор, добавленный в таблицу \a TimeIntervals
 */
int DataBaseWT::lastIntervalID()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::lastIntervalID()";
#endif

    QSqlQuery GetLastIdQuery( DataBase );

    GetLastIdQuery.prepare( "SELECT ID FROM TimeIntervals ORDER BY ID DESC LIMIT 1" );

    if( GetLastIdQuery.exec() )
    {
        if( GetLastIdQuery.last() )
            return GetLastIdQuery.value(0).toInt();
    }
    else
    {
        qCritical() << "[1] lastIntervalID() :: "<< GetLastIdQuery.lastError().text();
    }

    return -1;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::intervalFromDayID
 * \param DateID Индентификатор даты
 * \param id Идентификатор интервала в рамках дня
 * \return Идентификатор интервала из \a id из таблицы \a TimeIntervals
 */
int DataBaseWT::intervalFromDayID( const int DateID, const int id )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::intervalFromDayID( " << DateID << ", " << id << " )";
#endif

    if( DateID == -1 )
    {
        qCritical() << "[1] intervalFromDayID( " << DateID << ", "
                                                 << id     << ") :: uncorrect date ID==-1";
    }
    else
    {
        QSqlQuery GetIdQuery( DataBase );

        GetIdQuery.prepare( "SELECT ID FROM TimeIntervals WHERE ID_WorkDay=:ID_WD" );
        GetIdQuery.bindValue( ":ID_WD", DateID );

        if( GetIdQuery.exec() )
        {
            int countID = 0;

            while( GetIdQuery.next() )
            {
                if( countID == id )
                    return GetIdQuery.value(0).toInt();
                else
                    countID++;
            }
        }
        else
        {
            qCritical() << "[1] intervalFromDayID( " << DateID << ", "
                                                     << id     << ") :: "<< GetIdQuery.lastError().text();
        }
    }

    return -1;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::addInterval
 * \param date Дата, для которой нужно добавить интервал
 * \param rateID Идентификатор рабочей ставки
 * \param title Назвавание интервала
 */
void DataBaseWT::addInterval( const QDate & date, const int rateID, const QString & title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::addInterval( " << date << ", " << rateID << ", " << title << " )";
#endif

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] addInterval(" << date   << ", "
                                          << rateID << ", "
                                          << title  << ") :: uncorrect date ID==-1";
    }
    else
    {
        int interval = lastIntervalID() + 1;

        if( interval >= 0 )
        {
            QSqlQuery InsertIntervalQuery( DataBase );

            InsertIntervalQuery.prepare( "INSERT                 " \
                                            "INTO TimeIntervals( " \
                                                "ID,             " \
                                                "ID_WorkDay,     " \
                                                "Caption,        " \
                                                "TimeStart,      " \
                                                "TimeEnd )       " \
                                            "VALUES(             " \
                                                 ":ID,           " \
                                                 ":ID_WD,        " \
                                                 ":Title,        " \
                                                 ":Start,        " \
                                                 ":End )         ");

            InsertIntervalQuery.bindValue( ":ID"   , interval           );
            InsertIntervalQuery.bindValue( ":ID_WD", DateID             );
            InsertIntervalQuery.bindValue( ":Title", title              );
            InsertIntervalQuery.bindValue( ":Start", WTime().toString() );
            InsertIntervalQuery.bindValue( ":End"  , WTime().toString() );

            if( InsertIntervalQuery.exec() == false )
                qCritical() << "[3] addInterval(" << date   << ", "
                                                  << rateID << ", "
                                                  << title  << ") ::"  << InsertIntervalQuery.lastError().text();
        }
        else
        {
            qCritical() << "[2] addInterval(" << date   << ", "
                                              << rateID << ", "
                                              << title  << ") :: interval not found";
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::removeInterval
 * \param Date Дата, для которой нужно удалить интервал
 * \param RateID Идентификатор рабочей ставки
 * \param id Идентификатор интервала в рамках дня, который нужно удалить
 */
void DataBaseWT::removeInterval( const QDate& Date, const int RateID, const int id )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::removeInterval( " << date << ", " << rateID << " )";
#endif

    int DateID = dateID( Date, RateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] removeInterval(" << Date   << ", "
                                             << RateID << ", "
                                             << id     << ") :: uncorrect date ID==-1";
    }
    else
    {
        int interval = intervalFromDayID( DateID, id );

        if( interval >= 0 )
        {
            QSqlQuery RemoveIntervalQuery( DataBase );

            RemoveIntervalQuery.prepare( "DELETE FROM TimeIntervals WHERE ID=:ID" );
            RemoveIntervalQuery.bindValue( ":ID", interval );

            if( RemoveIntervalQuery.exec() == false )
                qCritical() << "[3] removeInterval(" << Date   << ", "
                                                     << RateID << ", "
                                                     << id     << ") :: "
                                                     << RemoveIntervalQuery.lastError().text();
        }
        else
        {
            qCritical() << "[2] removeInterval(" << Date   << ", "
                                                 << RateID << ", "
                                                 << id     << ") :: interval not found";

        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::renameInterval
 * \param Date Дата, для которой нужно переименовать интервал
 * \param RateID Идентификатор рабочей ставки
 * \param id Идентификатор интервала, для которого нужно изменить название
 * \param title Новое название интервала
 */
void DataBaseWT::renameInterval( const QDate & date, const int rateID, const int id, const QString & title )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::renameInterval( " << date << ", " << rateID << ", " << id << ", " << title << " )";
#endif

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] renameInterval(" << date   << ", "
                                             << rateID << ", "
                                             << id     << ", "
                                             << title  << ") :: uncorrect date ID==-1";
    }
    else
    {
        int interval = intervalFromDayID( DateID, id );

        if( interval >= 0 )
        {
            QSqlQuery RenameIntervalQuery( DataBase );

            RenameIntervalQuery.prepare( "UPDATE TimeIntervals SET Caption=:Title WHERE ID=:ID" );
            RenameIntervalQuery.bindValue( ":Title", title    );
            RenameIntervalQuery.bindValue( ":ID"   , interval );

            if( RenameIntervalQuery.exec() == false )
                qCritical() << "[3] renameInterval(" << date   << ", "
                                                     << rateID << ", "
                                                     << id     << ", "
                                                     << title  << ") :: "
                                                     << RenameIntervalQuery.lastError().text();
        }
        else
        {
            qCritical() << "[2] renameInterval(" << date   << ", "
                                                 << rateID << ", "
                                                 << id     << ") :: interval not found";

        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::setTimeStart
 * \param date Дата, для которой нужно изменить время начала
 * \param rateID Идентификатор рабочей ставки
 * \param id Идентификатор интервала, для которого нужно изменить время начала
 * \param time Время начала
 */
void DataBaseWT::setTimeStart(const QDate & date, const int rateID, const int id , const WTime & time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::setTimeStart( " << date << ", " << rateID << ", " << id << ", " << time.toString() << " )";
#endif

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] changeTimeStart(" << date            << ", "
                                              << rateID          << ", "
                                              << id              << ", "
                                              << time.toString() << ") :: uncorrect date ID==-1";
    }
    else
    {
        int interval = intervalFromDayID( DateID, id );

        if( interval >= 0 )
        {
            QSqlQuery ChangeStartQuery( DataBase );

            ChangeStartQuery.prepare( "UPDATE TimeIntervals SET TimeStart=:Time WHERE ID=:ID" );
            ChangeStartQuery.bindValue( ":Time", time.toString() );
            ChangeStartQuery.bindValue( ":ID"  , interval        );

            if( ChangeStartQuery.exec() == false )
                qCritical() << "[3] changeTimeStart(" << date            << ", "
                                                      << rateID          << ", "
                                                      << id              << ", "
                                                      << time.toString() << ChangeStartQuery.lastError().text();
        }
        else
        {
            qCritical() << "[2] changeTimeStart(" << date            << ", "
                                                  << rateID          << ", "
                                                  << id              << ", "
                                                  << time.toString() << ") :: interval not found";

        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::setTimeEnd
 * \param date Дата, для которой нужно изменить время конца
 * \param rateID Идентификатор рабочей ставки
 * \param id Идентификатор интервала, для которого нужно изменить время конца
 * \param time Время конца
 */
void DataBaseWT::setTimeEnd( const QDate & date, const int rateID, const int id , const WTime & time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::setTimeEnd( " << date << ", " << rateID << ", " << id << ", " << time.toString() << " )";
#endif

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] changeTimeEnd(" << date           << ", "
                                            << rateID          << ", "
                                            << id              << ", "
                                            << time.toString() << ") :: uncorrect date ID==-1";
    }
    else
    {
        int interval = intervalFromDayID( DateID, id );

        if( interval >= 0 )
        {
            QSqlQuery ChangeStartQuery( DataBase );

            ChangeStartQuery.prepare( "UPDATE TimeIntervals SET TimeEnd=:Time WHERE ID=:ID" );
            ChangeStartQuery.bindValue( ":Time", time.toString() );
            ChangeStartQuery.bindValue( ":ID"  , interval        );

            if( ChangeStartQuery.exec() == false )
                qCritical() << "[3] changeTimeEnd(" << date            << ", "
                                                    << rateID          << ", "
                                                    << id              << ", "
                                                    << time.toString() << ChangeStartQuery.lastError().text();
        }
        else
        {
            qCritical() << "[2] changeTimeEnd(" << date            << ", "
                                                << rateID          << ", "
                                                << id              << ", "
                                                << time.toString() << ") :: interval not found";

        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::setTypeDay
 * \param date Дата, дял которой нужно изменить тип дня
 * \param rateID Идентификатор рабочей ставки
 * \param type Тип дня
 */
void DataBaseWT::setTypeDay( const QDate & date, const int rateID, const int type )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::setTypeDay( " << date << ", " << rateID << ", " << type << " )";
#endif

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] changeTypeDay(" << date << ", " << rateID << ", " << type << ") :: date not found";
    }
    else
    {
        QSqlQuery CheckIdQuery( DataBase );
        CheckIdQuery.prepare( "SELECT ID FROM ScheduleTime WHERE ID_WorkDay=:ID" );
        CheckIdQuery.bindValue( ":ID", DateID );

        if( CheckIdQuery.exec() )
        {
            if( CheckIdQuery.last() )
            {
                QSqlQuery ChangeTypeDay( DataBase );
                ChangeTypeDay.prepare( "UPDATE ScheduleTime SET UserTypeWorkDay=:Type WHERE ID_WorkDay=:ID" );
                ChangeTypeDay.bindValue( ":Type", type   );
                ChangeTypeDay.bindValue( ":ID"  , DateID );

                if( ChangeTypeDay.exec() == false )
                    qCritical() << "[4] changeTypeDay(" << date << ", " << rateID << ", " << type << ") :: " << ChangeTypeDay.lastError().text();
            }
            else
            {
                qCritical() << "[3] changeTypeDay(" << date << ", " << rateID << ", " << type << ") :: Not found record for date";
            }
        }
        else
        {
            qCritical() << "[2] changeTypeDay(" << date << ", " << rateID << ", " << type << ") :: " << CheckIdQuery.lastError().text();
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::noteId
 * \param DateID Идектификатор дня
 * \return Идентификатор заметки
 */
int DataBaseWT::noteId( const int DateID )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::noteId( " << DateID << " )";
#endif

    if( DateID == -1 )
    {
        qCritical() << "[1] noteId( " << DateID << ") :: uncorrect date ID==-1";
    }
    else
    {
        QSqlQuery GetLastIdQuery( DataBase );
        GetLastIdQuery.prepare( "SELECT ID FROM Notes WHERE ID_WorkDay=:ID_WD ORDER BY ID DESC LIMIT 1" );
        GetLastIdQuery.bindValue(":ID_WD", DateID );

        if( GetLastIdQuery.exec() )
        {
            if( GetLastIdQuery.last() )
                return GetLastIdQuery.value(0).toInt();
        }
        else
        {
            qCritical() << "[1] noteId( " << DateID <<  " ) :: "<< GetLastIdQuery.lastError().text();
        }
    }

    return -1;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::lastNoteID
 * \return Последний идентификатор, добавленный в таблицу \a Notes
 */
int DataBaseWT::lastNoteID()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::lastNoteID()";
#endif

    QSqlQuery GetLastIdQuery( DataBase );

    GetLastIdQuery.prepare( "SELECT ID FROM Notes ORDER BY ID DESC LIMIT 1" );

    if( GetLastIdQuery.exec() )
    {
        if( GetLastIdQuery.last() )
            return GetLastIdQuery.value(0).toInt();
    }
    else
    {
        qCritical() << "[1] lastNoteID() :: "<< GetLastIdQuery.lastError().text();
    }

    return -1;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::setNote
 * \param date Дата, для которой нужно изменить заметку
 * \param RateID Идентификатор рабочей ставки
 * \param note Текст заметки
 */
void DataBaseWT::setNote( const QDate & date, const int rateID, const QString & note )
{
    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] changeNote(" << date << ", " << rateID << ", " << note << ") :: uncorrect date ID==-1";
    }
    else
    {
        int noteID = noteId( DateID );

        QSqlQuery ChangeNoteQuery( DataBase );

        if( noteID >= 0 )
        {
            ChangeNoteQuery.prepare( "UPDATE Notes SET Content=:Note WHERE ID_WorkDay=:ID_WD" );
            ChangeNoteQuery.bindValue( ":Note" , note   );
            ChangeNoteQuery.bindValue( ":ID_WD", DateID );
        }
        else
        {

            noteID = lastNoteID() + 1;

            ChangeNoteQuery.prepare( "INSERT INTO Notes (ID, ID_WorkDay, Content) VALUES (:ID, :ID_WD, :Note)" );

            ChangeNoteQuery.bindValue( ":ID"   , noteID );
            ChangeNoteQuery.bindValue( ":ID_WD", DateID );
            ChangeNoteQuery.bindValue( ":Note" , note   );
        }

        if( ChangeNoteQuery.exec() == false )
            qCritical() << "[2] changeNote(" << date << ", " << rateID << ", " << note << ") :: " << ChangeNoteQuery.lastError().text();
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::scheduleId
 * \param DateID
 * \return Идентификатор распиания для даты с \a DateID
 */
int DataBaseWT::scheduleId( const int DateID )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::scheduleId( " << DateID << " )";
#endif

    if( DateID == -1 )
    {
        qCritical() << "[1] scheduleId( " << DateID << ") :: uncorrect date ID==-1";
    }
    else
    {
        QSqlQuery GetLastIdQuery( DataBase );
        GetLastIdQuery.prepare( "SELECT ID FROM ScheduleTime WHERE ID_WorkDay=:ID_WD ORDER BY ID DESC LIMIT 1" );
        GetLastIdQuery.bindValue(":ID_WD", DateID );

        if( GetLastIdQuery.exec() )
        {
            if( GetLastIdQuery.last() )
                return GetLastIdQuery.value(0).toInt();
        }
        else
        {
            qCritical() << "[1] scheduleId( " << DateID <<  " ) :: "<< GetLastIdQuery.lastError().text();
        }
    }

    return -1;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::lastScheduleID
 * \return Последний индентификатор, добавленный в таблицу \a ScheduleTime.
 */
int DataBaseWT::lastScheduleID()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::lastScheduleID()";
#endif

    QSqlQuery GetLastIdQuery( DataBase );

    GetLastIdQuery.prepare( "SELECT ID FROM ScheduleTime ORDER BY ID DESC LIMIT 1" );

    if( GetLastIdQuery.exec() )
    {
        if( GetLastIdQuery.last() )
            return GetLastIdQuery.value(0).toInt();
    }
    else
    {
        qCritical() << "[1] lastScheduleID() :: "<< GetLastIdQuery.lastError().text();
    }

    return -1;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::setTimeNeed
 * \param date Дата, для которой нужно изменить время работы
 * \param rateID Идентификатор рабочей ставки
 * \param time Время, сколько нужно работать
 * \param type Признак: Время расписания или пользовательское время
 *
 * Если тип времени - расписание, то время будет также изменено и в
 * пользовательском поле
 */
void DataBaseWT::setTimeNeed( const QDate & date, const int rateID , WTime time, TypeTimeNeed type )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::setTimeNeed( " << date << ", " << rateID << ", " << time.toString() << ", " << type << " )";
#endif

    int DateID = dateID( date, rateID );

    if( DateID == -1 )
    {
        qCritical() << "[1] setTimeNeed(" << date            << ", "
                                          << rateID          << ", "
                                          << time.toString() << ", "
                                          << type << ") :: date ID == -1";
    }
    else
    {
        int ScheduleID = scheduleId( DateID );

        QSqlQuery ChangeTimeNeedQuery( DataBase );

        if( ScheduleID == -1 )
        {
            ScheduleID = lastScheduleID() + 1;

            ChangeTimeNeedQuery.prepare("INSERT INTO ScheduleTime( ID,        " \
                                                            "ID_WorkDay,      " \
                                                            "TypeWorkDay,     " \
                                                            "TimeNeed,        " \
                                                            "UserTypeWorkDay, " \
                                                            "UserTimeNeed )   " \
                                   "VALUES ( :ID, :ID_WD, :Type, :Time, :UType, :UTime )" );

            ChangeTimeNeedQuery.bindValue( ":ID"   , ScheduleID             );
            ChangeTimeNeedQuery.bindValue( ":ID_WD", DateID                 );
            ChangeTimeNeedQuery.bindValue( ":Type" , time.isEmpty() ? 1 : 0 );
            ChangeTimeNeedQuery.bindValue( ":Time" , time.toString()        );
            ChangeTimeNeedQuery.bindValue( ":UType", time.isEmpty() ? 1 : 0 );
            ChangeTimeNeedQuery.bindValue( ":UTime", time.toString()        );
        }
        else
        {
            if( type == Schedule )
            {
                ChangeTimeNeedQuery.prepare( "UPDATE ScheduleTime       " \
                                             "SET                       " \
                                               "TypeWorkDay=:Type,      " \
                                               "TimeNeed   =:Time,      " \
                                               "UserTypeWorkDay=:TypeU, " \
                                               "UserTimeNeed   =:TimeU  " \
                                             "WHERE                     " \
                                                "ID=:ID                 " );

                ChangeTimeNeedQuery.bindValue( ":Type" , time.isEmpty() ? 1 : 0 );
                ChangeTimeNeedQuery.bindValue( ":Time" , time.toString()        );
                ChangeTimeNeedQuery.bindValue( ":TypeU", time.isEmpty() ? 1 : 0 );
                ChangeTimeNeedQuery.bindValue( ":TimeU", time.toString()        );
            }
            else
            {
                ChangeTimeNeedQuery.prepare( "UPDATE ScheduleTime       " \
                                             "SET                       " \
                                                "UserTypeWorkDay=:Type, " \
                                                "UserTimeNeed   =:Time  " \
                                             "WHERE                     " \
                                                "ID=:ID                 " );

                ChangeTimeNeedQuery.bindValue( ":Type", time.isEmpty() ? 1 : 0 );
                ChangeTimeNeedQuery.bindValue( ":Time", time.toString()        );
            }

            ChangeTimeNeedQuery.bindValue( ":ID"  , ScheduleID             );
        }

        if( ChangeTimeNeedQuery.exec() == false )
            qCritical() << "[2] setTimeNeed(" << date            << ", "
                                              << rateID          << ", "
                                              << time.toString() << ", "
                                              << type            << ") :: "
                                              << ChangeTimeNeedQuery.lastError().text();
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::insertYear
 * \param date Дата, из которой будет получен год
 * \param rateID Идентификатор рабочей ставки
 */
void DataBaseWT::insertYear( const QDate & date, int rateID )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::insertYear(" << date << ", " << rateID << ")";
#endif

    if( isExists(date, rateID) == false )
    {
        QString InsertYearQuery = "INSERT INTO WorkDays(ID, ID_WorkingRate, Year, Month, Day) VALUES ";

        int lastDateID = lastWorkDaysID();

        int year = date.year();

        for( int month = 1; month <= 12; month++ )
        {
            int countDays = QDate(year, month, 1).daysInMonth();

            for( int day = 1; day <= countDays; day++ )
            {
                lastDateID++;

                InsertYearQuery += QString( "(%1, %2, %3, %4, %5)" )
                                            .arg( lastDateID )
                                            .arg( rateID     )
                                            .arg( year       )
                                            .arg( month      )
                                            .arg( day        );

                if( month != 12 || (month == 12 && day != countDays) )
                    InsertYearQuery += ", ";
            }
        }

        QSqlQuery InsertYearSql( DataBase );

        InsertYearSql.prepare( InsertYearQuery );

        if( InsertYearSql.exec() == false )
        {
            qWarning() << "DataBaseWT::insertYear (" << date.toString("dd.MM.yyyy")<< ", "
                                                     << rateID
                                                     << ") :: "<< InsertYearSql.lastError().text();
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::insertScheduleYear
 * \param date Дата, из которой будет использован год
 * \param rateID Идектификатор рабочей ставки
 */
void DataBaseWT::insertScheduleYear(  const QDate & date, int rateID  )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::insertScheduleYear( " << date << ", " << rateID << " )";
#endif

    int year = date.year();

    for( int month = 1; month <= 12; month++ )
    {
        int countDays = QDate(year, month, 1).daysInMonth();

        for( int day = 1; day <= countDays; day++ )
        {
            QDate CycleDate(year, month, day);

            setTimeNeed( CycleDate,
                         rateID,
                         HelperWT::timeNeedSchedule( CycleDate, rateID),
                         DataBaseWT::Schedule );
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::insertWorkingRates
 * \param ratesList Список названий рабочих ставок
 */
void DataBaseWT::insertWorkingRates( const QStringList & ratesList )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::insertWorkingRates( " << ratesList << " )";
#endif

    QString InsertRatesSQL = "INSERT OR IGNORE INTO WorkingRates(ID, Rate) VALUES ";

    for( int rate = 0; rate < ratesList.count(); rate++ )
    {
        InsertRatesSQL += QString("(%1, '%2')").arg( rate ).arg( ratesList.at(rate) );

        if( rate != ratesList.count() - 1 )
            InsertRatesSQL += ", ";
    }

    QSqlQuery InsertQuery( DataBase );
    InsertQuery.prepare( InsertRatesSQL );

    if( InsertQuery.exec() == false )
        qCritical() << "DataBaseWT::insertWorkingRates(" << ratesList << ") :: " << InsertQuery.lastError().text();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::sqlDatabase
 * \return Ссылку на объект QSqlDatabase
 */
QSqlDatabase & DataBaseWT::sqlDatabase()
{
    return DataBase;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::openDataBase
 * \return Признак успешного открытия базы данных
 */
bool DataBaseWT::openDataBase()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::openDataBase()";
#endif

    if( isOpenDataBase() )
        return true;

    if( DataBase.open() )
        return true;

    return false;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::closeDataBase
 *
 * Закрытие соединения с базой данных
 */
void DataBaseWT::closeDataBase()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::closeDataBase()";
#endif

    DataBase.close();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::isOpenDataBase
 * \return Признак соединения с базой данных
 */
bool DataBaseWT::isOpenDataBase()
{
    return DataBase.isOpen();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::isExistsDataBase
 * \return Признак существования базы данных
 */
bool DataBaseWT::isExistsDataBase()
{
    return QFile(HelperWT::pathToDataBase()).exists();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief DataBaseWT::configureDataBase
 * \return  Признак успешной конфигурации базы данных.
 */
bool DataBaseWT::configureDataBase()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call DataBaseWT::configureDataBase()";
#endif

    DataBase.exec( "PRAGMA encoding = \"UTF-8\"" );

    QSqlQuery QueryCreateTableWorkingRates    ( DataBase );
    QSqlQuery QueryCreateTableWorkDays        ( DataBase );
    QSqlQuery QueryCreateTableScheduleTime    ( DataBase );
    QSqlQuery QueryCreateTableTimeIntervals   ( DataBase );
    QSqlQuery QueryCreateTableNotes           ( DataBase );
    QSqlQuery QueryCreateTableSalary          ( DataBase );

    QueryCreateTableWorkingRates.prepare ("CREATE TABLE IF NOT EXISTS WorkingRates( "\
                                              "ID INTEGER NOT NULL UNIQUE,          " \
                                              "Rate TEXT NOT NULL UNIQUE,           " \
                                          "PRIMARY KEY(ID) );                       " );

    QueryCreateTableWorkDays.prepare( "CREATE TABLE IF NOT EXISTS WorkDays (                     " \
                                            "ID             INTEGER NOT NULL UNIQUE,             " \
                                            "ID_WorkingRate INTEGER NOT NULL,                    " \
                                            "Year           INTEGER NOT NULL,                    " \
                                            "Month          INTEGER NOT NULL,                    " \
                                            "Day            INTEGER NOT NULL,                    " \
                                      "PRIMARY KEY(ID),                                          " \
                                      "FOREIGN KEY(ID_WorkingRate) REFERENCES WorkingRates(ID) );" );

    QueryCreateTableScheduleTime.prepare( "CREATE TABLE IF NOT EXISTS ScheduleTime (          " \
                                                "ID              INTEGER NOT NULL UNIQUE,     " \
                                                "ID_WorkDay      INTEGER NOT NULL UNIQUE,     " \
                                                "TypeWorkDay     INTEGER NOT NULL,            " \
                                                "TimeNeed        TEXT  NOT NULL,              " \
                                                "UserTypeWorkDay INTEGER,                     " \
                                                "UserTimeNeed    TEXT,                        " \
                                          "FOREIGN KEY(ID_WorkDay) REFERENCES WorkDays(ID) ); " );

    QueryCreateTableTimeIntervals.prepare( "CREATE TABLE IF NOT EXISTS TimeIntervals (         " \
                                                "ID              INTEGER NOT NULL UNIQUE,      " \
                                                "ID_WorkDay      INTEGER NOT NULL,             " \
                                                "Caption         TEXT    NOT NULL,             " \
                                                "TimeStart       TEXT,                         " \
                                                "TimeEnd         TEXT,                         " \
                                            "FOREIGN KEY(ID_WorkDay) REFERENCES WorkDays(ID) );" );

    QueryCreateTableNotes.prepare( "CREATE TABLE IF NOT EXISTS Notes (                " \
                                        "ID          INTEGER NOT NULL UNIQUE,         " \
                                        "ID_WorkDay  INTEGER NOT NULL,                " \
                                        "Content     TEXT,                            " \
                                   "FOREIGN KEY(ID_WorkDay) REFERENCES WorkDays(ID) );" );

    QueryCreateTableSalary.prepare( "CREATE TABLE IF NOT EXISTS Salaries (             " \
                                        "ID          INTEGER NOT NULL UNIQUE,          " \
                                        "ID_WorkDay  INTEGER NOT NULL,                 " \
                                        "Salary      REAL,                             " \
                                    "FOREIGN KEY(ID_WorkDay) REFERENCES WorkDays(ID) );" );

    bool isSuccessCreateTables = true;

    QList<QSqlQuery *> CreateTableQueryList;

    CreateTableQueryList.append( &QueryCreateTableWorkingRates     );
    CreateTableQueryList.append( &QueryCreateTableWorkDays         );
    CreateTableQueryList.append( &QueryCreateTableScheduleTime     );
    CreateTableQueryList.append( &QueryCreateTableTimeIntervals    );
    CreateTableQueryList.append( &QueryCreateTableNotes            );
    CreateTableQueryList.append( &QueryCreateTableSalary           );

    foreach( QSqlQuery* Query, CreateTableQueryList )
    {
        if( isSuccessCreateTables )
        {
            isSuccessCreateTables = Query->exec();
        }
        else
        {
            qCritical() << "configureDataBase() :: " << Query->lastError().text();
            break;
        }
    }

    return isSuccessCreateTables;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief Получение даты в виде строки
 * \param Date - Объект класса QDate для преобразования даты в строку
 * \return Дата в виде строки
 */
QString DataBaseWT::getStringFromDate( const QDate& Date )
{
    return Date.toString( "dd.MM.yyyy" );
}
// ------------------------------------------------------------------------ //

/*!
 * \brief Получение времени в виде строки
 * \param Time - Объект класса QTime для преобразования времени в строку
 * \return Время в виде строки
 */
QString DataBaseWT::getStringFromTime( const QTime& Time )
{
    return Time.toString( "hh:mm" );
}
// ------------------------------------------------------------------------ //

/*!
 * \brief Получение объекта класса QTime из строки
 * \param TimeString - Строка времени в формате "hh:mm.ss"
 * \return Объекта класса QTime
 */
QTime DataBaseWT::getTimeFromString( const QString& TimeString )
{
    return QTime().fromString( TimeString, "hh:mm" );
}
// ------------------------------------------------------------------------ //

/*!
 * \brief Получение объекта класса QDate из строки
 * \param DateString - Строка даты в формате "dd.MM.yyyy"
 * \return Объекта класса QDate
 */
QDate DataBaseWT::getDateFromString( const QString& DateString )
{
    return QDate().fromString( DateString, "dd.MM.yyyy" );
}
// ------------------------------------------------------------------------ //
