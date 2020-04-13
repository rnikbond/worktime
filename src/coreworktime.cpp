// ---------------------------- //
#include <QFile>
#include <QMenu>
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
#define VERSION 0
// ---------------------------- //

CoreWorkTime::CoreWorkTime( QObject * parent ) : QObject( parent )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::CoreWorkTime(...)";
#endif

    workingRate           = HelperWT::UnknownWR;
    themeIndex            = 0;
    opacityValue          = 1000;
    isAutorun             = true;
    isTray                = false;
    isContextMenu         = true;
    isCheckUpdates        = true;
    updatePath            = HelperWT::pathToUpdates();
    LaunchStartTime       = QTime( 12, 00 );
    LaunchEndTime         = QTime( 16, 00 );
    LaunchLengthTime      = QTime( 00, 45 );
    MaxTime               = QTime( 21, 00 );
    BeforeTime            = QTime( 00, 05 );
    AfterTime             = QTime( 00, 06 );
    isViewWidget          = true;
    isTopWidget           = true;
    opacityWidget         = 1000;
    WorkTimeGeometry      = QRect( 100, 100, 800, 600 );
    WidgetDesktopGeometry = QRect( 100, 100, 200, 80  );
    isShownMenu           = true;
    selectedPage          = 0;

    QRect DesktopRect = qApp->desktop()->rect();

    WorkTimeGeometry.moveTopLeft( QPoint((DesktopRect.width () - WorkTimeGeometry.width ()) / 2,
                                         (DesktopRect.height() - WorkTimeGeometry.height()) / 2) );

    if( QDir(HelperWT::pathToWorkDir()).exists() == false )
        QDir().mkdir( HelperWT::pathToWorkDir() );

    createLoaded   ();
    createObjects  ();
    createTray     ();
    connectModel   ();
    connectWorkTime();
    connectSettings();
}
// ------------------------------------------------------------------------------------ //

CoreWorkTime::~CoreWorkTime()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::~CoreWorkTime()";
#endif

    DataBaseThread->terminate();

    delete Settings;
    delete WorkTime;
}
// ------------------------------------------------------------------------------------ //

void CoreWorkTime::initialize()
{
    ModelWT->setWorkingRate     ( workingRate      );
    ModelWT->setTimeLaunchLength( LaunchLengthTime );
    ModelWT->setTimeLaunchStart ( LaunchStartTime  );
    ModelWT->setTimeLaunchEnd   ( LaunchEndTime    );
    ModelWT->setTimeMax         ( MaxTime          );
    ModelWT->setTimeBefore      ( BeforeTime       );
    ModelWT->setTimeAfter       ( AfterTime        );

    WorkTime->setWindowOpacity( toOpacity(opacityValue) );
    WorkTime->setGeometry     ( WorkTimeGeometry        );
    WorkTime->setShownMenu    ( isShownMenu             );
    WorkTime->setSelectedPage ( selectedPage            );
    WorkTime->setSelectedDate ( HelperWT::currentDate() );
}
// ------------------------------------------------------------------------------------ //

void CoreWorkTime::updateSettings()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::updateSettings()" ;
#endif

    Settings->setWorkingRates( HelperWT::namesWorkingRates() );
    Settings->setThemes      ( themeNames()                  );

    Settings->setWorkingRate  ( workingRate      );
    Settings->setTheme        ( themeIndex       );
    Settings->setOpacityValue ( opacityValue     );
    Settings->setAutorun      ( isAutorun        );
    Settings->setTray         ( isTray           );
    Settings->setContextMenu  ( isContextMenu    );
    Settings->setUpdates      ( isCheckUpdates   );
    Settings->setUpdatesPath  ( updatePath       );
    Settings->setLaunchStart  ( LaunchStartTime  );
    Settings->setLaunchEnd    ( LaunchEndTime    );
    Settings->setLaunchTime   ( LaunchLengthTime );
    Settings->setMaxTime      ( MaxTime          );
    Settings->setBeforeTime   ( BeforeTime       );
    Settings->setAfterTime    ( AfterTime        );
    Settings->setViewWidget   ( isViewWidget     );
    Settings->setTopWidget    ( isTopWidget      );
    Settings->setOpacityWidget( opacityWidget    );
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
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::wait( " << isWait << " )" ;
#endif

    if( isWait )
    {
        WaitLabel->setGeometry( WorkTime->geometry() );
        WaitLabel->show();
        WaitLabel->raise();
    }
    else
    {
        WaitLabel->hide();
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

        closeApp();
    }
    else
    {
        readConfig();

        WorkTime->setGeometry( WorkTimeGeometry );

        DataBaseThread->start();

        DataBase->insertWorkingRates( HelperWT::namesWorkingRates() );

        //applyTheme( themeIndex );

        if( workingRate == HelperWT::UnknownWR )
        {
            updateSettings();
            showWindow( Settings, true );

        }
        else
        {
            initialize();
            WorkTime->show();
        }
    }
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

    int saveWorkingRate = workingRate;

    workingRate = rate;

    if( saveWorkingRate == HelperWT::UnknownWR )
    {
        initialize();
        WorkTime->show();
    }

    ModelWT->setWorkingRate( rate );

    writeConfig();

    QMessageBox MsgBox( QMessageBox::Information, "", QObject::tr("Рабочая ставка успешно сохранена") );
    MsgBox.exec();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeTheme
 * \param theme Индекс выбранной темы
 */
void CoreWorkTime::changeTheme( int theme )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeTheme( " << theme << " )";
#endif

    themeIndex = theme;

    writeConfig();

    applyTheme( themeIndex );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeOpacityValue
 * \param value Значение прозрачности от 100 до 1000
 */
void CoreWorkTime::changeOpacityValue( int value )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeOpacityValue( " << value << " )";
#endif

    opacityValue = value;

    WorkTime->setWindowOpacity( toOpacity(value) );

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeAutorun
 * \param isSet Признак использования автозапуска
 */
void CoreWorkTime::changeAutorun( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeAutorun( " << isSet << " )";
#endif

    isAutorun = isSet;

    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if( isSet )
        settings.setValue(QApplication::applicationName(), "\"" + QApplication::applicationFilePath().replace("/", "\\") + "\"");
    else
        settings.remove( QApplication::applicationName() );

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeTray
 * \param isSet Признак использования трея
 */
void CoreWorkTime::changeTray( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeTray( " << isSet << " )";
#endif

    isTray = isSet;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeContextMenu
 * \param isSet Признак добавления программы в контекстное меню проводника
 */
void CoreWorkTime::changeContextMenu( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeContextMenu( " << isSet << " )";
#endif

    isContextMenu = isSet;

    if( isSet )
    {
        QSettings settings("HKEY_CLASSES_ROOT\\Directory\\Background\\Shell\\WorkTime\\command", QSettings::NativeFormat);
        settings.setValue("Default", QDir::toNativeSeparators(QApplication::applicationFilePath()) );
    }
    else
    {
        QSettings settings("HKEY_CLASSES_ROOT\\Directory\\Background\\Shell", QSettings::NativeFormat);
        settings.remove( "WorkTime" );
    }

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeUpdates
 * \param isSet Признак проверки обновений
 */
void CoreWorkTime::changeUpdates( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeUpdates( " << isSet << " )";
#endif

    isCheckUpdates = isSet;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeUpdatePath
 * \param path Путь к директории, в которой будет поиск обновлений
 */
void CoreWorkTime::changeUpdatePath( QString path )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeUpdatePath( " << path << " )";
#endif

    updatePath = path;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeLaunchStart
 * \param time Время начала обеда
 */
void CoreWorkTime::changeLaunchStart( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeLaunchStart( " << time << " )";
#endif

    LaunchStartTime = time;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeLaunchEnd
 * \param time Время конца обеда
 */
void CoreWorkTime::changeLaunchEnd( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeLaunchEnd( " << time << " )";
#endif

    LaunchEndTime = time;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeLaunchTime
 * \param time Время длительности обеда
 */
void CoreWorkTime::changeLaunchTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeLaunchTime( " << time << " )";
#endif

    LaunchLengthTime = time;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeMaxTime
 * \param time Максимальное время конца рабочего дня
 */
void CoreWorkTime::changeMaxTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeMaxTime( " << time << " )";
#endif

    MaxTime = time;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeBeforeTime
 * \param time Время, которое будет отниматься от времени начала
 */
void CoreWorkTime::changeBeforeTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeBeforeTime( " << time << " )";
#endif

    BeforeTime = time;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeAfterTime
 * \param time Время, которое будет добавляться ко времени конца
 */
void CoreWorkTime::changeAfterTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeAfterTime( " << time << " )";
#endif

    AfterTime = time;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeViewWidget
 * \param isSet Признак использования виджета рабочего стола
 */
void CoreWorkTime::changeViewWidget( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeViewWidget( " << isSet << " )";
#endif

    isViewWidget = isSet;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeTopWidget
 * \param isSet Признак того, что виджет рабочего стола будет всегда поверх других окон
 */
void CoreWorkTime::changeTopWidget( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeTopWidget( " << isSet << " )";
#endif

    isTopWidget = isSet;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeOpacityWidget
 * \param value Значение прозрачности виджет от 100 до 1000
 */
void CoreWorkTime::changeOpacityWidget( int value )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeOpacityWidget( " << value << " )";
#endif

    opacityWidget = value;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::closeWorkTimeWindow
 *
 * Если используется трей, то программа будет свернута в трей.
 * Иначе программа будет закрыта.
 */
void CoreWorkTime::closeWorkTimeWindow()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::closeWorkTimeWindow()";
#endif

    if( isTray )
    {
        WorkTime->hide();
    }
    else
    {
        closeApp();
    }
}
// ------------------------------------------------------------------------------------ //

void CoreWorkTime::showWorkTime()
{
    WorkTime->show();
    WorkTime->raise();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeVisibleMenu
 * \param isShown Признак отображения меню
 */
void CoreWorkTime::changeVisibleMenu( bool isShown )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeVisibleMenu( " << isShown << " )";
#endif
    isShownMenu = isShown;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changePage
 * \param page Номер выбранной страницы
 */
void CoreWorkTime::changePage( int page )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changePage( " << page << " )";
#endif

    selectedPage = page;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeGeometryWidget
 *
 * В файл конфигурации сохраняется геметрия главного окна
 */
void CoreWorkTime::changeGeometryWidget( QRect geometry )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeGeometryWidget( " << geometry << " )";
#endif

    WidgetDesktopGeometry = geometry;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::changeGeometryWorkTime
 *
 * В файл конфигурации сохраняется геметрия виджета рабочего стола
 */
void CoreWorkTime::changeGeometryWorkTime( QRect geometry )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::changeGeometryWorkTime( " << geometry << " )";
#endif

    WorkTimeGeometry = geometry;

    writeConfig();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::resetUpdatePath
 *
 * Установка пути для проверки обновлений по умолчанию
 */
void CoreWorkTime::resetUpdatePath()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::resetUpdatePath()";
#endif

    Settings->setUpdatesPath( HelperWT::pathToUpdates() );
    changeUpdatePath( HelperWT::pathToUpdates() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::readConfig
 *
 * Чтений файла конфигурации.
 *
 * Из файла конфигурации считываются следующие данные:
 *
 * 1.  Версия программы.
 * 2.  Идентификатор рабочей ставки.
 * 3.  Прозрачность главного окна (от 100 до 1000).
 * 4.  Идентификатор темы.
 * 5.  Признак отображения меню в главном окне.
 * 6.  Номер выбранной страницы в поле статистики (День/Неделя/Месяц).
 * 7.  Признак отображения виджета рабочего стола.
 * 8.  Признак отображения виджета рабочего стола поверх других окон.
 * 9.  Позиция виджета рабочего стола на экране.
 * 10. Прозрачность виджета рабочего стола.
 * 11. Признак использования трея.
 * 12. Признак автозапуска.
 * 13. Признак добавления программы в контекстное меню проводника.
 * 14. Признак проверки обновлений.
 * 15. Путь к обновлениям.
 * 16. Время начала и конца обеда.
 * 17. Длительность обеда.
 * 18. Максимальное время конца рабочего дня.
 * 19. Время, сколько отнимать от времени начала.
 * 20. Время, сколько добавлять ко времени конца.
 * 21. Позиция главного окна на экране.
 */
void CoreWorkTime::readConfig()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::readConfig()";
#endif

    int VersionConfig = -1;

    int HoursLaunchStart    = 0;
    int MinutesLaunchStart  = 0;
    int HoursLaunchEnd      = 0;
    int MinutesLaunchEnd    = 0;
    int HoursLaunchLength   = 0;
    int MinutesLaunchLength = 0;
    int HoursMax            = 0;
    int MinutesMax          = 0;
    int HourseBefore        = 0;
    int MinutesBefore       = 0;
    int HourseAfter         = 0;
    int MinutesAfter        = 0;

    if( QFile(HelperWT::pathToConfig()).exists() == false )
        writeConfig();

    QSettings Config( HelperWT::pathToConfig(), QSettings::IniFormat );
    Config.setIniCodec( "UTF-8" );
    Config.setPath( QSettings::IniFormat, QSettings::UserScope, HelperWT::pathToConfig() );

    Config.beginGroup( "COMMON" );
        if( Config.contains("VERSION"       ) ) VersionConfig = Config.value("VERSION"       ).toInt();
        if( Config.contains("WORKING_RATE"  ) ) workingRate   = Config.value("WORKING_RATE"  ).toInt();
        if( Config.contains("WINDOWS_MENU"  ) ) isContextMenu = Config.value("WINDOWS_MENU"  ).toBool();
        if( Config.contains("OPACITY"       ) ) opacityValue  = Config.value("OPACITY"       ).toInt();
        if( Config.contains("THEME"         ) ) themeIndex    = Config.value("THEME"         ).toInt();
        if( Config.contains("SHOWN_MENU"    ) ) isShownMenu   = Config.value("SHOWN_MENU"    ).toBool();
        if( Config.contains("STATISTIC_PAGE") ) selectedPage  = Config.value("STATISTIC_PAGE").toInt();
    Config.endGroup(); // COMMON

    Config.beginGroup("DESKTOP_WIDGET");
        if( Config.contains("VISIBLE") ) isViewWidget = Config.value("VISIBLE").toBool();
        if( Config.contains("IS_TOP" ) ) isTopWidget  = Config.value("IS_TOP" ).toBool();
        if( Config.contains("X")       ) WidgetDesktopGeometry.setX( Config.value("X").toInt() );
        if( Config.contains("Y")       ) WidgetDesktopGeometry.setY( Config.value("Y").toInt() );
        if( Config.contains("OPACITY") ) opacityWidget = Config.value("OPACITY" ).toInt();
    Config.endGroup(); // DESKTOP_WIDGET

    Config.beginGroup( "AUTO" );
        if( Config.contains("TRAY"          ) ) isTray         = Config.value("TRAY"         ).toBool();
        if( Config.contains("AUTORUN"       ) ) isAutorun      = Config.value("AUTORUN"      ).toBool();
        if( Config.contains("UPDATES"       ) ) isCheckUpdates = Config.value("UPDATES"      ).toBool();
        if( Config.contains("UPDATES_PATH"  ) ) updatePath     = Config.value("UPDATES_PATH" ).toString();
    Config.endGroup(); // AUTO

    Config.beginGroup( "TIMES" );
        if( Config.contains("LAUNCH_HOURS"           ) ) HoursLaunchLength   = Config.value("LAUNCH_HOURS"           ).toInt();
        if( Config.contains("LAUNCH_MINUTES"         ) ) MinutesLaunchLength = Config.value("LAUNCH_MINUTES"         ).toInt();
        if( Config.contains("LAUNCH_START_HOURS"     ) ) HoursLaunchStart    = Config.value("LAUNCH_START_HOURS"     ).toInt();
        if( Config.contains("LAUNCH_START_MINUTES"   ) ) MinutesLaunchStart  = Config.value("LAUNCH_START_MINUTES"   ).toInt();
        if( Config.contains("LAUNCH_END_HOURS"       ) ) HoursLaunchEnd      = Config.value("LAUNCH_END_HOURS"       ).toInt();
        if( Config.contains("LAUNCH_END_MINUTES"     ) ) MinutesLaunchEnd    = Config.value("LAUNCH_END_MINUTES"     ).toInt();
        if( Config.contains("MAX_HOURS"              ) ) HoursMax            = Config.value("MAX_HOURS"              ).toInt();
        if( Config.contains("MAX_MINUTES"            ) ) MinutesMax          = Config.value("MAX_MINUTES"            ).toInt();
        if( Config.contains("SUBSTRACT_START_HOURS"  ) ) HourseBefore        = Config.value("SUBSTRACT_START_HOURS"  ).toInt();
        if( Config.contains("SUBSTRACT_START_MINUTES") ) MinutesBefore       = Config.value("SUBSTRACT_START_MINUTES").toInt();
        if( Config.contains("SUBSTRACT_END_HOURS"    ) ) HourseAfter         = Config.value("SUBSTRACT_END_HOURS"    ).toInt();
        if( Config.contains("SUBSTRACT_END_MINUTES"  ) ) MinutesAfter        = Config.value("SUBSTRACT_END_MINUTES"  ).toInt();
    Config.endGroup(); // TIMES

    Config.beginGroup( "GEOMETRY" );
        WorkTimeGeometry.setX     ( Config.value( "X"      ).toInt() );
        WorkTimeGeometry.setY     ( Config.value( "Y"      ).toInt() );
        WorkTimeGeometry.setWidth ( Config.value( "WIDTH"  ).toInt() );
        WorkTimeGeometry.setHeight( Config.value( "HEIGHT" ).toInt() );
    Config.endGroup(); // GEOMETRY

    LaunchStartTime  = QTime( HoursLaunchStart , MinutesLaunchStart  );
    LaunchEndTime    = QTime( HoursLaunchEnd   , MinutesLaunchEnd    );
    LaunchLengthTime = QTime( HoursLaunchLength, MinutesLaunchLength );
    MaxTime          = QTime( HoursMax         , MinutesMax          );
    BeforeTime       = QTime( HourseBefore     , MinutesBefore       );
    AfterTime        = QTime( HourseAfter      , MinutesAfter        );

    normalizeGeometryWorkTime();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::writeConfig
 *
 * Запись файла конфигурации.
 *
 * В файл конфигурации записываются следующие поля:
 * 1.  Версия программы.
 * 2.  Идентификатор рабочей ставки.
 * 3.  Прозрачность главного окна (от 100 до 1000).
 * 4.  Идентификатор темы.
 * 5.  Признак отображения меню в главном окне.
 * 6.  Номер выбранной страницы в поле статистики (День/Неделя/Месяц).
 * 7.  Признак отображения виджета рабочего стола.
 * 8.  Признак отображения виджета рабочего стола поверх других окон.
 * 9.  Позиция виджета рабочего стола на экране.
 * 10. Прозрачность виджета рабочего стола.
 * 11. Признак использования трея.
 * 12. Признак автозапуска.
 * 13. Признак добавления программы в контекстное меню проводника.
 * 14. Признак проверки обновлений.
 * 15. Путь к обновлениям.
 * 16. Время начала и конца обеда.
 * 17. Длительность обеда.
 * 18. Максимальное время конца рабочего дня.
 * 19. Время, сколько отнимать от времени начала.
 * 20. Время, сколько добавлять ко времени конца.
 * 21. Позиция главного окна на экране.
 */
void CoreWorkTime::writeConfig()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::writeConfig()";
#endif

    QSettings Config( HelperWT::pathToConfig(), QSettings::IniFormat );
    Config.setIniCodec( "UTF-8" );

    // Очищаем конфиг перед записью
    Config.clear();

    Config.beginGroup("COMMON");
        Config.setValue("VERSION"       , VERSION       );
        Config.setValue("WORKING_RATE"  , workingRate   );
        Config.setValue("OPACITY"       , opacityValue  );
        Config.setValue("THEME"         , themeIndex    );
        Config.setValue("SHOWN_MENU"    , isShownMenu   );
        Config.setValue("STATISTIC_PAGE", selectedPage  );
    Config.endGroup(); // COMMON

    Config.beginGroup("DESKTOP_WIDGET");
        Config.setValue("VISIBLE", isViewWidget       );
        Config.setValue("IS_TOP" , isTopWidget        );
        Config.setValue("X"      , WidgetDesktopGeometry.x() );
        Config.setValue("Y"      , WidgetDesktopGeometry.y() );
        Config.setValue("OPACITY", opacityWidget );
    Config.endGroup(); // DESKTOP_WIDGET

    Config.beginGroup( "AUTO" );
        Config.setValue("TRAY"         , isTray         );
        Config.setValue("AUTORUN"      , isAutorun      );
        Config.setValue("WINDOWS_MENU" , isContextMenu  );
        Config.setValue("UPDATES"      , isCheckUpdates );
        Config.setValue("UPDATES_PATH" , updatePath     );
    Config.endGroup(); // AUTO

    Config.beginGroup( "TIMES" );
        Config.setValue( "LAUNCH_HOURS"        , LaunchLengthTime.hour()   );
        Config.setValue( "LAUNCH_MINUTES"      , LaunchLengthTime.minute() );
        Config.setValue( "LAUNCH_START_HOURS"  , LaunchStartTime.hour()    );
        Config.setValue( "LAUNCH_START_MINUTES", LaunchStartTime.minute()  );
        Config.setValue( "LAUNCH_END_HOURS"    , LaunchEndTime.hour()      );
        Config.setValue( "LAUNCH_END_MINUTES"  , LaunchEndTime.minute()    );
        Config.setValue( "MAX_HOURS"           , MaxTime.hour()            );
        Config.setValue( "MAX_MINUTES"         , MaxTime.minute()          );
        Config.setValue( "BEFORE_HOURS"        , BeforeTime.hour()         );
        Config.setValue( "BEFORE_MINUTES"      , BeforeTime.minute()       );
        Config.setValue( "AFTER_HOURS"         , AfterTime.hour()          );
        Config.setValue( "AFTER_MINUTES"       , AfterTime.minute()        );
    Config.endGroup(); // TIMES

    Config.beginGroup( "GEOMETRY" );
        Config.setValue( "X"     , WorkTimeGeometry.x()      );
        Config.setValue( "Y"     , WorkTimeGeometry.y()      );
        Config.setValue( "WIDTH" , WorkTimeGeometry.width()  );
        Config.setValue( "HEIGHT", WorkTimeGeometry.height() );
    Config.endGroup(); // GEOMETRY
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

    updateSettings();
    showWindow( Settings, false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::normalizeGeometryWorkTime
 *
 * Нормализация геометрии главного окна.
 * Если находится за экраном, оно будет перемещено в центр экрана
 */
void CoreWorkTime::normalizeGeometryWorkTime()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::normalizeGeometryWorkTime()";
#endif

    QRect DesktopRect = qApp->desktop()->rect();

    if( DesktopRect.contains(WorkTimeGeometry) == false )
    {
        WorkTimeGeometry.moveTopLeft( QPoint((DesktopRect.width () - WorkTimeGeometry.width ()) / 2,
                                             (DesktopRect.height() - WorkTimeGeometry.height()) / 2) );
    }
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
         int deltaWidth  = 0;
         int deltaHeight = 0;

         if( Window->width() > WorkTime->width() )
             deltaWidth = (Window->width() - WorkTime->width()) / 2;

         if( Window->height() > WorkTime->height() )
             deltaHeight = (Window->height() - WorkTime->height()) / 2;

         Center = WorkTime->mapToGlobal( QPoint((WorkTime->width () - Window->width ()) / 2 - deltaWidth,
                                                (WorkTime->height() - Window->height()) / 2 - deltaHeight) );
     }

     Window->move( Center );

     if( Window->isVisible() == false )
         Window->show();

     Window->raise();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::themePathes
 * \return Список путей к схемам
 */
QStringList CoreWorkTime::themePathes()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::themePathes()";
#endif

    QDir ThemesDir( HelperWT::pathToThemes() );
    ThemesDir.setFilter( QDir::Dirs | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot | QDir::NoDotAndDotDot );
    ThemesDir.setSorting( QDir::Size | QDir::Reversed );

    QFileInfoList PathesInfo = ThemesDir.entryInfoList();
    QStringList PathesList;

    for( int theme = 0; theme < PathesInfo.count(); theme++ )
        PathesList << QFileInfo(PathesInfo.at(theme)).filePath();


    return PathesList;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::applyTheme
 * \param theme Индекс темы
 *
 * Применяется тема для всего приложения
 */
void CoreWorkTime::applyTheme( int theme )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::applyTheme( " << theme << " )";
#endif

    QStringList ThemesNames = themeNames();
    QStringList ThemesList  = themePathes();

    for( int themeIndex = 0; themeIndex < ThemesList.count(); themeIndex++ )
    {
        if( themeIndex == theme )
        {
            QString ThemePath = HelperWT::pathToThemes() + "/" + ThemesNames.at(theme) + "/" + ThemesNames.at(theme) + ".css";

            QFile ThemeFile( ThemePath );

            if( ThemeFile.exists() )
            {
                if( ThemeFile.open(QIODevice::ReadOnly) )
                {
                    qApp->setStyleSheet( ThemeFile.readAll() );
                    ThemeFile.close();
                }
            }

            break;
        }
    }

}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::themeNames
 * \return Список названий тем
 */
QStringList CoreWorkTime::themeNames()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::themeNames()";
#endif

    QStringList ThemesNames;
    QStringList ThemesList = themePathes();

    for( int theme = 0; theme < ThemesList.count(); theme++ )
    {
        ThemesNames << QFileInfo(ThemesList.at(theme)).fileName();
    }

    return ThemesNames;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::toOpacity
 * \param value Значение прозрачности от 100 до 1000
 * \return Прозрачность от 0.1 до 1.0
 */
float CoreWorkTime::toOpacity( int value )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::toOpacity( " << value << " )";
#endif

    return float( value / 1000. );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::closeApp
 *
 * Закрытие приложения
 */
void CoreWorkTime::closeApp()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::closeApp()";
#endif

    TrayWorkTime->hide();

    qApp->closeAllWindows();
    qApp->quit();
}
// ------------------------------------------------------------------------------------ //

void CoreWorkTime::iconActivated( QSystemTrayIcon::ActivationReason reason )
{
    switch( reason )
    {
        case QSystemTrayIcon::Trigger:
        {
            if( isTray )
            {
                if( WorkTime->isHidden() ) WorkTime->show();
                else                       WorkTime->hide();
            }

            break;
        }

        default: break;
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::createLoaded
 *
 * Создание виджета, отображающего процесс загрузки
 */
void CoreWorkTime::createLoaded()
{
    WaitLabel = new QLabel( tr("Загрузка данных...\nПожалуйста, подождите") );
    WaitLabel->setWindowFlags( Qt::FramelessWindowHint | Qt::CustomizeWindowHint );
    WaitLabel->setAlignment( Qt::AlignCenter );
    WaitLabel->setStyleSheet( "color: black; font-size: 14px;" );
    WaitLabel->setWindowOpacity( 0.6 );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::createObjects
 *
 * Создание и настрйока объектов
 */
void CoreWorkTime::createObjects()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::createObjects()";
#endif

    DataBase       = new DataBaseWT       (          );
    ModelWT        = new ModelWorkTime    ( this     );
    WorkTime       = new WorkTimeWindow   (          );
    PresenterWT    = new PresenterWorkTime( this     );
    Settings       = new SettingsWindow   (          );
    TablesWindow   = new TablesDataBase   ( DataBase );

    DataBaseThread = new QThread          ( this     );
    DataBase->moveToThread( DataBaseThread );

    WorkTime->setSettingsExists( true );

    Settings->setWindowModality( Qt::ApplicationModal );

    ModelWT->setDataBase( DataBase );

    PresenterWT->setModel( ModelWT  );
    PresenterWT->setView ( WorkTime );

    //TablesWindow->show();
}
// ------------------------------------------------------------------------------------ //

void CoreWorkTime::createTray()
{
    // Инициализируем иконку трея, устанавливаем иконку из набора системных иконок, а также задаем всплывающую подсказку
    TrayWorkTime = new QSystemTrayIcon( this );
    TrayWorkTime->setIcon(QIcon(":/icons/icons/logo/worktime.png"));
    TrayWorkTime->setToolTip( tr("WorkTime") );

    // После чего создаем контекстное меню из двух пунктов
    QMenu  * Menu       = new QMenu(                         WorkTime );
    QAction* ViewWindow = new QAction( tr("Открыть"       ), this );
    QAction* QuitAction = new QAction( tr("Выход"         ), this );

    // Подключаем сигналы нажатий на пункты меню к соответсвующим слотам
    // Первый пункт меню разворачивает приложение из трея, а второй пункт меню завершает приложение
    connect( ViewWindow, SIGNAL(triggered()), this, SLOT(showWorkTime()) );
    connect( QuitAction, SIGNAL(triggered()), qApp, SLOT(quit()) );

    Menu->addAction( ViewWindow );
    Menu->addAction( QuitAction );

    // Устанавливаем контекстное меню на иконку и показываем иконку приложения в трее
    TrayWorkTime->setContextMenu( Menu );
    TrayWorkTime->show();

    // Также подключаем сигнал нажатия на иконку к обработчику данного нажатия
    connect( TrayWorkTime, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::connectWorkTime
 *
 * Соединение сигналов от View WorkTime
 */
void CoreWorkTime::connectWorkTime()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::connectWorkTime()";
#endif

    connect( WorkTime, SIGNAL(closeWindow    (     )), SLOT(closeWorkTimeWindow    (     )) );
    connect( WorkTime, SIGNAL(changedGeometry(QRect)), SLOT(changeGeometryWorkTime(QRect)) );
    connect( WorkTime, SIGNAL(showSettings   (     )), SLOT(showSettings           (     )) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::connectSettings
 *
 * Соединение сигналов от View Settings
 */
void CoreWorkTime::connectSettings()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::connectSettings()";
#endif

    connect( Settings, SIGNAL(closeApp()       ), SLOT(closeApp       ()) );
    connect( Settings, SIGNAL(resetUpdatePath()), SLOT(resetUpdatePath()) );

    connect( Settings, SIGNAL(changedWorkingRate  (int    )), SLOT(changedWorkingRate (int    )) );
    connect( Settings, SIGNAL(changedTheme        (int    )), SLOT(changeTheme        (int    )) );
    connect( Settings, SIGNAL(changedOpacityValue (int    )), SLOT(changeOpacityValue (int    )) );
    connect( Settings, SIGNAL(changedAutorun      (bool   )), SLOT(changeAutorun      (bool   )) );
    connect( Settings, SIGNAL(changedTray         (bool   )), SLOT(changeTray         (bool   )) );
    connect( Settings, SIGNAL(changedContextMenu  (bool   )), SLOT(changeContextMenu  (bool   )) );
    connect( Settings, SIGNAL(changedUpdates      (bool   )), SLOT(changeUpdates      (bool   )) );
    connect( Settings, SIGNAL(changedUpdatePath   (QString)), SLOT(changeUpdatePath   (QString)) );
    connect( Settings, SIGNAL(changedLaunchStart  (QTime  )), SLOT(changeLaunchStart  (QTime  )) );
    connect( Settings, SIGNAL(changedLaunchEnd    (QTime  )), SLOT(changeLaunchEnd    (QTime  )) );
    connect( Settings, SIGNAL(changedLaunchTime   (QTime  )), SLOT(changeLaunchTime   (QTime  )) );
    connect( Settings, SIGNAL(changedMaxTime      (QTime  )), SLOT(changeMaxTime      (QTime  )) );
    connect( Settings, SIGNAL(changedBeforeTime   (QTime  )), SLOT(changeBeforeTime   (QTime  )) );
    connect( Settings, SIGNAL(changedAfterTime    (QTime  )), SLOT(changeAfterTime    (QTime  )) );
    connect( Settings, SIGNAL(changedViewWidget   (bool   )), SLOT(changeViewWidget   (bool   )) );
    connect( Settings, SIGNAL(changedTopWidget    (bool   )), SLOT(changeTopWidget    (bool   )) );
    connect( Settings, SIGNAL(changedOpacityWidget(int    )), SLOT(changeOpacityWidget(int    )) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::connectModel
 *
 * Соединение сигналов от Model
 */
void CoreWorkTime::connectModel()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call CoreWorkTime::connectModel()";
#endif

    connect( ModelWT, SIGNAL(enabledWait(bool)), SLOT(wait(bool)) );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::infoLog
 * \param logText Тест для логгирования информации
 */
void CoreWorkTime::infoLog( const QString & logText )
{
    qDebug() << "intoLog == " << logText;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief CoreWorkTime::errorLog
 * \param logText Тест для логгирования ошибки
 */
void CoreWorkTime::errorLog( const QString & logText )
{
    qDebug() << "errorLog == " << logText;
}
// ------------------------------------------------------------------------------------ //
