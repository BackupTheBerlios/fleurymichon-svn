#include "decomparg.h"

struct commande * decompcom(const char* s)
{
 int i,j;
 struct commande *cmd=NULL;
 struct arguments **argu;
 
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
     *argu=(struct arguments *)malloc(sizeof(struct arguments));
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
 return cmd;
}

void freecmd(struct commande *cmd)
{
    struct arguments **argu,*tmp;
     
    argu=&(cmd->args);
    while((*argu)!=NULL)
    {
	tmp=*argu;
	argu=&((*argu)->next);
	free(tmp);
    }
    free(cmd);
}
