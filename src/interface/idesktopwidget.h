// ---------------------------- //
#ifndef IDESKTOPWIDGET
#define IDESKTOPWIDGET
// ---------------------------- //
#include <QRect>
// ---------------------------- //
#include "wtime.h"
#include "wtimeext.h"
// ---------------------------- //

class IDesktopWidget
{

public:

    virtual void setReverseInfo( QString info ) = 0;

    virtual void setOpacityBackground( float value ) = 0;
    virtual void setOpacityText( float value ) = 0;

public: // Signals

    virtual void switchVisible() = 0;
    virtual void changedPosition( QRect ) = 0;
};

#endif // IDESKTOPWIDGET

