// ---------------------------- //
#ifndef CALCTIMEWINDOW_H
#define CALCTIMEWINDOW_H
// ---------------------------- //
#include <QWidget>
// ---------------------------- //
#include "wtimeext.h"
// ---------------------------- //
namespace Ui { class CalcTimeWindow; }
// ---------------------------- //

class TimeOperations
{
public:

    enum Operations
    {
        None ,
        Plus ,
        Minus
    };

    enum Positions
    {
        Hour  ,
        Minute,
        Second
    };

public:

    WTimeExt   Time     ;
    Operations Operation;
    Positions  Position ;

    TimeOperations()
    {
        clear();
    }

    void clear()
    {
        Time      = WTimeExt();
        Operation = None;
        Position  = Hour;

        Time.setRecheck( false );
    }

    void next()
    {
             if( Position == Hour   ) Position = Minute;
        else if( Position == Minute ) Position = Second;
    }

    void back()
    {
             if( Position == Second ) Position = Minute;
        else if( Position == Minute ) Position = Hour;
    }
};

class CalcTimeWindow : public QWidget
{
    Q_OBJECT

    QList<TimeOperations> TimeList;

public:

    explicit CalcTimeWindow( QWidget *parent = 0 );
    ~CalcTimeWindow();

private:

    Ui::CalcTimeWindow *gui;

    void updateView();

private slots:

    void clear();

    void plusOperation();
    void minusOperation();

    void nextPosition();
    void backPosition();

    void removeLast();

    void pressZero ();
    void pressOne  ();
    void pressTwo  ();
    void pressThree();
    void pressFour ();
    void pressFive ();
    void pressSix  ();
    void pressSeven();
    void pressEight();
    void pressNine ();

    void calculateTime();

    void setNumber( int number );

protected:

    void keyPressEvent( QKeyEvent *KeyEvent );
    void showEvent( QShowEvent *ShowEvent );
};

#endif // CALCTIMEWINDOW_H
