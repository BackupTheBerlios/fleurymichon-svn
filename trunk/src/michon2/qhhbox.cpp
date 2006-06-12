#include "qhhbox.h"

QHHBox::QHHBox(QWidget * parent, const char * name, WFlags f)
{
    
}

QHHBox::~QHHBox()
{

}

void QHHBox::setItems(QTextEdit *nte, QListBox *nlb)
{
  te = nte;
  lb = nlb;   
}
