#include "listchan.h"

QRefChan * findchan(QString *name, QPtrList<QRefChan> *cl)
{
    QRefChan *rc;
    
    rc = cl->first();
    
    while (rc && (strcmp(*(rc->id), *name)))
    {	
	rc = cl->next();
    }
    
    return rc;    
}

void delchan(QString *name, QPtrList<QRefChan> *cl)
{
    unsigned int i;
    
    for (i = 0; i < cl->count(); i++)
    {
	if (!strcmp(*((cl->at(i))->id), *name))
	{
	    cl->remove(i);
	}
    }
}
