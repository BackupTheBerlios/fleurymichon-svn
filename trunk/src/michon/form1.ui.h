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

void fprnt(FILE *f,const char* s)
{
   int i;
   
   i=0;
   while(s[i])
       fputc(s[i++],f);
   fputc('\n',f);
}

const char* qstrtostr(const char *s)
{
    return s;
}
	
void dlgmain::pbsendclick()
{
    struct commande *cmd;
    int i,j;
    const char *s;
    starg **argu,*tmp;
    
    s=qstrtostr(lemess->text());
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
		while((s[i])&&(s[i]!=' '))
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
	majchat();
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


void dlgmain::majchat()
{
    fprnt(logf,(lemess->text()));
    tbchat->append(lemess->text());    
}
