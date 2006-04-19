/****************************************************************************
** Form implementation generated from reading ui file 'moker.ui'
**
** Created: Wed Apr 19 18:38:27 2006
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "moker.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qtextedit.h>
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

    QWidget* privateLayoutWidget = new QWidget( this, "Layout1" );
    privateLayoutWidget->setGeometry( QRect( 10, 320, 506, 40 ) );
    Layout1 = new QHBoxLayout( privateLayoutWidget, 0, 6, "Layout1"); 

    buttonHelp = new QPushButton( privateLayoutWidget, "buttonHelp" );
    buttonHelp->setAutoDefault( TRUE );
    Layout1->addWidget( buttonHelp );
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton( privateLayoutWidget, "buttonOk" );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton( privateLayoutWidget, "buttonCancel" );
    buttonCancel->setAutoDefault( TRUE );
    Layout1->addWidget( buttonCancel );

    ValButton = new QPushButton( this, "ValButton" );
    ValButton->setGeometry( QRect( 350, 280, 80, 30 ) );

    theEdit = new QLineEdit( this, "theEdit" );
    theEdit->setGeometry( QRect( 10, 280, 330, 30 ) );

    ConnectButton = new QPushButton( this, "ConnectButton" );
    ConnectButton->setGeometry( QRect( 430, 280, 90, 30 ) );

    tabWidget = new QTabWidget( this, "tabWidget" );
    tabWidget->setGeometry( QRect( 10, 10, 510, 260 ) );

    Widget8 = new QWidget( tabWidget, "Widget8" );

    statusEdit = new QTextEdit( Widget8, "statusEdit" );
    statusEdit->setEnabled( TRUE );
    statusEdit->setGeometry( QRect( 7, 7, 491, 210 ) );
    statusEdit->setWordWrap( QTextEdit::WidgetWidth );
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
    languageChange();
    resize( QSize(534, 371).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( statusEdit, SIGNAL( textChanged() ), this, SLOT( undoSlot() ) );
    connect( ValButton, SIGNAL( clicked() ), this, SLOT( valSlot() ) );
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
    buttonHelp->setText( tr( "&Help" ) );
    buttonHelp->setAccel( QKeySequence( tr( "F1" ) ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
    ValButton->setText( tr( "&Send" ) );
    ValButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    ConnectButton->setText( tr( "&Connect" ) );
    ConnectButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
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
}

