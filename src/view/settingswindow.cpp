// ---------------------------- //
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
// ---------------------------- //
#include "helperwt.h"
// ---------------------------- //
#include "settingswindow.h"
#include "ui_settingswindow.h"
// ---------------------------- //

SettingsWindow::SettingsWindow( QWidget * parent ) : QWidget( parent ), gui( new Ui::SettingsWindow )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::SettingsWindow(...)";
#endif

    workingRate = HelperWT::UnknownWR;

    configureGUI();
    connectGUI();
}
// ------------------------------------------------------------------------------------ //

SettingsWindow::~SettingsWindow()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::~SettingsWindow()";
#endif

    delete gui;
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::selectMenuItem
 * \param item Индекс выбранного пункта меню
 */
void SettingsWindow::selectMenuItem( int item )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::selectMenuItem( " << item << " )";
#endif

    gui->SettingsWStack->setCurrentIndex( item );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::selectWorkingRate
 * \param rate Выбранная рабочая ставка
 *
 * При выборе рабочей ставки изменяется ее описание
 */
void SettingsWindow::selectWorkingRate( int rate )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::selectWorkingRate()";
#endif

    rate -= 1;

    QString InfoWorkingRate;

    switch( rate )
    {
        case -1:
            InfoWorkingRate = tr("Выберете рабочую ставку");
        break;

        case HelperWT::Hours_40_Lanch:
            InfoWorkingRate = tr("40 часов с обедом");
        break;

        case HelperWT::Hours_40_NoLanch:
            InfoWorkingRate = tr("40 часов без обеда");
        break;

        case HelperWT::Hours_40_4_Days:
            InfoWorkingRate = tr("4 рабочих дня в неделю");
        break;

        case HelperWT::Hours_35_NoLanch:
            InfoWorkingRate = tr("35 часов с обедом");
        break;

        case HelperWT::Hours_30_NoLanch:
            InfoWorkingRate = tr("30 часов с обедом");
        break;

        case HelperWT::Hours_20_NoLanch:
            InfoWorkingRate = tr("20 часов без обеда");
        break;

        default:
        break;

    }

    gui->WorkingRatesInfo->setText( "<center>" + InfoWorkingRate + "</center>" );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::currentWorkingRate
 *
 * Выбор текущей рабочей ставки
 */
void SettingsWindow::currentWorkingRate()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::currentWorkingRate()";
#endif

    gui->WorkingRatesLWidget->blockSignals( true );
    gui->WorkingRatesLWidget->setCurrentRow( workingRate );
    gui->WorkingRatesLWidget->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setWorkingRate
 *
 * Применение выбранной рабочей ставки.
 *
 * Испускается сигнал \a SettingsWindow::changedWorkingRate.
 */
void SettingsWindow::saveWorkingRate()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::saveWorkingRate()";
#endif

    int rate = gui->WorkingRatesLWidget->currentRow();

    if( rate > 0 )
    {
        workingRate = rate;
        emit changedWorkingRate( workingRate - 1 );

        updateMenuEnabled();
    }
    else
    {
        QMessageBox MsgBox( QMessageBox::Warning, "", QObject::tr("Рабочая ставка не выбрана!") );
        MsgBox.exec();
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::changeThemeCBox
 * \param theme Индекс темы
 *
 * При изменении темы испускается сигнал \a SettingsWindow::changedTheme.
 */
void SettingsWindow::changeThemeCBox( int theme )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::changeThemeCBox( " << theme << " )";
#endif

    emit changedTheme( theme );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::changeOpacitySlider
 * \param value Значение прозрачности
 *
 * При изменении прозрачности испускается сигнал \a SettingsWindow::changedOpacityValue.
 */
void SettingsWindow::changeOpacitySlider( int value )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::changeOpacitySlider( " << value << " )";
#endif

    emit changedOpacityValue( value );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::checkedAutorun
 *
 * Выбор автозапуска. Испускается сигнал \a SettingsWindow::changedAutorun.
 */
void SettingsWindow::checkedAutorun()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::checkedAutorun()";
#endif

    emit changedAutorun( gui->AutorunCheck->isChecked() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::checkedTray
 *
 * Выбор использования трея. Испускается сигнал \a SettingsWindow::changedTray.
 */
void SettingsWindow::checkedTray()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::checkedTray()";
#endif

    emit changedTray( gui->TrayCheck->isChecked() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::checkedContextMenu
 *
 * Выбор добавления программы в контестное меню проводника.
 * Испускается сигнал \a SettingsWindow::changedContextMenu.
 */
void SettingsWindow::checkedContextMenu()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::checkedContextMenu()";
#endif

    emit changedContextMenu( gui->ContextMenuCheck->isChecked() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::checkedUpdates
 *
 * Выбор проверки обновлений.
 * Испускается сигнал \a SettingsWindow::changedUpdates.
 */
void SettingsWindow::checkedUpdates()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::checkedUpdates()";
#endif

    bool isUpdates = gui->UpdatesCheck->isChecked();

    gui->UpdatePathEdit->setEnabled  ( isUpdates );
    gui->SelectPathButton->setEnabled( isUpdates );
    gui->ResetPathButton->setEnabled ( isUpdates );

    emit changedUpdates( isUpdates );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::clickSelectPath
 *
 * Вызов диалога выбора директории.
 *
 * При успешном выборе директории испускается сигнал \a SettingsWindow::changedUpdatePath.
 */
void SettingsWindow::clickSelectPath()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::clickSelectPath()";
#endif

    QString UpdatesPath = gui->UpdatePathEdit->text();

    QString AutoPath = UpdatesPath.isEmpty() ? QDir::homePath() + "/Desktop"  : UpdatesPath;

    QString NewPath = QFileDialog::getExistingDirectory( this, tr("Путь к обновлениям"), AutoPath ) ;

    if( NewPath.isEmpty() == false )
    {
        setUpdatesPath( NewPath );
        emit changedUpdatePath( NewPath );
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::clickResetPath
 *
 * Сброс пути для проверки обновления файла.
 * Испускается сигнал \a SettingsWindow::resetUpdatePath.
 */
void SettingsWindow::clickResetPath()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::clickResetPath()";
#endif

    emit resetUpdatePath();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::changeLaunchStart
 * \param time Время начала обеда
 *
 * Испускается сигнал \a SettingsWindow::changedLaunchStart.
 */
void SettingsWindow::changeLaunchStart( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::changeLaunchStart( " << time << " )";
#endif

    emit changedLaunchStart( time );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::changeLaunchEnd
 * \param time Время конца обеда
 *
 * Испускается сигнал \a SettingsWindow::changedLaunchEnd.
 */
void SettingsWindow::changeLaunchEnd( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::changeLaunchEnd( " << time << " )";
#endif

    emit changedLaunchEnd( time );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::changeLaunchTime
 * \param time Время длительности обеда
 *
 * Испускается сигнал \a SettingsWindow::changedLaunchTime.
 */
void SettingsWindow::changeLaunchTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::changeLaunchTime( " << time << " )";
#endif

    emit changedLaunchTime( time );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::changeMaxTime
 * \param time Максимальное время конца рабочего дня
 *
 * Испускается сигнал \a SettingsWindow::changedMaxTime.
 */
void SettingsWindow::changeMaxTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::changeMaxTime( " << time << " )";
#endif

    emit changedMaxTime( time );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::changeBeforeTime
 * \param time Время, сколько отнимать от времени начала
 *
 * Испускается сигнал \a SettingsWindow::changedBeforeTime.
 */
void SettingsWindow::changeBeforeTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::changeBeforeTime( " << time << " )";
#endif

    emit changedBeforeTime( time );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::changeAfterTime
 * \param time Время, сколько добавлять ко времени конца
 *
 * Испускается сигнал \a SettingsWindow::changedAfterTime.
 */
void SettingsWindow::changeAfterTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::changeAfterTime( " << time << " )";
#endif

    emit changedAfterTime( time );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::checkedViewWidget
 *
 * Выбор отображения виджета рабочего стола.
 *
 * Испускается сигнал \a SettingsWindow::changedViewWidget.
 */
void SettingsWindow::checkedViewWidget()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::checkedViewWidget()";
#endif

    bool isView = gui->ViewWidgetCheck->isChecked();

    gui->TopWidgetCheck     ->setEnabled( isView );
    gui->OpacityWidgetSlider->setEnabled( isView );

    emit changedViewWidget( isView );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::checkedTopWidget
 *
 * Выбор отображения виджета рабочего стола поверх других окон.
 *
 * Испускается сигнал \a SettingsWindow::changedTopWidget.
 */
void SettingsWindow::checkedTopWidget()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::checkedTopWidget()";
#endif

    emit changedTopWidget( gui->TopWidgetCheck->isChecked() );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::changeOpacityWidget
 * \param value Прозрачность виджета
 *
 * Испускается сигнал \a SettingsWindow::changedOpacityWidget.
 */
void SettingsWindow::changeOpacityWidget( int value )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::changeOpacityWidget( " << value << " )";
#endif

    emit changedOpacityWidget( value );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setWorkingRate
 * \param rate Текущая рабоча ставка
 */
void SettingsWindow::setWorkingRate( int rate )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setWorkingRate( " << rate << " )";
#endif

    workingRate = rate + 1;

    gui->SelectWorkingRateLabel->setVisible( workingRate < 1 );

    gui->WorkingRatesLWidget->setCurrentRow( workingRate );

    selectWorkingRate( workingRate );

    updateMenuEnabled();
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setWorkingRates
 * \param ratesList Список названий рабочих ставок
 */
void SettingsWindow::setWorkingRates( QStringList ratesList )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setWorkingRate( " << ratesList << " )";
#endif

    ratesList.prepend( tr("Выберета рабочую ставку...") );

    gui->WorkingRatesLWidget->blockSignals( true );

    gui->WorkingRatesLWidget->clear();

    for( int rate = 0; rate < ratesList.count(); rate++ )
    {
        QListWidgetItem * WorkingRate = new QListWidgetItem( ratesList.at(rate), gui->WorkingRatesLWidget );
        WorkingRate->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

        gui->WorkingRatesLWidget->addItem( WorkingRate );
    }

    gui->WorkingRatesLWidget->setCurrentRow( -1 );

    gui->WorkingRatesLWidget->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setTheme
 * \param theme Индекс текущей темы
 */
void SettingsWindow::setTheme( int theme )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setTheme( " << theme << " )";
#endif

    gui->ThemeCBox->blockSignals( true  );
    gui->ThemeCBox->setCurrentIndex( theme );
    gui->ThemeCBox->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setThemes
 * \param themesList Список названий тем
 */
void SettingsWindow::setThemes( QStringList themesList )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setThemes( " << themesList << " )";
#endif

    gui->ThemeCBox->blockSignals( true  );
    gui->ThemeCBox->clear();
    gui->ThemeCBox->addItems( themesList );
    gui->ThemeCBox->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setOpacityValue
 * \param value Значение прозрачности в диапазоне от 100 до 1000
 */
void SettingsWindow::setOpacityValue( int value )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setOpacityValue( " << value << " )";
#endif

    gui->OpacitySlider->blockSignals( true  );
    gui->OpacitySlider->setValue( value );
    gui->OpacitySlider->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setAutorun
 * \param isSet Признак выбора автозапуска.
 */
void SettingsWindow::setAutorun( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setAutorun( " << isSet << " )";
#endif

    gui->AutorunCheck->blockSignals( true  );
    gui->AutorunCheck->setChecked( isSet );
    gui->AutorunCheck->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setTray
 * \param isSet Признак использования трея.
 */
void SettingsWindow::setTray( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setTray( " << isSet << " )";
#endif

    gui->TrayCheck->blockSignals( true  );
    gui->TrayCheck->setChecked( isSet );
    gui->TrayCheck->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setContextMenu
 * \param isSet Признак добаления программы в контекстное меню.
 */
void SettingsWindow::setContextMenu( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setContextMenu( " << isSet << " )";
#endif

    gui->ContextMenuCheck->blockSignals( true  );
    gui->ContextMenuCheck->setChecked( isSet );
    gui->ContextMenuCheck->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setUpdates
 * \param isSet Признак проверки обновлений.
 */
void SettingsWindow::setUpdates( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setUpdates( " << isSet << " )";
#endif

    gui->UpdatesCheck->blockSignals( true  );
    gui->UpdatesCheck->setChecked( isSet );
    gui->UpdatesCheck->blockSignals( false );

    gui->UpdatePathEdit->setEnabled  ( isSet );
    gui->SelectPathButton->setEnabled( isSet );
    gui->ResetPathButton->setEnabled ( isSet );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setUpdatesPath
 * \param path Путь к обновлениям
 */
void SettingsWindow::setUpdatesPath( QString path )
{
    gui->UpdatePathEdit->setText( path );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setLaunchStart
 * \param time Время начала обеда
 */
void SettingsWindow::setLaunchStart( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setLaunchStart( " << time << " )";
#endif

    gui->LaunchStartValue->blockSignals( true  );
    gui->LaunchStartValue->setTime( time );
    gui->LaunchStartValue->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setLaunchEnd
 * \param time Время окончания обеда.
 */
void SettingsWindow::setLaunchEnd( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setLaunchEnd( " << time << " )";
#endif

    gui->LaunchEndValue->blockSignals( true  );
    gui->LaunchEndValue->setTime( time );
    gui->LaunchEndValue->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setLaunchTime
 * \param time Время длительности обеда
 */
void SettingsWindow::setLaunchTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setLaunchTime( " << time << " )";
#endif

    gui->LaunchTimeValue->blockSignals( true  );
    gui->LaunchTimeValue->setTime( time );
    gui->LaunchTimeValue->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setMaxTime
 * \param time Максимальное время конца рабочего дня.
 */
void SettingsWindow::setMaxTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setMaxTime( " << time << " )";
#endif

    gui->MaxTimeValue->blockSignals( true  );
    gui->MaxTimeValue->setTime( time );
    gui->MaxTimeValue->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setBeforeTime
 * \param time Время, которое будет отниматься от времени начала
 */
void SettingsWindow::setBeforeTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setBeforeTime( " << time << " )";
#endif

    gui->BeforeTimeValue->blockSignals( true  );
    gui->BeforeTimeValue->setTime( time );
    gui->BeforeTimeValue->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setAfterTime
 * \param time Время, которое будет добавляться ко времени конца.
 */
void SettingsWindow::setAfterTime( QTime time )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setAfterTime( " << time << " )";
#endif

    gui->AfterTimeValue->blockSignals( true  );
    gui->AfterTimeValue->setTime( time );
    gui->AfterTimeValue->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setViewWidget
 * \param isSet Признак использования виджета рабочего стола
 */
void SettingsWindow::setViewWidget( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setViewWidget( " << isSet << " )";
#endif

    gui->ViewWidgetCheck->blockSignals( true  );
    gui->ViewWidgetCheck->setChecked( isSet );
    gui->ViewWidgetCheck->blockSignals( false );

    gui->TopWidgetCheck     ->setEnabled( isSet );
    gui->OpacityWidgetSlider->setEnabled( isSet );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setTopWidget
 * \param isSet Признак того, что виджет рабочего стола будет всегда поверх других окон.
 */
void SettingsWindow::setTopWidget( bool isSet )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setTopWidget( " << isSet << " )";
#endif

    gui->TopWidgetCheck->blockSignals( true  );
    gui->TopWidgetCheck->setChecked( isSet );
    gui->TopWidgetCheck->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::setOpacityWidget
 * \param value Значение прозрачности виджета от 100 до 1000.
 */
void SettingsWindow::setOpacityWidget( int value )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::setOpacityWidget( " << value << " )";
#endif

    gui->OpacityWidgetSlider->blockSignals( true  );
    gui->OpacityWidgetSlider->setValue( value );
    gui->OpacityWidgetSlider->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::updateMenuEnabled
 *
 * Если рабочая ставка не выбрана, разблокируется только пункт меню "Время работы".
 */
void SettingsWindow::updateMenuEnabled()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::updateEnabled()";
#endif

    gui->MenuLWidget->item(0)->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );

    for( int item = 1; item < gui->MenuLWidget->count(); item++ )
    {
        QListWidgetItem * MenuItem = gui->MenuLWidget->item( item );

        if( workingRate < 1)
        {
            MenuItem->setFlags( Qt::NoItemFlags );
        }
        else
        {
            MenuItem->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable  );
        }
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::showEvent
 * \param ShowEvent Указатель на событие открытия окна
 *
 * При открытии окна выбирается первый пункт меню
 */
void SettingsWindow::showEvent( QShowEvent * ShowEvent )
{
    gui->MenuLWidget->setCurrentRow( 0 );

    QWidget::showEvent( ShowEvent );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::closeEvent
 * \param CloseEvent Указатель на событие закрытия окна
 *
 * Если при закрытии окна не выбрано время работы,
 * то пользователю будет выведено сообщение о том, что
 * без выбранного времени работы дальнейшая работа невозможна.
 * В случае, если пользователь не захочет снова выбрать время работы,
 * приложение закроется.
 */
void SettingsWindow::closeEvent( QCloseEvent * CloseEvent )
{
    if( workingRate < 1 )
    {
        QMessageBox MessageBox;

        MessageBox.setText( tr("Рабочее время не выбрано") );
        MessageBox.setInformativeText( tr("Без выбора рабочего времени дальнейшая работа невозможна!\nВыбрать рабочее время?") );
        MessageBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
        MessageBox.setDefaultButton( QMessageBox::Yes );

        int userChoise = MessageBox.exec();

        if( userChoise == QMessageBox::Yes )
        {
            CloseEvent->ignore();
        }
        else
        {
            emit closeApp();
        }
    }
    else
    {
        QWidget::closeEvent( CloseEvent );
    }
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::configureGUI
 *
 * Настройка GUI
 */
void SettingsWindow::configureGUI()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::configureGUI()";
#endif

    setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint );

    gui->setupUi( this );

    gui->WorkingRatesLWidget->setSelectionMode( QAbstractItemView::SingleSelection );

    gui->WorkingRatesInfo->setReadOnly( true );

    gui->SelectWorkingRateLabel->setAlignment( Qt::AlignCenter );
    gui->SelectWorkingRateLabel->setText( tr("ВЫБЕРЕТЕ РАБОЧУЮ СТАВКУ ДЛЯ НАЧАЛА РАБОТЫ") );

    gui->MenuLWidget->addItem( tr("Время работы" ) );
    gui->MenuLWidget->addItem( tr("Внешний вид"  ) );
    gui->MenuLWidget->addItem( tr("Автоматизация") );
    gui->MenuLWidget->addItem( tr("Время"        ) );
    gui->MenuLWidget->addItem( tr("Виджет"       ) );

    gui->OpacitySlider->setMinimum( 100 );
    gui->OpacitySlider->setMaximum( 1000 );

    gui->OpacityWidgetSlider->setMinimum( 100 );
    gui->OpacityWidgetSlider->setMaximum( 1000 );
}
// ------------------------------------------------------------------------------------ //

/*!
 * \brief SettingsWindow::connectGUI
 *
 * Соединение сигналов от GUI со слотами
 */
void SettingsWindow::connectGUI()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::connectGUI()";
#endif

    connect( gui->MenuLWidget             , SIGNAL(currentRowChanged(int )), SLOT(selectMenuItem    (int)) );

    // Working rate PAGE
    connect( gui->WorkingRatesLWidget     , SIGNAL(currentRowChanged(int )), SLOT(selectWorkingRate (int)) );
    connect( gui->SaveWorkingRateButton   , SIGNAL(clicked          (bool)), SLOT(saveWorkingRate   (   )) );
    connect( gui->CurrentWorkingRateButton, SIGNAL(clicked          (bool)), SLOT(currentWorkingRate(   )) );

    // View PAGE
    connect( gui->ThemeCBox    , SIGNAL(currentIndexChanged(int)), SLOT(changeThemeCBox    (int)) );
    connect( gui->OpacitySlider, SIGNAL(valueChanged       (int)), SLOT(changeOpacitySlider(int)) );

    // Auto PAGE
    connect( gui->AutorunCheck    , SIGNAL(clicked(bool)), SLOT(checkedAutorun    ()) );
    connect( gui->TrayCheck       , SIGNAL(clicked(bool)), SLOT(checkedTray       ()) );
    connect( gui->ContextMenuCheck, SIGNAL(clicked(bool)), SLOT(checkedContextMenu()) );
    connect( gui->UpdatesCheck    , SIGNAL(clicked(bool)), SLOT(checkedUpdates    ()) );
    connect( gui->SelectPathButton, SIGNAL(clicked(bool)), SLOT(clickSelectPath   ()) );
    connect( gui->ResetPathButton , SIGNAL(clicked(bool)), SLOT(clickResetPath    ()) );

    // Time PAGE
    connect( gui->LaunchStartValue, SIGNAL(timeChanged(QTime)), SLOT(changeLaunchStart(QTime)) );
    connect( gui->LaunchEndValue  , SIGNAL(timeChanged(QTime)), SLOT(changeLaunchEnd  (QTime)) );
    connect( gui->LaunchTimeValue , SIGNAL(timeChanged(QTime)), SLOT(changeLaunchTime (QTime)) );
    connect( gui->MaxTimeValue    , SIGNAL(timeChanged(QTime)), SLOT(changeMaxTime    (QTime)) );
    connect( gui->BeforeTimeValue , SIGNAL(timeChanged(QTime)), SLOT(changeBeforeTime (QTime)) );
    connect( gui->AfterTimeValue  , SIGNAL(timeChanged(QTime)), SLOT(changeAfterTime  (QTime)) );

    // Widget PAGE
    connect( gui->ViewWidgetCheck    , SIGNAL(clicked     (bool)), SLOT(checkedViewWidget  (   )) );
    connect( gui->TopWidgetCheck     , SIGNAL(clicked     (bool)), SLOT(checkedTopWidget   (   )) );
    connect( gui->OpacityWidgetSlider, SIGNAL(valueChanged(int )), SLOT(changeOpacityWidget(int)) );
}
// ------------------------------------------------------------------------------------ //
