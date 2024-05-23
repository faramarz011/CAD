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

#ifndef __GR_H
#define __GR_H

#include <iostream>
#include <list>
#include <vector>
using namespace std;
#include "atlasDB.h"
#include "atlasDBStructs.h"

#define MAX_LAYER_NUM 20

enum edgeType { LEFT=0, RIGHT, TOP, BOTTOM };

struct simpleGrPoint
{
	unsigned left;
	unsigned top;
};

// Checker board graph
struct simpleGrEdge
{
	unsigned capacity;
	unsigned used_tracks;
	struct simpleGrNode *node1;
	struct simpleGrNode *node2;
};

struct simpleGrNode
{
	unsigned id;
	long int centerRow;
	long int centerCol;
	simpleGrEdge *left;
	simpleGrEdge *right;
	simpleGrEdge *top;
	simpleGrEdge *bottom;
};

struct TechLayer
{
	string name;
	TRLayerDirection dir;
	unsigned pitch;
};

struct simpleNetTerminal
{
	unsigned id;
	unsigned row;
	unsigned col;
	bool isSrource;
	bool tag;
	int parentID;
	int Lambda;
};

struct simpleGrNet
{
	string netName;
	list <simpleNetTerminal *> *terminalBins;
	list <simpleGrEdge *> *routingTree;
};

struct mstTree
{
	vector < simpleGrNode *> nodes;
	vector < simpleGrEdge *> edges;
};

class simpleGlobalRouter
{
	public:
		vector < simpleGrNode *> nodes;
		vector < simpleGrEdge *> edges;
		vector <simpleGrNet *> nets;
		unsigned layerNumber;
		TechLayer layers[MAX_LAYER_NUM];
		atlasDB *db;
		unsigned cellH, cellW;
		unsigned rowNumber, columnNumber;
		long int layoutWidth, layoutHeight;
		bool PrimMST ( simpleGrNet * );

	public:
		simpleGlobalRouter ( atlasDB *_db );
		~simpleGlobalRouter() {};
		void createGrMesh ( unsigned rowNumber, unsigned columnNumber, long int layoutWidth, long int layoutHeight );
		void simpleGrRoute ( );
		void reportCapacityViolations( );

	private:
		simpleGrEdge *createEdge ( simpleGrNode *n1, edgeType dir, unsigned capacity );
		void MapNetsOnGrMesh();
		simpleGrEdge *createNode ( simpleGrNode *, edgeType dir, unsigned capacity );
		void computeVerticaledgesCapacity();
		void computeHorizontaledgesCapacity();
		void readRoutingLayersInfo();
		unsigned computeHorizontalEdgesCapacity ( unsigned cellW );
		unsigned computeVerticalEdgesCapacity ( unsigned cellH );
		simpleGrEdge *findEdge ( simpleGrNode *n1, simpleGrNode *n2 );
		int readNetlist ( );
		simpleNetTerminal *findMinLambdaTerminal ( simpleGrNet * );
		void printMST ( simpleGrNet * );

};

#endif
