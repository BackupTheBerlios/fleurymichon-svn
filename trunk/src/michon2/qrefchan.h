#ifndef QREFCHAN_H
#define QREFCHAN_H

#include <qwidget.h>
#include <qtextedit.h>
#include <qlistbox.h>
#include <qhbox.h>

class QRefChan : public QObject
{
  Q_OBJECT
public : 
    QRefChan();
  ~QRefChan();
  void setItems(QTextEdit *nte = NULL, QListBox *nlb = NULL, QString *nid = NULL);

  QTextEdit *te;
  QListBox *lb;
  QString *id;

};

#endif
