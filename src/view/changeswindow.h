// ---------------------------- //
#ifndef CHANGESWINDOW_H
#define CHANGESWINDOW_H
// ---------------------------- //
#include <QWidget>
// ---------------------------- //
namespace Ui { class ChangesWindow; }
// ---------------------------- //

class ChangesWindow : public QWidget
{
    Q_OBJECT

public:

    explicit ChangesWindow( QWidget * parent = 0 );
    ~ChangesWindow();

private:

    Ui::ChangesWindow * gui;

    void setInfo();
};

#endif // CHANGESWINDOW_H
