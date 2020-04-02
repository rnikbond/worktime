// ---------------------------- //
#ifndef COREWORKTIME_H
#define COREWORKTIME_H
// ---------------------------- //
#include <QLabel>
#include <QMovie>
#include <QObject>
// ---------------------------- //
#include "modelworktime.h"
#include "worktimewindow.h"
#include "presenterworktime.h"
// ---------------------------- //

class CoreWorkTime : public QObject
{
    Q_OBJECT

    QLabel * WaitLabel;
    QMovie * WaitMovie;

private:

    ModelWorkTime     * ModelWT;
    WorkTimeWindow    * WorkTime;
    PresenterWorkTime * PresenterWT;

public:

    explicit CoreWorkTime( QObject * parent = 0 );
    ~CoreWorkTime();

    void start();

private:

    void removeOld();
    bool isAlreadyRunning();

private slots:

    void wait(bool isWait );

    // Slots from settings
    void changedWorkingRate( int );
};

#endif // COREWORKTIME_H
