// ---------------------------- //
#ifndef DESKTOPWIDGET_H
#define DESKTOPWIDGET_H
// ---------------------------- //
#include <QTimer>
#include <QWidget>
// ---------------------------- //
#include "idesktopwidget.h"
// ---------------------------- //
namespace Ui { class DesktopWidget; }
// ---------------------------- //

class DesktopWidget : public QWidget, public IDesktopWidget
{
    Q_OBJECT

    enum AngleTypes
    {
        AngleNone        = 0x0000,
        AngleLeftTop     = 0x0001,
        AngleLeftBottom  = 0x0002,
        AngleRightTop    = 0x0004,
        AngleRightBottom = 0x0008,
    };

    int AngleType;

    bool isShowWorkTime;
    QPoint OldPos;

    QString StyleText;

    QTimer *BlinkTimer;

public:

    explicit DesktopWidget( QWidget * parent = 0 );
    ~DesktopWidget();

    void blink();

private:

    Ui::DesktopWidget * gui;

    void updateAngleStyle();

public: // From Interface

    void setOpacityBackground( float value );
    void setOpacityText( float value );
    void setReverseInfo( QString info );

protected:

    void mousePressEvent  ( QMouseEvent * MouseEvent );
    void mouseReleaseEvent( QMouseEvent * MouseEvent );
    void mouseMoveEvent   ( QMouseEvent * MouseEvent );

    void moveEvent(QMoveEvent * event);


private slots:

    void blickTimeOut();

signals:

    void switchVisible();
    void changedPosition(QRect);
};

#endif // DESKTOPWIDGET_H
