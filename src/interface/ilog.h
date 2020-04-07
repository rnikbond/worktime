// ---------------------------- //
#ifndef ILOG
#define ILOG
// ---------------------------- //
#include <QString>
// ---------------------------- //

class ILog
{

public: // Signals

    virtual void infoLog ( const QString & ) = 0;
    virtual void errorLog( const QString & ) = 0;
};
#endif // ILOG

