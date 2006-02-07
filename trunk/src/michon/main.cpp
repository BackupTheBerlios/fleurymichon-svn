#include <qapplication.h>
#include <stdio.h>
#include "form1.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    dlgmain w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
