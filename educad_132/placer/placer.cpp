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

#include <stdlib.h>
#include <string.h>
#include <placer.h>
#include <stdio.h>
#include <math.h>
#include <qrect.h>

#include <iostream>
#include <list>
#include <vector>
#include <map>
#include "part.h"

using namespace std;
#include <atlasDB.h>


#define DBU 2000
bool sortFunc ( simpleInstance *i1, simpleInstance *i2 );
extern unsigned cellHeight;
int A[10000][10000];
int Bx[10000];
int By[10000];

//-----------------------------------------------------------
// Constructors
//-----------------------------------------------------------
SimplePlacer::SimplePlacer()
{
}

//-----------------------------------------------------------
// Destructors
//-----------------------------------------------------------
SimplePlacer::~SimplePlacer()
{
}

unsigned int SimplePlacer::getInstNum()
{
	return mList.size();
}

simpleInstance *SimplePlacer::getInst ( unsigned index )
{
	return mList[index];
}

int SimplePlacer::simPlacer()
{
	ComputeMatrixA();
	ComputeVectorB();
	GlobalPlace();
	printf ( "\t\tGP: Total Wirelength = %lf\n", computeTHPWL() );
	printf ( "\t\tGP: Total overlap = %lf , OV percentage = %f\% \n", TOV(), TOV() /maxlayoutH/maxlayoutW );

	DetailPlace();
	printf ( "\t\tDP: Total Wirelength = %lf\n", computeTHPWL() );
	printf ( "\t\tDP: Total overlap = %lf , OV percentage = %f\% \n", TOV(), TOV() /maxlayoutH/maxlayoutW );
	return 1;
}

void SimplePlacer::GlobalPlace()
{
	float sumX, sumY, sumCost;
	simpleInstance *inst2;
	list <simpleInstance *>::iterator iter1;
	list <int>::iterator iter2;
	simpleInstance *anInst;
	int rowNo = 0;
	float rowWidth = 0;
	fmNode *anFmNode;
	fmPart *sPR;
	sPR = new fmPart();
	sPR->fmReadDB ( _db );
	
/*	for(int i = 0;;i ++)
	{
		if (sPR->parentPartSize(i) < 20)
			break;
		sPR->BiPart (i);
	}

	for ( unsigned i = 0; i < mList.size(); i++ )
	{
		anInst = mList[i];
		anFmNode = sPR->getNodeByName(anInst->instName);
		if(anFmNode == NULL)
		{
			printf("Cannot find FMNode %s\n", anInst->instName);
			exit(1);
		}
		anFmNode = sPR->getNodeByIndex(anInst->instID);
		if(anFmNode == NULL)
		{
			printf("Cannot find FMNode %d\n", anInst->instID); 
			exit(1);
		}
	}
*/

	cout<<"\tStart of global placer ..."<<endl;
	rowNumber = maxlayoutH / rowHeight;
	for ( unsigned i = 0; i < mList.size(); i++ )
	{
		anInst = mList[i];
		if ((anInst->instType == core_inst) && (anInst->placeType == placable_cell)) 
		{
			if ( (rowWidth + anInst->Width) < maxlayoutW )
			{
				anInst->rowNo = rowNo;
				anInst->TL.x = rowWidth;
				anInst->TL.y = rowNo * rowHeight;
				rowWidth += anInst->Width * (1 + WHITE_SPACE_RATIO);
			}
			else
			{
				rowNo ++;
				rowWidth = 0;
				anInst->rowNo = rowNo;
				anInst->TL.x = 0;
				anInst->TL.y = rowNo * rowHeight;
				rowWidth += anInst->Width * (1 + WHITE_SPACE_RATIO);
			}
		}
	}
	SA();
}

void SimplePlacer::DetailPlace()
{
	//ScaleLocations();
	//SpreadVertical2();
	LegalizeByShift();
}

void SimplePlacer::SpreadVertical()
{
	for ( unsigned i = 0; i < rowNumber; i++ )
	{
		for ( unsigned j = 0; j < mList.size(); j++ )
		{
			if ( (mList[j]->TL.y >= (i * rowHeight) ) && (mList[j]->TL.y < ((i+1) * rowHeight) ) )
			{
				mList[j]->TL.y = i * rowHeight;
				mList[j]->rowNo = i;
			}
		}
	}
}

void SimplePlacer::SpreadVertical2()
{
	float width, minY;
	int k;
	for ( unsigned j = 0; j < mList.size(); j++ )	
		mList[j]->tag = false;

	for ( unsigned i = 0; i < rowNumber; i++ )
	{
		width = 0;
		minY = 100000000;
// 		cout<<"Row "<<i<<endl;
		while (width < (maxlayoutW * 0.9) )
		{
// 			cout<<"\tWidth = "<<width<<endl;
			for ( unsigned j = 0; j < mList.size(); j++ )
			{			
				if ( mList[j]->instType == core_inst )
				{
					if ( (mList[j]->tag == false) && (mList[j]->TL.y < minY) )
					{
						k = j;
					}
				}
			}
			if ( (mList[k]->Width + width) < maxlayoutW)
			{
				mList[k]->TL.y = i * rowHeight;
				mList[k]->rowNo = i;
				mList[k]->tag = true;
				width += mList[k]->Width;
			}
		}
	}
}

void SimplePlacer::ComputeMatrixA()
{
	int i,j;
	simpleInstance *inst1, *inst2, *inst3;
	list <simpleInstance *>::iterator iter;
	printf("Start of ComputeMatrixA\n");
	for ( i = 0; i < 10000; i++ )	
		for ( j = 0; j < 10000; j++ )
			A[i][j] = 0;

	for ( i = 0; i < mList.size(); i++ )	
	{
		inst1 = mList[i];
//		printf("i = %d, inst:%s <%d>:\n", i, inst1->instName, inst1->neighbors->size());
		for ( iter = inst1->neighbors->begin(); iter != inst1->neighbors->end(); iter++ )	
		{
			inst2 = (simpleInstance *)(*iter);
//			printf("\t-inst:%s", inst2->instName);
			for ( j = 0; j < mList.size(); j++ )	
			{
				inst3 = mList[j];
				if (inst2-> instID == inst3-> instID) 
				{
					A[i][j] ++;
//					printf(" A[%d][%d] = %d\n", i, j, A[i][j]);
				}
			}
		}
		A[i][i] = inst1->neighbors->size();
		if (A[i][i] == 0)
		{
			printf("\tA[%d][%d].%s is Zero.\n", i, i, mList[i]->instName, A[i][i]);
			getchar();
		}
	}
}

void SimplePlacer::ComputeVectorB()
{
	int i,j;
	simpleInstance *inst1, *inst2, *inst3;
	list <simpleInstance *>::iterator iter;
	printf("Start of ComputeVectorBx\n");
	for ( i = 0; i < 10000; i++ )	
	{
		Bx[i] = 0;
		By[i] = 0;
	}

	for ( i = 0; i < mList.size(); i++ )	
	{
		inst1 = mList[i];
//		printf("i = %d, inst:%s <%d>:\n", i, inst1->instName, inst1->neighbors->size());
		for ( iter = inst1->neighbors->begin(); iter != inst1->neighbors->end(); iter++ )	
		{
			inst2 = (simpleInstance *)(*iter);
//			printf("\t-inst:%s", inst2->instName);
			if (inst2->instType == pin_inst)
			{
				Bx[i] += (int)(inst2->TL.x);
				By[i] += (int)(inst2->TL.y);
//				printf("\t%f %f  ", inst2->TL.x, inst2->TL.y);			
			}
		}
//		printf("\tB[%d] = <%d,%d>\n", i, Bx[i], By[i]);
//		getchar();
	}
}

void SimplePlacer::ScaleLocations()
{
	float minX, maxX, minY, maxY;
	float scaleX, scaleY;
	float avgX, avgY;

	cout<<"\tStart of detailed placer ..."<<endl;
	minX = maxX = mList[0]->TL.x;
	minY = maxY = mList[0]->TL.y;
	for ( int i = 0; i < mList.size();i ++ )
	{
		if ( mList[i]->instType == core_inst )
		{
			if (mList[i]->TL.x < minX)
				minX = mList[i]->TL.x;
			if (mList[i]->TL.y < minY)
				minY = mList[i]->TL.y;
			if (mList[i]->TL.x > maxX)
				maxX = mList[i]->TL.x;
			if (mList[i]->TL.y > maxY)
				maxY = mList[i]->TL.y;
		}
	}
	avgX = (maxX + minX) / 2;
	avgY = (maxY + minY) / 2;
	scaleX = maxlayoutW / (maxX - minX);
	scaleY = maxlayoutH / (maxY - minY);
	
	for ( int i = 0; i < mList.size();i ++ )
	{
		if ( mList[i]->instType == core_inst )
		{
			mList[i]->TL.x = (mList[i]->TL.x - avgX) * scaleX + maxlayoutW / 2;
			mList[i]->TL.y = (mList[i]->TL.y - avgY) * scaleY + maxlayoutH / 2;
		}
	}
}


void SimplePlacer::LegalizeByShift()
{
	list <simpleInstance *> *rowCells;
	list <simpleInstance *>::iterator iter;
	simpleInstance *aCell;
	rowCells = new list <simpleInstance *>;

	cout<<"\tStart of detailed placer ..."<<endl;
	for ( int i = 0; i < rowNumber;i ++ )
	{
		for ( int j = 0; j < mList.size();j ++ )
		{
			if ( mList[j]->instType == core_inst )
				if ( mList[j]->rowNo == i )
					rowCells->push_back ( mList[j] );
		}
		rowCells -> sort ( sortFunc );
		int width = 0;
		for ( iter = rowCells->begin(); iter != rowCells ->end();iter ++ )
		{
			aCell = ( simpleInstance * ) ( *iter );
			aCell->TL.x = width;
			width += aCell->Width;
		}
		rowCells->erase ( rowCells->begin(), rowCells->end() );
	}
}

double SimplePlacer::computeTHPWL()
{
	list <simpleInstance *>::iterator iter2;
	simpleNet *aNet;
	simpleInstance *anInst;
	long int minx, miny, maxx, maxy;
	double THPWL = 0;

	for ( int i = 0; i < nList.size();i ++ )
	{
		aNet = nList[i];
		minx = 0x7FFFFFFF;
		miny = 0x7FFFFFFF;
		maxx = -1;
		maxy = -1;
		for ( iter2 = aNet->instances->begin();iter2 != aNet->instances->end();iter2 ++ )
		{
			anInst = ( simpleInstance * ) ( * iter2 );
			if ( anInst->TL.x < minx )
				minx = anInst->TL.x;
			if ( anInst->TL.x > maxx )
				maxx = anInst->TL.x;
			if ( anInst->TL.y < miny )
				miny = anInst->TL.y;
			if ( anInst->TL.y > maxy )
				maxy = anInst->TL.y;
		}
		THPWL += ( ( maxy - miny ) + ( maxx - minx ) );
	}
	THPWL /= PLACE_DBU;
	return THPWL;
}

bool SimplePlacer::isNetInList ( list <simpleNet *> hotNets, simpleNet *_net )
{
	list <simpleNet *>::iterator iter;

	for ( iter = hotNets.begin();iter != hotNets.end();iter ++ )
	{
		simpleNet *aNet = ( simpleNet * ) ( * iter );
		if ( aNet->NetID == _net->NetID )
			return true;
	}
	return false;
}

double SimplePlacer::updateTHPWL ( double oldCost, simpleInstance *i1, simpleInstance *i2 )
{
	simplePoint p;
	list <simpleNet *>::iterator iter1;
	list <simpleInstance *>::iterator iter2;

	simpleNet *aNet;
	simpleInstance *anInst;
	long int minx, miny, maxx, maxy;
	double ThpwlBeforeSwap = 0, ThpwlAfterSwap = 0;
	list <simpleNet *> hotNets;

	for ( iter1 = i1->nets->begin();iter1 != i1->nets->end();iter1 ++ )
	{
		aNet = ( simpleNet * ) ( * iter1 );
		if ( isNetInList ( hotNets, aNet ) == false )
			hotNets.push_back ( aNet );
	}
	for ( iter1 = i2->nets->begin();iter1 != i2->nets->end();iter1 ++ )
	{
		aNet = ( simpleNet * ) ( * iter1 );
		if ( isNetInList ( hotNets, aNet ) == false )
			hotNets.push_back ( aNet );
	}
	for ( iter1 = hotNets.begin();iter1 != hotNets.end();iter1 ++ )
	{
		aNet = ( simpleNet * ) ( * iter1 );
		minx = 0x7FFFFFFF;
		miny = 0x7FFFFFFF;
		maxx = -1;
		maxy = -1;
		for ( iter2 = aNet->instances->begin();iter2 != aNet->instances->end();iter2 ++ )
		{
			anInst = ( simpleInstance * ) ( * iter2 );
			if ( anInst->TL.x < minx )
				minx = anInst->TL.x;
			if ( anInst->TL.x > maxx )
				maxx = anInst->TL.x;
			if ( anInst->TL.y < miny )
				miny = anInst->TL.y;
			if ( anInst->TL.y > maxy )
				maxy = anInst->TL.y;
		}
		ThpwlAfterSwap += ( ( maxy - miny ) + ( maxx - minx ) );
	}

	swapCells ( i1, i2 );
	for ( iter1 = hotNets.begin();iter1 != hotNets.end();iter1 ++ )
	{
		aNet = ( simpleNet * ) ( * iter1 );
		minx = 0x7FFFFFFF;
		miny = 0x7FFFFFFF;
		maxx = -1;
		maxy = -1;
		for ( iter2 = aNet->instances->begin();iter2 != aNet->instances->end();iter2 ++ )
		{
			anInst = ( simpleInstance * ) ( * iter2 );
			if ( anInst->TL.x < minx )
				minx = anInst->TL.x;
			if ( anInst->TL.x > maxx )
				maxx = anInst->TL.x;
			if ( anInst->TL.y < miny )
				miny = anInst->TL.y;
			if ( anInst->TL.y > maxy )
				maxy = anInst->TL.y;
		}
		ThpwlBeforeSwap += ( ( maxy - miny ) + ( maxx - minx ) );
	}

	swapCells ( i1, i2 );
	hotNets.erase ( hotNets.begin(), hotNets.end() );
	double THPWL = oldCost + ( ThpwlAfterSwap - ThpwlBeforeSwap ) / PLACE_DBU ;
	return ( THPWL );
}

double SimplePlacer::TOV()
{
	simpleInstance *anInst, *prevInst, *nxtInst;
	list <simpleInstance *> *row = new list <simpleInstance *>;
	list <simpleInstance *>::iterator iter;

	int ov = 0;
	double tov = 0;
	for ( unsigned i = 0;i < rowNumber;i ++ )
	{
		for ( unsigned j = 0;j < mList.size();j ++ )
		{
			anInst = mList[j];
			if ( anInst->rowNo == i )
				row->push_back ( anInst );
		}
		row->sort ( sortFunc );

		iter = row->begin();
		prevInst = ( simpleInstance * ) ( *iter );
		iter ++;
		for ( ;iter != row->end();iter ++ )
		{
			nxtInst = ( simpleInstance * ) ( *iter );
			ov = rowHeight * ( ( prevInst->TL.x + prevInst->Width ) - nxtInst->TL.x );
			if ( ov > 0 )
				tov += ov;
			prevInst = nxtInst;
		}
		row->erase ( row->begin(),row->end() );
		ov = 0;
	}
	return ( tov  / DBU / DBU );
}

double SimplePlacer::OV ( simpleInstance *inst1 )
{
	double ov = 0;

	for ( unsigned j = 0;j < mList.size();j ++ )
	{
		simpleInstance *inst2 = mList[j];
		if ( inst1->instID != inst2->instID )
		{
			QRect r1 ( inst1->TL.x, inst1->TL.y, inst1->Width, inst1->Height );
			QRect r2 ( inst2->TL.x, inst2->TL.y, inst2->Width, inst2->Height );
			if ( r1.intersects ( r2 ) )
			{
				QRect r3 = r1.intersect ( r2 );
				ov += r3.width() * r3.height();
			}
		}
	}
	return ( ov / DBU / DBU );
}
void SimplePlacer::swapCells ( simpleInstance *inst1, simpleInstance *inst2 )
{
	simplePoint p;
	int r;
	p = inst1->TL;
	inst1->TL = inst2->TL;
	inst2->TL = p;
	r = inst1->rowNo;
	inst1->rowNo = inst2->rowNo;
	inst2->rowNo = r;
}

bool sortFunc ( simpleInstance *i1, simpleInstance *i2 )
{
	if ( i1->TL.x < i2->TL.x )
		return true;
	else
		return false;
}

void SimplePlacer::distributeWhiteSpaces()
{
	simpleInstance *anInst, *prevInst, *nxtInst;
	list <simpleInstance *> *row = new list <simpleInstance *>;
	list <simpleInstance *>::iterator iter;
	float totalW;
	unsigned k ;
	long int cellWS ;

	for ( unsigned i = 0;i < rowNumber;i ++ )
	{
		row-> erase ( row->begin(), row->end() );
		for ( unsigned j = 0;j < mList.size();j ++ )
		{
			anInst = mList[j];
			if ( anInst->rowNo == i )
				row->push_back ( anInst );
		}

		totalW = 0;
		for ( iter = row->begin(); iter != row->end(); iter ++ )
		{
			anInst = ( simpleInstance * ) ( *iter );
			totalW = totalW + anInst->Width;
		}
		cellWS = ( ( float ) ( maxlayoutW ) - ( float ) ( totalW ) ) / ( ( float ) ( row->size() ) );;
		if ( cellWS> 0 )
		{
			k = 0;
			for ( iter = row->begin(); iter != row->end(); iter ++ )
			{
				anInst = ( simpleInstance * ) ( *iter );
				anInst->TL.x = anInst->TL.x + k * cellWS;
				totalW += anInst->Width;
				k ++;
			}
		}
	}
	for ( unsigned i = 0; i < mList.size(); i++ )
	{
		anInst = mList[i];
		if ( ( anInst->TL.x + anInst -> Width ) > maxlayoutW )
		{
			printf ( "ERROR: cell %s is placed out of layout.\n", anInst->instName );
			getchar();
		}
	}
}

