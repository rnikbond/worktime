// ------------------------------
#ifndef UPDATESWORKTIME_H
#define UPDATESWORKTIME_H
// ------------------------------
#include <QTimer>
#include <QObject>
// ------------------------------

class UpdatesWorkTime : public QObject
{
    Q_OBJECT

    QTimer *UpdateTimer;

    QString path;

public:

    UpdatesWorkTime( QObject *parent = 0 );

    void setUpdatePath( QString pathUpdate );

protected:

    void run();

private slots:

    void checkUpdates();

signals:

    void foundUpdate();
};

#endif // UPDATESWORKTIME_H
