// ---------------------------- //
#include <QDebug>
#include <QKeyEvent>
// ---------------------------- //
// ---------------------------- //
#include "calctimewindow.h"
#include "ui_calctimewindow.h"
// ---------------------------- //

CalcTimeWindow::CalcTimeWindow( QWidget *parent ) : QWidget( parent ), gui( new Ui::CalcTimeWindow )
{
    gui->setupUi( this );

    connect( gui->ZeroButton , SIGNAL(clicked(bool)), SLOT(pressZero ()) );
    connect( gui->OneButton  , SIGNAL(clicked(bool)), SLOT(pressOne  ()) );
    connect( gui->TwoButton  , SIGNAL(clicked(bool)), SLOT(pressTwo  ()) );
    connect( gui->ThreeButton, SIGNAL(clicked(bool)), SLOT(pressThree()) );
    connect( gui->FourButton , SIGNAL(clicked(bool)), SLOT(pressFour ()) );
    connect( gui->FiveButton , SIGNAL(clicked(bool)), SLOT(pressFive ()) );
    connect( gui->SixButton  , SIGNAL(clicked(bool)), SLOT(pressSix  ()) );
    connect( gui->SevenButton, SIGNAL(clicked(bool)), SLOT(pressSeven()) );
    connect( gui->EightButton, SIGNAL(clicked(bool)), SLOT(pressEight()) );
    connect( gui->NineButton , SIGNAL(clicked(bool)), SLOT(pressNine ()) );

    connect( gui->ResetButton , SIGNAL(clicked(bool)), SLOT(clear         ()) );
    connect( gui->MinusButton , SIGNAL(clicked(bool)), SLOT(minusOperation()) );
    connect( gui->PlusButton  , SIGNAL(clicked(bool)), SLOT(plusOperation ()) );
    connect( gui->EqualButton , SIGNAL(clicked(bool)), SLOT(calculateTime ()) );
    connect( gui->LeftButton  , SIGNAL(clicked(bool)), SLOT(backPosition  ()) );
    connect( gui->RightButton , SIGNAL(clicked(bool)), SLOT(nextPosition  ()) );
    connect( gui->DeleteButton, SIGNAL(clicked(bool)), SLOT(removeLast    ()) );

}
// ------------------------------------------------------------------------------------ //

CalcTimeWindow::~CalcTimeWindow()
{
    delete gui;
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::clear()
{
    gui->InfoLabel->clear();

    TimeList.clear();

    TimeOperations Time;

    TimeList.append( Time );

    updateView();
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::updateView()
{
    QString ViewString;

    for( int i = 0; i < TimeList.count() - 1; i++ )
    {
        ViewString += TimeList.at(i).Time.toString();

        if( TimeList.at(i).Operation == TimeOperations::None )
        {
            break;
        }
        else if( TimeList.at(i).Operation == TimeOperations::Plus )
        {
            ViewString += " + ";
        }
        else if( TimeList.at(i).Operation == TimeOperations::Minus )
        {
            ViewString += " - ";
        }
    }

    WTimeExt TimeFocus = TimeList.last().Time;

    if( TimeList.last().Position == TimeOperations::Hour )
    {
        ViewString += "<b>" + QString::number(TimeFocus.hours  ()).rightJustified(2, '0') + "</b>" + ":" +
                              QString::number(TimeFocus.minutes()).rightJustified(2, '0') + ":" +
                              QString::number(TimeFocus.seconds()).rightJustified(2, '0');
    }
    else if( TimeList.last().Position == TimeOperations::Minute )
    {
        ViewString +=         QString::number(TimeFocus.hours  ()).rightJustified(2, '0') +  ":" +
                      "<b>" + QString::number(TimeFocus.minutes()).rightJustified(2, '0') + "</b>" + ":" +
                              QString::number(TimeFocus.seconds()).rightJustified(2, '0');
    }
    else if( TimeList.last().Position == TimeOperations::Second )
    {
        ViewString +=         QString::number(TimeFocus.hours  ()).rightJustified(2, '0') + ":" +
                              QString::number(TimeFocus.minutes()).rightJustified(2, '0') + ":" +
                      "<b>" + QString::number(TimeFocus.seconds()).rightJustified(2, '0') + "</b>";
    }

    gui->DisplayEdit->setText( ViewString );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::plusOperation()
{
    if( TimeList.count() == 30 )
    {
        gui->InfoLabel->setText( tr("Достигнут максимум операций") );
        return;
    }
    else
    {
        gui->InfoLabel->clear();
    }

    TimeList.last().Operation = TimeOperations::Plus;

    TimeList.append( TimeOperations() );

    updateView();
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::minusOperation()
{
    if( TimeList.count() == 30 )
    {
        gui->InfoLabel->setText( tr("Достигнут максимум операций") );
        return;
    }
    else
    {
        gui->InfoLabel->clear();
    }

    TimeList.last().Operation = TimeOperations::Minus;

    TimeList.append( TimeOperations() );

    updateView();
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::removeLast()
{
    if( TimeList.last().Time.isEmpty() )
    {
        if( TimeList.count() > 1  )
        {
            TimeList.takeAt( TimeList.count() - 1 );
            gui->InfoLabel->clear();
        }

        TimeList.last().Operation = TimeOperations::None;
    }
    else
    {
        if( TimeList.last().Position == TimeOperations::Second )
        {
            QString Seconds = QString::number( TimeList.last().Time.seconds() );

            if( Seconds.length() > 0 )
                TimeList.last().Time.setSeconds( Seconds.remove( Seconds.length() - 1, 1 ).toInt() );
            else
                TimeList.last().Time.setSeconds( 0 );
        }
        else if( TimeList.last().Position == TimeOperations::Minute )
        {
            QString Minutes = QString::number( TimeList.last().Time.minutes() );

            if( Minutes.length() > 0 )
                TimeList.last().Time.setMinutes( Minutes.remove( Minutes.length() - 1, 1 ).toInt() );
            else
                TimeList.last().Time.setMinutes( 0 );
        }
        else if( TimeList.last().Position == TimeOperations::Hour )
        {
            QString Hours = QString::number( TimeList.last().Time.hours() );

            if( Hours.length() > 0 )
                TimeList.last().Time.setHour( Hours.remove( Hours.length() - 1, 1 ).toInt() );
            else
                TimeList.last().Time.setHour( 0 );
        }
    }

    updateView();
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::nextPosition()
{
    if( TimeList.count() > 0 )
    {
        TimeList.last().next();

        updateView();
    }
    else
    {
        qDebug() << "ERROR! CalcTimeWindow::nextPosition() = count == 0";
    }
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::backPosition()
{
    if( TimeList.count() > 0 )
    {
        TimeList.last().back();

        updateView();
    }
    else
    {
        qDebug() << "ERROR! CalcTimeWindow::nextPosition() = count == 0";
    }
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::pressZero()
{
    setNumber( 0 );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::pressOne()
{
    setNumber( 1 );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::pressTwo()
{
    setNumber( 2 );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::pressThree()
{
    setNumber( 3 );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::pressFour()
{
    setNumber( 4 );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::pressFive()
{
    setNumber( 5 );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::pressSix()
{
    setNumber( 6 );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::pressSeven()
{
    setNumber( 7 );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::pressEight()
{
    setNumber( 8 );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::pressNine()
{
    setNumber( 9 );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::calculateTime()
{
    gui->InfoLabel->clear();

    WTimeExt ResultTime = TimeList[0].Time;

    for( int i = 1; i < TimeList.count(); i++ )
    {
        if( TimeList.at(i - 1).Operation == TimeOperations::Plus )
        {
            ResultTime += TimeList.at(i).Time;
        }
        else if( TimeList.at(i - 1).Operation == TimeOperations::Minus )
        {
            ResultTime -= TimeList.at(i).Time;
        }
    }

    ResultTime.checkTime();

    TimeList.clear();

    TimeOperations TimeObj;

    TimeObj.Time = ResultTime;

    TimeList.append( TimeObj );

    updateView();
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::setNumber( int number )
{
    QString Time;

    if( TimeList.last().Position == TimeOperations::Hour )
    {
        Time = QString::number(TimeList.last().Time.hours()) + QString::number(number);

        TimeList.last().Time.setHour( Time.toInt() );
    }
    else if( TimeList.last().Position == TimeOperations::Minute )
    {
        Time = QString::number(TimeList.last().Time.minutes()) + QString::number(number);

        TimeList.last().Time.setMinutes( Time.toInt() );
    }
    else if( TimeList.last().Position == TimeOperations::Second )
    {
        Time = QString::number(TimeList.last().Time.seconds()) + QString::number(number);

        TimeList.last().Time.setSeconds( Time.toInt() );
    }

    if( Time.toInt() >= 10 )
        nextPosition();

    updateView();
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::keyPressEvent( QKeyEvent *KeyEvent )
{
    switch( KeyEvent->key() )
    {
        case Qt::Key_0: pressZero (); break;
        case Qt::Key_1: pressOne  (); break;
        case Qt::Key_2: pressTwo  (); break;
        case Qt::Key_3: pressThree(); break;
        case Qt::Key_4: pressFour (); break;
        case Qt::Key_5: pressFive (); break;
        case Qt::Key_6: pressSix  (); break;
        case Qt::Key_7: pressSeven(); break;
        case Qt::Key_8: pressEight(); break;
        case Qt::Key_9: pressNine (); break;

        case Qt::Key_Plus : plusOperation (); break;
        case Qt::Key_Minus: minusOperation(); break;

        case Qt::Key_Escape: clear(); break;

        case Qt::Key_Enter :
        case Qt::Key_Return:
        case Qt::Key_Equal :
            calculateTime();
        break;

        case Qt::Key_Right:
        case Qt::Key_Tab  :
            nextPosition();
        break;

        case Qt::Key_Left     : backPosition(); break;
        case Qt::Key_Backspace: removeLast  (); break;

        default:
        break;
    }


    QWidget::keyPressEvent( KeyEvent );
}
// ------------------------------------------------------------------------------------ //

void CalcTimeWindow::showEvent( QShowEvent *ShowEvent )
{
    clear();

    QWidget::showEvent( ShowEvent );
}
// ------------------------------------------------------------------------------------ //

