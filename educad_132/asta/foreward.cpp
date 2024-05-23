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
#include <list>
#include <vector>
#include <map>
#include "asta.h"
using namespace std;
#include "atlasDB.h"

//-------------------------------------------------------
// Add INPUT and INOUT pads to active nodes
//-------------------------------------------------------
void ASTA::addInputNodestoActiveList()
{
	staNode *aNode;
	int piNodesNumber = 0, ffNodesNumber = 0;

//   printf("\tAdd input nodes to active list #nodes = %d, ", _nodes.size());
	for ( unsigned i = 0; i < _nodes.size();i ++ )
	{
		aNode = _nodes[i];
		if ( ( aNode->type == primary_pin ) && ( aNode->dir == sta_pin_input_dir ) )
		{
			_activeList.push_back ( aNode );
//        fprintf ( staFP, "%s\t", aNode->name.c_str() );
			piNodesNumber++;
		}
		else if ( ( aNode->type == flipflop_pin ) && ( aNode->dir == sta_pin_output_dir ) )
		{
			_activeList.push_back ( aNode );
//       fprintf ( staFP, "%s\t", aNode->name.c_str() );
			ffNodesNumber++;
		}
	}
	fprintf ( staFP, "#PI = %d, ##FFout = %d\n",piNodesNumber, ffNodesNumber );
//   printf("#PrimaryI = %d, #FFout = %d\n",piNodesNumber, ffNodesNumber);
}

//-------------------------------------------------------
// Check that is the node in _tmpActiveList?
//-------------------------------------------------------
bool ASTA::isInList ( vector<staNode *> _tmpList, staNode *snkNode )
{
	for ( unsigned i = 0;i < _tmpList.size();i ++ )
		if ( _tmpList[i] == snkNode )
			return true;
	return false;
}


//-------------------------------------------------------
// Process a level of active nodes (one level of DFS)
//-------------------------------------------------------
bool ASTA::forewardProcessActiveNodes()
{
	staNode *srcNode, *snkNode;
	double newDAT;
	int PoCntr = 0, FiCntr = 0, InCntr = 0;

	for ( unsigned i = 0; i < _activeList.size();i ++ )
	{
		srcNode = _activeList[i];
		fprintf ( staFP, "\n\t\t%s --> ", srcNode->name.c_str() );
		for ( unsigned j = 0; j < srcNode->connections.size();j ++ )
		{
			snkNode = srcNode->connections[j];
			if ( j > 0 )
				fprintf ( staFP, "\n\t\t%s --> ", srcNode->name.c_str() );

			newDAT = srcNode->dat + calculateNewDelayWeight ( operationType, srcNode, snkNode );
			fprintf ( staFP, "%s : ", snkNode->name.c_str() );
			if ( ( snkNode->type == primary_pin )  && ( snkNode->dir == sta_pin_output_dir ) )
			{
//         fprintf ( staFP, "\nprobe 1\n" );

				if ( newDAT > snkNode->dat )
				{
					snkNode->dat = newDAT;
					PoCntr ++;
					fprintf ( staFP, "dat = %f (OV)", snkNode->dat );
				}
				else
					fprintf ( staFP, "dat = %f", snkNode->dat );
			}
			// sink is a flip flop input pin
			else if ( ( snkNode->type == flipflop_pin ) && ( snkNode->dir == sta_pin_input_dir ) )
			{
//         fprintf ( staFP, "\nprobe 2\n" );
				if ( newDAT > snkNode->dat )
				{
					snkNode->dat = newDAT;
					FiCntr ++;
					fprintf ( staFP, "dat = %f (OV)", snkNode->dat );
				}
				else
					fprintf ( staFP, "dat = %f", snkNode->dat );
			}
			// sink is an internal pin
			else
			{
				if ( newDAT > snkNode->dat )
				{
					snkNode->dat = newDAT;
					if ( !isInList ( _tmpActiveList, snkNode ) )
						_tmpActiveList.push_back ( snkNode );
					fprintf ( staFP, "dat = %f (OV)", snkNode->dat );
					InCntr ++;
				}
				else
					fprintf ( staFP, "dat = %f", snkNode->dat );
			}
		}
	}

	return true;
}

//-------------------------------------------------------
// Update the active nodes
//-------------------------------------------------------
void ASTA::updateActiveNodes()
{

	_activeList.erase ( _activeList.begin(), _activeList.end() );

	for ( unsigned i = 0; i < _tmpActiveList.size();i ++ )
	{
		_activeList.push_back ( _tmpActiveList[i] );
	}
	_tmpActiveList.erase ( _tmpActiveList.begin(), _tmpActiveList.end() );

}

//--------------------------------------------
// Checking the presence of an item in list
//--------------------------------------------
bool ASTA::thereIsNotInList ( list <staNode *> *_list, staNode *item )
{
	list <staNode *>::iterator iter;

	for ( iter = _list->begin(); iter != _list->end(); iter ++ )
	{
		if ( ( ( staNode * ) ( *iter ) ) == item )
			return false;
	}
	return true;
}
