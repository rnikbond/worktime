// ---------------------------- //
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QSharedMemory>
#include <QDesktopWidget>
// ---------------------------- //
#include "helperwt.h"
// ---------------------------- //
#include "coreworktime.h"
// ---------------------------- //

CoreWorkTime::CoreWorkTime( QObject * parent ) : QObject( parent )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::CoreWorkTime(...)";
#endif

    workingRate = HelperWT::UnknownWR;

    ModelWT     = new ModelWorkTime    ( this );
    WorkTime    = new WorkTimeWindow   (      );
    PresenterWT = new PresenterWorkTime( this );

    Settings    = new SettingsWindow();
    Settings->setWindowModality( Qt::ApplicationModal );

    WorkTime->setSettingsExists( true );

    PresenterWT->setModel( ModelWT  );
    PresenterWT->setView ( WorkTime );

    WaitLabel = new QLabel();
    WaitMovie = new QMovie( "loaded.gif" );

    WaitLabel->setMovie( WaitMovie );

    connect( WorkTime, SIGNAL(showSettings()), SLOT(showSettings()) );

    connect( ModelWT, SIGNAL(enabledWait(bool)), SLOT(wait(bool)) );

    connect( Settings, SIGNAL(changedWorkingRate(int)), SLOT(changedWorkingRate(int)) );
}
// ------------------------------------------------------------------------------------ //

CoreWorkTime::~CoreWorkTime()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::~CoreWorkTime()";
#endif

    delete Settings;
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

    readConfig();

    Settings->setWorkingRates( HelperWT::namesWorkingRates() );
    Settings->setWorkingRate( workingRate );

    if( workingRate == HelperWT::UnknownWR )
    {
        showSettings();
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
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changedWorkingRate( " << rate << " )";
#endif

    ModelWT->setWorkingRate( rate );

    QMessageBox MsgBox( QMessageBox::Information, "", QObject::tr("Рабочая ставка успешно сохранена") );
    MsgBox.exec();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::readConfig
 *
 * Чтений файла конфигурации.
 *
 * Из файла конфигурации считываются следующие поля:
 */
void CoreWorkTime::readConfig()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::readConfig()";
#endif

    workingRate = HelperWT::UnknownWR;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::writeConfig
 *
 * Запись файла конфигурации.
 *
 * В файл конфигурации записываются следующие поля:
 *
 *
 */
void CoreWorkTime::writeConfig()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::writeConfig()";
#endif
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::showSettings
 *
 * Отображение окна "Настройки".
 * Если окно уже открыто, оно будет поднято на передний план.
 */
void CoreWorkTime::showSettings()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::showSettings()";
#endif

    showWindow( Settings, false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::showWindow
 * \param Window Указатель на окно, которое нужно отобразить
 * \param isCenterDisplay Признак отображения в центре экрана
 *
 * Если == TRUE, окно \a Window будет отображено в центре экрана.
 * Иначе окно будет отображено в центре главного окна.
 */
void CoreWorkTime::showWindow( QWidget * Window, bool isCenterDisplay )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::showWindow( " << Window->objectName() << ", " << isCenterDisplay << " )";
#endif

     QPoint Center;

     if( isCenterDisplay )
     {
         QWidget * Display = qApp->desktop()->screen(0);

         Center = QPoint( Display->width () / 2 - Window->width () / 2,
                          Display->height() / 2 - Window->height() / 2 );
     }
     else
     {
         Center = WorkTime->mapToGlobal( QPoint(WorkTime->width () / 2 - Window->width () / 2,
                                                WorkTime->height() / 2 - Window->height() / 2) );
     }

     Window->move( Center );

     if( Window->isVisible() == false )
         Window->show();

     Window->raise();
}
// ------------------------------------------------------------------------------------ //
