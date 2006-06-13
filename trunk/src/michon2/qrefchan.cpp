#include "qrefchan.h"

QRefChan::QRefChan()
{
    
}

QRefChan::~QRefChan()
{

}

void QRefChan::setItems(QTextEdit *nte, QListBox *nlb, QString *nid)
{
  te = nte;
  lb = nlb;   
  id = nid;
}
