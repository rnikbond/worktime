// ---------------------------- //
#ifndef HELPERWT_H
#define HELPERWT_H
// ---------------------------- //
#include <QObject>
// ---------------------------- //

class HelperWT : public QObject
{
    Q_OBJECT

public:

    HelperWT( QObject * parent = 0 );

    static const QDate currentDate();

    static const QString nameMonthInGenitiveCase( const QDate & Date, bool isLowerChars );
    static const QString nameMonthInDativeCase  ( const QDate & Date, bool isLowerChars );

    static const QString pathToWorkTime();
    static const QString pathToOldWorkTime();
    static const QString pathToNewWorkTime();

};

#endif // HELPERWT_H
