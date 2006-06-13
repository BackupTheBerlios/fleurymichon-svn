#include "listchan.h"

QRefChan * findchan (QString name, QPtrList<QRefChan> *cl)
{
    QRefChan *rc;
    
    rc = cl->first();
    
    while (rc && (strcmp(*(rc->id), name)))
    {	
	rc = cl->next();
    }
    
    return rc;    
}
