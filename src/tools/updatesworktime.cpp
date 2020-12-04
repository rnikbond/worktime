// ------------------------------
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QApplication>
#include <QDateTime>
// ------------------------------
#include "updatesworktime.h"
// ------------------------------

UpdatesWorkTime::UpdatesWorkTime( QObject *parent ) : QObject( parent )
{
    UpdateTimer = new QTimer( this );

    connect( UpdateTimer, SIGNAL(timeout()), SLOT(checkUpdates()) );
}
// ------------------------------------------------------------------------------------ //

void UpdatesWorkTime::setUpdatePath( QString pathUpdate )
{
    path = pathUpdate;

    checkUpdates();

    UpdateTimer->start( 1000 );
}
// ------------------------------------------------------------------------------------ //

void UpdatesWorkTime::checkUpdates()
{
    if( QFile(path).exists() )
    {
        if( QFileInfo(path).lastModified() > QFileInfo(QApplication::applicationFilePath()).lastModified() )
            emit foundUpdate();
    }
}
// ------------------------------------------------------------------------------------ //
