// ---------------------------- //
#include <QDir>
#include <QDate>
#include <QDebug>
#include <QLocale>
#include <QApplication>
// ---------------------------- //
#include "helperwt.h"
// ---------------------------- //

HelperWT::HelperWT( QObject * parent ) : QObject( parent )
{
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief HelperWT::currentDate
 * \return Сегодняшний день в виде объекта QDate
 */
const QDate HelperWT::currentDate()
{
    return QDate::currentDate();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief HelperWT::pathToDataBase
 * \return Абсолютный путь к базе данных
 */
const QString HelperWT::pathToDataBase()
{
    return QDir::homePath() + "/AppData/worktime/WorkTime.db";
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief HelperWT::pathToWorkTime
 * \return Абсолютный путь к программе
 */
const QString HelperWT::pathToWorkTime()
{
    return QDir::toNativeSeparators( QApplication::applicationFilePath() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief HelperWT::pathToWorkTime
 * \return Абсолютный путь к старой программе
 */
const QString HelperWT::pathToOldWorkTime()
{
    return QDir::toNativeSeparators( QApplication::applicationDirPath() + "/old_WorkTime.exe" );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief HelperWT::pathToWorkTime
 * \return Абсолютный путь к новой программе
 */
const QString HelperWT::pathToNewWorkTime()
{
    return QDir::toNativeSeparators( QApplication::applicationDirPath() + "/new_WorkTime.exe" );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief HelperWT::nameMonthInGenitiveCase
 * \param Date - Дата, из которой используется номер месяца
 * \param isLowerChars - если == TRUE, первая буква месяца будет в нижнем регистре
 * \return Название месяца в родительном падеже
 */
const QString HelperWT::nameMonthInGenitiveCase( const QDate & Date, bool isLowerChars )
{
    QString MonthInGenitiveCase = QLocale().monthName( Date.month() );

    switch( Date.month() )
    {
        case 3: // Март
        case 8: // Август
        {
            MonthInGenitiveCase += tr("a");
            break;
        }

        case 1 : // Январь
        case 2 : // Февраль
        case 4 : // Апрель
        case 5 : // Май
        case 6 : // Июнь
        case 7 : // Июль
        case 9 : // Сентябрь
        case 10: // Октябрь
        case 11: // Ноябрь
        case 12: // Декабрь
        {
            MonthInGenitiveCase = MonthInGenitiveCase.replace( MonthInGenitiveCase.length() - 1, 1, tr("я") );
            break;
        }

        default:
        {
            qDebug() << "#ERROR = HelperWT::nameMonthInGenitiveCase( " << Date << " ) - uncorrect month: " << Date.month() << ".#";
            break;
        }
    }

    if( isLowerChars )
        MonthInGenitiveCase = MonthInGenitiveCase.toLower();

    return MonthInGenitiveCase;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief MainWindow::nameMonthInDativeCase
 * \param Date - Дата, из которой используется номер месяца
 * \param isLowerChars - если == TRUE, первая буква месяца будет в нижнем регистре
 * \return Название месяца в дательном падеже
 */
const QString HelperWT::nameMonthInDativeCase( const QDate & Date, bool isLowerChars )
{
    QString MonthInDativeCase = QLocale().monthName( Date.month() );

    switch( Date.month() )
    {
        case 3: // Март
        case 8: // Август
        {
            MonthInDativeCase += tr("е");
            break;
        }

        case 1 : // Январь
        case 2 : // Февраль
        case 4 : // Апрель
        case 5 : // Май
        case 6 : // Июнь
        case 7 : // Июль
        case 9 : // Сентябрь
        case 10: // Октябрь
        case 11: // Ноябрь
        case 12: // Декабрь
        {
            MonthInDativeCase = MonthInDativeCase.replace( MonthInDativeCase.length() - 1, 1, tr("е") );
            break;
        }

        default:
        {
            qDebug() << "#ERROR = MainWindow::nameMonthInGenitiveCase( " << Date << " ) - uncorrect month: " << Date.month() << ".#";
            break;
        }
    }

    if( isLowerChars )
        MonthInDativeCase = MonthInDativeCase.toLower();

    return MonthInDativeCase;
}
// ------------------------------------------------------------------------------------ //
