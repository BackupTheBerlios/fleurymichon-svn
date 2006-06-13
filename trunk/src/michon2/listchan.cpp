#include "listchan.h"

QRefChan * findchan (QString *name, QPtrList<QRefChan> *cl)
{
    QRefChan *rc;
    
    rc = cl->first();
    
    while (rc && (strncmp(*(rc->id), *name, strlen(*name))))
    {	
	rc = cl->next();
    }
    
    return rc;    
}
