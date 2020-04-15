// ---------------------------- //
#ifndef DESKTOPWIDGET_H
#define DESKTOPWIDGET_H
// ---------------------------- //
#include <QWidget>
// ---------------------------- //
#include "idesktopwidget.h"
// ---------------------------- //
namespace Ui { class DesktopWidget; }
// ---------------------------- //

class DesktopWidget : public QWidget, public IDesktopWidget
{
    Q_OBJECT

    bool isShowWorkTime;
    QPoint OldPos;

public:

    explicit DesktopWidget( QWidget * parent = 0 );
    ~DesktopWidget();

private:

    Ui::DesktopWidget * gui;

public: // From Interface

    void setReverseInfo( QString info );

protected:

    void mousePressEvent  ( QMouseEvent * MouseEvent );
    void mouseReleaseEvent( QMouseEvent * MouseEvent );
    void mouseMoveEvent   ( QMouseEvent * MouseEvent );

signals:

    void switchVisible();
    void changedPosition(QRect);
};

#endif // DESKTOPWIDGET_H
