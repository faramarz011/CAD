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
#include <part.h>
#include <stdio.h>
#include <math.h>

#include <iostream>
#include <list>
#include <vector>
#include <map>

using namespace std;
#include <atlasDB.h>



//-----------------------------------------------------------
// Read DB information
//-----------------------------------------------------------
int fmPart::fmReadDB ( atlasDB *db )
{
	list <fmNode *>::iterator iter;
	fmNode *aNode;
	char * netname = new char[100];
	int cnt = 0;
	list <fmEdge *>::iterator iter1;
	list <fmNode *>::iterator iter2, iter3;
	fmEdge *anEdge;


	//-------------------------------------------
	// Reading pins, create a node for each pin
	//-------------------------------------------
	printf ( "\tReading %d primary pins ... \n", db->getGlobalPinNum() );
	db->resetGlobalPinListPos();
	for ( unsigned i = 0;i < db->getGlobalPinNum();i ++ )
	{
		if ( ( db->getGlobalPinName() == "POWR" ) || ( db->getGlobalPinName() == "GRND" ) )
		{
			db->globalPinListPosGoForward();
			continue;
		}
		aNode = new fmNode;
		aNode->neighborNodes = new list <fmNode *>;
		aNode->connectedEdges = new list <fmEdge *>;
		aNode -> nodeID = cnt;
		aNode -> nodeName = new char[100];
		strcpy ( aNode -> nodeName, db->getGlobalPinName().c_str() );
		aNode -> nodeType = new char[100];
		strcpy ( aNode -> nodeType, "PPIN" );
		aNode->pn = 0;
		aNode->gpn = 0;
		aNode->type = FM_PIN;
		aNode->lock = false;
		aNode->fs = 0;
		aNode->te = 0;
		aNode->gain = 0;
		nList->push_back ( aNode );
		cnt ++;
		db->globalPinListPosGoForward();
	}

	//-----------------------------------------------------
	// Reading Instances, create a node for each instance
	//-----------------------------------------------------
	printf ( "\tReading %d cells ... \n", db->getInstanceNum() );
	db->resetInstanceListPos();
	for ( unsigned i = 0;i < db->getInstanceNum();i ++ )
	{
		db->setActiveMacroRandomAccess ( db->getInstanceMacroName() );
		aNode = new fmNode;
		aNode->neighborNodes = new list <fmNode *>;
		aNode->connectedEdges = new list <fmEdge *>;
		aNode -> nodeID = cnt;
		aNode -> nodeName = new char[100];
		strcpy ( aNode -> nodeName, db->getInstanceName().c_str() );
		aNode -> nodeType = new char[100];
		strcpy ( aNode -> nodeType, db->getInstanceMacroName().c_str() );
		aNode->pn = 0;
		aNode->gpn = 0;
		aNode->fs = 0;
		aNode->te = 0;
		aNode->gain = 0;
		aNode->type = FM_CELL;
		nList->push_back ( aNode );
		cnt ++;
		db->InstanceListPosGoForward();
	}

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
			db->netListPosGoForward();
			continue;
		}
		fmEdge *anEdge;
		anEdge = new fmEdge;
		anEdge->edgeID = cnt ++;
		anEdge -> edgeName = new char[100];
		anEdge->nodes = new list <struct fmNode *>;
		anEdge->srcTerminal = NULL;
		strcpy ( anEdge -> edgeName, db->getNetName().c_str() );


		if ( db->isNetGlobalPinNameValid() == true )
		{
			if ( ( db->getNetGlobalPinName() != "POWR" ) && ( db->getNetGlobalPinName() != "GRND" ) )
			{
				aNode =  findNode ( db->getNetGlobalPinName() );
				anEdge->nodes->push_back ( aNode );
				if ( hasEdge ( aNode, anEdge ) == false )
					aNode->connectedEdges->push_back ( anEdge );
			}
		}

		db->resetNetPinListPos();
		for ( int j = 0; j < db->getNetPinNum();j ++ )
		{
			fmNode *aNode =  findNode ( db->getNetPinInstanceName() );
			anEdge->nodes->push_back ( aNode );
			if ( hasEdge ( aNode, anEdge ) == false )
				aNode->connectedEdges->push_back ( anEdge );
			db->netPinListPosGoForward();
		}
		if ( anEdge->nodes->size() > 1 )
			eList->push_back ( anEdge );
		db->netListPosGoForward();
	}

	// fill neighborNodes of the nodes
	for ( iter1 = eList->begin(); iter1 != eList->end(); iter1 ++ )
	{
		anEdge = ( fmEdge * ) ( * iter1 );
		for ( iter2 = anEdge->nodes->begin(); iter2 != anEdge->nodes->end(); iter2 ++ )
		{
			fmNode *aNode1 = ( fmNode * ) ( * iter2 );
			for ( iter3 = anEdge->nodes->begin(); iter3 != anEdge->nodes->end(); iter3 ++ )
			{
				fmNode *aNode2 = ( fmNode * ) ( * iter3 );
				if ( ( aNode1->nodeID != aNode2->nodeID ) && ( hasNode ( aNode1, aNode2 ) == false ) )
				{
// 					printf("Node %s -> node %s\n", aNode1->nodeName, aNode2->nodeName);
					aNode1->neighborNodes->push_back ( aNode2 );
				}
			}
		}
	}

	printf ( "\tRead DB: %d nodes are read, %d cells and %d pins.\n", nList->size(), db->getInstanceNum(), db->getGlobalPinNum() );
	printf ( "\tRead DB: %d edges are read.\n" , eList->size() );

	return 0;
}


fmNode *fmPart::findNode ( string nodeName )
{
	list <fmNode *>::iterator iter;
	fmNode *aNode;

	for ( iter = nList->begin(); iter != nList->end(); iter ++ )
	{
		aNode = ( fmNode * ) ( * iter );
		if ( strcmp ( aNode-> nodeName, nodeName.c_str() ) == 0 )
			return aNode;
	}
	return NULL;
}

bool fmPart::hasNode ( fmNode *n1, fmNode *n2 )
{
	list <fmNode *>::iterator iter;
	fmNode *aNode;

	for ( iter = n1->neighborNodes->begin(); iter != n1->neighborNodes->end(); iter ++ )
	{
		aNode = ( fmNode * ) ( * iter );
		if ( aNode->nodeID == n2->nodeID )
			return true;
	}
	return false;
}

bool fmPart::hasEdge ( fmNode *n1, fmEdge *edge )
{
	list <fmEdge *>::iterator iter;
	fmEdge *anEdge;

	for ( iter = n1->connectedEdges->begin(); iter != n1->connectedEdges->end(); iter ++ )
	{
		anEdge = ( fmEdge * ) ( * iter );
		if ( anEdge->edgeID == edge->edgeID )
			return true;
	}
// 	return false;
}
