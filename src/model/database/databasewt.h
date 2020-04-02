// ------------------------------------ //
#ifndef DATABASEWT_H
#define DATABASEWT_H
// ------------------------------------ //
#include <QtSql>
#include <QObject>
// ------------------------------------ //
#include "wtime.h"
#include "interval.h"
#include "tablesdatabase.h"
// ------------------------------------ //

class DataBaseWT : public QObject
{
    Q_OBJECT

    QSqlDatabase DataBase;
    TablesDataBase * TablesWindow;

	static DataBaseWT * Instance;

public:

    static DataBaseWT * instance();

    DataBaseWT( QObject* parent = NULL );
    ~DataBaseWT();

    bool isOpenDataBase  ();
    bool isExistsDataBase();

    bool openDataBase    ();
    void closeDataBase   ();

    QSqlDatabase & sqlDatabase();

private:

    bool connectDataBase  ();
    bool configureDataBase();
};

#endif // DATABASEWT_H
