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
#include "settingswindow.h"
#include "presenterworktime.h"
// ---------------------------- //

class CoreWorkTime : public QObject
{
    Q_OBJECT

    int workingRate;

    QLabel * WaitLabel;
    QMovie * WaitMovie;

    ModelWorkTime     * ModelWT;
    WorkTimeWindow    * WorkTime;
    PresenterWorkTime * PresenterWT;

    SettingsWindow    * Settings;

public:

    explicit CoreWorkTime( QObject * parent = 0 );
    ~CoreWorkTime();

    void start();

private:

    void removeOld();
    bool isAlreadyRunning();

    void readConfig();
    void writeConfig();

    void showWindow( QWidget * Window, bool isCenterDisplay );

private slots:

    void wait(bool isWait );

    // Slots from settings
    void changedWorkingRate( int );

    void showSettings();
};

#endif // COREWORKTIME_H
