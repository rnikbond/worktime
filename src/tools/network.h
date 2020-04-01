// ------------------------------
#ifndef NETWORK_H
#define NETWORK_H
// ------------------------------
#include <QObject>
#include <QProcess>
// ------------------------------

class Network : public QObject
{
	Q_OBJECT

    QProcess * NetProcess;

public:

	explicit Network(QObject *parent = 0);

	void run( QString IP );

private:

	void ping( QString IP );

private slots:

	void finished(int, QProcess::ExitStatus);

signals:

	void pingResult( bool );
};

#endif // NETWORK_H
