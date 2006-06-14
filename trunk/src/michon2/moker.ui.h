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

#include "client.h"
#include "decomparg.h"

void MyDialog1::undoSlot()
{
    statusEdit->undo();
}

void MyDialog1::tSlot()
{
    //OpenChannel(lineEdit1->text());
}

void MyDialog1::OpenChannel(QString s)
{
    QHHBox *hb = new QHHBox();        
    QTextEdit *te = new QTextEdit(hb);
    QListBox *lb = new QListBox(hb);
    
    QRefChan *rc = new QRefChan();
    ((c_client *)michon)->lchan->append(rc);
    
    hb->setStretchFactor(te, 4);
    hb->setStretchFactor(lb, 1);  
    hb->setItems(te, lb);
    tabWidget->addTab(hb, s);  
    rc->setItems(te, lb, new QString(s), tabWidget->page(tabWidget->count() - 1));
    //connect(SIGNAL(textChanged(), MyDialog1, SLOT());
}

void MyDialog1::valSlot()
{    
    QRefChan *rc;     
    struct commande * cmd;
    const char *s;    
    int i;    
    int n;
    int p;
    
    cmd=decompcom(theEdit->text());
    
    if(!strncmp(theEdit->text(), "/", 1))
    {
	if (!strncmp(theEdit->text(), "/join ", 6))
	{
	    OpenChannel(theEdit->text().ascii() + 6);
	    ((c_client *)michon)->sendToServer("JOIN " + QString(theEdit->text().ascii() + 6) + "\r\n");
	}
	else
	{
	    if (!strncmp(theEdit->text().ascii(), "/connect ", 9))		
	    {
		ConnectMichon(theEdit->text().right(theEdit->text().length() - 9), 6667);	
	    }
	    else
	    {
		if(!strncmp(theEdit->text().ascii(), "/quit", 5))
		{
		    ((c_client *)michon)->sendToServer("QUIT :" + QString(theEdit->text().ascii() + 6) + "\r\n");
		    ((c_client *)michon)->closeConnection();
		     
		    for (i = ((c_client *)michon)->lchan->count() - 1; i >= 0; i--)
		    {
			tabWidget->removePage(((c_client *)michon)->lchan->at(i)->pt);	
			((c_client *)michon)->lchan->remove(i);
		    }
    
		    dellist(((c_client *)michon)->lchan);
		    ConnectButton->setEnabled(true);
		}
		else
		{
		    if(!strncmp(theEdit->text().ascii(), "/msg ", 5))
		    {
			s = theEdit->text().ascii();
			
			n = QString(s + 5).find(' ', 0);
			p = strlen(s);
			
			((c_client *)michon)->sendToServer("PRIVMSG " + QString(s + 5).left(n)+ " :" + QString(s + 6 + n).left(p)+ "\r\n"); 
			
			rc = findchan(new QString(QString(s + 5).left(n)), ((c_client *)michon)->lchan);
			if (rc)
			{
			    rc->te->append(lineEditNick->text() + ": " + QString(s + 6 + n).left(p) + "\n");
			}
		    }

		    else
		    {	
			if(!strncmp(theEdit->text().ascii(), "/part ", 6))
			{
			    ((c_client *)michon)->sendToServer("PART " + QString(theEdit->text().ascii() + 6) + "\r\n");
			    rc = findchan(new QString(theEdit->text().ascii() + 6), ((c_client *)michon)->lchan);
			    if (rc)
			    {
				tabWidget->removePage(rc->pt);
				delchan(new QString(theEdit->text().ascii() + 6), ((c_client *)michon)->lchan);
			    }			    
			}
			else	
			{

			    if (!strncmp(theEdit->text().ascii(), "/kick ", 6))
			    { 
				s = theEdit->text().ascii();
				n = QString(s + 6).find(' ', 0);
				p = QString(s + 7 + n).find(' ', 0);
				((c_client *)michon)->sendToServer("KICK " + QString(s + 6).left(n) + " " + QString(s + 7 + n).left(p) + " :" + QString(s + 8 + n + p) + "\r\n");		      
			    }
			    else
			    {
				if (false)
				{
			      
				}
				else
				{
			      
				}
			    }
			    
			}
		    
		    }    
		    

		}
	    }
	}
    }
    else
    {
	//envoi d'un message
	
    }
    
    theEdit->setText("");
}

void MyDialog1::connectSlot()
{
    ConnectMichon(lineEditServer->text(), atoi(lineEditPort->text()));
}

void MyDialog1::ConnectMichon( QString s, unsigned int p )
{
    michon = new c_client(s, p, this);
    ((c_client *)michon)->sendToServer("NICK " + lineEditNick->text() + "\r\n");
    ((c_client *)michon)->sendToServer("USER " + lineEditUser->text() + " 0 " + s + " :" + lineEditReal->text() + "\r\n");
    ConnectButton->setEnabled(false);
    
    ((c_client *)michon)->lchan = new QPtrList<QRefChan>;
    
}
