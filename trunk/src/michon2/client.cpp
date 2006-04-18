#include <qsocket.h>

class c_client : {
public : {
    c_client(const QString &host, Q_UINT16 port)
    {
	socket = new QSocket(this);
	connect(socket, SIGNAL(connected()), SLOT(socketConnected()));
	connect(socket, SIGNAL(connectionClosed()), SLOT(socketConnectionClosed()));
	connect(socket, SIGNAL(readyRead()), SLOT(socketReadyRead()));
	connect(socket, SIGNAL(error(int)), SLOT(socketError(int)));
	
	MyDialog1->statusEdit->append("Trying to connect to the server\n")
	socket->connectedToHost(host, port);
    }
   
    ~c_client()
    {
	
    }    
}
private slots : {
    void closeConnection()
    {
	socket->close();
	if (socket->state() == QSocket::Closing)
	{
	    connect(socket, SIGNAL(delayedCloseFinished()), SLOT(socketClosed()))
	}
	else
	{
	    socketClosed();
	}
    }
    
    void sendToServer()
    {
	QTextStream os(socket);
	
	os << MyDialog1->theEdit->text() << "\n";
	MyDialog1->theEdit->setText("");
    }

    void socketReadyRead()
    {
        while (socket->canReadLine())
	{
	    MyDialog1->statusEdit->append(socket->readLine());
	}	
    }	

    void socketConnected()
    {
	MyDialog1->statusEdit->append("Connected to server\n");
    }

    void socketConnectionClosed()
    {
	MyDialog1->statusEdit->append("Connection closed by the server\n");
    }

    void socketError(int e)
    {
	char text[64];
	
	sprintf(text, "Error number %i occurred\n", e);	
	MyDialog1->statusEdit->append(text);
    }
    
    void socketClosed()
    {
	MyDialog1->statusEdit->append("Connection closed\n");
    }	
    
}
    
private :
{	
    QSocket *socket;
}	
};

