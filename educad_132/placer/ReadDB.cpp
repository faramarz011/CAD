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

#include <iostream>
#include <list>
#include <vector>
#include <map>

using namespace std;
#include <atlasDB.h>

unsigned layoutH, layoutW, cellHeight;


bool SimplePlacer::hasNet ( simpleInstance *anInst, simpleNet *aNet )
{
	list<simpleNet *>::iterator iter;

	for ( iter = anInst->nets->begin();iter != anInst->nets->end();iter ++ )
	{
		simpleNet *net = ( simpleNet * ) ( *iter );
		if ( net->NetID == aNet->NetID )
			return true;
	}
	return false;
}

//-----------------------------------------------------------
// Read DB information
//-----------------------------------------------------------
int SimplePlacer::readDB ( atlasDB *db )
{
	vector <simpleInstance *>::iterator iter;
	list<simpleNet *>::iterator iter1;
	list<simpleInstance *>::iterator iter2;
	simpleNet *aNet;
	simpleInstance *anInst, *anInst1, *anInst2;
	float layoutArea = 0, Dim, modDim;
	char * netname = new char[100];
	int cnt = 0;

	_db = db;


	//---------------------------------
	// Create I/O nodes from I/O pins
	//---------------------------------
	globalPinNumber = 0;
	db->resetGlobalPinListPos();
	for ( int i = 0;i < db->getGlobalPinNum();i ++ )
	{
		if ( ( db->getGlobalPinName() == "POWR" ) || ( db->getGlobalPinName() == "GRND" ) )
		{
			printf ( "\t\tGlobal pin %s is rejected.\n", db->getGlobalPinName().c_str() );
			db->globalPinListPosGoForward();
			continue;
		}

		anInst = new simpleInstance;
		anInst->nets = new list <simpleNet *>;
		anInst->neighbors = new list <simpleInstance *>;
		anInst->neighbor_costs = new list <int>;
		anInst->placeType = fixed_cell;
		anInst->instType = pin_inst;
		anInst->rowNo = -1;
		anInst -> instID = cnt;
		anInst -> instName = new char[100];
		strcpy ( anInst -> instName, db->getGlobalPinName().c_str() );
		anInst -> TL.x = 0;
		anInst -> TL.y = 0;
		anInst -> cellName = new char[100];
		strcpy ( anInst -> cellName, "PPIN" );
		anInst -> Width = -1;
		anInst -> Height = -1;
		anInst -> tag = false;
		mList.push_back ( anInst );
		globalPinNumber ++;
// 		printf ( "PIN %s %s \n", anInst->cellName, anInst->instName );
		cnt ++;
		db->globalPinListPosGoForward();
	}

	//------------------
	// Reading Instances
	//------------------
	printf ( "\tReading %d cells ... \n", db->getInstanceNum() );
	db->resetInstanceListPos();
	for ( unsigned i = 0;i < db->getInstanceNum();i ++ )
	{
		db->setActiveMacroRandomAccess ( db->getInstanceMacroName() );
		anInst = new simpleInstance;
		anInst->nets = new list <simpleNet *>;
		anInst->neighbors = new list <simpleInstance *>;
		anInst->neighbor_costs = new list <int>;
		anInst->placeType = placable_cell;
		anInst->instType = core_inst;
		anInst->rowNo = 0;
		anInst -> instID = cnt;
		anInst -> instName = new char[100];
		strcpy ( anInst -> instName, db->getInstanceName().c_str() );
		if ( db->getInstancePlaceType() == placed_inst )
		{
			anInst -> TL.x = db->getInstancePlacePointX();
			anInst -> TL.y = db->getInstancePlacePointY();
		}
		else
		{
			anInst -> TL.x = 0;
			anInst -> TL.y = 0;
		}
		anInst -> cellName = new char[100];
		strcpy ( anInst -> cellName, db->getInstanceMacroName().c_str() );
		anInst -> Width = db->getMacroWidth();
		anInst -> Height = db->getMacroHeight();
		anInst -> tag = false;
		rowHeight = anInst -> Height;
		mList.push_back ( anInst );
		cnt ++;
		db->InstanceListPosGoForward();
	}
	cellHeight = rowHeight;


	//----------------
	// Reading nets
	//----------------
	printf ( "\tReading and elaborating %d nets ... \n", db->getNetNum() );
	cnt = 0;
	db->resetNetListPos();
	for ( unsigned i = 0; i < db->getNetNum();i ++ )
	{
		strcpy ( netname, db->getNetName().c_str() );
		if ( ( strcmp ( netname, "POWR" ) == 0 ) || ( strcmp ( netname, "GRND" ) == 0 ) )
		{
// 			printf ( "\n\t\tSpecial net %s is ignored.\n", netname);
			db->netListPosGoForward();
			continue;
		}
		simpleNet *aNet;
		aNet = new simpleNet;
		aNet -> netName = new char[100];
		aNet->instances = new list <struct simpleInstance *>;
		aNet->srcTerminal = NULL;
		aNet->netDepth = -1;
		aNet->tag = false;
		aNet->NetID = cnt ++;
		strcpy ( aNet -> netName, db->getNetName().c_str() );

		db->resetNetPinListPos();
		for ( int j = 0; j < db->getNetPinNum();j ++ )
		{
			simpleInstance *anInst =  findInstance ( db->getNetPinInstanceName() );
			if ( anInst != NULL )
			{
				aNet->instances->push_back ( anInst );
				if ( hasNet ( anInst, aNet ) == false )
					anInst->nets->push_back ( aNet );
			}
			else
				printf ( "Netlist annotation error.\n" );

			db->netPinListPosGoForward();
		}
		if ( db->isNetGlobalPinNameValid() == true )
		{
			simpleInstance *anInst =  findInstance ( db->getNetGlobalPinName() );
			if ( anInst != NULL )
			{
				aNet->instances->push_back ( anInst );
				if ( hasNet ( anInst, aNet ) == false )
					anInst->nets->push_back ( aNet );
			}
			else
				printf ( "Netlist annotation error.\n" );
		}
		nList.push_back ( aNet );
		db->netListPosGoForward();
	}

	//----------------------------
	// Fill instances' neighbors
	//----------------------------
	for ( unsigned ii = 0; ii < mList.size(); ii++ )
	{
		anInst1 = mList[ii];
		for ( iter1 = anInst1->nets->begin();iter1 != anInst1->nets->end();iter1 ++ )
		{
			aNet = ( simpleNet * ) ( *iter1 );
			for ( iter2 = aNet->instances->begin(); iter2 != aNet->instances->end();iter2++ )
			{
				simpleInstance *anInst2 = ( simpleInstance * ) ( *iter2 );
				if ( anInst1->instID != anInst2->instID )
				{
					anInst1->neighbors->push_back ( anInst2 );
					anInst1->neighbor_costs->push_back ( aNet->netDepth );
				}
			}
		}
	}
// 	for ( unsigned ii = 0; ii < mList.size(); ii++ )
// 	{
// 		anInst1 = mList[ii];
// 		cout<<"Inst "<<anInst1->instName<<":"<<endl;
// 		for ( iter2 = anInst1->neighbors->begin(); iter2 != anInst1->neighbors->end();iter2++ )
// 		{
// 			simpleInstance *anInst2 = ( simpleInstance * ) ( *iter2 );
// 			cout<<anInst2->instName<<" , ";
// 		}
// 		cout<<endl;
// 	}

	//----------------------------
	// Estimate layout dimension
	//----------------------------
	for ( unsigned ii = 0; ii < mList.size(); ii++ )
	{
		anInst = mList[ii];
		layoutArea += anInst->Width * anInst->Height;
	}
  layoutArea  *= (1 + WHITE_SPACE_RATIO);
	Dim = floor(sqrt(layoutArea));
	int xx = (int) Dim % (int) rowHeight;
	if (xx != 0)
		modDim = (Dim - xx) + rowHeight;
	else
		modDim = Dim;
	maxlayoutW = maxlayoutH = ( unsigned ) ( modDim );
	layoutH = layoutW = maxlayoutW;
	printf ( "\tRead DB: %d instances and pins are read.\n", mList.size() );
	printf ( "\tRead DB: %d nets are read.\n" , nList.size() );
	printf ( "\tRow number = %d.\n", maxlayoutW / rowHeight );
	shuffleCells();
// 	sortCells();
	setNetsDepth();
	placePins();
	return 0;
}


void SimplePlacer::placePins()
{
	unsigned int pinNumPerEdge = globalPinNumber / 4;
	unsigned int pinVerticalDistance = maxlayoutH / pinNumPerEdge;
	unsigned int pinHorizontalDistance = maxlayoutW / pinNumPerEdge;
	unsigned int pinCnt = 0;

	for ( unsigned i = 0; i < mList.size();i ++ )
	{
		if ( ( pinCnt < pinNumPerEdge ) )
		{
			if ( mList[i]->instType == pin_inst )
			{
				mList[i]->TL.x = pinCnt * pinHorizontalDistance;
				mList[i]->TL.y = 0;
				mList[i]->placeType = fixed_cell;
// 				cout<<mList[i]->TL.x<<","<<mList[i]->TL.y<<endl;
				pinCnt ++;
			}
		}
		else if ( pinCnt < ( 2 * pinNumPerEdge ) )
		{
			if ( mList[i]->instType == pin_inst )
			{
				mList[i]->TL.x = maxlayoutW;
				mList[i]->TL.y = ( pinCnt  - pinNumPerEdge ) * pinVerticalDistance;
				mList[i]->placeType = fixed_cell;
// 				cout<<mList[i]->TL.x<<","<<mList[i]->TL.y<<endl;
				pinCnt ++;
			}
		}
		else if ( pinCnt  < ( 3 * pinNumPerEdge ) )
		{
			if ( mList[i]->instType == pin_inst )
			{
				mList[i]->TL.x = ( pinCnt - ( pinNumPerEdge*2 ) ) * pinHorizontalDistance;
				mList[i]->TL.y = maxlayoutW;
				mList[i]->placeType = fixed_cell;
// 				cout<<mList[i]->TL.x<<","<<mList[i]->TL.y<<endl;
				pinCnt ++;
			}
		}
		else
		{
			if ( mList[i]->instType == pin_inst )
			{
				mList[i]->TL.x = 0;
				mList[i]->TL.y = ( pinCnt - ( 3*pinNumPerEdge ) ) * pinVerticalDistance;
// 				cout<<mList[i]->TL.x<<","<<mList[i]->TL.y<<endl;
				mList[i]->placeType = fixed_cell;
				pinCnt ++;
			}
		}
	}

	printf ( "\t\t%d pins are placed.\n", pinCnt );
	/*	for ( unsigned i = 0; i < mList.size(); i++ )
			if ( mList[i]->instType == pin_inst )
				cout<<mList[i]->TL.x<<","<<mList[i]->TL.y<<endl;*/
}

//-----------------------------------
// Read Placed Netlist from file
//-----------------------------------
void SimplePlacer::readDump ( char *dumpFile )
{
	FILE *fp;
	char iName[64];
	float iLeft, iTop;
	unsigned int cnt = 0, iNumber = 0;

	fp = fopen ( dumpFile, "r+t" );
	if ( fp == NULL )
	{
		printf ( "Cannot read dump file %s\n", dumpFile );
		exit ( 1 );
	}

	fscanf ( fp,"%d %d %d %d\n", &iNumber, &maxlayoutW, &maxlayoutH, &rowNumber );
// 	printf("#cells = %d, W = %d, H = %d, Row number = %d\n", iNumber, maxlayoutW, maxlayoutH, rowNumber);

	for ( unsigned i = 0;i < iNumber;i ++ )
	{
		fscanf ( fp,"%s %f %f \n",iName, &iLeft, &iTop );
// 	    printf("i = %d, name = %s , x = %f , y = %f \n",i, iName, iLeft, iTop);
		simpleInstance *iPtr = findInstance ( iName );
		if ( iPtr == NULL )
		{
			printf ( "Instance %s is not found in instance list.\n" );
			exit ( 1 );
		}
		else
		{
			iPtr -> TL.x = iLeft;
			iPtr -> TL.y = iTop;
			iPtr -> rowNo = ( int ) ( iPtr -> TL.y / cellHeight );
			cnt ++;
		}
	}
	printf ( "%d in stances are loaded from dump\n", cnt );
	computeTHPWL();
}

simpleInstance *SimplePlacer::findInstance ( string instName )
{
	simpleInstance *anInst;

	for ( unsigned ii = 0; ii < mList.size(); ii++ )
	{
		anInst = mList[ii];
		if ( strcmp ( anInst-> instName, instName.c_str() ) == 0 )
			return anInst;
	}
	return NULL;
}


void SimplePlacer::updateDB ( atlasDB *db )
{
	simpleInstance *iPtr;
	unsigned maxX = maxlayoutW, maxY = maxlayoutH;
	int i;
	int totalCellNo = 0;

	for ( unsigned ii = 0; ii < mList.size(); ii++ )
	{
		iPtr = mList[ii];
		if ( iPtr->instType == core_inst )
		{
			bool isOK = db->setInstanceTopLeft (	iPtr->instName,
			                                     ( long int ) ( iPtr->TL.y ),
			                                     ( long int ) ( iPtr->TL.x ),
			                                     n_orientation,
			                                     placed_inst );
			if ( isOK == false )
			{
				printf ( "Update DB violation!\n" );
				exit ( 1 );
			}
			totalCellNo ++;
			if ( ( iPtr->TL.x + iPtr->Width ) > maxX )
				maxX = iPtr->TL.x + iPtr->Width;

			if ( ( iPtr->TL.y + iPtr->Height ) > maxY )
				maxY = iPtr->TL.y + iPtr->Height;

			maxlayoutW = maxX;
			maxlayoutH = maxY;
		}
	}

	printf ( "\t\tUpdate: %d instances are updated in DB\n", totalCellNo );
	printf ( "\t\tUpdate: layout area: w = %d , h = %d\n", maxlayoutW, maxlayoutH );

	//	set layout dimensions
	db->setNetlistPlacementResults (	( long int ) ( maxlayoutW ),
	                                 ( long int ) ( maxlayoutH ),
	                                 rowNumber,
	                                 ( long int ) ( rowHeight ) );
}

void SimplePlacer::dumpPlacement ( char *dumpFile )
{
	simpleInstance *anInst;
	FILE *fp;


	fp = fopen ( dumpFile, "w+t" );
	if ( fp == NULL )
	{
		printf ( "Cannot write dump file %s\n", dumpFile );
		exit ( 1 );
	}
	fprintf ( fp,"%d %d %d %d\n",mList.size(), ( unsigned int ) ( maxlayoutW ), ( unsigned int ) ( maxlayoutH ), rowNumber );

	for ( unsigned ii = 0; ii < mList.size(); ii++ )
	{
		anInst = mList[ii];
		fprintf ( fp,"%s %f %f\n", anInst->instName, ( float ) ( anInst->TL.x ), ( float ) ( anInst->TL.y ) );
	}
	fclose ( fp );
}

void SimplePlacer::shuffleCells()
{
	vector <simpleInstance *>::iterator iter;
	list <simpleInstance *>::iterator iter2;

	list <simpleInstance *> newList;
	simpleInstance *inst;
	unsigned i, k;

	while ( mList.size() > 0 )
	{
		i = rand() % mList.size();
		for ( k = 0,iter = mList.begin(); k < i;iter ++, k ++ );
		inst = ( simpleInstance * ) ( * iter );
		newList.push_back ( inst );
		mList.erase ( iter );
	}
	for ( iter2 = newList.begin(); iter2 != newList.end();iter2 ++ )
	{
		inst = ( simpleInstance * ) ( * iter2 );
		mList.push_back ( inst );
	}

}

bool sortFuncW ( simpleInstance *i1, simpleInstance *i2 )
{
	if ( i1->Width > i2->Width )
		return true;
	else
		return false;
}

void SimplePlacer::sortCells()
{
	simpleInstance *anInst;
	list <simpleInstance *> *mList_temp = new list <simpleInstance *>;
	list <simpleInstance *>::iterator iter;

	for ( unsigned i = 0;i < mList.size();i ++ )
	{
		anInst = mList[i];
		mList_temp->push_back ( anInst );
	}

	mList_temp->sort ( sortFuncW );

	mList.erase ( mList.begin(), mList.end() );

	for ( iter = mList_temp->begin();iter != mList_temp->end();iter ++ )
	{
		anInst = ( simpleInstance * ) ( * iter );
		mList.push_back ( anInst );
	}
	mList_temp->erase ( mList_temp->begin(), mList_temp->end() );
}

void SimplePlacer::setNetsDepth()
{
	vector <simpleInstance *> activeList, activeList2;
	list <simpleNet *>::iterator iter1;
	list <simpleInstance *>::iterator iter2;
	unsigned int currDepth = 0;

	for ( unsigned i = 0;i < mList.size();i ++ )
	{

		simpleInstance *anInst= mList[i];
		if ( anInst->instType == pin_inst )
		{
			anInst->tag = true;
			activeList.push_back ( anInst );
		}
	}
	while ( activeList.size() > 0 )
	{
		for ( unsigned i = 0;i < activeList.size();i ++ )
		{
			simpleInstance *activeInst= activeList[i];
			for ( iter1 = activeInst->nets->begin(); iter1 != activeInst->nets->end();iter1 ++ )
			{
				simpleNet *aNet = ( simpleNet * ) ( *iter1 );
				if ( aNet->tag == false )
				{
					aNet->netDepth = currDepth;
					aNet->tag = true;
					for ( iter2 = aNet->instances->begin(); iter2 != aNet->instances->end();iter2++ )
					{
						simpleInstance *anInst = ( simpleInstance * ) ( *iter2 );
						if ( anInst->tag == false )
						{
							anInst->tag = true;
							activeList2.push_back ( anInst );
						}
					}
				}
			}
		}
		activeList.erase ( activeList.begin(), activeList.end() );
		for ( unsigned k = 0;k < activeList2.size();k ++ )
			activeList.push_back ( activeList2[k] );
		activeList2.erase ( activeList2.begin(), activeList2.end() );
		currDepth ++;
	}
	/*	for ( int i = 0; i < nList.size();i ++)
		{
			simpleNet *n = nList[i];
			printf ( "Net %s , Depth = %d\n", n->netName, n->netDepth );
		}
	*/
}
