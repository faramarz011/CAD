/****************************************************************************
** CannonField meta object code from reading C++ file 'gui.h'
**
** Created: Mon Apr 8 05:23:34 2024
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "gui.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *CannonField::className() const
{
    return "CannonField";
}

QMetaObject *CannonField::metaObj = 0;
static QMetaObjectCleanUp cleanUp_CannonField( "CannonField", &CannonField::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString CannonField::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CannonField", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString CannonField::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "CannonField", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* CannonField::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"zoomIn", 0, 0 };
    static const QUMethod slot_1 = {"zoomOut", 0, 0 };
    static const QUMethod slot_2 = {"zoomFull", 0, 0 };
    static const QUMethod slot_3 = {"leftArrow", 0, 0 };
    static const QUMethod slot_4 = {"rightArrow", 0, 0 };
    static const QUMethod slot_5 = {"upArrow", 0, 0 };
    static const QUMethod slot_6 = {"downArrow", 0, 0 };
    static const QUMethod slot_7 = {"runAll", 0, 0 };
    static const QUMethod slot_8 = {"rowViewToggled", 0, 0 };
    static const QUMethod slot_9 = {"cellViewToggled", 0, 0 };
    static const QUMethod slot_10 = {"obstacleViewToggled", 0, 0 };
    static const QUMethod slot_11 = {"netViewToggled", 0, 0 };
    static const QUMethod slot_12 = {"meshViewToggled", 0, 0 };
    static const QUMethod slot_13 = {"congViewToggled", 0, 0 };
    static const QUMethod slot_14 = {"criticalViewToggled", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "zoomIn()", &slot_0, QMetaData::Public },
	{ "zoomOut()", &slot_1, QMetaData::Public },
	{ "zoomFull()", &slot_2, QMetaData::Public },
	{ "leftArrow()", &slot_3, QMetaData::Public },
	{ "rightArrow()", &slot_4, QMetaData::Public },
	{ "upArrow()", &slot_5, QMetaData::Public },
	{ "downArrow()", &slot_6, QMetaData::Public },
	{ "runAll()", &slot_7, QMetaData::Public },
	{ "rowViewToggled()", &slot_8, QMetaData::Public },
	{ "cellViewToggled()", &slot_9, QMetaData::Public },
	{ "obstacleViewToggled()", &slot_10, QMetaData::Public },
	{ "netViewToggled()", &slot_11, QMetaData::Public },
	{ "meshViewToggled()", &slot_12, QMetaData::Public },
	{ "congViewToggled()", &slot_13, QMetaData::Public },
	{ "criticalViewToggled()", &slot_14, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"CannonField", parentObject,
	slot_tbl, 15,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_CannonField.setMetaObject( metaObj );
    return metaObj;
}

void* CannonField::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "CannonField" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool CannonField::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: zoomIn(); break;
    case 1: zoomOut(); break;
    case 2: zoomFull(); break;
    case 3: leftArrow(); break;
    case 4: rightArrow(); break;
    case 5: upArrow(); break;
    case 6: downArrow(); break;
    case 7: runAll(); break;
    case 8: rowViewToggled(); break;
    case 9: cellViewToggled(); break;
    case 10: obstacleViewToggled(); break;
    case 11: netViewToggled(); break;
    case 12: meshViewToggled(); break;
    case 13: congViewToggled(); break;
    case 14: criticalViewToggled(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool CannonField::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool CannonField::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool CannonField::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
