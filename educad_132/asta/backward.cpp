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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <map>

#include "asta.h"
using namespace std;
#include "atlasDB.h"

#define PRINT_PIN_NAMES 0

//--------------------------------
// Find most critical delay node
//--------------------------------
staNode *ASTA::getMostCriticalDelayNode()
{
	double maxWeight = -1;
	staNode *aNode, *maxNode = NULL;

	for ( unsigned i = 0; i < _nodes.size();i ++ )
	{
		aNode = _nodes[i];
		//     printf("node.weight = %f\n", aNode->dat);
		if ( aNode->dat > maxWeight )
		{
			maxWeight = aNode->dat;
			maxNode = aNode;
		}

	}

	//   printf("Node %s is maxNode with w=%f\n", maxNode->name.c_str(), maxNode->dat);
	return maxNode;
}

//-------------------------------------------------------
// Add OUTPUT pads to active nodes
//-------------------------------------------------------
void ASTA::addOutputNodestoActiveList()
{
	staNode *aNode;
	int poNodesNumber = 0, ffNodesNumber = 0;
	double TW = getMostCriticalDelayNode()->dat;

//   printf("\tAdd input nodes to active list #nodes = %d, ", _nodes.size());
	for ( unsigned i = 0; i < _nodes.size();i ++ )
	{
		aNode = _nodes[i];
		if ( ( aNode->type == primary_pin ) && ( aNode->dir == sta_pin_output_dir ) )
		{
			_activeList.push_back ( aNode );
			aNode->drt = TW;
			poNodesNumber++;
		}
		else if ( ( aNode->type == flipflop_pin ) && ( aNode->dir == sta_pin_input_dir ) )
		{
			_activeList.push_back ( aNode );
			aNode->drt = TW;
			ffNodesNumber++;
		}
	}
	fprintf ( staFP, "#PO = %d, ##FFin = %d\n",poNodesNumber, ffNodesNumber );
}



//-------------------------------------------------------
// Backward Process a level of active nodes (one level of DFS)
//-------------------------------------------------------
bool ASTA::backwardProcessActiveNodes()
{
	staNode *srcNode, *snkNode;
	double newDRT;
	int PoCntr = 0, FiCntr = 0, InCntr = 0;

	fprintf ( staFP, "\n\n**********************************************\n" );
	fprintf ( staFP, "Calculating the data required time\n" );
	fprintf ( staFP, "**********************************************\n" );
	for ( unsigned i = 0; i < _activeList.size();i ++ )
	{
		snkNode = _activeList[i];
		for ( unsigned j = 0; j < snkNode->prevs.size();j ++ )
		{
			srcNode = snkNode->prevs[j];

			fprintf ( staFP, "\n\t\t%s ", srcNode->name.c_str() );
			newDRT = snkNode->drt - calculateNewDelayWeight ( operationType, srcNode, snkNode );

			if ( ( srcNode->type == primary_pin )  && ( srcNode->dir == sta_pin_input_dir ) )
			{
				if ( newDRT < srcNode->drt )
				{
					srcNode->drt = newDRT;
					PoCntr ++;
				}
				fprintf ( staFP, "(DRT=%f)", srcNode->drt );
			}
			// sink is a flip flop input pin
			else if ( ( srcNode->type == flipflop_pin ) && ( srcNode->dir == sta_pin_output_dir ) )
			{
				if ( newDRT < srcNode->drt )
				{
					srcNode->drt = newDRT;
					FiCntr ++;
				}
				fprintf ( staFP, "(DRT=%f)", srcNode->drt );
			}
			// sink is an internal pin
			else
			{
				if ( newDRT < srcNode->drt )
				{
					srcNode->drt = newDRT;
					if ( !isInList ( _tmpActiveList, srcNode ) )
						_tmpActiveList.push_back ( srcNode );
					InCntr ++;
				}
				fprintf ( staFP, "(DRT=%f)", srcNode->drt );
			}
			fprintf ( staFP, " --> %s (DRT=%f)", snkNode->name.c_str(), snkNode->drt );
		}
	}
	return true;
}

//-------------------------------------------------------
// Add OUTPUT pads to active nodes
//-------------------------------------------------------
void ASTA::computeSlacks()
{
	FILE *slackFile;
	unsigned wireSegmentNumber = 0;
	double TW;

	TW = getMostCriticalDelayNode()->dat;

	fprintf ( staFP, "\n\n**********************************************\n" );
	fprintf ( staFP, "Calculating the slack\n" );
	fprintf ( staFP, "**********************************************\n" );

	for ( unsigned i = 0; i < _nodes.size();i ++ )
	{
		staNode *aNode = _nodes[i];
		aNode->slack = aNode->drt - aNode->dat;
		if ( aNode->slack  < 0 )
			aNode->slack = 0;
		if ( aNode->slack  > TW )
			aNode->slack = TW;
	}
}

//----------------------
// Find minimum slack
//----------------------
double ASTA::findMinimumSlack()
{
	double minSlack = 1000000000;
	for ( unsigned i = 0; i < _nodes.size();i ++ )
	{
		staNode *aNode = _nodes[i];
		if ( ( ( aNode->type == primary_pin ) && ( aNode->dir == sta_pin_output_dir ) ) || ( ( aNode->type == flipflop_pin ) && ( aNode->dir == sta_pin_input_dir ) ) )
			if ( aNode->slack < minSlack )
				minSlack = aNode->slack;
	}
	return minSlack;
}

//-------------------------------------------------------
// Backward Process a level of active nodes (one level of DFS)
//-------------------------------------------------------
void ASTA::createNearCriticalPaths ( double cSlack )
{
	unsigned pathNum = 0;

	/***************************************
	Add primary output pins to active paths
	****************************************/
	for ( unsigned i = 0; i < _nodes.size();i ++ )
	{
		staNode *aNode = _nodes[i];
		if ( ( ( aNode->type == primary_pin ) && ( aNode->dir == sta_pin_output_dir ) ) || ( ( aNode->type == flipflop_pin ) && ( aNode->dir == sta_pin_input_dir ) ) )
		{
// 			printf ( "Node slack = %g , Critical slack = %g\n", aNode->slack, cSlack );
			if ( aNode->slack <= cSlack )
			{
				staPath *newPath = new staPath;
				newPath->id = pathNum ++;
				newPath->nodes.insert ( newPath->nodes.begin(), aNode );
				activePaths.push_back ( newPath );
			}
		}
	}
	fprintf ( staFP, "\nNumber of critical end nodes = %d\n", activePaths.size() );

	for ( unsigned i = 0; i < activePaths.size();i ++ )
	{
		staPath *aPath = activePaths[i];
		staNode * aNode = aPath->nodes[0];
		fprintf ( staFP, "\t- %s  slack = %f\n", aNode->name.c_str(), aNode->slack );
	}

	/***************************************
	         Process active nodes
	****************************************/
	fprintf ( staFP, "Start backward tracing to create the critical paths\n-------------------------\n" );
	while ( activePaths.size() > 0 )
	{
		staPath *currPath = activePaths[0];
		staNode *snkNode = currPath->nodes[currPath->nodes.size()-1];
		fprintf ( staFP, "\tActive list has %d paths\n", activePaths.size() );
		fprintf ( staFP, "\t\tCurrent path: id = %d , Size = %d\n", currPath->id, currPath->nodes.size() );

		for ( unsigned j = 0; j < snkNode->prevs.size();j ++ )
		{
			staNode *srcNode = snkNode->prevs[j];
			fprintf ( staFP, "\t\t\tSRC node: %s , slack = %f\n", srcNode->name.c_str(), srcNode->slack );

			/****************
			 Start STA pins
			*****************/
			if ( ( ( srcNode->type == primary_pin )  && ( srcNode->dir == sta_pin_input_dir ) ) || ( ( srcNode->type == flipflop_pin ) && ( srcNode->dir == sta_pin_output_dir ) ) )
			{
				currPath->nodes.push_back ( srcNode );
				currPath->maxSlack = pathMaxSlack ( currPath );
				finalPaths.push_back ( currPath );
				fprintf ( staFP, "\t\t\t\tNode is PI, path %d with %d nodes is finalized , Final list has %d paths.\n", currPath->id, currPath->nodes.size(), finalPaths.size() );
			}
			/****************
			internal STA pins
			*****************/
			else
			{
				if ( srcNode->slack <= cSlack )
				{
					staPath *newPath= new staPath;
					newPath->id = pathNum ++;
					for ( unsigned ii = 0; ii < currPath->nodes.size();ii++ )
						newPath->nodes.push_back ( currPath->nodes[ii] );
					newPath->nodes.push_back ( srcNode );
					activePaths.push_back ( newPath );
					fprintf ( staFP, "\t\t\t\tNode is internal, new path id=%d is generated with %d nodes.\n", newPath->id, newPath->nodes.size() );
				}
			}
		}
		activePaths.erase ( activePaths.begin() );
		//delete currPath;
		fprintf ( staFP, "\tActive paths.SIZE = %d\n", activePaths.size() );
		fprintf ( staFP, "\tFinal  paths.SIZE = %d\n", finalPaths.size() );

		for ( unsigned i = 0; i < finalPaths.size();i ++ )
		{
			staPath *aPath = finalPaths[i];
			fprintf ( staFP, "\t*******Final path[%d]: id = %d  #nodes = %d.\n", aPath->id, aPath->nodes.size() );
		}
	}

}


double ASTA::getMaximumSlack()
{
	unsigned wireSegmentNumber = 0;
	double maxSlack = 0;

	for ( unsigned i = 0; i < _nodes.size();i ++ )
	{
		staNode *aNode = _nodes[i];
		wireSegmentNumber += aNode->prevs.size();
		if ( aNode->slack > maxSlack )
			maxSlack = aNode->slack;
	}
	return maxSlack;
}

bool softFunc ( staPath *p1, staPath *p2 )
{
	if ( p1->maxSlack < p2->maxSlack )
		return true;
	else
		return false;
}

float ASTA::pathMaxSlack ( staPath *p )
{
	float maxSlack = 0;
	for ( unsigned i = 0; i < p->nodes.size();i ++ )
		if ( p->nodes[i]->slack > maxSlack )
			maxSlack = p->nodes[i]->slack;
	return maxSlack;
}

void ASTA::createStaReports ( atlasDB *db )
{
	FILE *slackFile, *pathFile;
	unsigned wireSegmentNumber = 0;
	double TW;

	slackFile = fopen ( "sta_slacks.txt","w+t" );
	pathFile = fopen ( "sta_paths.txt","w+t" );

	TW = getMostCriticalDelayNode()->dat;


	if ( ( slackFile == NULL ) || ( pathFile == NULL ) )
	{
		printf ( "Cannot create slack or path report file\n" );
		exit ( 1 );
	}
	printf ( "\n\n\tDelay report:\n" );
	printf ( "\t\tMost critical delay = %f ps\n", TW );
	printf ( "\t\tNear critical slack range: 0ps ... %fps\n",staTimingWallRatio * getMaximumSlack() );
	printf ( "\t\tNumber of near critical paths = %d\n", finalPaths.size() );
	printf ( "\t\tCritical paths  are written into sta_paths.txt file.\n" );
	printf ( "\t\tDetailed slacks are written into sta_slacks.txt file.\n" );

	double maxSlack = 0;
	for ( unsigned i = 0; i < _nodes.size();i ++ )
	{
		staNode *aNode = _nodes[i];
		wireSegmentNumber += aNode->prevs.size();
		if ( aNode->slack > maxSlack )
			maxSlack = aNode->slack;
	}

	fprintf ( slackFile, "#WIRESEGMENTS %d MINSLACK 0 MAXSLACK %f\n", wireSegmentNumber , TW );
	db->resetNetListPos();
	if ( db->hasAnyNetInNetList() == true )
	{
		for ( int i = 0; i < db->getNetNum(); i ++ )
		{
			string netName = db->getNetName();
			for ( unsigned i = 0; i < _nodes.size();i ++ )
			{
				staNode *aNode = _nodes[i];
				if ( aNode->net == netName )
				{
					if ( ( ( aNode->type == primary_pin ) && ( aNode->dir == sta_pin_input_dir ) ) ||
					        ( ( aNode->type == flipflop_pin ) && ( aNode->dir == sta_pin_output_dir ) ) ||
					        ( ( aNode->type == internal_pin ) && ( aNode->dir == sta_pin_output_dir ) ) )

						fprintf ( slackFile, "%s SRC %s.%s %f\n",aNode->net.c_str(), aNode->inst.c_str(), aNode->pin.c_str(), aNode->slack );
					else
						fprintf ( slackFile, "%s SNK %s.%s %f\n",aNode->net.c_str(), aNode->inst.c_str(), aNode->pin.c_str(), aNode->slack );
				}
			}
			db->netListPosGoForward();
		}
	}

	sort ( finalPaths.begin(), finalPaths.end(), softFunc );
	fprintf ( pathFile, "%d", finalPaths.size() );
// 	fprintf ( staFP,"%d", finalPaths.size() );
	for ( unsigned ii = 0; ii < finalPaths.size();ii ++ )
	{
		staPath *path = finalPaths[ii];
		fprintf ( pathFile, "\n%d %f ", path->nodes.size(), path->maxSlack );
// 		fprintf ( staFP, "\n\t path %d: id = %d , size = %d  ", ii, path->id, path->nodes.size() );
		for ( int jj = path->nodes.size()-1; jj >= 0;jj -- )
		{
			staNode *node = path->nodes[jj];
//			fprintf ( pathFile, " %f %f %f", node->dat, node->drt, node->slack );
// 			fprintf ( staFP, "%s.%s  ", node->inst.c_str(), node->pin.c_str()  );
#ifdef PRINT_PIN_NAMES
			fprintf ( pathFile, "%s.%s ", node->inst.c_str(),  node->pin.c_str(), node->slack );
#else
			fprintf ( pathFile, "%s", node->inst.c_str() );
#endif
			if ( jj > 0 )
				fprintf ( pathFile, " " );

		}
		fprintf ( staFP, "\n" );
	}

	fclose ( slackFile );
	fclose ( pathFile );
}
