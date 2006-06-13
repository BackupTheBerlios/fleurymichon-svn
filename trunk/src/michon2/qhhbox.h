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
    QHHBox();
  ~QHHBox();
  void setItems(QTextEdit *nte = NULL, QListBox *nlb = NULL);

  QTextEdit *te;
  QListBox *lb;

};

#endif
