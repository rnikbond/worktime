// ---------------------------- //
#include <QDebug>
#include <QPixmap>
#include <QBitmap>
#include <QMouseEvent>
#include <QDesktopWidget>
// ---------------------------- //
#include "desktopwidget.h"
#include "ui_desktopwidget.h"
// ---------------------------- //

DesktopWidget::DesktopWidget( QWidget * parent ) : QWidget( parent ), gui( new Ui::DesktopWidget )
{
    gui->setupUi( this );

    AngleType = AngleNone;

    isShowWorkTime = false;

    BlinkTimer = new QTimer( this );

    connect( BlinkTimer, SIGNAL(timeout()), SLOT(blickTimeOut()) );
}
// ------------------------------------------------------------------------------------ //

void DesktopWidget::updateAngleStyle()
{
    if( AngleType & AngleLeftTop )
    {
        QPixmap Pixmap(":/mask/desktop-widget/top-left.png");
        setMask( Pixmap.scaled(size()).mask() );
    }
    else if( AngleType & AngleLeftBottom )
    {
        QPixmap Pixmap(":/mask/desktop-widget/bottom-left.png");
        setMask( Pixmap.scaled(size()).mask() );
    }
    else if( AngleType & AngleRightTop )
    {
        QPixmap Pixmap(":/mask/desktop-widget/top-right.png");
        setMask( Pixmap.scaled(size()).mask() );
    }
    else if( AngleType & AngleRightBottom )
    {
        QPixmap Pixmap(":/mask/desktop-widget/bottom-right.png");
        setMask( Pixmap.scaled(size()).mask() );
    }
    else
    {
        QPixmap Pixmap(":/mask/desktop-widget/all-angles.png");
        setMask( Pixmap.mask() );
    }
}
// ------------------------------------------------------------------------------------ //

void DesktopWidget::blink()
{
    StyleText = styleSheet();

    BlinkTimer->start( 100 );
}
// ------------------------------------------------------------------------------------ //

DesktopWidget::~DesktopWidget()
{
    delete gui;
}
// ------------------------------------------------------------------------------------ //

void DesktopWidget::blickTimeOut()
{
    static int countBlink = 0;

    if( countBlink == 10 )
    {
        BlinkTimer->stop();

        countBlink = 0;
        setStyleSheet( StyleText );

        emit changedPosition( geometry() );
    }
    else
    {
        if( countBlink % 2 == 0 )
            setStyleSheet( "background-color: #008080; color: #B22222;" );
        else
            setStyleSheet( StyleText );

        countBlink++;
    }
}
// ------------------------------------------------------------------------------------ //

void DesktopWidget::setOpacityBackground( float value )
{
    setWindowOpacity( value );
}
// ------------------------------------------------------------------------------------ //

void DesktopWidget::setOpacityText( float value )
{
    gui->ReverseTimer->setWindowOpacity( value );
}
// ------------------------------------------------------------------------------------ //

void DesktopWidget::setReverseInfo( QString info )
{
    gui->ReverseTimer->setText( info );
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

void DesktopWidget::moveEvent( QMoveEvent * event )
{
    AngleType = AngleNone;

    QRect screenRect = qApp->desktop()->geometry();

    QRect leftTop    ( screenRect.x()        , screenRect.y()         , screenRect.width() / 2, screenRect.height() / 2 );
    QRect leftBottom ( screenRect.x()        , screenRect.height() / 2, screenRect.width() / 2, screenRect.height()     );
    QRect rightTop   ( screenRect.width() / 2, screenRect.y()         , screenRect.width()    , screenRect.height() / 2 );
    QRect rightBottom( screenRect.width() / 2, screenRect.height() / 2, screenRect.width()    , screenRect.height()     );

    if( leftTop    .contains(geometry()) ) AngleType |= AngleLeftTop    ;
    if( leftBottom .contains(geometry()) ) AngleType |= AngleLeftBottom ;
    if( rightTop   .contains(geometry()) ) AngleType |= AngleRightTop   ;
    if( rightBottom.contains(geometry()) ) AngleType |= AngleRightBottom;

    updateAngleStyle();

    QWidget::moveEvent( event );
}
// ------------------------------------------------------------------------------------ //
