// ---------------------------- //
#include <QDebug>
#include <QMouseEvent>
// ---------------------------- //
#include "desktopwidget.h"
#include "ui_desktopwidget.h"
// ---------------------------- //

DesktopWidget::DesktopWidget( QWidget * parent ) : QWidget( parent ), gui( new Ui::DesktopWidget )
{
    gui->setupUi( this );

    isShowWorkTime = false;
}
// ------------------------------------------------------------------------------------ //

DesktopWidget::~DesktopWidget()
{
    delete gui;
}
// ------------------------------------------------------------------------------------ //

void DesktopWidget::setReverseInfo( QString info )
{
    gui->ReverseTimeLabel->setText( info );
}
// ------------------------------------------------------------------------------------ //

void DesktopWidget::mousePressEvent( QMouseEvent * MouseEvent )
{
    OldPos = MouseEvent->globalPos();

    isShowWorkTime = true;

    QWidget::mousePressEvent( MouseEvent );
}
// ------------------------------------------------------------------------------------ //

void DesktopWidget::mouseReleaseEvent( QMouseEvent * MouseEvent )
{
    if( isShowWorkTime )
        emit switchVisible();
    else
        emit changedPosition( geometry() );

    QWidget::mouseReleaseEvent( MouseEvent );
}
// ------------------------------------------------------------------------------------ //

void DesktopWidget::mouseMoveEvent( QMouseEvent * MouseEvent )
{
    if( MouseEvent->buttons() & Qt::LeftButton )
    {
        QPoint Delta = MouseEvent->globalPos() - OldPos;

        move( x() + Delta.x(), y() + Delta.y() );

        OldPos = MouseEvent->globalPos();

        isShowWorkTime = false;
    }

    QWidget::mouseMoveEvent( MouseEvent );
}
// ------------------------------------------------------------------------------------ //
