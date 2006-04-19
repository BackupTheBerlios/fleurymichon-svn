/****************************************************************************
** Form interface generated from reading ui file 'moker.ui'
**
** Created: Tue Apr 18 17:46:31 2006
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MYDIALOG1_H
#define MYDIALOG1_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QTextEdit;
class QPushButton;
class QLineEdit;

class MyDialog1 : public QDialog
{
    Q_OBJECT

public:
    MyDialog1( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~MyDialog1();

    QTabWidget* tabWidget;
    QWidget* Widget8;
    QTextEdit* statusEdit;
    QPushButton* pushButton18;
    QLineEdit* theEdit;
    QPushButton* pushButton4;
    QPushButton* ValButton;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void theSlot();
    virtual void undoSlot();
    virtual void lineEdit1_textChanged( const QString & s );
    virtual void tSlot();
    virtual void OpenChannel( QString s );
    virtual void valSlot();

protected:
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;
    void *michon;

protected slots:
    virtual void languageChange();

};

#endif // MYDIALOG1_H
