// ---------------------------- //
#include <QMessageBox>
#include <QApplication>
#include <QSharedMemory>
// ---------------------------- //
// ---------------------------- //
#include "coreworktime.h"
// ---------------------------- //

CoreWorkTime::CoreWorkTime( QObject * parent ) : QObject( parent )
{
#ifdef QT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::CoreWorkTime(...)";
#endif

    WorkTime    = new WorkTimeWindow();
    PresenterWT = new PresenterWorkTime( WorkTime, this );
}
// ------------------------------------------------------------------------------------ //

CoreWorkTime::~CoreWorkTime()
{
#ifdef QT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::~CoreWorkTime()";
#endif

    delete WorkTime;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::start
 *
 * При запуске происходит удаление старой версии WorkTime и
 * проверка уже запущенной программы.
 */
void CoreWorkTime::start()
{
#ifdef QT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::start()";
#endif

    removeOld();

    if( isAlreadyRunning() )
    {
        QMessageBox MsgBox( QMessageBox::Critical, "", QObject::tr("Программа уже запущена") );
        MsgBox.exec();

        qApp->quit();
    }

    WorkTime->show();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::isAlreadyRunning
 * \return FALSE, если программа не запущена. Иначе TRUE.
 *
 * Проверка уже запущенной программы реализована через класс QSharedMemory.
 */
bool CoreWorkTime::isAlreadyRunning()
{
#ifdef QT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::checkAlreadyRunning()";
#endif

    QSharedMemory SharedWorkTime ("work-time-15041996-c333de4444f5");

    if( !SharedWorkTime.create(512, QSharedMemory::ReadWrite) )
        return true;

    SharedWorkTime.attach( QSharedMemory::ReadOnly );

    return false;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::removeOld
 * Удаление старой версии WorkTime
 */
void CoreWorkTime::removeOld()
{
#ifdef QT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::removeOld()";
#endif
}
// ------------------------------------------------------------------------------------ //
