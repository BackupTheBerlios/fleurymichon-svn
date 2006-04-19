/****************************************************************************
** MyDialog1 meta object code from reading C++ file 'moker.h'
**
** Created: Wed Apr 19 17:29:21 2006
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "moker.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MyDialog1::className() const
{
    return "MyDialog1";
}

QMetaObject *MyDialog1::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MyDialog1( "MyDialog1", &MyDialog1::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MyDialog1::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MyDialog1", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MyDialog1::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MyDialog1", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MyDialog1::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"theSlot", 0, 0 };
    static const QUMethod slot_1 = {"undoSlot", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "s", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"lineEdit1_textChanged", 1, param_slot_2 };
    static const QUMethod slot_3 = {"tSlot", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "s", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"OpenChannel", 1, param_slot_4 };
    static const QUMethod slot_5 = {"valSlot", 0, 0 };
    static const QUMethod slot_6 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "theSlot()", &slot_0, QMetaData::Public },
	{ "undoSlot()", &slot_1, QMetaData::Public },
	{ "lineEdit1_textChanged(const QString&)", &slot_2, QMetaData::Public },
	{ "tSlot()", &slot_3, QMetaData::Public },
	{ "OpenChannel(QString)", &slot_4, QMetaData::Public },
	{ "valSlot()", &slot_5, QMetaData::Public },
	{ "languageChange()", &slot_6, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MyDialog1", parentObject,
	slot_tbl, 7,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MyDialog1.setMetaObject( metaObj );
    return metaObj;
}

void* MyDialog1::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MyDialog1" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool MyDialog1::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: theSlot(); break;
    case 1: undoSlot(); break;
    case 2: lineEdit1_textChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: tSlot(); break;
    case 4: OpenChannel((QString)static_QUType_QString.get(_o+1)); break;
    case 5: valSlot(); break;
    case 6: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MyDialog1::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MyDialog1::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool MyDialog1::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
