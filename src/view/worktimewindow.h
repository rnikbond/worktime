// ---------------------------- //
#ifndef WORKTIMEWINDOW_H
#define WORKTIMEWINDOW_H
// ---------------------------- //
#include <QWidget>
// ---------------------------- //
#include "iworktime.h"
// ---------------------------- //
namespace Ui { class WorkTimeWindow; }
// ---------------------------- //

class WorkTimeWindow : public QWidget, public IWorkTime
{
    Q_OBJECT

public:

    explicit WorkTimeWindow( QWidget * parent = 0 );
    ~WorkTimeWindow();

private:

    void configuringGUI();

    Ui::WorkTimeWindow * gui;

signals:

    void selectDate( const QDate & date );
};
// ------------------------------------------------------------------------------------ //

#endif // WORKTIMEWINDOW_H
