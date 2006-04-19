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
    QString *stext = new QString("text");
    tabWidget->addTab(new QTextEdit(NULL, QString((s + *stext)).ascii()), s);
    //connect(SIGNAL(textChanged(), MyDialog1, SLOT());
}

void MyDialog1::valSlot()
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
    ((c_client *)michon)->sendToServer("USER " + lineEditUser->text() + " 0 " + s + lineEditReal->text() + "\r\n");
    ConnectButton->setEnabled(false);
}
