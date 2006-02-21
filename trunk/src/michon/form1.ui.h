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
    struct commande *cmd;
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
	    while(s[i])
	    {
		i++;
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
	    afferreur(execcmd(cmd));	  
	    /*liberation*/
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
    /*WSADATA WSAData;
    WSAStartup(MAKEWORD(2,0), &WSAData);
    SOCKET sock;
    SOCKADDR_IN sin;*/
 
    cmd=(struct commande *)cmd1;
    if(strcmp(cmd->com,"connect"))
    {
	if(cmd->args==NULL)
	    //dlgmain::printchat("erreur",-1);
	    result=-1;
	else
	{
	    /*sock = socket(AF_INET, SOCK_STREAM, 0);
	    sin.sin_family = AF_INET;
	    sin.sin_addr.s_addr = inet_addr(cmd->args);*/
	    /* connection a cmd->args:serveur */
	    if(cmd->args->next==NULL)
		//sin.sin_port = htons(6667);
		5;
	    else
		5;
		//sin.sin_port = htons(strtoint(cmd->args->next->arg));
	    /*connection a cmd->args->arg:port*/
	    //connect(sock, (SOCKADDR *)&sin, sizeof(sin));
	}
 
    } 
 
    if(strcmp(cmd->com,"quit"))
    {
	5;
	/* envoie du message QUIT au serveur pas fait*/
	/*closesocket(sock);
	WSACleanup();*/
	/* fermeture du socket */
 }
    return result;
}
