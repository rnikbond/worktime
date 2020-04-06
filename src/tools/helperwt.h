// ---------------------------- //
#ifndef HELPERWT_H
#define HELPERWT_H
// ---------------------------- //
#include <QObject>
// ---------------------------- //
#include "wtime.h"
// ---------------------------- //

class HelperWT : public QObject
{
    Q_OBJECT

public:

    enum WorkingRates
    {
        UnknownWR        = -1, ///< Неизвестная рабочая ставка
        Hours_40_Lanch   =  0, ///< 40 часовая неделя с включенным обедом
        Hours_40_NoLanch =  1, ///< 40 часовая неделя с динамическим обедом
        Hours_40_4_Days  =  2, ///< 40 часовая неделя с 4-мя рабочими днями
        Hours_35_NoLanch =  3, ///< 35 часовая неделя с динамическим обедом
        Hours_30_NoLanch =  4, ///< 30 часовая неделя с динамическим обедом
        Hours_20_NoLanch =  5  ///< 20 часовая неделя без обеда
    };

    HelperWT( QObject * parent = 0 );

    static const QDate currentDate();

    static const WTime timeNeedSchedule( const QDate & date, const int rate );

    static const QString nameMonthInGenitiveCase( const QDate & Date, bool isLowerChars );
    static const QString nameMonthInDativeCase  ( const QDate & Date, bool isLowerChars );

    static const QString pathToUpdates();

    static const QString pathToDataBase();

    static const QString pathToThemes();

    static const QString pathToWorkDir();
    static const QString pathToConfig();

    static const QString pathToWorkTime();
    static const QString pathToOldWorkTime();
    static const QString pathToNewWorkTime();

    static const QStringList namesWorkingRates();
};

#endif // HELPERWT_H
