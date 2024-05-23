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

#include <math.h>
#include "gui.h"
#include <qpainter.h>
#include <qpixmap.h>

#include "iostream"
#include "list"
using namespace std;

#include "atlasDB.h"
#include "gr.h"
#include "placer.h"

#define __SCALE 0.03
#define congestionFontSize 160
#define totalWLFontSize 200
#define WLFontSize 160
#define binIDFontSize 100
#define rowNo 260
#define colNo 260

extern unsigned cellHeight;
extern SimplePlacer *sPL;
extern unsigned layoutH, layoutW;
//------------------------------------------------------------------
// Global definitions
//------------------------------------------------------------------
bool firstPaint = true;
bool doPaint = false;
QColor layersColors[6] = {Qt::blue, Qt::red, Qt::gray, Qt::darkGray, Qt::green, Qt::yellow};
string layersNames[12];
unsigned layersNumber;
QPoint startPnt, endPnt;
bool drawZoomArea = false;

//------------------------------------------------------------------
// External definitions
//------------------------------------------------------------------
extern QTextEdit *cmdEditBox;
extern atlasDB *db;
extern simpleGlobalRouter *sGR;


//------------------------------------------------------------------
// Paint cells
//------------------------------------------------------------------
// Paint cells
//------------------------------------------------------------------
void CannonField::paintRows ( QPainter *p )
{
	int i;

	for ( i = 0;i < db->getPlacemnetResultRowNumber();i ++ )
	{
		p->setPen ( QPen ( Qt::darkRed , 1, SolidLine ) );
		p->setBrush ( NoBrush );
		p->drawRect ( QRect (	0,
		                      i*db->getPlacementResultRowHeight(),
		                      db->getPlacementResultWidth(),
		                      db->getPlacementResultRowHeight() ) );
		p->drawLine (	( int ) ( db->getPlacementResultWidth() * 0.95 ),
		              i*db->getPlacementResultRowHeight(),
		              db->getPlacementResultWidth(),
		              ( int ) ( ( i + 0.4 ) * db->getPlacementResultRowHeight() ) );
	}
	cmdEditBox->append ( QString ( "View std. cell rows" ) );
}

//------------------------------------------------------------------
void CannonField::paintCells ( QPainter *p )
{
	p->setPen ( QPen ( Qt::green , 1, SolidLine ) );
	p->setBrush ( Qt::darkGreen );
	db->resetInstanceListPos();
	for ( unsigned i = 0;i < db->getInstanceNum();i ++ )
	{
		db->setActiveMacroRandomAccess ( db->getInstanceMacroName() );
		p->drawRect ( QRect (	db->getInstanceLeft(),
		                      db->getInstanceTop(),
		                      db->getMacroWidth(),
		                      db->getMacroHeight() ) );
		unsigned rowIndex = db->getInstanceTop() / db->getMacroHeight();
		if ( ( rowIndex % 2 ) == 0 )
			p->drawLine ( ( int ) ( db->getInstanceLeft() + db->getMacroWidth() * 0.15 ) ,
			              db->getInstanceTop(),
			              db->getInstanceLeft(),
			              ( int ) ( db->getInstanceTop() + db->getMacroHeight() * 0.2 ) );
		else
			p->drawLine ( ( int ) ( db->getInstanceLeft() + db->getMacroWidth() * 0.15 ) ,
			              db->getInstanceTop() + db->getMacroHeight(),
			              db->getInstanceLeft(),
			              ( int ) ( db->getInstanceTop() + 0.8 * db->getMacroHeight() ) );
		db->InstanceListPosGoForward();
	}

	// Draw pins
	p->setPen ( QPen ( Qt::red , 1, SolidLine ) );
	p->setBrush ( Qt::red );
	for ( unsigned i = 0; i < sPL->getInstNum(); i++ )
	{
		simpleInstance *anInst = sPL->getInst(i);
		if ( anInst->instType == pin_inst )
		{
			p->drawRect ( QRect (	( int ) anInst->TL.x,
			                      ( int ) anInst->TL.y,
			                      10000,
			                      10000) );
		}
	}
	cmdEditBox->append ( QString ( "View cells" ) );
}

//------------------------------------------------------------------
void CannonField::paintObstacles ( QPainter *p )
{

	db->resetLayerListPos();
	for ( unsigned i = 0; i < db->getLayerNum();i ++ )
	{
		layersNames[i] = db->getLayerName();
		db->layerListPosGoForward();
	}

	db->resetInstanceListPos();
	for ( unsigned i = 0;i < db->getInstanceNum();i ++ )
	{
		db->setActiveMacroRandomAccess ( db->getInstanceMacroName() );
		db->resetMacroObstructionListPos();
		for ( unsigned j = 0; j < db->getMacroObstructionLayerNum();j ++ )
		{
			if ( db->getMacroObstructionLayerName() == layersNames[0] )
			{
				p->setPen ( QPen ( layersColors[0] , 1, SolidLine ) );
				p->setBrush ( layersColors[0] );
			}
			else if ( db->getMacroObstructionLayerName() == layersNames[1] )
			{
				p->setPen ( QPen ( layersColors[1] , 1, SolidLine ) );
				p->setBrush ( layersColors[1] );
			}
			db->resetMacroObstructionRectListPos();
			for ( unsigned k = 0; k < db->getMacroObstructionRectsNum();k ++ )
			{
				long int top = db->getMacroObstructionRectTop() + db->getInstanceTop();
				long int left = db->getMacroObstructionRectLeft() + db->getInstanceLeft();
				long int bottom = db->getMacroObstructionRectBottom() + db->getInstanceTop();
				long int right = db->getMacroObstructionRectRight() + db->getInstanceLeft();
				p->drawRect ( QRect ( left, top, ( bottom - top ), ( right - left ) ) );
				db->macroObstructionRectListGoForward();
			}
			db->macroObstructionListGoForward();
		}
		db->InstanceListPosGoForward();
	}
	cmdEditBox->append ( QString ( "View obstacles" ) );
}

//------------------------------------------------------------------
// Paint nets
//------------------------------------------------------------------
void CannonField::paintNets ( QPainter *p )
{
	TPointData src, aPoint;
	list<TPointData> sinks;
	list<TPointData>::iterator iter;
	char * netname = new char[100];

	src.left = -1;
	src.top = -1;
	db->resetNetListPos();
	for ( unsigned i = 0; i < db->getNetNum();i ++ )
	{
		strcpy ( netname, db->getNetName().c_str() );
		if ( ( strcmp ( netname, "POWR" ) == 0 ) || ( strcmp ( netname, "GRND" ) == 0 ) )
		{
// 			printf ( "\n\t\tSpecial net %s is 	ignored.\n", netname);
			db->netListPosGoForward();
			continue;
		}
		db->resetNetPinListPos();
		for ( int j = 0; j < db->getNetPinNum();j ++ )
		{
			// Read net pin location
			db->setActiveInstanceRandomAccess ( db->getNetPinInstanceName() );
			// Read pin direction
			db->setActiveMacroRandomAccess ( db->getInstanceMacroName() );
			db->setActiveMacroPinRandomAccess ( db->getNetPinName() );
			TPinDirection pinDir = db->getMacroPinDirection();
			if ( pinDir == pin_output_dir )
			{
				src.left = db->getInstanceLeft();
				src.top = db->getInstanceTop();
			}
			else
			{
				aPoint.left = db->getInstanceLeft();
				aPoint.top = db->getInstanceTop();
				sinks.push_back ( aPoint );
			}
			db->netPinListPosGoForward();
		}

		if ( ( src.left == -1 ) || ( src.top == -1 ) )
		{
// 			printf("Net %s has no source.\n", netname);
			db->netListPosGoForward();
			continue;
		}
		p->setBrush ( NoBrush );
		p->setPen ( QPen ( Qt::white , 1, SolidLine ) );
// 		cerr<<"Net "<<dbNet->name<<" has "<<sinks.size() <<" sinks."<<endl;
		if ( sinks.size() > 0 )
		{
			for ( iter = sinks.begin();iter != sinks.end();iter ++ )
			{
				aPoint = ( TPointData ) ( *iter );
				p->drawLine ( src.left, src.top, aPoint.left, aPoint.top );
// 				cerr<<"Line "<<src.left<<" , "<<src.top<<" , "<<aPoint.left<<" , "<<aPoint.top <<endl;
			}
		}
		sinks.erase ( sinks.begin(), sinks.end() );
		src.left = -1;
		src.top = -1;
		db->netListPosGoForward();
	}
	cmdEditBox->append ( QString ( "View nets" ) );
}
//------------------------------------------------------------------
// Paint global routing mesh
//------------------------------------------------------------------
void CannonField::paintGrMesh ( QPainter *p )
{
	p->setBrush ( NoBrush );
	p->setPen ( QPen ( Qt::darkGray , 1, SolidLine ) );
	for ( unsigned i = 0; i < sGR->nodes.size();i ++ )
	{
		simpleGrNode *aNode = sGR->nodes[i];
		p->drawRect ( QRect ( aNode->centerCol*sGR->cellW,
		                      aNode->centerRow*sGR->cellH,
		                      sGR->cellW,
		                      sGR->cellH ) );
	}
	p->setBrush ( Qt::blue );
	p->setPen ( QPen ( Qt::blue , 4, SolidLine ) );
	for ( unsigned i = 0; i < sGR->nodes.size();i ++ )
	{
		simpleGrNode *aNode = sGR->nodes[i];
		p->drawEllipse ( QRect (	aNode->centerCol*sGR->cellW+sGR->cellW/2-3500,
		                         aNode->centerRow*sGR->cellH+sGR->cellH/2-3500,
		                         7000,
		                         7000 ) );
	}
	p->setBrush ( Qt::blue );
	p->setPen ( QPen ( Qt::blue , 1, SolidLine ) );
	for ( unsigned i = 0; i < sGR->edges.size();i ++ )
	{
		simpleGrEdge *anEdge = sGR->edges[i];
		p->drawLine ( anEdge->node1->centerCol * sGR->cellW+sGR->cellW/2,
		              anEdge->node1->centerRow * sGR->cellH+sGR->cellH/2,
		              anEdge->node2->centerCol * sGR->cellW+sGR->cellW/2,
		              anEdge->node2->centerRow * sGR->cellH+sGR->cellH/2 );
	}

	cmdEditBox->append ( QString ( "View global routing mesh" ) );
}

//------------------------------------------------------------------
// Paint critical path
//------------------------------------------------------------------
void CannonField::paintCiticalPath ( QPainter *p )
{
	cmdEditBox->append ( QString ( "View critical path" ) );
}

//------------------------------------------------------------------
// Paint pins
//------------------------------------------------------------------
void CannonField::paintCong ( QPainter *p )
{
	float c60 = 0, c80 = 0, c90 = 0, c100 = 0, c150 = 0, c200 = 0;

	for ( unsigned i = 0; i < sGR->edges.size();i ++ )
	{
		simpleGrEdge *anEdge = sGR->edges[i];

		float ov = 100.0 * ( ( anEdge->used_tracks - anEdge->capacity ) / anEdge->capacity ) ;
		if ( ov <= 60 )
			p->setPen ( QPen ( Qt::blue , 2, SolidLine ) );
		else if ( ov <= 80 )
			p->setPen ( QPen ( Qt::blue , 2, SolidLine ) );
		else if ( ov <= 90 )
			p->setPen ( QPen ( Qt::yellow , 2, SolidLine ) );
		else if ( ov <= 100 )
			p->setPen ( QPen ( Qt::yellow , 2, SolidLine ) );
		else if ( ov < 150 )
			p->setPen ( QPen ( Qt::red , 2, SolidLine ) );
		else
			p->setPen ( QPen ( Qt::red , 2, SolidLine ) );

		p->setBrush ( NoBrush );
		p->drawLine ( anEdge->node1->centerCol * sGR->cellW+sGR->cellW/2,
		              anEdge->node1->centerRow * sGR->cellH+sGR->cellH/2,
		              anEdge->node2->centerCol * sGR->cellW+sGR->cellW/2,
		              anEdge->node2->centerRow * sGR->cellH+sGR->cellH/2 );
	}
	cmdEditBox->append ( QString ( "View congestion map" ) );
}


//------------------------------------------------------------------
// Paint Event Handler
//------------------------------------------------------------------
void CannonField::paintEvent ( QPaintEvent *e )
{

	if ( !e->rect().intersects ( cannonRect() ) )
		return;

	if ( !e->rect().intersects ( cannonRect() ) )
		return;

	//--------------------------
	// Adjust Graphical view
	//--------------------------

	QRect cr = cannonRect();
	QPixmap pix ( cr.size() );
	pix.fill ( this, cr.topLeft() );

	QPainter p ( &pix );
	if ( firstPaint )
	{
		scaleFactorX = 550.0 / db->getPlacementResultWidth();
		scaleFactorY = 400.0 / db->getPlacementResultHeight();
		translateFactorX = 0;
		translateFactorY = 0;
		cmdEditBox->append ( QString ( "White box is initial placement area." ) );
		cmdEditBox->append ( QString ( "Blue box is final placement area." ) );
		firstPaint = false;
	}
	p.scale ( scaleFactorX,  scaleFactorY );
	p.translate ( translateFactorX + 10, translateFactorY );

	//--------------------------
	// Paint layout outline
	//--------------------------
	p.setPen ( QPen ( Qt::gray , 3, SolidLine ) );
	p.setBrush ( NoBrush );
	p.drawRect ( QRect ( 0, 0, layoutW, layoutH ) );
	//--------------------------
	// Paint views on design
	//--------------------------

	if ( rowViewVar == true )
		paintRows ( &p );

	if ( cellViewVar == true )
		paintCells ( &p );

	if ( obstacleViewVar == true )
		paintObstacles ( &p );

	if ( netViewVar == true )
		paintNets ( &p );


	if ( meshViewVar == true )
		paintGrMesh ( &p );

	if ( congViewVar == true )
		paintCong ( &p );

	if ( critViewVar == true )
		paintCiticalPath ( &p );

	p.end();

	p.begin ( this );
	p.drawPixmap ( cr.topLeft(), pix );

}
void CannonField::mousePressEvent ( QMouseEvent *me )
{
	startPnt.setX ( me->x() );
	startPnt.setY ( me->y() );
	//	drawZoomArea = true;
}

void CannonField::mouseMoveEvent ( QMouseEvent *me )
{
	/*	endPnt.setX(me->x());
	endPnt.setY(me->y());
	repaint();*/
}

void CannonField::mouseReleaseEvent ( QMouseEvent *me )
{
	endPnt.setX ( me->x() );
	endPnt.setY ( me->y() );
	if ( ( endPnt.x() - startPnt.x() > 10 ) && ( ( endPnt.y() - startPnt.y() ) > 10 ) )
	{
		translateFactorX -= startPnt.x() /scaleFactorX;
		translateFactorY -= startPnt.y() /scaleFactorX;
		scaleFactorX = scaleFactorX * ( 750.0 / ( endPnt.x() - startPnt.x() ) );
		scaleFactorY = scaleFactorY * ( 550.0 / ( endPnt.y() - startPnt.y() ) );
		cmdEditBox->append ( QString ( "Zoom area" ) );
		repaint();
	}
	else
		cmdEditBox->append ( QString ( "Low size area for zoom" ) );
	//	drawZoomArea = false;
}
