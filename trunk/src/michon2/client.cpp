#include "client.h"

c_client::c_client(const QString &host, Q_UINT16 port, MyDialog1 *mdlg)
{
  socket = new QSocket(this);
  connect(socket, SIGNAL(connected()), SLOT(socketConnected()));
  connect(socket, SIGNAL(connectionClosed()), SLOT(socketConnectionClosed()));
  connect(socket, SIGNAL(readyRead()), SLOT(socketReadyRead()));
  connect(socket, SIGNAL(error(int)), SLOT(socketError(int)));
  
  mydlg = mdlg;
  // mydlg->statusEdit->append("Michon: Trying to connect to the server\n");
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

void c_client::sendToServer(QString s)
{
  QTextStream os(socket);
  
  os << s << "\n";
  
  /* os << mydlg->theEdit->text() << "\n";
  mydlg->theEdit->setText(""); */
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
    mydlg->statusEdit->append("Michon: Connected to server\n");
}

void c_client::socketConnectionClosed()
{
    mydlg->statusEdit->append("Michon: Connection reset by server\n");
}

void c_client::socketError(int e)
{
  char text[64];
  
  switch (e)  {
  case 0:
      sprintf(text, "Michon socket error: Service unavailable on host\n");
      break;
  case 1:
      sprintf(text, "Michon socket error: Host not reachable\n");
      break;
  default:
      sprintf(text, "Michon socket error: %i\n", e);
  }
  
  mydlg->statusEdit->append(text);
}

void c_client::socketClosed()
{
    mydlg->statusEdit->append("Michon: Connection closed\n");
}	

