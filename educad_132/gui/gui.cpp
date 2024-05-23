/********************************************************************************
***    Copyright (c) 2008-2012 Regents of the Shahid Beheshti University
***
***    Ali Jahanian
***    Contact author: jahanian@sbu.ac.ir
***    Affiliation:   Shahid Beheshti University, VLSI Design Automation Lab.
***    Velenjak, Tehran, IRAN.
***
***    Permission is hereby granted, free of charge, to any person obtaining
***    a copy of this software and associated documentation files, to deal in
***    the Software without restriction, including without limitation
***    the rights to use, copy, modify, merge, publish, distribute, sublicense,
***    and/or sell copies of the Software, and to permit persons to whom the
***    Software is furnished to do so, subject to the following conditions:
***        1. The above copyright notice and this permission notice shall be
***           included in all copies or substantial portions of the Software.
***        2. The software is provided "AS IS", without warranty of any kind,
***           express or implied.
*********************************************************************************/

#include "gui.h"
#include "iostream"
#include "list"
using namespace std;

#include "atlasDB.h"

extern atlasDB *db;
extern bool doPaint;
extern int pd_main();
QTextEdit *cmdEditBox;

MyWidget::MyWidget ( QWidget *parent, const char *name )
		: QWidget ( parent, name )
{
	CannonField *cannonField = new CannonField ( this, "cannonField" );

	//------------------------------------------
	// Radio Buttons
	//------------------------------------------

	QRadioButton *rowView = new QRadioButton ( "&Rows", this, "rowView" );
	rowView->setFont ( QFont ( "Helvetica", 10, QFont::Normal ) );
	connect ( rowView, SIGNAL ( clicked() ), cannonField, SLOT ( rowViewToggled() ) );

	QRadioButton *cellView = new QRadioButton ( "&Cells", this, "cellView" );
	cellView->setFont ( QFont ( "Helvetica", 10, QFont::Normal ) );
	connect ( cellView, SIGNAL ( clicked() ), cannonField, SLOT ( cellViewToggled() ) );

	QRadioButton *obstacleView = new QRadioButton ( "&Obstacles", this, "obstacleView" );
	obstacleView->setFont ( QFont ( "Helvetica", 10, QFont::Normal ) );
	connect ( obstacleView, SIGNAL ( clicked() ), cannonField, SLOT ( obstacleViewToggled() ) );

	QRadioButton *netView = new QRadioButton ( "&Nets", this, "netView" );
	netView->setFont ( QFont ( "Helvetica", 10, QFont::Normal ) );
	connect ( netView, SIGNAL ( clicked() ), cannonField, SLOT ( netViewToggled() ) );

	QRadioButton *meshView = new QRadioButton ( "&GR mesh", this, "meshView" );
	meshView->setFont ( QFont ( "Helvetica", 10, QFont::Normal ) );
	connect ( meshView, SIGNAL ( clicked() ), cannonField, SLOT ( meshViewToggled() ) );

	QRadioButton *congView = new QRadioButton ( "&Congestion map", this, "congView" );
	congView->setFont ( QFont ( "Helvetica", 10, QFont::Normal ) );
	connect ( congView, SIGNAL ( clicked() ), cannonField, SLOT ( congViewToggled() ) );

	QRadioButton *cpView = new QRadioButton ( "&Critical path", this, "critView" );
	cpView->setFont ( QFont ( "Helvetica", 10, QFont::Normal ) );
	connect ( cpView, SIGNAL ( clicked() ), cannonField, SLOT ( criticalViewToggled() ) );

	//------------------------------------------
	// Push Buttons
	//------------------------------------------
	QPushButton *runAll = new QPushButton ( "Run &All", this, "runAll" );
	runAll->setFont ( QFont ( "Helvetica", 14, QFont::Bold ) );
	connect ( runAll, SIGNAL ( clicked() ), cannonField, SLOT ( runAll() ) );

	QPushButton *zoomFull = new QPushButton ( "Zoom &Full", this, "zoomFull" );
	zoomFull->setFont ( QFont ( "Helvetica", 12, QFont::Bold ) );
	connect ( zoomFull, SIGNAL ( clicked() ), cannonField, SLOT ( zoomFull() ) );

	QPushButton *zoomIn = new QPushButton ( "Zoom &In", this, "zoomIn" );
	zoomIn->setFont ( QFont ( "Helvetica", 12, QFont::Bold ) );
	connect ( zoomIn, SIGNAL ( clicked() ), cannonField, SLOT ( zoomIn() ) );

	QPushButton *zoomOut = new QPushButton ( "Zoom &Out", this, "zoomOut" );
	zoomOut->setFont ( QFont ( "Helvetica", 12, QFont::Bold ) );
	connect ( zoomOut, SIGNAL ( clicked() ),cannonField, SLOT ( zoomOut() ) );

	QPushButton *leftArrow = new QPushButton ( "&Left Arrow", this, "leftArrow" );
	leftArrow->setFont ( QFont ( "Helvetica", 12, QFont::Bold ) );
	connect ( leftArrow, SIGNAL ( clicked() ), cannonField, SLOT ( leftArrow() ) );

	QPushButton *rightArrow = new QPushButton ( "&Right Arrow", this, "rightArrow" );
	rightArrow->setFont ( QFont ( "Helvetica", 12, QFont::Bold ) );
	connect ( rightArrow, SIGNAL ( clicked() ), cannonField, SLOT ( rightArrow() ) );

	QPushButton *upArrow = new QPushButton ( "&Up Arrow", this, "*upArrow" );
	upArrow->setFont ( QFont ( "Helvetica", 12, QFont::Bold ) );
	connect ( upArrow, SIGNAL ( clicked() ), cannonField, SLOT ( upArrow() ) );

	QPushButton *downArrow = new QPushButton ( "&Down Arrow", this, "downArrow" );
	downArrow->setFont ( QFont ( "Helvetica", 12, QFont::Bold ) );
	connect ( downArrow, SIGNAL ( clicked() ), cannonField, SLOT ( downArrow() ) );

	QGridLayout *grid = new QGridLayout ( this, 3, 2, 10 );
	grid->addWidget ( cannonField, 0, 1 );
	grid->setColStretch ( 1, 10 );

	cmdEditBox = new QTextEdit ( this,"cmdEditBox" );
	QVBoxLayout *leftBox = new QVBoxLayout;
	grid->addLayout ( leftBox, 0, 0 );

	QHBoxLayout *editBox = new QHBoxLayout;
	grid->addLayout ( editBox, 2, 1 );

	editBox->addWidget ( cmdEditBox );

	leftBox->addWidget ( rowView );
	leftBox->addWidget ( cellView );
	leftBox->addWidget ( obstacleView );
	leftBox->addWidget ( netView );
	leftBox->addWidget ( meshView );
	leftBox->addWidget ( congView );
	leftBox->addWidget ( cpView );
	leftBox->addWidget ( zoomIn );
	leftBox->addWidget ( zoomOut );
	leftBox->addWidget ( zoomFull );
	leftBox->addWidget ( leftArrow );
	leftBox->addWidget ( rightArrow );
	leftBox->addWidget ( upArrow );
	leftBox->addWidget ( downArrow );
	leftBox->addWidget ( runAll );

}

CannonField::CannonField ( QWidget *parent, const char *name )
		: QWidget ( parent, name )
{
	setPalette ( QPalette ( QColor ( 0, 0, 0 ) ) );
	scaleFactorX = 1;
	scaleFactorY = 1;
	translateFactorX = 50;
	translateFactorY = 50;
	rowViewVar = false;
	meshViewVar = false;
	cellViewVar = false;
	obstacleViewVar = false;
	netViewVar = false;
	congViewVar = false;
	critViewVar = false;
}

void  CannonField::zoomIn()
{
	if ( doPaint )
	{
		scaleFactorX *= 1.1;
		scaleFactorY *= 1.1;
		cmdEditBox->append ( QString ( "Zoom in" ) );
		repaint();
	}
}
void  CannonField::zoomOut()
{
	if ( doPaint )
	{
		scaleFactorX /= 1.1;
		scaleFactorY /= 1.1;
		cmdEditBox->append ( QString ( "Zoom out" ) );
		repaint();
	}
}
void  CannonField::zoomFull()
{
	if ( doPaint )
	{
		scaleFactorX = 750.0 / db->getPlacementResultWidth();
		scaleFactorY = 550.0 / db->getPlacementResultHeight();
		translateFactorX = 0;
		translateFactorY = 0;
		cmdEditBox->append ( QString ( "Zoom full" ) );
		repaint();
	}
}
void  CannonField::leftArrow()
{
	if ( doPaint )
	{
		translateFactorX += 100 / scaleFactorX;
		cmdEditBox->append ( QString ( "Left" ) );
		repaint();
	}
}
void  CannonField::rightArrow()
{
	if ( doPaint )
	{
		translateFactorX -= 100 / scaleFactorX;
		cmdEditBox->append ( QString ( "Right" ) );
		repaint();
	}
}

void  CannonField::upArrow()
{
	if ( doPaint )
	{
		translateFactorY += 100 / scaleFactorY;
		cmdEditBox->append ( QString ( "Up" ) );
		repaint();
	}
}

void  CannonField::downArrow()
{
	translateFactorY -= 100 / scaleFactorY;
	cmdEditBox->append ( QString ( "Down" ) );
	repaint();
}

void  CannonField::runAll()
{
	cmdEditBox->append ( QString ( "Run All" ) );
	repaint();
}

void  CannonField::rowViewToggled()
{
	if ( rowViewVar == true )
		rowViewVar = false;
	else
		rowViewVar = true;
	repaint();
}

void  CannonField::meshViewToggled()
{
	if ( meshViewVar == true )
		meshViewVar = false;
	else
		meshViewVar = true;
	repaint();
}

void  CannonField::congViewToggled()
{
	if ( congViewVar == true )
		congViewVar = false;
	else
		congViewVar = true;
	repaint();
}

void  CannonField::criticalViewToggled()
{
	if ( critViewVar == true )
		critViewVar = false;
	else
		critViewVar = true;
	repaint();
}

void  CannonField::obstacleViewToggled()
{
	if ( obstacleViewVar == true )
		obstacleViewVar = false;
	else
		obstacleViewVar = true;
	repaint();
}

void  CannonField::cellViewToggled()
{
	if ( cellViewVar == true )
		cellViewVar = false;
	else
		cellViewVar = true;
	repaint();
}

void  CannonField::netViewToggled()
{
	if ( netViewVar == true )
		netViewVar = false;
	else
		netViewVar = true;
	repaint();
}


QRect CannonField::cannonRect() const
{
	QRect r ( 0, 0, 750, 550 );
	r.moveTopLeft ( rect().topLeft() );
	return r;
}


QSizePolicy CannonField::sizePolicy() const
{
	return QSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding );
}
