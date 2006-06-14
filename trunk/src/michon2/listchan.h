#include <qptrlist.h>
#include "qrefchan.h"

QRefChan * findchan(QString *, QPtrList<QRefChan> *);
QRefChan * findchanfrompage(QWidget *, QPtrList<QRefChan> *);
void delchan(QString *, QPtrList<QRefChan> *);
void dellist(QPtrList<QRefChan> *);
