/****************************************************************************
** Form interface generated from reading ui file 'michon.ui'
**
** Created: Wed Jun 14 05:14:02 2006
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MYDIALOG1_H
#define MYDIALOG1_H

#include <qvariant.h>
#include <qdialog.h>
#include <qptrlist.h>
#include "qrefchan.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLineEdit;
class QPushButton;
class QTabWidget;
class QWidget;
class QTextEdit;
class QLabel;

class MyDialog1 : public QDialog
{
    Q_OBJECT

public:
    MyDialog1( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~MyDialog1();

    QLineEdit* theEdit;
    QPushButton* buttonOk;
    QPushButton* ValButton;
    QTabWidget* tabWidget;
    QWidget* Widget8;
    QTextEdit* statusEdit;
    QWidget* TabPage;
    QLabel* textLabelUser;
    QLabel* textLabelReal;
    QLineEdit* lineEditUser;
    QLineEdit* lineEditReal;
    QLabel* textLabel2;
    QLabel* textLabel3;
    QLineEdit* lineEditPort;
    QLineEdit* lineEditServer;
    QLabel* textLabelNick;
    QLineEdit* lineEditNick;
    QPushButton* ConnectButton;

    QPtrList<QRefChan> *lchan;

public slots:
    virtual void undoSlot();
    virtual void tSlot();
    virtual void OpenChannel( QString s );
    virtual void valSlot();
    virtual void connectSlot();
    virtual void ConnectMichon( QString s, unsigned int p );

protected:
    void *michon;


protected slots:
    virtual void languageChange();

};

#endif // MYDIALOG1_H
