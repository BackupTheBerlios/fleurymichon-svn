#include "moker.h"
#include <qsocket.h>
#include <qtextedit.h>
#include <qlineedit.h>

class c_client : public QObject
{
private :
	
    QSocket *socket;
    MyDialog1 *mydlg;
	
public : 
    c_client(const QString &host, Q_UINT16 port)
    {
	socket = new QSocket(this);
	connect(socket, SIGNAL(connected()), SLOT(socketConnected()));
	connect(socket, SIGNAL(connectionClosed()), SLOT(socketConnectionClosed()));
	connect(socket, SIGNAL(readyRead()), SLOT(socketReadyRead()));
	connect(socket, SIGNAL(error(int)), SLOT(socketError(int)));
	
	mydlg->statusEdit->append("Trying to connect to the server\n");
	socket->connectToHost(host, port);
    }
   
    ~c_client()
    {
	
    }    

private slots : 
    void closeConnection()
    {
	socket->close();
	if (socket->state() == QSocket::Closing)
	{
	  connect(socket, SIGNAL(delayedCloseFinished()), SLOT(socketClosed()));
	}
	else
	{
	    socketClosed();
	}
    }
    
    void sendToServer()
    {
	QTextStream os(socket);
	
	os << mydlg->theEdit->text() << "\n";
	mydlg->theEdit->setText("");
    }

    void socketReadyRead()
    {
        while (socket->canReadLine())
	{
	    mydlg->statusEdit->append(socket->readLine());
	}	
    }	

    void socketConnected()
    {
	mydlg->statusEdit->append("Connected to server\n");
    }

    void socketConnectionClosed()
    {
	mydlg->statusEdit->append("Connection closed by the server\n");
    }

    void socketError(int e)
    {
	char text[64];
	
	sprintf(text, "Error number %i occurred\n", e);	
	mydlg->statusEdit->append(text);
    }
    
    void socketClosed()
    {
	mydlg->statusEdit->append("Connection closed\n");
    }	
    

    
};

