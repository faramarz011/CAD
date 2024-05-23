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

#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <iostream>
#include <list>
#include <vector>
#include <map>

#include "gr.h"

using namespace std;
#include <atlasDB.h>

//------------------------------
// Simple global router
//------------------------------
void simpleGlobalRouter::simpleGrRoute ( )
{
	list <simpleGrNet *>::iterator iter1;
	simpleGrNet *aNet;
	list <simpleNetTerminal *>::iterator iter2;
	simpleNetTerminal *srcTerm;
	simpleGrNode *currNode;
	int i = 0;
	unsigned noSourceNets = 0;

	printf ( "\tStart of global routing, #nets = %d:\n", nets.size() );
//	for ( unsigned k = 0;k < nets.size();k ++ )
//	{
//		aNet = nets[k];
//		PrimMST ( aNet );
//	}

	for ( unsigned k = 0;k < nets.size();k ++ )
	{
		aNet = nets[k];

		srcTerm = NULL;
		for ( iter2 = aNet->terminalBins->begin(); iter2 != aNet->terminalBins->end(); iter2 ++ )
		{
			simpleNetTerminal *aTerm = ( simpleNetTerminal * ) ( * iter2 );
			if ( aTerm-> isSrource == true )
			{
				srcTerm = aTerm;
				break;
			}
		}

		if ( srcTerm == NULL )  // Ignore the nets with no source
		{
			noSourceNets ++;
			//printf ( "Net %s has no source, net is ignored\n", aNet->netName.c_str() );
			continue;
		}


		for ( iter2 = aNet->terminalBins->begin(); iter2 != aNet->terminalBins->end(); iter2 ++ )
		{
			simpleNetTerminal *snkTerm = ( simpleNetTerminal * ) ( * iter2 );
// 			printf ( "\t\tSRC @ <%d,%d> and SNK @ <%d,%d> \n", srcTerm->row, srcTerm->col, snkTerm->row, snkTerm->col );
			if ( snkTerm != srcTerm )
			{
				currNode = nodes[srcTerm->row * columnNumber + srcTerm->col];
				if ( srcTerm->col < snkTerm->col )
				{
					for ( int i = srcTerm->col+1;i <= snkTerm->col;i ++ )
					{
						simpleGrNode *newNode = nodes[srcTerm->row * columnNumber + i];
						simpleGrEdge *currEdge = findEdge ( currNode, newNode );
						if ( currEdge == NULL )
						{
							printf ( "ERROR: illegal edge\n" );
							exit ( 1 );
						}
						currEdge->used_tracks ++;
						aNet->routingTree->push_back ( currEdge );
// 						printf ( "\t\t\t<%d,%d> -> <%d,%d>\n", currEdge->node1->centerRow, currEdge->node1->centerCol, currEdge->node2->centerRow,currEdge->node2->centerCol );
						currNode = newNode;
					}
				}
				else
				{
					for ( int i = srcTerm->col-1;i >= ( int ) ( snkTerm->col );i -- )
					{
						simpleGrNode *newNode = nodes[srcTerm->row * columnNumber + i];
						simpleGrEdge *currEdge = findEdge ( currNode, newNode );
						if ( currEdge == NULL )
						{
							printf ( "ERROR: illegal edge\n" );
							exit ( 1 );
						}
						currEdge->used_tracks ++;
						aNet->routingTree->push_back ( currEdge );
// 						printf ( "\t\t\t<%d,%d> -> <%d,%d>\n", currEdge->node1->centerRow, currEdge->node1->centerCol, currEdge->node2->centerRow,currEdge->node2->centerCol );
						currNode = newNode;
					}
				}

				if ( srcTerm->row < snkTerm->row )
				{
					for ( int i = srcTerm->row+1;i <= snkTerm->row;i ++ )
					{
						simpleGrNode *newNode = nodes[i * columnNumber + snkTerm->col];
						simpleGrEdge *currEdge = findEdge ( currNode, newNode );
						if ( currEdge == NULL )
						{
							printf ( "ERROR: illegal edge\n" );
							exit ( 1 );
						}
						currEdge->used_tracks ++;
						aNet->routingTree->push_back ( currEdge );
// 						printf ( "\t\t\t<%d,%d> -> <%d,%d>\n", currEdge->node1->centerRow, currEdge->node1->centerCol, currEdge->node2->centerRow,currEdge->node2->centerCol );
						currNode = newNode;
					}
				}
				else
				{
					for ( int i = srcTerm->row-1;i >= int ( snkTerm->row );i -- )
					{
						simpleGrNode *newNode = nodes[i * columnNumber + snkTerm->col];
						simpleGrEdge *currEdge = findEdge ( currNode, newNode );
						if ( currEdge == NULL )
						{
							printf ( "ERROR: illegal edge\n" );
							exit ( 1 );
						}
						currEdge->used_tracks ++;
						aNet->routingTree->push_back ( currEdge );
// 						printf ( "\t\t\t<%d,%d> -> <%d,%d>\n", currEdge->node1->centerRow, currEdge->node1->centerCol, currEdge->node2->centerRow,currEdge->node2->centerCol );
						currNode = newNode;
					}
				}
			}
			srcTerm = snkTerm;
		}
	}
	printf ( "\tTotaly %d nets has no source\n", noSourceNets );
}

simpleNetTerminal *simpleGlobalRouter::findMinLambdaTerminal ( simpleGrNet *aNet )
{
	list <simpleNetTerminal *>::iterator iter1;
	simpleNetTerminal *term1, *minTerm = NULL;
	unsigned minLambda = 1000000000;

	for ( iter1 = aNet->terminalBins->begin(); iter1 != aNet->terminalBins->end(); iter1 ++ )
	{
		term1 = ( simpleNetTerminal * ) ( * iter1 );
		if ( term1->tag == false )
		{
			if ( term1->Lambda < minLambda )
			{
				minTerm = term1;
				minLambda = term1->Lambda;
			}
		}
	}
	return minTerm;
}

unsigned ABS ( int a )
{
	if ( a >= 0 )
		return a;
	else
		return ( -a );
}

bool simpleGlobalRouter::PrimMST ( simpleGrNet *aNet )
{
	list <simpleNetTerminal *>::iterator iter1, iter2;
	simpleNetTerminal *term1, *term2;
	int newDistance, i;

	if ( aNet->terminalBins->size() == 0 )
	{
		printf ( "Net %s has no terminal.\n", aNet->netName.c_str() );
		return false;
	}
	printf ( "\nStart of MST construction.\n" );

	// Initialization of parameters
	for ( i = 0, iter1 = aNet->terminalBins->begin(); iter1 != aNet->terminalBins->end(); iter1 ++ , i ++ )
	{
		term1 = ( simpleNetTerminal * ) ( * iter1 );
		term1->tag = false;
		term1->Lambda = 1000000000;
		term1->parentID = -1;
	}
	// Set parameters of source neighbors
	term1 = ( simpleNetTerminal * ) ( * ( aNet->terminalBins->begin() ) );
	term1->Lambda = 0;
	term1->tag = true;
	term1->parentID = term1->id;
	for ( iter2 = aNet->terminalBins->begin() ; iter2 != aNet->terminalBins->end(); iter2 ++ )
	{
		term2 = ( simpleNetTerminal * ) ( * iter2 );

		if ( term2->tag == false )
		{
			term2->Lambda = ABS ( term1->col - term2->col ) + ABS ( term1->row - term2->row ) ;
			term2->parentID = term1->id;
		}
	}

	for ( iter1 = aNet->terminalBins->begin(); iter1 != aNet->terminalBins->end(); iter1 ++ )
	{
		term1 = findMinLambdaTerminal ( aNet );
		if ( term1 == NULL )
			break;
		// 		printf ( "SRC %d : L = %d  P=%d\n", term1->id, term1->Lambda, term1->parentID );

		for ( iter2 = aNet->terminalBins->begin(); iter2 != aNet->terminalBins->end(); iter2 ++ )
		{
			term2 = ( simpleNetTerminal * ) ( * iter2 );
			if ( ( term2->tag == false ) && ( term2->id != term1->id ) )
			{
				newDistance = ABS ( term1->col - term2->col ) + ABS ( term1->row - term2->row );
				if ( term2->Lambda > newDistance )
				{
					term2->parentID = term1->id;
					term2->Lambda = newDistance;
				}
			}
		}
		term1->tag = true;
	}
	printMST ( aNet );
	return true;
}

void simpleGlobalRouter::printMST ( simpleGrNet *aNet )
{
	list <simpleNetTerminal *>::iterator iter1;
	simpleNetTerminal *term1;

	printf ( "\tNet %s\n", aNet->netName.c_str() );
	for ( iter1 = aNet->terminalBins->begin() ; iter1 != aNet->terminalBins->end(); iter1 ++ )
	{
		term1 = ( simpleNetTerminal * ) ( * iter1 );
		printf ( "\t\tNode %d @<%d , %d> L = %d  P=%d", term1->id, term1->col, term1->row, term1->Lambda, term1->parentID );
		if ( term1->parentID == term1->id )
			printf ( " ROOT" );
		printf ( "\n" );
	}
}

void simpleGlobalRouter::reportCapacityViolations( )
{
	float c60 = 0, c80 = 0, c90 = 0, c100 = 0, c150 = 0, c200 = 0;
	unsigned TWL = 0;
	for ( unsigned k = 0;k < edges.size();k ++ )
	{
		simpleGrEdge *anEdge = edges[k];
		float ov = 100.0 * ( float ) ( ( float ) anEdge->used_tracks / ( float ) anEdge->capacity ) ;
		if ( ov <= 60 )
			c60 ++;
		else if ( ov <= 80 )
			c80 ++;
		else if ( ov <= 90 )
			c90 ++;
		else if ( ov <= 100 )
			c100 ++;
		else if ( ov < 150 )
			c150 ++;
		else
			c200 ++;
		TWL += anEdge->used_tracks  ;
	}
	printf ( "\tCongestion report:\n" );
	printf ( "\t\tBins with congestion less than 60%%     = %5.1f%%\n", 100.0* ( c60 / edges.size() ) );
	printf ( "\t\tBins with congestion from 61%% to 80%%   = %5.1f%%\n", 100.0* ( c80 / edges.size() ) );
	printf ( "\t\tBins with congestion from 81%% to 90%%   = %5.1f%%\n", 100.0* ( c90 / edges.size() ) );
	printf ( "\t\tBins with congestion from 91%% to 100%%  = %5.1f%%\n", 100.0* ( c100 / edges.size() ) );
	printf ( "\t\tBins with congestion from 101%% to 150%% = %5.1f%%\n", 100.0* ( c150 / edges.size() ) );
	printf ( "\t\tBins with congestion more than 150%%    = %5.1f%%\n", 100.0* ( c200 / edges.size() ) );
	printf ( "\t\tTotal used capacities (TWL) after global routing = %d tracks.\n", TWL );

	/*	printf ( "\n\nCapacity violations:\n" );
		for ( unsigned k = 0;k < edges.size();k ++ )
		{
			simpleGrEdge *anEdge = edges[k];
			float ov = float(anEdge->used_tracks) / float(anEdge->capacity);
			if ( ov > 1 )
				printf ( "\t Edge <%d,%d> --> <%d,%d>, Overflow = %5.1f tracks.\n", anEdge->node1->centerRow, anEdge->node1->centerCol, anEdge->node2->centerRow, anEdge->node2->centerCol, ov);

		}*/
}


