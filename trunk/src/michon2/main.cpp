#include <qapplication.h>
#include "michon.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    MyDialog1 w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
