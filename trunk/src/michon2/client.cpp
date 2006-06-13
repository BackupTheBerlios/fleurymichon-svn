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
    QString s;
    char* nick;
    int i,j,k;
    
    while (socket->canReadLine())
    {
	s=socket->readLine();
	mydlg->statusEdit->append(s);
	if(!strncmp(s, "PING", 4))
	{
	    mydlg->statusEdit->append("-> PONG");
	    this->sendToServer("PONG " + QString(s.ascii() + 5) + "\r\n");
	}
	else
	{
	    //va jusqu'au premier espace, pour detecter les commandes JOIN et PART
	    i=0;
	    while(((s.ascii())[i])&&(s[i]!=' '))
	    {
		i++;
	    }
	    if((s.ascii())[i])
	    {
		if(!strncmp(s.ascii()+i+1, "JOIN", 4))
		{
		    i=1;
		    while(((s.ascii())[i])&&(s[i]!='!'))
		    {
			i++;
		    }
		    nick = (char *) malloc((i)*sizeof(char));
		    i--;
		    nick[i]=0;
		    k = i;
		    
		    for(;i;i--)
		    {
			nick[i-1]=s.ascii()[i];
		    }
		    if(strncmp(nick, (mydlg->lineEditNick->text()).ascii(), k) &&  strncmp(nick, (mydlg->textLabelNick->text()).ascii(),3))
			mydlg->userslist->insertItem(QString(nick),-1);
		    free(nick);
		}
		else
		{
		    if(!strncmp(s.ascii()+i+1, "PART", 4))
		    {
			i=1;
			while(((s.ascii())[i])&&(s[i]!='!'))
			{
			    i++;
			}
			nick = (char *) malloc((i)*sizeof(char));
			i--;
			nick[i]=0;			
			for(;i;i--)
			{
			    nick[i-1]=s.ascii()[i];
			}
			for(i=0;i<mydlg->userslist->count();i++)
			{
			    if(!strcmp(mydlg->userslist->text(i).ascii(),nick))
				break;
			}
			mydlg->userslist->removeItem(i);
			free(nick);
		    }
		    else
		    {
			if(!strncmp(s.ascii()+i+1, "353", 3))
			{
			    while(((s.ascii())[i])&&(s[i]!=':'))
			    {
				i++;
			    }
			    i++;
			    do
			    {
				j=i;
				while(((s.ascii())[i])&&(s[i]!=' ')&&(s[i]!='\r')&&(s[i]!='\n'))
				{
				    i++;
				}			    
				nick = (char *) malloc((i-j+1)*sizeof(char));
				k=i;
				nick[i-j]=0;
				i--;
				for(;i-j>=0;i--)
				{
				    nick[i-j]=s.ascii()[i];
				}
				mydlg->userslist->insertItem(QString(nick),-1);
				free(nick);
				i=k+1;
			    } while((s[i] != '\n') && (s[i] != '\r'));
			}
			else
			{
			    if(!strncmp(s.ascii()+i+1, "QUIT", 4))
			    {
				i=1;
				while(((s.ascii())[i]) && (s[i] != '!'))
				{
				    i++;
				}	
				nick = (char *) malloc((i)*sizeof(char));
				i--;
				nick[i]=0;			
				for ( ; i ; i--)
				{
				    nick[i-1]=s.ascii()[i];
				}
				for (i = 0 ; i < mydlg->userslist->count() ; i++)
				{
				    if(!strcmp(mydlg->userslist->text(i).ascii(), nick))
					break;
				}	
				mydlg->userslist->removeItem(i);
				free(nick);	
			    }
			    
			}
		    } 
		    
		}
	    }
	}
    }	
}	

void c_client::socketConnected()
{
    mydlg->statusEdit->append("Michon: Connected to server\n");
}

void c_client::socketConnectionClosed()
{
    mydlg->statusEdit->append("Michon: Connection reset by server\n");
    mydlg->ConnectButton->setEnabled(true);
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

