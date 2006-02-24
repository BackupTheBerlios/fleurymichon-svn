/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <qsocket.h>
#include <iostream>
#include <qobject.h>
#include <qsocket.h>
#include <qsocketdevice.h>
#include <qtextstream.h>

#define CMDSMAX 32
#define starg struct arguments

struct arguments
{
    char arg[CMDSMAX];
    starg *next;
};

struct commande
{
    char com[CMDSMAX];
    int size;
    starg *args;
};


QSocket *socket; //socket du serveur

int strtoint(char *s)
{
    int i;
    long l;
    
    i=0;
    l=0;
    while(s[i])
    {
	l=l*10+(s[i]-'0');
	i++;
    }    
    return l;
    return 0;
}

void fprnt(FILE *f,const char* s)
{
   int i;
   
   i=0;
   while(s[i])
       fputc(s[i++],f);
   fputc('\n',f);
}

void dlgmain::pbsendclick()
{
    struct commande *cmd=NULL;
    int i,j;
    const char *s;
    starg **argu,*tmp;
    QString str,temp;
    
    temp = s;
    s=lemess->text();
    if( s[0]!=0 )
    {      
	if(s[0]=='/')
	{
	    /*saisie commande*/
	    i=1;	    
	    cmd=(commande *)malloc(sizeof(struct commande));
	    while((s[i]!=0)&&(s[i]!=' ')&&(i<=CMDSMAX))
	    {
		cmd->com[i-1]=s[i];	
		i++;
	    }
	    cmd->size=i-1;
	    cmd->com[i-1]=0;
	    /*saisie argument(s)*/
	    str=temp.mid(i-1,temp.length()-i-+1);
	    argu=&(cmd->args);
	    while(s[++i])
	    {
		*argu=(starg*)malloc(sizeof(starg));
		j=0;
		while((s[i])&&(s[i]!=' ')&&(j<CMDSMAX))
		{
		    (*argu)->arg[j]=s[i];
		    i++;
		    j++;
		}
		(*argu)->arg[j]=0;
		argu=&((*argu)->next);
	    }	
	    (*argu)=NULL;
	    /*execution*/
	    execcmd(cmd,str);
	    argu=&(cmd->args);
	    while((*argu)!=NULL)
	    {
		tmp=*argu;
		argu=&((*argu)->next);
		free(tmp);
	    }
	    free(cmd);
	}
	printchat(lemess->text(),0);
	lemess->clear();
    }
}

void dlgmain::init()
{   
    if(!(logf=fopen("log.txt","w")))
	printf("erreur, imposible de creer log.txt");
    logf=freopen("log.txt","a+",logf);
    //creation du socket et des signaux
    socket = new QSocket( this );
    QObject::connect(socket, SIGNAL(connected()),SLOT(slotConnected()) );
    QObject::connect(socket, SIGNAL(connectionClosed()),SLOT(slotConnectionClosed()) );
    QObject::connect(socket, SIGNAL(readyRead()),SLOT(slotReadyRead()) );
    QObject::connect(socket, SIGNAL(hostfound()),	SLOT(slothostfound()) );
    QObject::connect(socket, SIGNAL(error(int)),	SLOT(sloterror(int)) );
}

void dlgmain::slotconnected()
{
    printchat("connecté au serveur",2);
}

void dlgmain::slothostfound()
{
    printchat("serveur trouvé, en attente de connection",2);
}

void dlgmain::slotConnectionClosed()
{
    printchat("déconnecté du serveur",2);
}

void dlgmain::sloterror(int e)
{
    printchat("erreur",e);
}

void dlgmain::slotReadyRead()
{
    struct commande *cmd=NULL;
    int i,j;
    const char *s;
    starg **argu,*tmp;
     QString str,temp;
    
    temp = s;
    if (socket->canReadLine() )
    {
	cmd=(commande *)malloc(sizeof(struct commande));
	str = socket->readLine();
	/*saisie commande*/
	i=1;	    
	while((s[i]!=0)&&(s[i]!=' ')&&(i<=CMDSMAX))
	{
	    cmd->com[i-1]=s[i];	
	    i++;
	}
	cmd->size=i-1;
	cmd->com[i-1]=0;
	/*saisie argument(s)*/
	str=temp.mid(i-1,temp.length()-i-+1);
	argu=&(cmd->args);
	while(s[++i])
	{
	    *argu=(starg*)malloc(sizeof(starg));
	    j=0;
	    while((s[i])&&(s[i]!=' ')&&(j<CMDSMAX))
	    {
		(*argu)->arg[j]=s[i];
		i++;
		j++;
	    }
	    (*argu)->arg[j]=0;
	    argu=&((*argu)->next);
	}	
	(*argu)=NULL;
	 /*execution*/
	execcmd(cmd,str);
	argu=&(cmd->args);
	while((*argu)!=NULL)
	{
	    tmp=*argu;
	    argu=&((*argu)->next);
	    free(tmp);
	}
	free(cmd);
    }
}

void dlgmain::destroy()
{
    fclose(logf);
    socket->clearPendingData();
    socket->close();
}
    
void dlgmain::printchat(const char* s, long type)
{
    char* str;
    int l;
    l=0;
    while(s[l])
	l++;
    l+=30;
    str=(char*)malloc(l*sizeof(char));
    str[0]=0;
    strcat(str,"<font color=#");
    switch (type)
    {
    case -1 :{strcat(str,"ff0000");break;}
    default :{strcat(str,"0000ff");}
    }
    strcat(str,">");
    strcat(str,s);
    strcat(str,"</font>");
    fprnt(logf,s);
    tbchat->append(str);
    free(str);
}

void dlgmain::afferreur(int err)
{
    if (err)
	printchat("erreur",err);
}

int dlgmain::execcmd(void  * cmd1, QString str)
{
    struct commande *cmd;
    int result=0;
    int port;
    QString temp;
    QTextStream os(socket);
    cmd=(struct commande *)cmd1;
    if(cmd->com == "connect")
    {
	if(cmd->args==NULL)
	{
	    dlgmain::printchat("erreur pas de serveur specifie",-1);
	    result=-1;
	}
	else
	{
	    /* connection a cmd->args:serveur */
	    if(cmd->args->next==NULL)
		port  = 6667;
	    else
		port = strtoint(cmd->args->next->arg);
	    socket->connectToHost(cmd->args->arg,port);
	    /*connection a cmd->args->arg:port*/	    
	}
	return 0;
    } 
    
    if (cmd->com == "pass")
    {
	if (cmd->args == NULL)
	    os<< "PASS"<<"\n";
	else
	    os<< "PASS" <<cmd->args->arg<<"\n";
	return 0;
    }
    
    if (cmd->com == "nick")
    {
	if (cmd->args == NULL)
	{
	    printchat("veuillez indiquer un nick",-1);
	    result=-1;	    
	}
	else
	    os<< "NICK"<< cmd->args->arg<<"\n";
	return 0;
    }
    
    if (cmd->com == "user")
    {
	if (cmd->args == NULL)
	{
	    printchat("veuillez indiquer un username",-1);
	    result=-1;	    
	}
	else
	    os<< "USER"<< "guest"<<" * 0 :"<<str<<"\n";
	return 0;
    }
    
    if(cmd->com == "PING")
    {
	os<<"PONG"<<str<<"\n";
	return 0;
    }
 
    if(cmd->com == "quit")
    {
	os << "QUIT" << str << "\n";
	/* envoie du message QUIT au serveur pas fait*/
	/* fermeture du socket */
	destroy();
    }
    return result;
    return 0;
}
