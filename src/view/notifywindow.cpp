// ---------------------------- //
#include <QMenu>
// ---------------------------- //
#include "helperwt.h"
// ---------------------------- //
#include "notifywindow.h"
#include "ui_notifywindow.h"
// ---------------------------- //

NotifyWindow::NotifyWindow( QWidget *parent ) : QWidget( parent ), gui( new Ui::NotifyWindow )
{
    gui->setupUi( this );

    configureGUI();
    connectGUI();
}
// ------------------------------------------------------------------------------------ //

NotifyWindow::~NotifyWindow()
{
    delete gui;
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::selectPeriodocity( int selectedIndex )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call NotifyWindow::selectPeriodocity( " << selectedIndex << " )";
#endif

    gui->TypesTimeGBox->setEnabled( selectedIndex != NonePeriodicity );
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::selectTypeTime( int selectedIndex )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call NotifyWindow::selectTypeTime( " << selectedIndex << " )";
#endif

    gui->DateNotifyGBox->setEnabled( selectedIndex != NoneTypeNotify );

    if( selectedIndex == CertainTime )
    {
        gui->StartTimeNotifyEdit   ->setTime( QTime(0, 0, 0) );
        gui->IntervalTimeNotifyEdit->setTime( QTime(0, 0, 0) );
        gui->EndTimeNotifyEdit     ->setTime( QTime(0, 0, 0) );

        gui->CertainTimeGBox->setEnabled ( true );
        gui->IntervalTimeGBox->setEnabled( false );
    }
    else if( selectedIndex == IntervalTime )
    {
        gui->CertainTimeNotifyEdit->setTime( QTime(0, 0, 0) );

        gui->CertainTimeGBox ->setEnabled( false );
        gui->IntervalTimeGBox->setEnabled( true  );
    }
    else
    {
        gui->CertainTimeGBox ->setEnabled( false );
        gui->IntervalTimeGBox->setEnabled( false );
    }
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::addNotify()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call NotifyWindow::addNotify()";
#endif

    gui->NotifyList->blockSignals( true );

    QListWidgetItem* NewNotifyItem = new QListWidgetItem( tr("Новое оповещение"), gui->NotifyList );
    NewNotifyItem->setFlags( Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable );

    //emit userAddInterval( NewNotifyItem->text() );

    gui->NotifyList->blockSignals( false );

    gui->NotifyList->setCurrentRow( gui->NotifyList->count() - 1 );
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::removeNotify()
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call NotifyWindow::removeNotify()";
#endif
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::selectNotify( int row )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call NotifyWindow::selectNotify( " << row << " )";
#endif
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::contextMenuNotify( QPoint Point )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call NotifyWindow::contextMenuNotify( QPoint(" << Point << ") )";
#endif

    QMenu NotifyContextMenu;

    NotifyContextMenu.addAction( tr("Добавить оповещение"), this, SLOT(addNotify()) );

    if( gui->NotifyList->count() > 1 )
        NotifyContextMenu.addAction( tr("Удалить оповещение" ), this, SLOT(removeNotify()) );

    NotifyContextMenu.exec( gui->NotifyList->mapToGlobal(Point) );
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::renameNotify( QListWidgetItem *Item )
{
#ifdef WT_INFO_CALL_FUNC
    qDebug() << "#Call NotifyWindow::renameNotify( QListWidgetItem* )";
#endif

    if( Item == NULL )
    {
        clear();
    }
    else
    {

    }
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::clear()
{
    gui->StartTimeNotifyEdit   ->setTime( QTime(0, 0, 0) );
    gui->IntervalTimeNotifyEdit->setTime( QTime(0, 0, 0) );
    gui->EndTimeNotifyEdit     ->setTime( QTime(0, 0, 0) );

    gui->CertainTimeNotifyEdit->setTime( QTime(0, 0, 0) );

    gui->DateNotifyEdit->setDate( HelperWT::currentDate() );

    gui->TypesTimeGBox   ->setEnabled( false );
    gui->DateNotifyGBox  ->setEnabled( false );
    gui->CertainTimeGBox ->setEnabled( false );
    gui->IntervalTimeGBox->setEnabled( false );
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::showEvent( QShowEvent *ShowEvent )
{
    gui->NotifyList->setCurrentRow( -1 );

    clear();

    QWidget::showEvent( ShowEvent );
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::configureGUI()
{
    gui->PeriodicityCBox->insertItem( NonePeriodicity      , tr("Не выбрано"                          ) );
    gui->PeriodicityCBox->insertItem( OnceOnDate           , tr("Один раз в определённый день"        ) );
    gui->PeriodicityCBox->insertItem( EveryMonthOnDate     , tr("Каждый месяца в определённый день"   ) );
    gui->PeriodicityCBox->insertItem( EveryMonthLastWorkDay, tr("Каждый последний рабочий день месяца") );

    gui->TypesTimeNotifyCBox->insertItem( NoneTypeNotify , tr("Не выбрано"          ) );
    gui->TypesTimeNotifyCBox->insertItem( CertainTime    , tr("В определенное время") );
    gui->TypesTimeNotifyCBox->insertItem( IntervalTime   , tr("В интервал времени"  ) );

    gui->NotifyList->setSelectionMode    ( QAbstractItemView::SingleSelection );
    gui->NotifyList->setContextMenuPolicy( Qt::CustomContextMenu              );
}
// ------------------------------------------------------------------------------------ //

void NotifyWindow::connectGUI()
{
    connect( gui->NotifyList, SIGNAL(currentRowChanged         (int             )), SLOT(selectNotify     (int             )) );
    connect( gui->NotifyList, SIGNAL(customContextMenuRequested(QPoint          )), SLOT(contextMenuNotify(QPoint          )) );
    connect( gui->NotifyList, SIGNAL(itemChanged               (QListWidgetItem*)), SLOT(renameNotify     (QListWidgetItem*)) );

    connect( gui->PeriodicityCBox    , SIGNAL(currentIndexChanged(int)), SLOT(selectPeriodocity(int)) );
    connect( gui->TypesTimeNotifyCBox, SIGNAL(currentIndexChanged(int)), SLOT(selectTypeTime   (int)) );
}
// ------------------------------------------------------------------------------------ //

