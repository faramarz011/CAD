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

#ifndef PART_H
#define PART_H

#include <iostream>
#include <list>

using namespace std;
#include "atlasDB.h"
#define BUCKET_MAX_SIZE 101

enum fm_cell_type {FM_NONE=0, FM_PIN, FM_CELL};


struct fmNode {
	int	nodeID;
	char 	*nodeName;
	char 	*nodeType;
	fm_cell_type type;
	int fs;
	int te;
	int gain;
	unsigned pn; // part no
	unsigned gpn;// global part no
	bool lock;
	list <struct fmNode *> *neighborNodes; // list of neighboring nodes of instance
	list <struct fmEdge *> *connectedEdges;  // list of neighboring nets of instance
};

struct fmEdge {
	char *edgeName;
	int edgeID;
	list <fmNode *> *nodes;
	float weight;
	fmNode *srcTerminal;
};

struct fmMove {
  int gain;
  fmNode *node;
};

class fmPart {
public:
	fmPart();
	~fmPart();
	int BiPart(unsigned);
	void createReport();
	int fmReadDB(atlasDB *);
	fmNode *getNodeByName(char *);
	fmNode *getNodeByIndex(int );
	int parentPartSize(int ); 		// get size of parent partition
private:
	list <fmNode *> *nList; // whole nodes
	list <fmEdge *> *eList; // whole edges
	list <fmMove *> *moves;
	list <fmNode *> *updatedNodes;
	list <fmNode *> *bucket[BUCKET_MAX_SIZE];
	fmNode *findNode ( string );
	void initPart(int);
	fmNode * computeGains(int);         // compute gain of all the cells
	fmNode * updateGains(int, fmNode *);   // update gains after moving a cell
	bool analyzeMoves();
	unsigned cutsize(int);
	void printNetlist();
	void printParts(int);
	unsigned partSize(int, int); // get size of partition
	bool hasNode ( fmNode *, fmNode *);
  bool hasEdge (fmNode *, fmEdge *);
	void SetPartsIndex(unsigned);
	void PrintBucket();
	int findMaxNodeDegree();
	void unlockAll();
};
#endif



