// ---------------------------- //
#include <QApplication>
// ---------------------------- //
#include "worktimewindow.h"
// ---------------------------- //

int main( int argc, char * argv[] )
{
    QApplication App( argc, argv );

    WorkTimeWindow * WorkTime = new WorkTimeWindow();

    WorkTime->show();

    return App.exec();
}
// ------------------------------------------------------------------------------------ //
