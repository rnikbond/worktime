// ---------------------------- //
#ifndef PRESENTERWORKTIME_H
#define PRESENTERWORKTIME_H
// ---------------------------- //
#include <QObject>
// ---------------------------- //
class IWorkTime;
// ---------------------------- //

class PresenterWorkTime : public QObject
{
    Q_OBJECT

    IWorkTime * ViewWT;

public:

    explicit PresenterWorkTime( IWorkTime * View, QObject * parent = 0 );

private slots:

    // Slots for View
    void connectView( IWorkTime * View );

    void userSelectDate( const QDate & date );
};

#endif // PRESENTERWORKTIME_H
