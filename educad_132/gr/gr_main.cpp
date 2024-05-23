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

unsigned termNum = 0;
//------------------------------
// Constructor
//------------------------------
simpleGlobalRouter::simpleGlobalRouter ( atlasDB *_db )
{
	db = _db;
	readRoutingLayersInfo(); // Read routing layers information
};

simpleGrEdge *simpleGlobalRouter::findEdge ( simpleGrNode *n1, simpleGrNode *n2 )
{
	vector < simpleGrEdge *>::iterator iter;
	simpleGrEdge *anEdge;
	for ( unsigned i = 0; i < edges.size(); i ++ )
	{
		anEdge = edges[i];
		if ( ( anEdge->node1 == n1 ) && ( anEdge->node2 == n2 ) ||
		        ( anEdge->node1 == n2 ) && ( anEdge->node2 == n1 ) )
			return anEdge;
	}
	return NULL;
}

//------------------------------
// Read Placed Netlist from DB
//------------------------------
int simpleGlobalRouter::readNetlist ( )
{
	simpleGrPoint pinXY;
	float layoutArea = 0, Dim, modDim;
	int cnt, netNumber;
	char *netname = new char[256];
	//---------------------
	// Reading Nets
	//---------------------
	netNumber = db->getNetNum();
	db->resetNetListPos();
	for ( unsigned i = 0; i < db->getNetNum();i ++ )
	{
		strcpy ( netname, db->getNetName().c_str() );
		simpleGrNet *aNet;
		aNet = new simpleGrNet;
		aNet -> netName = db->getNetName();
		aNet -> terminalBins = new list <simpleNetTerminal *>;
		aNet -> routingTree = new list <simpleGrEdge *>;
		if ( ( db->getNetName() == "POWR" ) || ( db->getNetName() == "GRND" ) )
		{
			printf ( "\t\tNet %s is ignored.\n", netname );
			db->netListPosGoForward();
			continue;
		}

		db->resetNetPinListPos();
		for ( int j = 0; j < db->getNetPinNum();j ++ )
		{
			// Read net pin location
			db->setActiveInstanceRandomAccess ( db->getNetPinInstanceName() );
			pinXY.left = db->getInstanceLeft();
			pinXY.top  = db->getInstanceTop();
			// Read pin direction
			db->setActiveMacroRandomAccess ( db->getInstanceMacroName() );
			db->setActiveMacroPinRandomAccess ( db->getNetPinName() );
			TPinDirection pinDir = db->getMacroPinDirection();

			for ( unsigned i  = 0; i < rowNumber;i ++ )
			{
				for ( unsigned j  = 0; j < columnNumber;j ++ )
				{
					if ( ( pinXY.left >= ( j*cellW ) ) && ( pinXY.left < ( ( j+1 ) *cellW ) ) &&
					        ( pinXY.top  >= ( i*cellH ) ) && ( pinXY.top  < ( ( i+1 ) *cellH ) ) )
					{
						simpleNetTerminal *termBin = new simpleNetTerminal;
						termBin->col = j;
						termBin->row = i;
						termBin->id = termNum++;

						if ( pinDir == pin_output_dir )
						{
							termBin->isSrource = true;
// 							printf ( "SRC <%d,%d>\n", termBin->row, termBin->col );
						}
						else
						{
							termBin->isSrource = false;
// 							printf ( "SNK <%d,%d>\n", termBin->row, termBin->col );
						}
						aNet -> terminalBins -> push_back ( termBin );
					}
				}
			}
			db->netPinListPosGoForward();
		}
		nets.push_back ( aNet );
		db->netListPosGoForward();
	}
	return 0;
}

//------------------------------
// Read routing info
//------------------------------
void simpleGlobalRouter::readRoutingLayersInfo()
{
	TechLayer aLayer;
	layerNumber = 0;
	db->resetLayerListPos();
	for ( unsigned i = 0;i < db->getLayerNum();i ++ )
	{
		if ( db->getLayerType() == routing )
		{
			aLayer.name = db->getLayerName();
			aLayer.dir = db->getRoutingLayerDirection();
			aLayer.pitch = db->getRoutingLayerPitch();
			layers[layerNumber] = aLayer;
			layerNumber ++;
		}
		db->layerListPosGoForward();
	}
	printf ( "\tTotal number of routing layers = %d and %d layers are used for routing.\n", layerNumber, layerNumber-2 );
// 	for ( unsigned i = 0; i < layerNumber;i ++ )
// 	{
// 		printf ( "Layer[%d]:\n\tname = %s\n\tdirection = %d\n\tpitch = %d\n", i, layers[i].name.c_str(), layers[i].dir, layers[i].pitch );
// 	}
}

//------------------------------
// Creating a GR mesh node
//------------------------------
simpleGrEdge *simpleGlobalRouter::createEdge ( simpleGrNode *n1, edgeType dir, unsigned capacity )
{
	long int x, y;
	unsigned i = n1->centerRow;
	unsigned j = n1->centerCol;

	if ( dir == LEFT )
	{
		x = i;
		y = j-1;
	}
	else if ( dir == RIGHT )
	{
		x = i;
		y = j+1;
	}
	else if ( dir == TOP )
	{
		x = i-1;
		y = j;
	}
	else if ( dir == BOTTOM )
	{
		x = i+1;
		y = j;
	}

	if ( ( x < 0 ) || ( y < 0 ) || ( x >= rowNumber ) || ( y >= columnNumber ) )
	{
		printf ( "Indexing error, <%d, %d>\n", x, y );
		exit ( 1 );
	}
	simpleGrNode *n2 = nodes[x*columnNumber+y ];
	simpleGrEdge * e = findEdge ( n1, n2 );

// printf("n1 <%d,%d> and n2 <%d, %d>\n", n1->centerRow, n1->centerCol, n2->centerRow, n2->centerCol);
	if ( e != NULL )
	{
// printf("Found\n");
		return e;
	}
	else
	{
		simpleGrEdge *anEdge = new simpleGrEdge;
		anEdge -> capacity = capacity;
		anEdge -> used_tracks = 0;
		anEdge -> node1 = n1;
		anEdge -> node2 = n2;
		edges.push_back ( anEdge );
		return anEdge;
	}
}

//------------------------------
// Compute horizontal capacity
//------------------------------
unsigned simpleGlobalRouter::computeHorizontalEdgesCapacity ( unsigned cellW )
{
	unsigned cap = 0;

	for ( unsigned i = 2;i < layerNumber;i += 2 )
	{
		if ( layers[i].dir == layer_horizontal_dir )
		{
			cap += cellW / layers[i].pitch;
// 			 printf ( "cellW = %d, pitch = %d, cap = %d\n", cellW, layers[i].pitch, cap );
		}
	}
// 	printf ( "\tHorizontal cap = %d\n", cap );
	return cap;
}

//------------------------------
// Compute vertical capacity
//------------------------------
unsigned simpleGlobalRouter::computeVerticalEdgesCapacity ( unsigned cellH )
{
	unsigned cap = 0;
	for ( unsigned i = 3;i < layerNumber;i += 2 )
	{
		if ( layers[i].dir == layer_vertical_dir )
		{
			cap += cellH / layers[i].pitch;
// 			printf ( "cellH = %d, pitch = %d, cap = %d\n", cellH, layers[i].pitch, cap );
		}
	}
// 	printf ( "\tVertical cap = %d\n", cap );
	return cap;
}

//------------------------------
// Creating a GR mesh
//------------------------------
void simpleGlobalRouter::createGrMesh ( unsigned _rowNumber, unsigned _columnNumber, long int _layoutWidth, long int _layoutHeight )
{
	list <simpleGrEdge *>::iterator iter;
	unsigned nodeNum = 0;
	rowNumber = _rowNumber;
	columnNumber = _columnNumber;
	layoutWidth = _layoutWidth;
	layoutHeight = _layoutHeight;

	cellW = ceil ( layoutWidth / columnNumber );
	cellH = ceil ( layoutHeight / rowNumber );
	unsigned vCap = computeHorizontalEdgesCapacity ( cellW );
	unsigned hCap = computeVerticalEdgesCapacity ( cellH );

	//---------------------
	// Create nodes
	//---------------------
	for ( unsigned i = 0; i < rowNumber;i ++ )
	{
		for ( unsigned j = 0; j < columnNumber;j ++ )
		{
			simpleGrNode *aNode = new simpleGrNode;
			aNode -> id = nodeNum++;
			aNode -> centerCol = j;// * cellW + cellW/2;
			aNode -> centerRow = i;// * cellH + cellH/2;
			nodes.push_back ( aNode );
		}
	};


	//----------------
	// Create edges
	//----------------
	for ( unsigned i = 0; i < rowNumber;i ++ )
	{
		for ( unsigned j = 0; j < columnNumber;j ++ )
		{
			simpleGrNode *currNode = nodes[i*columnNumber+j];
			if ( ( i == 0 ) && ( j == 0 ) ) // left-top cell
			{
				currNode -> left = NULL;
				currNode -> top = NULL;
				currNode -> right = createEdge ( currNode, RIGHT, hCap );
				currNode -> bottom = createEdge ( currNode, BOTTOM, vCap );
			}
			else if ( ( i == 0 ) && ( j == ( columnNumber-1 ) ) )  // right-top cell
			{
				currNode -> right = NULL;
				currNode -> top = NULL;
				currNode -> left = createEdge ( currNode, LEFT, hCap );
				currNode -> bottom = createEdge ( currNode, BOTTOM, vCap );
			}
			else if ( ( i == ( rowNumber-1 ) ) && ( j == 0 ) )  // left-bottom cell
			{
				currNode -> left = NULL;
				currNode -> bottom = NULL;
				currNode -> right = createEdge ( currNode, RIGHT, hCap );
				currNode -> top = createEdge ( currNode, TOP, vCap );
			}
			else if ( ( i == ( rowNumber-1 ) ) && ( j == ( columnNumber-1 ) ) )  // right-bottom cell
			{
				currNode -> right = NULL;
				currNode -> bottom = NULL;
				currNode -> left = createEdge ( currNode, LEFT, hCap );
				currNode -> top = createEdge ( currNode, TOP, vCap );
			}
			else if ( ( i == 0 ) && ( j != ( columnNumber-1 ) ) ) // first row
			{
				currNode -> top = NULL;
				currNode -> left = createEdge ( currNode, LEFT, hCap );
				currNode -> right = createEdge ( currNode, RIGHT, hCap );
				currNode -> bottom = createEdge ( currNode, BOTTOM, vCap );
			}
			else if ( ( j == 0 ) && ( i != ( rowNumber-1 ) ) ) // first column
			{
				currNode -> left = NULL;
				currNode -> right = createEdge ( currNode, RIGHT, hCap );
				currNode -> top = createEdge ( currNode, TOP, vCap );
				currNode -> bottom = createEdge ( currNode, BOTTOM, vCap );
			}
			else if ( i == ( rowNumber-1 ) ) // last row
			{
				currNode -> bottom = NULL;
				currNode -> left = createEdge ( currNode, LEFT, hCap );
				currNode -> right = createEdge ( currNode, RIGHT, hCap );
				currNode -> top = createEdge ( currNode, TOP, vCap );
			}
			else if ( j == ( columnNumber-1 ) ) // last column
			{
				currNode -> right = NULL;
				currNode -> left = createEdge ( currNode, LEFT, hCap );
				currNode -> top = createEdge ( currNode, TOP, vCap );
				currNode -> bottom = createEdge ( currNode, BOTTOM, vCap );
			}
			else // others
			{
				currNode -> left = createEdge ( currNode, LEFT, hCap );
				currNode -> right = createEdge ( currNode, RIGHT, hCap );
				currNode -> top = createEdge ( currNode, TOP, vCap );
				currNode -> bottom = createEdge ( currNode, BOTTOM, vCap );
			}
		}
	}

	//-------------------
	// Creating report
	//-------------------

// 	printf ( "Start of mesh creation:\n\tlayoutW = %d\n\tlayoutH = %d\n\t#rows = %d\n\t#cols = %d\n\tcellW = %d\n\tcellH = %d\n\tHcap = %d\n\tVcap = %d\n", layoutWidth, layoutHeight, rowNumber, columnNumber, cellW, cellH, hCap, vCap );

	for ( unsigned i = 0; i < rowNumber;i ++ )
	{
		for ( unsigned j = 0; j < columnNumber;j ++ )
		{
			simpleGrNode *aNode = nodes[i*columnNumber+j];
			unsigned portNum = 0;
			if ( aNode->left != NULL )
				portNum ++;
			if ( aNode->right != NULL )
				portNum ++;
			if ( aNode->top != NULL )
				portNum ++;
			if ( aNode->bottom != NULL )
				portNum ++;
// 			printf ( "Node[%d] row = %d, col = %d, #edges = %d.\n", aNode->id, i, j, portNum );
// 			printf ( "\tCenter = <%d , %d>, and edges:\n", aNode -> centerRow, aNode -> centerCol );
			/*			if ( aNode->left != NULL )
						{
							if ( aNode->left->node1 == aNode )
								printf ( "\t\tedge L, from <%d,%d> to <%d,%d>, cap = %d\n", aNode->left->node1->centerRow, aNode->left->node1->centerCol, aNode->left->node2->centerRow, aNode->left->node2->centerCol, aNode->left->capacity );
							else
								printf ( "\t\tedge L, from <%d,%d> to <%d,%d>, cap = %d\n", aNode->left->node2->centerRow, aNode->left->node2->centerCol, aNode->left->node1->centerRow, aNode->left->node1->centerCol, aNode->left->capacity );
						}*/
			/*			if ( aNode->right != NULL )
						{
							if ( aNode->right->node1 == aNode )
								printf ( "\t\tedge R, from <%d,%d> to <%d,%d>, cap = %d\n", aNode->right->node1->centerRow, aNode->right->node1->centerCol, aNode->right->node2->centerRow, aNode->right->node2->centerCol, aNode->right->capacity );
							else
								printf ( "\t\tedge R, from <%d,%d> to <%d,%d>, cap = %d\n", aNode->right->node2->centerRow, aNode->right->node2->centerCol, aNode->right->node1->centerRow, aNode->right->node1->centerCol, aNode->right->capacity );

						}*/
			/*			if ( aNode->top != NULL )
						{
							if ( aNode->top->node1 == aNode )
								printf ( "\t\tedge T, from <%d,%d> to <%d,%d>, cap = %d\n", aNode->top->node1->centerRow, aNode->top->node1->centerCol, aNode->top->node2->centerRow, aNode->top->node2->centerCol, aNode->top->capacity );
							else
								printf ( "\t\tedge T, from <%d,%d> to <%d,%d>, cap = %d\n", aNode->top->node2->centerRow, aNode->top->node2->centerCol, aNode->top->node1->centerRow, aNode->top->node1->centerCol, aNode->top->capacity );
						}
						if ( aNode->bottom != NULL )
						{
							if ( aNode->bottom->node1 == aNode )
								printf ( "\t\tedge B, from <%d,%d> to <%d,%d>, cap = %d\n", aNode->bottom->node1->centerRow, aNode->bottom->node1->centerCol, aNode->bottom->node2->centerRow, aNode->bottom->node2->centerCol, aNode->bottom->capacity );
							else
								printf ( "\t\tedge B, from <%d,%d> to <%d,%d>, cap = %d\n", aNode->bottom->node2->centerRow, aNode->bottom->node2->centerCol, aNode->bottom->node1->centerRow, aNode->bottom->node1->centerCol, aNode->bottom->capacity );
						}*/
		}
	}

	readNetlist();
}
