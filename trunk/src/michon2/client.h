#ifndef CLIENT_H
#define CLIENT_H

#include <qsocket.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qhbox.h>
#include "moker.h"

class c_client : public QObject
{
  Q_OBJECT
public : 
  c_client(const QString &host, Q_UINT16 port, MyDialog1 *mdlg);
 ~c_client();

public slots: 
  void closeConnection();
 void sendToServer(QString s);
 void socketReadyRead();
 void socketConnected();
 void socketConnectionClosed();
 void socketError(int e);
 void socketClosed();
    
private :
    QSocket *socket;
    MyDialog1 *mydlg;
};

#endif
