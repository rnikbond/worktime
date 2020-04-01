// ---------------------------- //
#ifndef COREWORKTIME_H
#define COREWORKTIME_H
// ---------------------------- //
#include <QObject>
// ---------------------------- //
#include "worktimewindow.h"
#include "presenterworktime.h"
// ---------------------------- //

class CoreWorkTime : public QObject
{
    Q_OBJECT

    WorkTimeWindow    * WorkTime;
    PresenterWorkTime * PresenterWT;

public:

    explicit CoreWorkTime( QObject * parent = 0 );
    ~CoreWorkTime();

    void start();

private:

    void removeOld();

    bool isAlreadyRunning();
};

#endif // COREWORKTIME_H
