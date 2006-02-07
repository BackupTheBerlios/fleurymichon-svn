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


void dlgmain::pbsendclick()
{
   if( lemess->text()[0]!=0 )
    {
       fprintf(logf,"%s\n",(lemess->text())::assci);
       tbchat->append(lemess->text());
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


/*void dlgmain::majchat()
{
    int i=0,j=0,c;
    char *s;
    
    tbchat->clear();
    if(!(s=(char*)malloc(512*sizeof(char))))
	printf("erreur d'allocation");
    fseek(logf,0,SEEK_SET);
    c=fgetc(logf);
    while(c!=EOF)
    {
	if(((unsigned char) c)!=10)
	{
	    s[j]=(unsigned char) c;
	    j++;
	}
	else
	{
	    i++;
	    s[j]=0;
	    j=0;
	    tbchat->insertParagraph(s,i);
	}
    }
    tbchat->append(lemess->text());
}*/
