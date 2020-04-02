// ---------------------------- //
#include <QDebug>
#include <QMessageBox>
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

    gui->setupUi( this );

    gui->WorkingRatesLWidget->setSelectionMode( QAbstractItemView::SingleSelection );

    connect( gui->WorkingRatesLWidget     , SIGNAL(currentRowChanged(int )), SLOT(selectWorkingRate (int)) );
    connect( gui->SaveWorkingRateButton   , SIGNAL(clicked          (bool)), SLOT(saveWorkingRate   (   )) );
    connect( gui->CurrentWorkingRateButton, SIGNAL(clicked          (bool)), SLOT(currentWorkingRate(   )) );
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

    QString InfoWorkingRate;

    switch( rate )
    {
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

    gui->WorkingRatesInfo->setText( InfoWorkingRate );
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
 * Применение выбранной рабочей ставки
 */
void SettingsWindow::saveWorkingRate()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call SettingsWindow::saveWorkingRate()";
#endif

    int rate = gui->WorkingRatesLWidget->currentRow();

    qDebug() << "selected rate: " << rate;

    if( rate != HelperWT::UnknownWR )
    {
        workingRate = rate;
        emit changedWorkingRate( gui->WorkingRatesLWidget->currentRow() );
    }
    else
    {
        QMessageBox MsgBox( QMessageBox::Warning, "", QObject::tr("Рабочая ставка не выбрана!") );
        MsgBox.exec();
    }
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

    if( rate < gui->WorkingRatesLWidget->count() )
    {
        workingRate = rate;

        gui->WorkingRatesLWidget->blockSignals( true );
        gui->WorkingRatesLWidget->setCurrentRow( rate );
        gui->WorkingRatesLWidget->blockSignals( false );
    }
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

    gui->WorkingRatesLWidget->blockSignals( true );

    for( int rate = 0; rate < ratesList.count(); rate++ )
    {
        QListWidgetItem * WorkingRate = new QListWidgetItem( ratesList.at(rate), gui->WorkingRatesLWidget );
        WorkingRate->setFlags( Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable );

        gui->WorkingRatesLWidget->addItem( WorkingRate );
    }

    gui->WorkingRatesLWidget->setCurrentRow( -1 );

    gui->WorkingRatesLWidget->blockSignals( false );
}
// ------------------------------------------------------------------------------------ //
