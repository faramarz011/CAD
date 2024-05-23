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
// Constructor of ASTA
//-------------------------------------------------------
ASTA::ASTA ( char *staLogFileName )
{
	operationType = sta_postplace_opr;
	ignoreSpecialNets = false;
	staTimingWallRatio = 0.01;
	staFP = fopen ( staLogFileName,"w+t" );
	if ( staFP == NULL )
		printf ( "Cannot open log file.\n" );
}


//---------------------------------------------------------
// Get information about ASTA
//---------------------------------------------------------
char *ASTA::getInfo()
{
	char *str = new char[1000];

	sprintf ( str,"\n*************************************************************\n" );
	strcat ( str,"Atlas Statistical Timing Analyzer (ASTA), Ver. 2.1 \n" );
	strcat ( str,"September 2006.\n" );
	strcat ( str,"Ali Jahanian.\n\n" );
	strcat ( str,"Last updates:\n" );
	strcat ( str,"Debugging and checking pre and post place (22/01/2007)\n" );
	strcat ( str,"*************************************************************\n" );

	return str;
}

//---------------------------------------------------------
// Setting the variables of ASTA
//---------------------------------------------------------
char *ASTA::setVar ( char *varName, char *varValue )
{
	char *s = new char[100];
	if ( strcmp ( varName,"sta_operation_mode" ) == 0 )
	{
		if ( strcmp ( varValue,"pre_place" ) == 0 )
		{
			operationType = sta_preplace_opr;
			sprintf ( s, "%s is set to %s", varName, "pre_place" );
		}
		else if ( strcmp ( varValue,"post_place" ) == 0 )
		{
			operationType = sta_postplace_opr;
			sprintf ( s, "%s is set to %s", varName, "post_place" );
		}
		else
			sprintf ( s, "Error: invalid variable value <%s>", varValue );
	}
	else if ( strcmp ( varName,"sta_timing_wall" ) == 0 )
	{
		staTimingWallRatio = atof ( varValue );
		sprintf ( s, "%s is set to %f", varName, staTimingWallRatio );
	}
	else if ( strcmp ( varName,"ignore_special_nets" ) == 0 )
	{
		if ( strcmp ( varValue,"ON" ) == 0 )
			ignoreSpecialNets = true;
		else
			ignoreSpecialNets = false;
		sprintf ( s, "%s is set to %d", varName, ignoreSpecialNets );
	}
	else if ( strcmp ( varName,"special_net_name" ) == 0 )
	{
		specialNetNames.push_back ( varValue );
		sprintf ( s, "%s is added to special nets", varValue );
	}
	else if ( strcmp ( varName,"ignore_special_pins" ) == 0 )
	{
		if ( strcmp ( varValue,"ON" ) == 0 )
			ignoreSpecialPins = true;
		else
			ignoreSpecialPins = false;
		sprintf ( s, "%s is set to %d", varName, ignoreSpecialNets );
	}
	else if ( strcmp ( varName,"special_pin_name" ) == 0 )
	{
		specialPinNames.push_back ( varValue );
		sprintf ( s, "%s is added to special pins", varValue );
	}
	else
		sprintf ( s, "Error: unknown variable <%s>", varName );

	return s;
}

//-----------------
// Is output pin?
//-----------------
bool ASTA::isOutputPin ( staNode *pin )
{
	if ( ( pin->dir == sta_pin_inout_dir ) || ( pin->dir == sta_pin_output_dir ) )
		return true;
	else
		return false;
}

//-----------------
// Is input pin?
//-----------------
bool ASTA::isInputPin ( staNode *pin )
{
	if ( ( pin->dir == sta_pin_inout_dir ) || ( pin->dir == sta_pin_input_dir ) )
		return true;
	else
		return false;
}

//------------------------
// Main function of ASTA
//------------------------
bool ASTA::Analysis ( atlasDB *db )
{
	int levelCntr = 0;

	// Foreward tracing the netlist to propagate the dat
	fprintf ( staFP, "Foreward trace to propagate data arrival times ...\n" );
	addInputNodestoActiveList();
	while ( _activeList.size() > 0 )
	{
		fprintf ( staFP, "\n\n\tLevel = %d, #active nodes = %d\n", levelCntr, _activeList.size() );
		forewardProcessActiveNodes();
		updateActiveNodes();
		levelCntr++;
	}
	levelCntr--;

	// Backward tracing the netlist to back propagate drt and slack
	fprintf ( staFP, "\n\nBackward trace to propagate data required times ...\n" );
	_activeList.erase ( _activeList.begin(), _activeList.end() );
	addOutputNodestoActiveList();
	while ( _activeList.size() > 0 )
	{
		fprintf ( staFP, "\n\n----------------------\nLevel = %d, #active nodes = %d\n", levelCntr, _activeList.size() );
		backwardProcessActiveNodes();
		updateActiveNodes();
		levelCntr--;
	}
	fprintf ( staFP, "\n\n--------------------------------------------\n" );
	fprintf ( staFP, "Create nearcritical paths ...\n" );
	computeSlacks();
	createNearCriticalPaths ( staTimingWallRatio * getMaximumSlack() );
	createStaReports ( db );

	fclose ( staFP );
	return "OK";
}
