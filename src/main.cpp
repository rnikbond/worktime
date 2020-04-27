// ---------------------------- //
#include <QMessageBox>
#include <QApplication>
// ---------------------------- //
#include "coreworktime.h"
// ---------------------------- //

int main( int argc, char * argv[] )
{
    QApplication App( argc, argv );

    QSharedMemory SharedWorkTime ("work-time-15041996-c333de4444f5");

    if( !SharedWorkTime.create(512, QSharedMemory::ReadWrite) )
    {
        QMessageBox MsgBox( QMessageBox::Critical, "", QObject::tr("Программа уже запущена") );
        MsgBox.exec();

        exit( 0 );
    }

    SharedWorkTime.attach( QSharedMemory::ReadOnly );

    CoreWorkTime CoreWT;
    CoreWT.start();

    return App.exec();
}
// ------------------------------------------------------------------------------------ //
