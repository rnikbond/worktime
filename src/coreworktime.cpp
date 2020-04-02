// ---------------------------- //
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QSharedMemory>
#include <QProgressDialog>
// ---------------------------- //
// ---------------------------- //
#include "coreworktime.h"
// ---------------------------- //

CoreWorkTime::CoreWorkTime( QObject * parent ) : QObject( parent )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::CoreWorkTime(...)";
#endif

    ModelWT     = new ModelWorkTime    ( this );
    WorkTime    = new WorkTimeWindow   (      );
    PresenterWT = new PresenterWorkTime( this );

    PresenterWT->setModel( ModelWT  );
    PresenterWT->setView ( WorkTime );

    WaitLabel = new QLabel();
    WaitMovie = new QMovie( "loaded.gif" );

    WaitLabel->setMovie( WaitMovie );


    connect( ModelWT, SIGNAL(enabledWait(bool)), SLOT(wait(bool)) );
}
// ------------------------------------------------------------------------------------ //

CoreWorkTime::~CoreWorkTime()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::~CoreWorkTime()";
#endif

    delete WorkTime;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::wait
 * \param isWait Признак отображения индикации ожидания
 *
 * Если \a isWait == TRUE, отображается индикация ожидания.
 * Иначе индикация ожидания скрывается.
 */
void CoreWorkTime::wait( bool isWait )
{
    if( isWait )
    {
        WaitLabel->setGeometry( WorkTime->geometry() );
        WaitMovie->setScaledSize( WaitLabel->size() );
        WaitMovie->start();
        WaitLabel->show();
        qDebug() << "\t WAIT ON";
    }
    else
    {
        WaitMovie->stop();
        WaitLabel->hide();
        qDebug() << "\t WAIT OFF";
    }
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
#ifdef WT_INFO_CALL_FUNC
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
#ifdef WT_INFO_CALL_FUNC
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
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::removeOld()";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changedWorkingRate
 * \param rate
 *
 * Смена рабочей ставки (расписания рабочего времени)
 *
 * Рабочая ставка передается модели
 */
void CoreWorkTime::changedWorkingRate( int rate )
{
    ModelWT->setWorkingRate( rate );
}
// ------------------------------------------------------------------------------------ //
