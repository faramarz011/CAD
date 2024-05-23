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

#ifndef GUI_H
#define GUI_H

#include <qwidget.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qlcdnumber.h>
#include <qfont.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qmenubar.h>
#include <qpopupmenu.h>

class CannonField : public QWidget
{
		Q_OBJECT
	public:
		CannonField ( QWidget *parent=0, const char *name=0 );

		QSizePolicy sizePolicy() const;


	public slots:
		void  zoomIn();
		void  zoomOut();
		void  zoomFull();
		void  leftArrow();
		void  rightArrow();
		void  upArrow();
		void  downArrow();
		void  runAll();

		void  rowViewToggled();
		void  cellViewToggled();
		void  obstacleViewToggled();
		void  netViewToggled();
		void  meshViewToggled();
		void  congViewToggled();
		void  criticalViewToggled();
	signals:

	protected:
		void  paintEvent ( QPaintEvent * );
		void  mousePressEvent ( QMouseEvent * );
		void  mouseMoveEvent ( QMouseEvent * );
		void  mouseReleaseEvent ( QMouseEvent * );

	private:
		void paintRows ( QPainter * );
		void paintCells ( QPainter * );
		void paintObstacles ( QPainter * );
		void paintNets ( QPainter * );
		void paintCong ( QPainter * );
		void paintGrMesh ( QPainter * );
		void paintCiticalPath ( QPainter * );

		QRect cannonRect() const;

		double scaleFactorX;
		double scaleFactorY;
		double translateFactorX;
		double translateFactorY;
		bool rowViewVar;
		bool cellViewVar;
		bool obstacleViewVar;
		bool netViewVar;
		bool meshViewVar;
		bool congViewVar;
		bool critViewVar;
};


class MyWidget: public QWidget
{
	public:
		MyWidget ( QWidget *parent=0, const char *name=0 );
};

#endif // GUI_H
