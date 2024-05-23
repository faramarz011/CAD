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

#include "gui.h"
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <curses.h>
#include <qdatetime.h>
#include <iomanip>
#include "iostream"
#include "list"
using namespace std;

#include "atlasLEF.h"
#include "atlasDEF.h"
#include "part.h"
#include "placer.h"
#include "gr.h"
#include "asta.h"
#include "atlasDB.h"

void runGUI ( int argc, char **argv );
void runAlgorithms ( int argc, char **argv );

extern bool doPaint;

atlasDB *db;
atlasLEF *lef_parser;
atlasDEF *def_parser;
SimplePlacer *sPL;
fmPart *sPR;
simpleGlobalRouter *sGR;
ASTA *sta;

int main ( int argc, char **argv )
{
	runAlgorithms ( argc, argv );
	runGUI ( argc, argv );

	return EXIT_SUCCESS;

}

void runAlgorithms ( int argc, char **argv )
{
	printf ( "EduCAD: Education CAD toolkit, Version 8.1\n" );
	printf ( "All rights reserved for SBU EDA Group,\n" );
	printf ( "Ali Jahanian, November 2011.\n\n" );
	if ( argc == 5 )
	{
		db = new atlasDB;
		lef_parser = new atlasLEF();
		def_parser = new atlasDEF();
		time_t t1, t2, t3;

		time(&t1);
		printf ( "1. LEF parser.\n" );
		lef_parser->parse ( ( char * ) argv[1], ( char * ) "lef_out.txt", db );

		printf ( "2. DEF parser.\n" );
		def_parser->parse ( ( char * ) argv[2], ( char * ) "Def_out.txt", db );


		if ( strcmp ( argv[3],"-place" ) == 0 )
		{

			time(&t2);
//			printf ( "3. FM partitioner.\n" );
//			sPR = new fmPart();
//			sPR->fmReadDB ( db );		
//			sPR->BiPart (0);
//			time(&t3);
//			printf("Elapsed time = %d sec\n", int(t3 - t2));
			printf ( "4. Simple placement.\n" );
			sPL = new SimplePlacer();
			// Read information from DB
			sPL->readDB ( db );
			// Do global Placement
			sPL->simPlacer();

			// Write placement results to DB
			sPL->updateDB ( db );
			sPL->dumpPlacement ( argv[4] );

 			printf ( "\n4. Simple global router.\n" );
 			sGR = new simpleGlobalRouter ( db );
 			sGR -> createGrMesh ( 10, 10, db->getPlacementResultWidth(), db->getPlacementResultHeight() );
 			sGR -> simpleGrRoute ();
 			sGR -> reportCapacityViolations();

			printf ( "\n5. Static Timing Analysis.\n" );
			sta = new ASTA ( ( char * ) "sta_log.txt" );
			sta -> setVar ( ( char * ) "sta_operation_mode", ( char * ) "post_place" ); 	// can be pre_place or post_place
			sta -> setVar ( ( char * ) "sta_timing_wall", ( char * ) "0.01" ); 		// between 0 to 1
			sta -> setVar ( ( char * ) "ignore_special_nets", ( char * ) "ON" );
			sta -> setVar ( ( char * ) "special_net_name", ( char * ) "POWR" );
			sta -> setVar ( ( char * ) "special_net_name", ( char * ) "GRND" );
			sta -> readNetlist ( db );
			sta -> Analysis ( db );
		}
		else if ( strcmp ( argv[3], "-load" ) == 0 )
		{
			printf ( "3. FM partitioner.\n" );
			//sPR = new fmPart();
			//sPR->fmReadDB ( db );
			//sPR->doPart ( 2 );

			printf ( "4. Read placed cells from dumped file.\n" );
			sPL = new SimplePlacer();
			// Read information from DB
			sPL->readDB ( db );
			// Do Placement
			sPL->readDump ( argv[4] );

			// Write placement results to DB
			sPL->updateDB ( db );

			printf ( "\n5. Simple global router.\n" );
			sGR = new simpleGlobalRouter ( db );
			sGR -> createGrMesh ( 10, 10, db->getPlacementResultWidth(), db->getPlacementResultHeight() );
			sGR -> simpleGrRoute ();
			sGR -> reportCapacityViolations();

			printf ( "\n6. Static Timing Analysis.\n" );
			sta = new ASTA ( ( char * ) "sta_log.txt" );
			sta -> setVar ( ( char * ) "sta_operation_mode", ( char * ) "post_place" ); 	// can be pre_place or post_place
			sta -> setVar ( ( char * ) "sta_timing_wall", ( char * ) "0.0" ); 		// between 0 to 1
			sta -> setVar ( ( char * ) "ignore_special_nets", ( char * ) "ON" );
			sta -> setVar ( ( char * ) "special_net_name", ( char * ) "POWR" );
			sta -> setVar ( ( char * ) "special_net_name", ( char * ) "GRND" );
			sta -> readNetlist ( db );
			sta -> Analysis ( db );
		}
		else
		{
			printf ( "Invalid option %s\n", argv[3] );
exit(1);
		}

		printf ( "7. Ready to view GUI\n" );
		doPaint = true;
	}
	else
	{
		printf ( "  Usage:./educad <LEF file> <DEF file> [-place|-load] <filename> \n\n" );
		printf ( "    <LEF file>: name and path of LEF file which is physical technology library.\n" );
		printf ( "    <DEF file>: name and path of DEF file which is synthesized design.\n" );
		printf ( "    -place: A new placement is performed by -place option and placed circuit is saved into dump file <filename>.\n" );
		printf ( "    -load: A pre-dumped palcement is loaded by -load option from the pre-dumped file <filename>.\n" );
		printf ( "    <filename>: name of the dump file which is dumped or loaded.\n\n" );
		printf ( "    Please send any found bug reports to jahanian@sbu.ac.ir\n    See Readme.txt for more details.\n\n" );
		exit ( 0 );
	}
}

void runGUI ( int argc, char **argv )
{
	QApplication::setColorSpec ( QApplication::CustomColor );
	QApplication a ( argc, argv );
	MyWidget w;
	w.setCaption ( "EduCAD Version 8.1 (SBU EDA Group)" );
	w.setGeometry ( QRect ( 0,0,1024,762 ) );
	a.setMainWidget ( &w );
	w.show();
	a.exec();
}

