#ifndef QHHBOX_H
#define QHHBOX_H

#include <qwidget.h>
#include <qtextedit.h>
#include <qlistbox.h>
#include <qhbox.h>

class QHHBox : public QHBox
{
  Q_OBJECT
public : 
    QHHBox(QWidget * parent = 0, const char * name = 0, WFlags f = 0);
  ~QHHBox();
  void setItems(QTextEdit *nte = NULL, QListBox *nlb = NULL);

  QTextEdit *te;
  QListBox *lb;

};

#endif
