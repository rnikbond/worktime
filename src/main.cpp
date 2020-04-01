// ---------------------------- //
#include <QApplication>
// ---------------------------- //
#include "coreworktime.h"
// ---------------------------- //

int main( int argc, char * argv[] )
{
    QApplication App( argc, argv );

    CoreWorkTime CoreWT;
    CoreWT.start();

    return App.exec();
}
// ------------------------------------------------------------------------------------ //
