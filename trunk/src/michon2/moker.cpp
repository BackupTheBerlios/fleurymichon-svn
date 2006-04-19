/****************************************************************************
** Form implementation generated from reading ui file 'moker.ui'
**
** Created: Tue Apr 18 18:12:34 2006
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "moker.h"

#include <qvariant.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlineedit.h>
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

    tabWidget = new QTabWidget( this, "tabWidget" );
    tabWidget->setGeometry( QRect( 11, 11, 506, 270 ) );

    Widget8 = new QWidget( tabWidget, "Widget8" );

    statusEdit = new QTextEdit( Widget8, "statusEdit" );
    statusEdit->setEnabled( TRUE );
    statusEdit->setGeometry( QRect( 7, 7, 491, 221 ) );
    statusEdit->setWordWrap( QTextEdit::WidgetWidth );
    tabWidget->insertTab( Widget8, QString::fromLatin1("") );

    pushButton18 = new QPushButton( this, "pushButton18" );
    pushButton18->setGeometry( QRect( 420, 280, 40, 30 ) );

    theEdit = new QLineEdit( this, "theEdit" );
    theEdit->setGeometry( QRect( 13, 283, 320, 30 ) );

    pushButton4 = new QPushButton( this, "pushButton4" );
    pushButton4->setGeometry( QRect( 460, 280, 60, 30 ) );

    ValButton = new QPushButton( this, "ValButton" );
    ValButton->setGeometry( QRect( 350, 280, 70, 30 ) );

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
    languageChange();
    resize( QSize(538, 383).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( pushButton18, SIGNAL( clicked() ), this, SLOT( theSlot() ) );
    connect( statusEdit, SIGNAL( textChanged() ), this, SLOT( undoSlot() ) );
    connect( theEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( lineEdit1_textChanged(const QString&) ) );
    connect( pushButton4, SIGNAL( clicked() ), this, SLOT( tSlot() ) );
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
    setCaption( tr( "MyDialog1" ) );
    statusEdit->setText( tr( "testttttttttt" ) );
    tabWidget->changeTab( Widget8, tr( "Status" ) );
    pushButton18->setText( tr( "No" ) );
    pushButton4->setText( tr( "Insulte" ) );
    ValButton->setText( tr( "&Send" ) );
    ValButton->setAccel( QKeySequence( tr( "Alt+S" ) ) );
    buttonHelp->setText( tr( "&Help" ) );
    buttonHelp->setAccel( QKeySequence( tr( "F1" ) ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonOk->setAccel( QKeySequence( QString::null ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setAccel( QKeySequence( QString::null ) );
}

