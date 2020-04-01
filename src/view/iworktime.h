// ---------------------------- //
#ifndef IWORKTIME
#define IWORKTIME
// ---------------------------- //
class QDate      ;
class QColor     ;
class QString    ;
class QStringList;
// ---------------------------- //

class IWorkTime
{

public: // Members



public: // Signals

    virtual void selectDate( const QDate & date ) = 0;

};

#endif // IWORKTIME

