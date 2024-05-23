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
***           included in all copies or sub`ntial portions of the Software.
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

#define MAXFLOAT  3.40282347e+38F

//-------------------------------------------------------
// Create an empty node
//-------------------------------------------------------
staNode *ASTA::createNode()
{
	staNode *aNode = new staNode;
	aNode->center.x = aNode->center.y = -1;
	aNode->name = "";
	aNode->macro = "";
	aNode->net = "NC";
	aNode->dir = sta_none_pin_dir;
	aNode->index = -1;
	aNode->type = none_pin;
	aNode->dat= 0.0;
	aNode->drt= MAXFLOAT;
	aNode->slack= 0.0;
	return aNode;
}

//-----------------------------------------------------------
// check the special nets
//-----------------------------------------------------------
bool ASTA::isAstaSpecialNet ( string netName )
{
	for ( unsigned i = 0; i < specialNetNames.size();i ++ )
	{
		if ( specialNetNames[i] == netName )
			return true;
	}
	return false;
}

//-----------------------------------------------------------
// check the special pins
//-----------------------------------------------------------
bool ASTA::isSpecialPin ( string pinName )
{
	for ( unsigned i = 0; i < specialPinNames.size();i ++ )
	{
		if ( specialPinNames[i] == pinName )
			return true;
	}
	return false;
}

//-------------------------------------------------------
// Check the connection list of a node
//-------------------------------------------------------
bool ASTA::notPresentConnection ( staNode *srcNode, staNode *snkNode )
{
	vector<staNode *>::iterator iter;
	staNode *aNode;
	for ( iter = srcNode->connections.begin(); iter != srcNode->connections.end(); iter ++ )
	{
		aNode = ( staNode * ) ( *iter );
		if ( aNode == snkNode )
			return false;
	}
	return true;
}

//-------------------------------------------------------
// Looking instances for Flip Flops
//-------------------------------------------------------
bool ASTA::checkForFF ( atlasDB *db, string instName )
{
	db->setActiveInstanceRandomAccess ( instName );
	db->setActiveMacroRandomAccess ( db->getInstanceMacroName() );

	db->resetMacroPinListPos();                 // goto first pin of current instance
	if ( db->hasAnyPinInMacroPinList() == true )
	{
		for ( int j = 0; j < db->getMacroPinsNum();j ++ ) // sweep in pins of macro
		{
			if ( db->getMacroPinUse() == pin_clock_use )
			{
				return true;
			}
			db->macroPinListGoForward();
		}
	}
	return false;
}

//-------------------------------------------------------
// Looking instances for Flip Flops
//-------------------------------------------------------
string ASTA::findOutputPin ( atlasDB *db, string instName )
{
	db->setActiveInstanceRandomAccess ( instName );
	db->setActiveMacroRandomAccess ( db->getInstanceMacroName() );
	if ( db->getMacroType() == macro_core_type )
	{
		db->resetMacroPinListPos();                 // goto first pin of current instance
		for ( int j = 0; j < db->getMacroPinsNum();j ++ ) // sweep in pins of macro
		{
			if ( ( db->getMacroPinDirection() == pin_output_dir ) || ( db->getMacroPinDirection() == pin_inout_dir ) )
				return db->getMacroPinName();
			db->macroPinListGoForward();
		}
	}
	return NULL;
}

//-------------------------------------------------------
// Create nodes for inout pins of cells
//-------------------------------------------------------
void ASTA::createInOutNodes ( int Index, bool isFF, string nodeName, string nodeMacroName, string nodePinName, double x, double y, list <staNode *> *nList )
{
	staNode *aNode;
	aNode = createNode();
	aNode->index = Index;
	if ( isFF == true )
		aNode->type = flipflop_pin;
	else
		aNode->type = internal_pin;

	aNode->name = nodeName;
	aNode->macro = nodeMacroName;
	aNode->pin = nodePinName;
	aNode->center.x = x;
	aNode->center.y = y;
	aNode->dir = sta_pin_inout_dir;
	nList->push_back ( aNode );
	nodeMap[aNode->name] = aNode;
//   printf("Node %s is added.\n", aNode->name.c_str());
}

//-------------------------------------------------------
// Create nodes for output pins of cells
//-------------------------------------------------------
void ASTA::createOutputNodes ( int Index, bool isFF, string nodeName, string nodeInstName, string nodeMacroName, string nodePinName, double x, double y, list <staNode *> *nList )
{
	staNode *aNode;
	aNode = createNode();
	aNode->index = Index;
	if ( isFF == true )
		aNode->type = flipflop_pin;
	else
		aNode->type = internal_pin;
	aNode->name = nodeName;
	aNode->macro = nodeMacroName;
	aNode->inst = nodeInstName;
	aNode->pin = nodePinName;
	aNode->center.x = x;
	aNode->center.y = y;
	aNode->dir = sta_pin_output_dir;
	nList->push_back ( aNode );
	nodeMap[aNode->name] = aNode;
//   printf("Node %s is added.\n", aNode->name.c_str());
}

//-------------------------------------------------------
// Create nodes for input pins of cells
//-------------------------------------------------------
void ASTA::createInputNodes ( int Index, bool isFF, string nodeName, string nodeInstName, string nodeMacroName, string nodePinName, double x, double y, list <staNode *> *nList )
{
	staNode *aNode;
	aNode = createNode();
	aNode->index = Index;
	if ( isFF == true )
		aNode->type = flipflop_pin;
	else
		aNode->type = internal_pin;
	aNode->name = nodeName;
	aNode->macro = nodeMacroName;
	aNode->inst = nodeInstName;
	aNode->pin = nodePinName;
	aNode->center.x = x;
	aNode->center.y = y;
	aNode->dir = sta_pin_input_dir;
	nList->push_back ( aNode );
	nodeMap[aNode->name] = aNode;
//   printf("Node %s is added.\n", aNode->name.c_str());

}

//-------------------------------------------------------
// Connet input pins of a cell to its output pins
//-------------------------------------------------------
void ASTA::connectInputNodesToOutputNodes ( list <staNode *> *oList, list <staNode *> *iList )
{
	staNode *iNode, *oNode, *ioNode;
	list <staNode *>::iterator iIter, oIter, ioIter;

	for ( oIter = oList->begin(); oIter != oList->end(); oIter ++ )
	{
		oNode = ( staNode * ) ( *oIter );

		for ( iIter = iList->begin(); iIter != iList->end(); iIter ++ )
		{
			iNode = ( staNode * ) ( *iIter );
			iNode->connections.push_back ( oNode );
			oNode->prevs.push_back ( iNode );
			//       fprintf(staFP, "\tNode %s ---> %s \n", iNode->name.c_str(), oNode->name.c_str());
		}
	}
}

//-------------------------------------------------------
// Copy sub-nodes of a gate into nodes
//-------------------------------------------------------
void ASTA::copyGateListsIntoNodes ( list <staNode *> *oList, list <staNode *> *iList )
{
	staNode *aNode;
	list <staNode *>::iterator iter;
	int cntr = 0;

	for ( iter = iList->begin(); iter != iList->end(); iter ++ )
	{
		aNode = ( staNode * ) ( * iter );
		_nodes.push_back ( aNode );
		cntr ++;
	}
	for ( iter = oList->begin(); iter != oList->end(); iter ++ )
	{
		aNode = ( staNode * ) ( * iter );
		_nodes.push_back ( aNode );
		cntr ++;
	}
}

//-------------------------------------------------------
// Check the number of previous links of nodes
// only output pins of gates may be have more
// than one prev.
//-------------------------------------------------------
bool ASTA::checkPrevLinks()
{
	int m[1000];
	unsigned maxFanIn = 0;
	staNode *aNode;
	vector<staNode *>::iterator iter;
	for ( int i = 0;i < 1000;i ++ )
		m[i] = 0;

	for ( iter = _nodes.begin();iter != _nodes.end(); iter ++ )
	{
		aNode = ( staNode * ) ( *iter );
		if ( aNode->prevs.size() > maxFanIn )
			maxFanIn = aNode->prevs.size();
		m[aNode->prevs.size() ] ++;
		if ( aNode->prevs.size() > 1 )
		{
			if ( aNode->type == primary_pin )
				printf ( "Connection error, PI %s has multiple source\n", aNode->name.c_str() );
			else if ( ( aNode->type == internal_pin ) || ( aNode->type == flipflop_pin ) )
			{
				if ( aNode->dir == sta_pin_input_dir )
					printf ( "Connection error, input pin %s has multiple source\n", aNode->name.c_str() );
			}
		}
	}

//   for ( unsigned i = 0;i < maxFanIn;i ++ )
//     fprintf ( staFP, "\tThe number of nodes with fan-in %d = %d\n", i, m[i] );

	return true;
}
//-------------------------------------------------------
// Read netlist from capo and create STA netlist
//-------------------------------------------------------
bool ASTA::readNetlist ( atlasDB *db )
{
	staNode *aNode, *srcNode = NULL, *snkNode = NULL;
	int pinNum, outpins  = 0, inpins  = 0;
	bool isFF;
	bool netHasSource = false, netHasSink = false;
	string outPinName, nodeName, nodeMacroName, nodeInstName;
	double nodeX, nodeY;
	list <staNode *> *gateOutPinList, *gateInPinList;
	int nodeIndex = 0;

	gateOutPinList = new list <staNode *>;
	gateInPinList = new list <staNode *>;

	//-----------------------
	// Create Internal Nodes
	//-----------------------
	db->resetInstanceListPos();
	if ( db->hasAnyInstanceInInstanceList() == true )
	{

		db->resetInstanceListPos();
		for ( int i = 0;i < db->getInstanceNum();i ++ )
		{
			isFF = checkForFF ( db, db->getInstanceName() );
			//       fprintf(staFP, "*********************\nInst %s , isFF = %d\n", db->getInstanceName().c_str(), isFF);

			nodeMacroName = db->getInstanceMacroName();
			db->setActiveMacroRandomAccess ( nodeMacroName );
			nodeX = db->getInstanceLeft() + db->getMacroWidth() / 2;
			nodeY = db->getInstanceTop() + db->getMacroHeight() / 2;

			// create list of output pins
			gateOutPinList->erase ( gateOutPinList->begin(), gateOutPinList->end() );
			gateInPinList->erase ( gateInPinList->begin(), gateInPinList->end() );

			db->resetMacroPinListPos();
			if ( db->hasAnyPinInMacroPinList() == true )
			{
				for ( int j = 0;j < db->getMacroPinsNum();j ++ )
				{

					if ( ( db->getMacroPinUse() != pin_ground_use ) && ( db->getMacroPinUse() != pin_power_use ) )
					{
						nodeName = db->getInstanceName() + "__" + db->getMacroPinName();
						nodeInstName = db->getInstanceName();
						if ( db->getMacroPinDirection() == pin_output_dir )
							createOutputNodes ( nodeIndex, isFF, nodeName, nodeInstName, nodeMacroName, db->getMacroPinName(), nodeX, nodeY, gateOutPinList );
					}
					db->macroPinListGoForward();
				}
			}

			// create list of input pins
			db->resetMacroPinListPos();
			if ( db->hasAnyPinInMacroPinList() == true )
			{
				for ( int j = 0;j < db->getMacroPinsNum();j ++ )
				{
					if ( ( db->getMacroPinUse() != pin_ground_use ) && ( db->getMacroPinUse() != pin_power_use ) )
					{
						nodeName = db->getInstanceName() + "__" + db->getMacroPinName();
						nodeInstName = db->getInstanceName();
						if ( db->getMacroPinDirection() == pin_input_dir )
						{
							if ( isFF )
							{
								if ( db->getMacroPinName() == "D" )
									createInputNodes ( nodeIndex, isFF, nodeName, nodeInstName, nodeMacroName, db->getMacroPinName(), nodeX, nodeY, gateInPinList );
							}
							else
							{
								createInputNodes ( nodeIndex, isFF, nodeName, nodeInstName, nodeMacroName, db->getMacroPinName(), nodeX, nodeY, gateInPinList );
							}
						}
					}
					db->macroPinListGoForward();
				}
			}

			// connect input pins to output pins

			connectInputNodesToOutputNodes ( gateOutPinList, gateInPinList );
			copyGateListsIntoNodes ( gateOutPinList, gateInPinList );

			nodeIndex ++;
			db->InstanceListPosGoForward();
		}
	}
	printf ( "\tReading internal nodes, OK.\n" );

	//---------------------------------
	// Create I/O nodes from I/O pins
	//---------------------------------
	db->resetGlobalPinListPos();
	if ( db->hasAnyGlobalPinInGlobalPinList() )
	{
		for ( int i = 0;i < db->getGlobalPinNum();i ++ )
		{
			if ( isSpecialPin ( db->getGlobalPinName() ) == true )
			{
				fprintf ( staFP, "Global pin %s is rejected.\n", db->getGlobalPinName().c_str() );
				db->globalPinListPosGoForward();
				continue;
			}

			aNode = createNode();
			aNode->index = nodeIndex++;
			aNode->name = "atlasgpin__"+db->getGlobalPinName();
			aNode->inst = "gpin";
			aNode->pin = db->getGlobalPinName();
			aNode->type = primary_pin;
			aNode->center.x = ( db->getGlobalPinGeomLeft() + db->getGlobalPinGeomRight() ) /2;
			aNode->center.y = ( db->getGlobalPinGeomTop() + db->getGlobalPinGeomBottom() ) /2;
			if ( db->getGlobalPinDirection() == pin_input_dir )
			{
				aNode->dir = sta_pin_input_dir;     // pin is input pin
				inpins ++;
			}
			else
			{
				aNode->dir = sta_pin_output_dir;    // pin is output pin
				outpins ++;
			}
			_nodes.push_back ( aNode );
			nodeMap[aNode->name] = aNode;
//       printf("Node %s is added.\n", aNode->name.c_str());
			pinNum ++;
			db->globalPinListPosGoForward();
		}
	}

// Creating a report about FFs;
	int ffInNumber = 0, ffInoutNumber = 0, ffOutNumber = 0;

	for ( unsigned i = 0; i < _nodes.size();i ++ )
	{
		staNode *aNode = _nodes[i];
		if ( aNode->type == flipflop_pin )
		{
			if ( aNode->dir == sta_pin_output_dir )
			{
				ffOutNumber++;
			}
			if ( aNode->dir == sta_pin_input_dir )
			{
				ffInNumber++;
			}
		}
	}

	printf ( "\tNetlist report:\n\t\t#STA Nodes = %d\n\t\t#Primary input pins = %d\n\t\t#Primary output pins = %d\n", _nodes.size(), inpins, outpins );
	printf ( "\t\t#FlipFlop output nodes = %d\n\t\t#FlipFlop input nodes = %d\n", ffOutNumber, ffInNumber );



	//-----------------------------
	// Create connections
	//-----------------------------
	fprintf ( staFP, "\n-------------------------------------\nCreating connections:\n" );
	db->resetNetListPos();
	if ( db->hasAnyNetInNetList() == true )
	{
		for ( int i = 0; i < db->getNetNum(); i ++ )
		{
//       fprintf ( staFP, "Processing the net %s:\n", db->getNetName().c_str() );
			if ( isAstaSpecialNet ( db->getNetName() ) == true )
			{
				fprintf ( staFP, "net %s is rejected.\n", db->getNetName().c_str() );
				fprintf ( staFP, "\tnet %s is ignored.\n", db->getNetName().c_str() );
				db->netListPosGoForward();
				continue;
			}

			//-----------------------------------------------------------------------
			// find source terminal of net, phase 1: search in internal terminals
			//-----------------------------------------------------------------------
			netHasSource = false;
			db->resetNetPinListPos();
			if ( db->hasAnyPinInNetPinList() == true )
			{
				for ( int j = 0; j < db->getNetPinNum();j ++ )
				{
					db->setActiveInstanceRandomAccess ( db->getNetPinInstanceName() );
					db->setActiveMacroRandomAccess ( db->getInstanceMacroName() );
					db->setActiveMacroPinRandomAccess ( db->getNetPinName() );
					if ( ( db->getMacroPinDirection() == pin_output_dir ) || ( db->getMacroPinDirection() == pin_inout_dir ) )
					{
						string nodeName = db->getNetPinInstanceName() + "__" + db->getNetPinName();
						if ( nodeMap.find ( nodeName ) != nodeMap.end() )
						{
							srcNode = nodeMap[nodeName];
							netHasSource = true;
							break;
						}
					}
					db->netPinListPosGoForward();
				}
			}

			//-------------------------------------------------------------------------
			// find source terminal of net, phase 2: search in boundary terminals
			//-------------------------------------------------------------------------
			if ( netHasSource == false )
			{
				if ( db->isNetGlobalPinNameValid() == true )
				{
					db->setActiveGlobalPinRandomAccess ( db->getNetGlobalPinName() );
					if ( db->getGlobalPinDirection() == pin_input_dir )
					{
						string nodeName = "atlasgpin__" + db->getNetGlobalPinName();
						if ( nodeMap.find ( nodeName ) != nodeMap.end() )
						{
							srcNode = nodeMap[nodeName];
							netHasSource = true;
						}
					}
				}
			}
			if ( netHasSource == false )
			{
				printf ( "\tNetlist warning: net %s has without no source terminal, Ignored.\n", db->getNetName().c_str() );
				fprintf ( staFP, "\tNetlist error: net %s has without no source terminal\n", db->getNetName().c_str() );
				db->netListPosGoForward();
//         getchar();
				continue;
			}

			//---------------------------------------------------------
			// add sink terminals to connections of the source node
			//---------------------------------------------------------
			db->resetNetPinListPos();
			if ( db->hasAnyPinInNetPinList() == true )
			{
				netHasSink = false;
				// adding the input pins of net
				for ( int j = 0; j < db->getNetPinNum();j ++ )
				{
					db->setActiveInstanceRandomAccess ( db->getNetPinInstanceName() );
					db->setActiveMacroRandomAccess ( db->getInstanceMacroName() );
					db->setActiveMacroPinRandomAccess ( db->getNetPinName() );
					if ( db->getMacroPinDirection() == pin_input_dir )
					{
						string nodeName = db->getNetPinInstanceName() + "__" + db->getNetPinName();
						if ( nodeMap.find ( nodeName ) != nodeMap.end() )
						{
							snkNode = nodeMap[nodeName];
//        printf("\t\tSNK: %s\n", snkNode->name.c_str());
							if ( ( snkNode != srcNode ) && notPresentConnection ( srcNode, snkNode ) )
							{
//                 fprintf ( staFP, "\t\t%s\n", snkNode->name.c_str() );
//                 printf("\t\tSNK: %s\n", snkNode->name.c_str());
								srcNode->connections.push_back ( snkNode );
								srcNode->net = db->getNetName();
								snkNode->net = db->getNetName();
								snkNode->prevs.push_back ( srcNode );
								netHasSink = true;
							}
						}
						else
						{
							fprintf ( staFP, "\t\tNode %s is not found in instance list\n", nodeName.c_str() );
						}
					}
					db->netPinListPosGoForward();
				}
			}

			// adding the global output pins of design
			if ( db->isNetGlobalPinNameValid() == true )
			{
				db->setActiveGlobalPinRandomAccess ( db->getNetGlobalPinName() );
				if ( db->getGlobalPinDirection() == pin_output_dir )
				{
					string nodeName = "atlasgpin__" + db->getNetGlobalPinName();
//      printf("\t\tSNK: %s\n", nodeName.c_str());
					if ( nodeMap.find ( nodeName ) != nodeMap.end() )
					{
						snkNode = nodeMap[nodeName];
						if ( ( snkNode != srcNode ) && notPresentConnection ( srcNode, snkNode ) )
						{
							srcNode->connections.push_back ( snkNode );
							snkNode->prevs.push_back ( srcNode );
//               fprintf ( staFP, "\t%s\n", snkNode->name.c_str() );
//               printf("\tSNK: %s\n", snkNode->name.c_str());
							netHasSink = true;
						}
						else
						{
							printf ( "\t\tNode %s is not found in instance list\n", nodeName.c_str() );
						}
					}
				}
			}

			if ( netHasSink == false )
			{
				fprintf ( staFP, "\tNetlist warning: net %s has no sink terminal, Ignored.\n", db->getNetName().c_str() );
//         printf ( "Netlist warning: net %s has no sink terminal\n", db->getNetName().c_str() );
				db->netListPosGoForward();
//  getchar();
				continue;
			}

			db->netListPosGoForward();
		}
	}
	DBU = ( double ) ( db->getDatabaseConvertFactor() );

	db->resetLayerListPos();
	if ( db->hasAnyLayerInLayerList() == true )
	{
		layerWidth = ( double ) ( db->getRoutingLayerWidth() );
	}
	else
	{
		printf ( "Error: There is no layer definition in library." );
		exit ( 0 );
	}

	checkPrevLinks();
	assignTechParametesOfNodes ( db );
	return true;
}

//-------------------------------------
// Return output resistance of cells.
// it is the mean of OR in 180nm tech.
//-------------------------------------
double ASTA::assignOutputResistance()
{
	return 600; // 600 ohms W/L = 2
}

//-------------------------------------
// Return input capacitance based on
// the fanout of inverter gates
//-------------------------------------
double ASTA::assignInputCapacitance ( int Fanout )
{
	double InputCapTable[4];
	// For 180:
	InputCapTable[0] = 0.0036;       // tsmc 0.18 PDF file in Picofarads
	InputCapTable[1] = 0.0071;
	InputCapTable[2] = 0.0104;
	InputCapTable[3] = 0.0136;
	/*
	  case :
	    InputCapTable[0] = 0.0025;        // tsmc 0.13 PDF file
	    InputCapTable[1] = 0.0047;
	    InputCapTable[2] = 0.0065;
	    InputCapTable[3] = 0.0092;
	*/

	if ( Fanout <= 1 )
		return InputCapTable[0];
	else if ( Fanout <= 2 )
		return InputCapTable[1];
	else if ( Fanout <= 4 )
		return InputCapTable[2];
	else
		return InputCapTable[3];
}

//-------------------------------------
// Return output resistance of cells.
// it is the mean of OR in 180nm tech.
//-------------------------------------
double ASTA::assignIntrinsicDelay ( int Fanout, staNode *aNode, atlasDB *db )
{
	staNode *aTerminal, *outputNode;
	double trunkX, trunkY, minX, minY, maxX, maxY, wireLength;
	double totalOC = 0;
	double OutputLoad[7], DelayINVX1[7], DelayINVX2[7], DelayINVX4[7], DelayINVX8[7];
	double CAPPERSQUAER;

	//---------------------------------------
	// Extracting the technology parameters
	//---------------------------------------

	// For 180:
	// Axis of output load
	OutputLoad[0] = 0.00035;
	OutputLoad[1] = 0.021;
	OutputLoad[2] = 0.0385;
	OutputLoad[3] = 0.084;
	OutputLoad[4] = 0.147;
	OutputLoad[5] = 0.231;
	OutputLoad[6] = 0.3115;

	// intrinsic delay of Inverters based on the output capacitance in tsmc 0.18u in pico seconds
	// cell_rise + cell_fall)/2
	DelayINVX1[0] = 1000* ( 0.026516+0.017116 ) /2;
	DelayINVX1[1] = 1000* ( 0.120965+0.065008 ) /2;
	DelayINVX1[2] = 1000* ( 0.200257+0.107102 ) /2;
	DelayINVX1[3] = 1000* ( 0.405724+0.216467 ) /2;
	DelayINVX1[4] = 1000* ( 0.689867+0.367865 ) /2;
	DelayINVX1[5] = 1000* ( 1.068599+0.569716 ) /2;
	DelayINVX1[6] = 1000* ( 1.431505+0.763154 ) /2;

	DelayINVX2[0] = 1000* ( 0.023676+0.014861 ) /2;
	DelayINVX2[1] = 1000* ( 0.118387+0.062988 ) /2;
	DelayINVX2[2] = 1000* ( 0.197702+0.104849 ) /2;
	DelayINVX2[3] = 1000* ( 0.403175+0.213640 ) /2;
	DelayINVX2[4] = 1000* ( 0.687328+0.364240 ) /2;
	DelayINVX2[5] = 1000* ( 1.066061+0.565029 ) /2;
	DelayINVX2[6] = 1000* ( 1.428967+0.757448 ) /2;

	DelayINVX4[0] = 1000* ( 0.021389+0.013106 ) /2;
	DelayINVX4[1] = 1000* ( 0.116760+0.061690 ) /2;
	DelayINVX4[2] = 1000* ( 0.196698+0.103548 ) /2;
	DelayINVX4[3] = 1000* ( 0.403817+0.212333 ) /2;
	DelayINVX4[4] = 1000* ( 0.690256+0.362931 ) /2;
	DelayINVX4[5] = 1000* ( 1.072047+0.563719 ) /2;
	DelayINVX4[6] = 1000* ( 1.437885+0.756138 ) /2;

	DelayINVX8[0] = 1000* ( 0.020686+0.013237 ) /2;
	DelayINVX8[1] = 1000* ( 0.112652+0.062588 ) /2;
	DelayINVX8[2] = 1000* ( 0.189660+0.105077 ) /2;
	DelayINVX8[3] = 1000* ( 0.389232+0.215499 ) /2;
	DelayINVX8[4] = 1000* ( 0.665237+0.368358 ) /2;
	DelayINVX8[5] = 1000* ( 1.033112+0.572156 ) /2;
	DelayINVX8[6] = 1000* ( 1.385620+0.767458 ) /2;

	// CAP PER SQUARE of metal1 in 180nm tech.
	CAPPERSQUAER = 0.00013153;

	/*  case 130:

	    // Axis of output load
	    OutputLoad[0] = 0.00079;
	    OutputLoad[1] = 0.002054;
	    OutputLoad[2] = 0.00474;
	    OutputLoad[3] = 0.010112;
	    OutputLoad[4] = 0.020856;
	    OutputLoad[5] = 0.042186;
	    OutputLoad[6] = 0.08532;

	    // intrinsic delay of Inverters based on the output capacitance in tsmc 0.13u
	    // cell_rise + cell_fall)/2
	    DelayINVX1[0] = 1000*(0.024593+0.015995)/2;
	    DelayINVX1[1] = 1000*(0.031198+0.019686)/2;
	    DelayINVX1[2] = 1000*(0.045250+0.026728)/2;
	    DelayINVX1[3] = 1000*(0.073085+0.040566)/2;
	    DelayINVX1[4] = 1000*(0.128228+0.068033)/2;
	    DelayINVX1[5] = 1000*(0.237232+0.122512)/2;
	    DelayINVX1[6] = 1000*(0.457276+0.232663)/2;

	    DelayINVX2[0] = 1000*(0.021717+0.016455)/2;
	    DelayINVX2[1] = 1000*(0.027546+0.020385)/2;
	    DelayINVX2[2] = 1000*(0.039857+0.027873)/2;
	    DelayINVX2[3] = 1000*(0.064167+0.042617)/2;
	    DelayINVX2[4] = 1000*(0.112450+0.072076)/2;
	    DelayINVX2[5] = 1000*(0.207878+0.130412)/2;
	    DelayINVX2[6] = 1000*(0.400637+0.248251)/2;

	    DelayINVX4[0] = 1000*(0.019647+0.014839)/2;
	    DelayINVX4[1] = 1000*(0.025448+0.019006)/2;
	    DelayINVX4[2] = 1000*(0.037649+0.026566)/2;
	    DelayINVX4[3] = 1000*(0.062063+0.041346)/2;
	    DelayINVX4[4] = 1000*(0.110337+0.070803)/2;
	    DelayINVX4[5] = 1000*(0.205784+0.129132)/2;
	    DelayINVX4[6] = 1000*(0.398532+0.246968)/2;

	    DelayINVX8[0] = 1000*(0.020046+0.014922)/2;
	    DelayINVX8[1] = 1000*(0.026024+0.019074)/2;
	    DelayINVX8[2] = 1000*(0.038596+0.026632)/2;
	    DelayINVX8[3] = 1000*(0.063644+0.041408)/2;
	    DelayINVX8[4] = 1000*(0.113236+0.070863)/2;
	    DelayINVX8[5] = 1000*(0.211254+0.129190)/2;
	    DelayINVX8[6] = 1000*(0.409176+0.247024)/2;

	    // CAP PER SQUARE of METAL1 in 130nm.
	    CAPPERSQUAER = 0.000092526;*/



	//---------------------------------------
	// Computing the output capacitance
	//---------------------------------------
	for ( unsigned i = 0; i < aNode->connections.size();i ++ )
	{
		outputNode = aNode->connections[i];
		if ( ( outputNode->dir != sta_pin_output_dir ) && ( outputNode->dir != sta_pin_inout_dir ) )
		{
			printf ( "Netlist Error: an input gate pin is connected to an input pin" );
			exit ( 1 );
		}

		maxX = outputNode->center.x;
		maxY = outputNode->center.y;
		minX = outputNode->center.x;
		minY = outputNode->center.y;
		for ( unsigned i = 0; i < outputNode->connections.size();i ++ )
		{
			aTerminal = outputNode->connections[i];
			if ( aTerminal->center.x > maxX )
				maxX = aTerminal->center.x;
			if ( aTerminal->center.y > maxY )
				maxY = aTerminal->center.y;
			if ( aTerminal->center.x < minX )
				minX = aTerminal->center.x;
			if ( aTerminal->center.y < minY )
				minY = aTerminal->center.y;
		}

		if ( ( maxX-minX ) > ( maxY-minY ) )  // rectangle is landscape
		{
			trunkY = ( minY + maxY ) / 2;
			wireLength = ( maxX - minX ) + fabs ( outputNode->center.y - trunkY );
			for ( unsigned i = 0; i < outputNode->connections.size();i ++ )
			{
				aTerminal = outputNode->connections[i];
				wireLength += fabs ( aTerminal->center.y - trunkY );
			}
		}
		else                              // rectangle is portrait
		{
			trunkX = ( minX + maxX ) / 2;
			wireLength = ( maxY - minY ) + fabs ( outputNode->center.x - trunkX );
			for ( unsigned i = 0; i < outputNode->connections.size();i ++ )
			{
				aTerminal = outputNode->connections[i];
				wireLength += fabs ( aTerminal->center.x - trunkX );
			}
		}
		totalOC += ( layerWidth / DBU ) * ( wireLength / DBU ) * CAPPERSQUAER;
	}

	if ( Fanout <= 1 )
	{
		if ( totalOC <= OutputLoad[0] )
			return DelayINVX1[0];
		else if ( totalOC <= OutputLoad[1] )
			return DelayINVX1[1];
		else if ( totalOC <= OutputLoad[2] )
			return DelayINVX1[2];
		else if ( totalOC <= OutputLoad[3] )
			return DelayINVX1[3];
		else if ( totalOC <= OutputLoad[4] )
			return DelayINVX1[4];
		else if ( totalOC <= OutputLoad[5] )
			return DelayINVX1[5];
		else
			return DelayINVX1[6];
	}
	else if ( Fanout <= 2 )
	{
		if ( totalOC <= OutputLoad[0] )
			return DelayINVX2[0];
		else if ( totalOC <= OutputLoad[1] )
			return DelayINVX2[1];
		else if ( totalOC <= OutputLoad[2] )
			return DelayINVX2[2];
		else if ( totalOC <= OutputLoad[3] )
			return DelayINVX2[3];
		else if ( totalOC <= OutputLoad[4] )
			return DelayINVX2[4];
		else if ( totalOC <= OutputLoad[5] )
			return DelayINVX2[5];
		else
			return DelayINVX2[6];
	}
	else if ( Fanout <= 4 )
	{
		if ( totalOC <= OutputLoad[0] )
			return DelayINVX4[0];
		else if ( totalOC <= OutputLoad[1] )
			return DelayINVX4[1];
		else if ( totalOC <= OutputLoad[2] )
			return DelayINVX4[2];
		else if ( totalOC <= OutputLoad[3] )
			return DelayINVX4[3];
		else if ( totalOC <= OutputLoad[4] )
			return DelayINVX4[4];
		else if ( totalOC <= OutputLoad[5] )
			return DelayINVX4[5];
		else
			return DelayINVX4[6];
	}
	else
	{
		if ( totalOC <= OutputLoad[0] )
			return DelayINVX8[0];
		else if ( totalOC <= OutputLoad[1] )
			return DelayINVX8[1];
		else if ( totalOC <= OutputLoad[2] )
			return DelayINVX8[2];
		else if ( totalOC <= OutputLoad[3] )
			return DelayINVX8[3];
		else if ( totalOC <= OutputLoad[4] )
			return DelayINVX8[4];
		else if ( totalOC <= OutputLoad[5] )
			return DelayINVX8[5];
		else
			return DelayINVX8[6];
	}
}

//---------------------------------------
// Return number of output pin of a cell
//---------------------------------------
int ASTA::getMacroFanout ( atlasDB *db, string macroName )
{
	int fanout = 0;

	db->setActiveMacroRandomAccess ( macroName );
	db->resetMacroPinListPos();
	for ( int i = 0;i < db->getMacroPinsNum();i ++ )
	{
		if ( ( db->getMacroPinDirection() == pin_output_dir ) || ( db->getMacroPinDirection() == pin_inout_dir ) )
			fanout ++;
		db->macroPinListGoForward();
	}
	return fanout;
}

//-------------------------------------
// Read routing layers cap and resistance
//-------------------------------------
void ASTA::readRoutingLayersInfo ( atlasDB *db )
{
	double r0 = 0.0, c0 = 0.0, pitch = 0.0;
	int layerNumber = 0;

	db->resetLayerListPos();
	for ( unsigned i = 0;i < db->getLayerNum();i ++ )
	{
		if ( db->getLayerType() == routing )
		{
			c0 += db->getRoutingLayerCapacitance();
			r0 += db->getRoutingLayerResistance();
			pitch += db->getRoutingLayerPitch();
			layerNumber ++;
		}
		db->layerListPosGoForward();
	}
	c0 = c0 / layerNumber;
	r0 = r0 / layerNumber;
	pitch = pitch / layerNumber / DBU;
	C_PER_MICRON = c0 * pitch;  // In Pico farads
	R_PER_MICRON = r0 / pitch;  // In ohm
	printf ( "\tTechnology parameters: \n" );
	printf ( "\t\tFS = 180nm\n\t\t#Metal layers = %d\n\t\tC(Metal-avg) = %f ff/um\n\t\tR(Metal-avg) = %f ohms/um\n\t\tPitch-avg = %f um\n", layerNumber, C_PER_MICRON, R_PER_MICRON, pitch );
}

//-------------------------------------
// Return output resistance of cells.
// it is the mean of OR in 180nm tech.
//-------------------------------------
bool ASTA::assignTechParametesOfNodes ( atlasDB *db )
{
	staNode *aNode;
	int cellFanOut;
	vector<staNode *>::iterator iter;
	readRoutingLayersInfo ( db );

	// output capacitance axis
	//   fprintf(staFP, "#######################\nSetting the parameters of nodes.\n\n");
	for ( iter = _nodes.begin();iter != _nodes.end(); iter ++ )
	{
		aNode = ( staNode * ) ( *iter );
		//     fprintf(staFP, "Current node is %s:%s\n", aNode->name.c_str(), aNode->macro.c_str());
		if ( ( aNode->type == internal_pin ) || ( aNode->type == flipflop_pin ) )
		{
			//       fprintf(staFP, "Pin is FFP or IP %d\n", aNode->dir);
			cellFanOut = getMacroFanout ( db, aNode->macro );
			//cellFanOut = aNode->macro[aNode->macro.length()-1] - '0';
			if ( aNode->dir == sta_pin_output_dir )
			{
				aNode->techInfo.ro = assignOutputResistance();
				//         fprintf(staFP, "\tOR = %f\n", aNode->techInfo.ro);
			}
			else if ( aNode->dir == sta_pin_input_dir )
			{
				aNode->techInfo.cin = assignInputCapacitance ( cellFanOut );
				aNode->techInfo.delay = assignIntrinsicDelay ( cellFanOut, aNode, db );
				//         fprintf(staFP, "\tIC = %f\n", aNode->techInfo.cin);
				//         fprintf(staFP, "\tID = %f\n", aNode->techInfo.delay);
			}
			else if ( aNode->dir == sta_pin_inout_dir )
			{
				aNode->techInfo.cin = assignInputCapacitance ( cellFanOut );
				aNode->techInfo.delay = assignIntrinsicDelay ( cellFanOut, aNode, db );
				aNode->techInfo.ro = assignOutputResistance();
				//         fprintf(staFP, "\tOR = %f\n", aNode->techInfo.ro);
				//         fprintf(staFP, "\tIC = %f\n", aNode->techInfo.cin);
				//         fprintf(staFP, "\tID = %f\n", aNode->techInfo.delay);
			}
		}
		else if ( aNode->type == primary_pin )
		{
			//       fprintf(staFP, "Pin is PP\n");
			if ( aNode->dir == sta_pin_output_dir )
			{
				aNode->techInfo.ro = assignOutputResistance();
			}
			else if ( aNode->dir == sta_pin_input_dir )
			{
				aNode->techInfo.cin = assignInputCapacitance ( 1 );
				aNode->techInfo.delay = 0;
			}
			else if ( aNode->dir == sta_pin_inout_dir )
			{
				aNode->techInfo.cin = assignInputCapacitance ( 1 );
				aNode->techInfo.delay = 0;
				aNode->techInfo.ro = assignOutputResistance();;
			}
		}
	}
	//   fprintf(staFP, "\n#######################\n");
	return true;
}
