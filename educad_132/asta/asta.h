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

#ifndef _ASTA_
#define _ASTA_

#include <iostream>
#include <list>
#include <vector>
using namespace std;
#include "atlasDB.h"

#define GATE_INPUT_CAPACITANCE 0.001
#define GATE_OUTPUT_RESISTANCE 10.00

enum PINTYPE {none_pin, primary_pin, internal_pin, flipflop_pin };
enum PINDIR {sta_none_pin_dir, sta_pin_input_dir, sta_pin_output_dir, sta_pin_inout_dir};
enum staOperationType {sta_none_opr, sta_preplace_opr, sta_postplace_opr };
enum staEdgeType {sta_cell, sta_net };

struct staPoint
{
	double x;
	double y;
};

struct staRect
{
	double top;
	double left;
	double width;
	double height;
};

struct CellParams
{
	double cin;
	double ro;
	double delay;
};
struct NetParams
{
	double r;
	double l;
};

// Each node is a pin
struct staNode
{
	int index;
	string name;
	string macro;
	string inst;
	string pin;
	staPoint center;
	PINTYPE type;
	PINDIR dir;
	std::vector<struct staNode *> connections;  // connections to other nodes
	std::vector<struct staNode *> prevs;        // reverse pointer to previous nodes
	double dat;
	double drt;
	double slack;
	CellParams techInfo;
	string net; // connecting net to each node
};


// Each node is a pin
struct staEdge
{
	staNode *src;
	staNode *snk;
	double weight;
	int order;
};

struct staPath
{
	unsigned id;
	float maxSlack;
	vector <staNode *> nodes;
};

class ASTA
{
		vector<staNode *> _nodes; // each node is a pin
		vector<staNode *> _activeList;
		vector<staNode *> _tmpActiveList;
		vector <staPath *> activePaths;
		vector <staPath *> finalPaths;
		map<string , staNode *> nodeMap;
		staOperationType operationType;
		bool ignoreSpecialNets;
		vector <string> specialNetNames;
		bool ignoreSpecialPins;
		vector <string> specialPinNames;
		double staTimingWallRatio;

		FILE *staFP;
		NetParams wireModel;
		double DBU;
		double layerWidth;

		double C_PER_MICRON, R_PER_MICRON;
		// create netlist
		bool isOutputPin ( staNode * );
		bool isInputPin ( staNode * );
		bool checkForFF ( atlasDB *, string );
		staNode *createNode();
		bool notPresentConnection ( staNode *, staNode * );
		bool isAstaSpecialNet ( string );
		bool isSpecialPin ( string );
		string findOutputPin ( atlasDB *, string );
		void createInOutNodes ( int, bool, string, string, string, double, double, list <staNode *> * );
		void createOutputNodes ( int, bool, string, string, string, string, double, double, list <staNode *> * );
		void createInputNodes ( int, bool, string, string, string, string, double, double, list <staNode *> * );
		void connectInputNodesToOutputNodes ( list <staNode *> *, list <staNode *> * );
		void copyGateListsIntoNodes ( list <staNode *> *, list <staNode *> * );
		bool checkPrevLinks();

		// calculate the gate and net delays
		bool assignTechParametesOfNodes ( atlasDB * );
		double assignOutputResistance();
		double assignInputCapacitance ( int );
		double assignIntrinsicDelay ( int, staNode *, atlasDB * );
		double calculatePureDelay ( staOperationType, staNode *, staNode * );
		double calculateNewDelayWeight ( staOperationType, staNode *, staNode * );
		double calculateWireDelay ( staNode *, staNode * );
		double calculateWireDelayForPI ( staNode *, staNode * );

		// Foreward traversing the tree
		void addInputNodestoActiveList();
		bool forewardProcessActiveNodes();
		bool isInList ( vector<staNode *> , staNode * );
		void updateActiveNodes();
		void computeSlacks();


		// backward traversing the tree
		void addOutputNodestoActiveList();
		bool backwardProcessActiveNodes();
		void createNearCriticalPaths ( double cSlack );

		// Create critical paths
		list <staNode *> *createListOfEndTerminals();
		void staBackTrace ( staNode *, staPath *, int );
		char *createCriticalPaths();
		bool thereIsNotInList ( list <staNode *> *, staNode * );
		bool CheckForMultipleCP();
		bool isEdgeInList ( list <staEdge *> *, staEdge * );
		bool printCriticalPaths();
		int getMacroFanout ( atlasDB *, string );
		void readRoutingLayersInfo ( atlasDB * );
		float pathMaxSlack ( staPath *p );
	public:
		ASTA ( char * );
		~ASTA() {};
		char *getInfo();
		char *setVar ( char *, char * );
		bool readNetlist ( atlasDB * );
		bool Analysis ( atlasDB *db );
		bool isCriticalNet ( string srcPinName, string snkPinName );
		double timingWall();
		staNode *getMostCriticalDelayNode();
		int getPathNumber();
		double findMinimumSlack();
		void createStaReports ( atlasDB * );
		double getMaximumSlack();
};

#endif


