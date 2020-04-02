// ------------------------------------ //
#include <QFile>
// ------------------------------------ //
#include "helperwt.h"
// ------------------------------------ //
#include "databasewt.h"
// ------------------------------------ //
DataBaseWT * DataBaseWT::Instance = NULL;
// ------------------------------------ //


DataBaseWT * DataBaseWT::instance()
{
    if( Instance == NULL )
        Instance = new DataBaseWT( NULL );

    return Instance;
}
// ------------------------------------------------------------------------------------ //

DataBaseWT::DataBaseWT( QObject * parent ) : QObject( parent )
{
    DataBase = QSqlDatabase::addDatabase( "QSQLITE" );
    DataBase.setDatabaseName( HelperWT::pathToDataBase() );

    //TablesWindow = new TablesDataBase( this );
    //TablesWindow->show();
}
// ------------------------------------------------------------------------------------ //

DataBaseWT::~DataBaseWT()
{
    //TablesWindow->close();
    closeDataBase();
}
// ------------------------------------------------------------------------------------ //

bool DataBaseWT::configureDataBase()
{
    DataBase.exec( "PRAGMA encoding = \"UTF-8\"" );

    QSqlQuery QueryCreateTableWorkingRates    ( DataBase );
    QSqlQuery QueryCreateTableWorkDays        ( DataBase );
    QSqlQuery QueryCreateTableScheduleTime    ( DataBase );
    QSqlQuery QueryCreateTableTimeIntervals   ( DataBase );
    QSqlQuery QueryCreateTableNotes           ( DataBase );
    QSqlQuery QueryCreateTableSalary          ( DataBase );

    QueryCreateTableWorkingRates.prepare ("CREATE TABLE WorkingRates (     " \
                                              "ID INTEGER NOT NULL UNIQUE, " \
                                              "Rate TEXT NOT NULL UNIQUE,  " \
                                          "PRIMARY KEY(ID) );              " );

    QueryCreateTableWorkDays.prepare( "CREATE TABLE WorkDays (                                   " \
                                            "ID             INTEGER NOT NULL UNIQUE,             " \
                                            "ID_WorkingRate INTEGER NOT NULL,                    " \
                                            "Year           INTEGER NOT NULL,                    " \
                                            "Month          INTEGER NOT NULL,                    " \
                                            "Day            INTEGER NOT NULL,                    " \
                                      "PRIMARY KEY(ID),                                          " \
                                      "FOREIGN KEY(ID_WorkingRate) REFERENCES WorkingRates(ID) );" );

    QueryCreateTableScheduleTime.prepare( "CREATE TABLE ScheduleTime (                        " \
                                                "ID              INTEGER NOT NULL UNIQUE,     " \
                                                "ID_WorkDay      INTEGER NOT NULL UNIQUE,     " \
                                                "TypeWorkDay     INTEGER NOT NULL,            " \
                                                "TimeNeed        TEXT  NOT NULL,              " \
                                                "UserTypeWorkDay INTEGER,                     " \
                                                "UserTimeNeed    TEXT,                        " \
                                          "FOREIGN KEY(ID_WorkDay) REFERENCES WorkDays(ID) ); " );

    QueryCreateTableTimeIntervals.prepare( "CREATE TABLE TimeIntervals (                       " \
                                                "ID              INTEGER NOT NULL UNIQUE,      " \
                                                "ID_WorkDay      INTEGER NOT NULL,             " \
                                                "Caption         TEXT    NOT NULL,             " \
                                                "TimeStart       TEXT,                         " \
                                                "TimeEnd         TEXT,                         " \
                                            "FOREIGN KEY(ID_WorkDay) REFERENCES WorkDays(ID) );" );

    QueryCreateTableNotes.prepare( "CREATE TABLE Notes (                              " \
                                        "ID          INTEGER NOT NULL UNIQUE,         " \
                                        "ID_WorkDay  INTEGER NOT NULL,                " \
                                        "Content     TEXT,                            " \
                                   "FOREIGN KEY(ID_WorkDay) REFERENCES WorkDays(ID) );" );

    QueryCreateTableSalary.prepare( "CREATE TABLE Salaries (                           " \
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


QSqlDatabase &DataBaseWT::sqlDatabase()
{
    return DataBase;
}
// ------------------------------------------------------------------------------------ //

bool DataBaseWT::connectDataBase()
{
    if( isOpenDataBase() ) return true;
    if( DataBase.open()  ) return true;

    return false;
}
// ------------------------------------------------------------------------------------ //

bool DataBaseWT::openDataBase()
{
    if( isOpenDataBase() )
        return true;

    // Устанавливаем флаг наличия БД до открытия, т.к. если ее не будет,
    // при открытии она будет создана и не удастся определить после открытия,
    // она была создана или создалась при открытии
    bool ExistsDataBase = isExistsDataBase();

    if( connectDataBase() )
    {
        if( ExistsDataBase == false )
        {
            if( configureDataBase() )
            {
                return true;
            }
            else
            {
                qCritical() << "DataBaseWT(...) :: tables was not created";
            }
        }
        else
        {
            return true;
        }
    }
    else
    {
        qCritical() << "DataBaseWT(...) :: failed open database: " << DataBase.lastError().text();
    }

    return false;
}
// ------------------------------------------------------------------------------------ //

void DataBaseWT::closeDataBase()
{
    DataBase.close();
}
// ------------------------------------------------------------------------------------ //

bool DataBaseWT::isOpenDataBase()
{
	return DataBase.isOpen();
}
// ------------------------------------------------------------------------------------ //

bool DataBaseWT::isExistsDataBase()
{
    return QFile(HelperWT::pathToDataBase()).exists();
}
// ------------------------------------------------------------------------------------ //
