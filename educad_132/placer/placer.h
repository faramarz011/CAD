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

#ifndef PLACER_H
#define PLACER_H

#include <iostream>
#include <list>
#include <vector>
using namespace std;
#include "atlasDB.h"

#define WHITE_SPACE_RATIO 0.20
#define PLACE_DBU 2000

enum PlacerRunMode { none_pl_mode, read_file_mode, read_db_mode };
enum CellPlaceType { fixed_cell, placable_cell};
enum InstType { pin_inst, core_inst};

struct simplePoint
{
	float x;
	float y;
};

struct simpleRect
{
	float t;
	float l;
	float b;
	float r;
};
struct simpleNet
{
	char 	*netName;
	int NetID;
	list <struct simpleInstance *> *instances;
	bool tag;
	simpleInstance *srcTerminal;
	int netDepth; // Distance of a net from I/O pins in netlist
};

struct simpleInstance
{
	int	instID;
	char 	*instName;
	char 	*cellName;
	simplePoint TL;
	float 	Width, Height;
	list <simpleNet *> *nets;
	list <simpleInstance *> *neighbors;
	list <int> *neighbor_costs;
	int rowNo;
	bool tag;
	CellPlaceType placeType;
	InstType instType;
};

struct PerturbLog
{
	simpleInstance *inst1;
	simpleInstance *inst2;
};

struct simpleDumpInst
{
	string instName;
	simplePoint TL;
};

class SimplePlacer
{
	public:
		SimplePlacer();
		~SimplePlacer();
		int simPlacer();
		int readDB ( atlasDB * );
		void updateDB ( atlasDB * );
		void dumpPlacement ( char *dumpFile );
		void readDump ( char *dumpFile );
		double TOV(); // Total Overlap of cells
		double OV ( simpleInstance *inst1 ); // Overlap of a cell with other cells
		unsigned int getInstNum();
		simpleInstance *getInst ( unsigned );
	private:
		vector <simpleInstance *> mList; // List of modules wherein design
		vector <simpleNet *> nList; // List of nets wherein design
		float rowHeight; // Height of row
		unsigned int maxlayoutW; // Layout width
		unsigned int maxlayoutH; // layout height
		unsigned int globalPinNumber;
		unsigned int coreOffsetX;
		unsigned int coreOffsetY;
		int rowNumber;
		atlasDB *_db;

		simpleInstance *findInstance ( string instName );
		bool hasNet ( simpleInstance *, simpleNet * );
		bool isNetInList ( list <simpleNet *> hotNets, simpleNet *_net );
		double computeTHPWL();
		double updateTHPWL ( double, simpleInstance *, simpleInstance * );
		void shuffleCells();
		void sortCells();
		void distributeWhiteSpaces();
		void swapCells ( simpleInstance *, simpleInstance * );
		void SA();
		void placePins();
		void setNetsDepth();
		void GlobalPlace();
		void DetailPlace();
		void LegalizeByShift();
		void ScaleLocations();
		void SpreadVertical();
		void SpreadVertical2();
		void ComputeMatrixA();
		void ComputeVectorB();
};
#endif



