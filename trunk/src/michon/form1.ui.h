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


QSocket *socket=new QSocket(); //socket du serveur

long strtoint(char *s)
{
    int i;
    long l;
    
    i=0;
    l=0;
    while(s[i])
    {
	l=l*10+(long)(s[i]-'0');
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
	    execcmd(cmd);
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

int dlgmain::execcmd( void  * cmd1 )
{
    struct commande *cmd;
    int result=0;
    char message[510];
 
    message[0]=0;
    cmd=(struct commande *)cmd1;
    if(!strcmp(cmd->com,"connect"))
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
		socket->connectToHost(cmd->args->arg,6667);
	    else
		socket->connectToHost(cmd->args->arg,strtoint(cmd->args->next->arg));		
	    /*connection a cmd->args->arg:port*/
	}
 
    } 
 
    if(strcmp(cmd->com,"quit"))
    {
	/* envoie du message QUIT au serveur pas fait*/
	socket->clearPendingData();
	socket->close();
 }
    return result;
    return 0;
}
