#include "client.h"

c_client::c_client(const QString &host, Q_UINT16 port, MyDialog1 *mdlg)
{
  socket = new QSocket(this);
  connect(socket, SIGNAL(connected()), SLOT(socketConnected()));
  connect(socket, SIGNAL(connectionClosed()), SLOT(socketConnectionClosed()));
  connect(socket, SIGNAL(readyRead()), SLOT(socketReadyRead()));
  connect(socket, SIGNAL(error(int)), SLOT(socketError(int)));
  
  mydlg = mdlg;
  mydlg->statusEdit->append("Trying to connect to the server\n");
  socket->connectToHost(host, port);
}

c_client::~c_client()
    {
      
    }    

void c_client::closeConnection()
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

void c_client::sendToServer()
{
  QTextStream os(socket);
  
  os << mydlg->theEdit->text() << "\n";
  mydlg->theEdit->setText("");
}

void c_client::socketReadyRead()
{
  while (socket->canReadLine())
    {
      mydlg->statusEdit->append(socket->readLine());
    }	
}	

void c_client::socketConnected()
{
  mydlg->statusEdit->append("Connected to server\n");
}

void c_client::socketConnectionClosed()
{
  mydlg->statusEdit->append("Connection closed by the server\n");
}

void c_client::socketError(int e)
{
  char text[64];
  
  sprintf(text, "Error number %i occurred\n", e);	
  mydlg->statusEdit->append(text);
}

void c_client::socketClosed()
{
  mydlg->statusEdit->append("Connection closed\n");
}	

