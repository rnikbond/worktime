// ---------------------------- //
#ifndef NOTIFYWINDOW_H
#define NOTIFYWINDOW_H
// ---------------------------- //
#include <QWidget>
#include <QListWidgetItem>
// ---------------------------- //
namespace Ui { class NotifyWindow; }
// ---------------------------- //

class NotifyWindow : public QWidget
{
    Q_OBJECT

    enum PeriodicityNotify
    {
        NonePeriodicity ,
        OnceOnDate      ,
        EveryMonthOnDate,
        EveryMonthLastWorkDay
    };

    enum TypesNotify
    {
        NoneTypeNotify,
        CertainTime   ,
        IntervalTime
    };

public:

    explicit NotifyWindow( QWidget *parent = 0 );
    ~NotifyWindow();

    void clear();

private:

    Ui::NotifyWindow *gui;

    void configureGUI();
    void connectGUI();

protected:

    void showEvent( QShowEvent *ShowEvent );

private slots:

    void selectNotify     ( int row               );
    void contextMenuNotify( QPoint Point          );
    void renameNotify     ( QListWidgetItem *Item );

    void addNotify   ();
    void removeNotify();

    void selectPeriodocity( int selectedIndex );
    void selectTypeTime  ( int selectedIndex );
};

#endif // NOTIFYWINDOW_H
