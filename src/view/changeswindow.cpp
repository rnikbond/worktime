// ---------------------------- //
#include "coreworktime.h"
// ---------------------------- //
#include "changeswindow.h"
#include "ui_changeswindow.h"
// ---------------------------- //

ChangesWindow::ChangesWindow( QWidget * parent ) : QWidget( parent ), gui( new Ui::ChangesWindow )
{
    gui->setupUi( this );

    setInfo();
}
// ------------------------------------------------------------------------------------ //

ChangesWindow::~ChangesWindow()
{
    delete gui;
}
// ------------------------------------------------------------------------------------ //

void ChangesWindow::setInfo()
{
    gui->TitleLabel->setText( tr("Текущая версия: %1.%2.%3")
                              .arg(VERSION_MAJOR   )
                              .arg(VERSION_MINOR   )
                              .arg(VERSION_SUBMINOR) );

    QString Info;

    Info += "<center><b>Добро пожаловать!</b></center><br/>" \
            "На этом месте могла бы быть Ваша реклама...<br/><br/>";

    gui->ChangesInfo->setText( Info );

}
// ------------------------------------------------------------------------------------ //
