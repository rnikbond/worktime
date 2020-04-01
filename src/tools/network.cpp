// ------------------------------
#include "network.h"
// ------------------------------

Network::Network( QObject *parent ) : QObject( parent )
{
	NetProcess = new QProcess( this );
	NetProcess->setProcessChannelMode(QProcess::MergedChannels);

	connect(NetProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
}
// ------------------------------------------------------------

void Network::run( QString IP )
{
	ping( IP );
}
// ------------------------------------------------------------

void Network::ping( QString IP )
{
	if( IP.isEmpty() )
		return;

	QStringList arguments;

    arguments << "-n" << "1" << IP;

	NetProcess->setObjectName("ping");
    NetProcess->start( "ping", arguments );
}
// ------------------------------------------------------------

void Network::finished(int, QProcess::ExitStatus)
{
	bool isContainsTTL = QString(NetProcess->readAll()).indexOf("ttl", 0, Qt::CaseInsensitive) != -1;
	emit pingResult( isContainsTTL );
}
// ------------------------------------------------------------

