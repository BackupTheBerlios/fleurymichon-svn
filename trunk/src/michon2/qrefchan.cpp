#include "qrefchan.h"

QRefChan::QRefChan()
{
    
}

QRefChan::~QRefChan()
{

}

void QRefChan::setItems(QTextEdit *nte, QListBox *nlb, QString *nid, QWidget *npt)
{
  te = nte;
  lb = nlb;   
  id = nid;
  pt = npt;
}
