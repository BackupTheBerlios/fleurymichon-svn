/****************************************************************************
** Form implementation generated from reading ui file 'moker.ui'
**
** Created: Thu Apr 20 11:04:34 2006
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "moker.h"

#include <qvariant.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qtextedit.h>
#include <qlistbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "moker.ui.h"

/*
 *  Constructs a MyDialog1 as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
MyDialog1::MyDialog1( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "MyDialog1" );
    setSizeGripEnabled( TRUE );

    theEdit = new QLineEdit( this, "theEdit" );
    theEdit->setGeometry( QRect( 10, 280, 330, 30 ) );

    ConnectButton = new QPushButton( this, "ConnectButton" );
    ConnectButton->setGeometry( QRect( 450, 280, 100, 30 ) );

    ValButton = new QPushButton( this, "ValButton" );
    ValButton->setGeometry( QRect( 350, 280, 100, 30 ) );

    tabWidget = new QTabWidget( this, "tabWidget" );
    tabWidget->setGeometry( QRect( 10, 10, 660, 260 ) );

    Widget8 = new QWidget( tabWidget, "Widget8" );

    statusEdit = new QTextEdit( Widget8, "statusEdit" );
    statusEdit->setEnabled( TRUE );
    statusEdit->setGeometry( QRect( 7, 7, 491, 210 ) );
    statusEdit->setWordWrap( QTextEdit::WidgetWidth );

    userslist = new QListBox( Widget8, "userslist" );
    userslist->setGeometry( QRect( 510, 10, 136, 210 ) );
    tabWidget->insertTab( Widget8, QString::fromLatin1("") );

    TabPage = new QWidget( tabWidget, "TabPage" );

    textLabelNick = new QLabel( TabPage, "textLabelNick" );
    textLabelNick->setGeometry( QRect( 20, 10, 91, 30 ) );

    textLabelUser = new QLabel( TabPage, "textLabelUser" );
    textLabelUser->setGeometry( QRect( 20, 50, 91, 30 ) );

    textLabelReal = new QLabel( TabPage, "textLabelReal" );
    textLabelReal->setGeometry( QRect( 20, 90, 91, 30 ) );

    lineEditUser = new QLineEdit( TabPage, "lineEditUser" );
    lineEditUser->setGeometry( QRect( 110, 50, 110, 28 ) );

    lineEditReal = new QLineEdit( TabPage, "lineEditReal" );
    lineEditReal->setGeometry( QRect( 110, 90, 110, 28 ) );

    textLabel2 = new QLabel( TabPage, "textLabel2" );
    textLabel2->setGeometry( QRect( 240, 10, 107, 31 ) );

    textLabel3 = new QLabel( TabPage, "textLabel3" );
    textLabel3->setGeometry( QRect( 310, 50, 40, 31 ) );

    lineEditPort = new QLineEdit( TabPage, "lineEditPort" );
    lineEditPort->setGeometry( QRect( 352, 48, 131, 31 ) );

    lineEditServer = new QLineEdit( TabPage, "lineEditServer" );
    lineEditServer->setGeometry( QRect( 352, 8, 131, 31 ) );

    lineEditNick = new QLineEdit( TabPage, "lineEditNick" );
    lineEditNick->setGeometry( QRect( 110, 10, 110, 28 ) );
    tabWidget->insertTab( TabPage, QString::fromLatin1("") );

    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setGeometry( QRect( 550, 280, 110, 30 ) );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    languageChange();
    resize( QSize(683, 326).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( statusEdit, SIGNAL( textChanged() ), this, SLOT( undoSlot() ) );
    connect( ValButton, SIGNAL( clicked() ), this, SLOT( valSlot() ) );
    connect( ConnectButton, SIGNAL( clicked() ), this, SLOT( connectSlot() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );

    // tab order
    setTabOrder( theEdit, ValButton );
    setTabOrder( ValButton, ConnectButton );
    setTabOrder( ConnectButton, tabWidget );
    setTabOrder( tabWidget, statusEdit );
    setTabOrder( statusEdit, lineEditUser );
    setTabOrder( lineEditUser, lineEditReal );
    setTabOrder( lineEditReal, lineEditPort );
    setTabOrder( lineEditPort, lineEditServer );
    setTabOrder( lineEditServer, lineEditNick );
}

/*
 *  Destroys the object and frees any allocated resources
 */
MyDialog1::~MyDialog1()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MyDialog1::languageChange()
{
    setCaption( tr( "Michon IRC Client" ) );
    ConnectButton->setText( tr( "&Connect" ) );
    ConnectButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
    ValButton->setText( tr( "&Send" ) );
    ValButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    statusEdit->setText( QString::null );
    tabWidget->changeTab( Widget8, tr( "Status" ) );
    textLabelNick->setText( tr( "Nickname" ) );
    textLabelUser->setText( tr( "Username" ) );
    textLabelReal->setText( tr( "Real name" ) );
    lineEditUser->setText( tr( "username" ) );
    lineEditReal->setText( tr( "real name" ) );
    textLabel2->setText( tr( "Default server" ) );
    textLabel3->setText( tr( "Port" ) );
    lineEditPort->setText( tr( "6667" ) );
    lineEditServer->setText( tr( "localhost" ) );
    lineEditNick->setText( tr( "nickname" ) );
    tabWidget->changeTab( TabPage, tr( "Settings" ) );
    buttonOk->setText( tr( "E&xit" ) );
    buttonOk->setAccel( QKeySequence( tr( "Alt+X" ) ) );
}

