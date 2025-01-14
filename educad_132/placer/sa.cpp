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

#include <stdlib.h>
#include <string.h>
#include <placer.h>
#include <stdio.h>
#include <math.h>

#include <iostream>
#include <list>
#include <vector>
#include <map>

using namespace std;
#include <atlasDB.h>

#define DBU 2000
bool sortFunc ( simpleInstance *i1, simpleInstance *i2 );
extern unsigned cellHeight;


void SimplePlacer::SA()
{
	float Tinit, Tfinal, t;
	float r, c1, c2, dc;
	float a1, a2, a3;
	unsigned inner_loop_count, i1, i2;
	simpleInstance *iPtr1, *iPtr2;
	int tCntr = 0;
/* default
	Tinit = 150;
	Tfinal = 0.001;
	inner_loop_count = 250;
	a1 = 0.001;
	a2 = 0.00001;
	a3 = 0.001;
*/
	Tinit = 100;
	Tfinal = 0.001;
	inner_loop_count = 100;
	a1 = 0.01;
	a2 = 0.0001;
	a3 = 0.01;
	t = Tinit;

	c1 = computeTHPWL();
	while ( t > Tfinal )
	{
		for ( unsigned i = 0;i < inner_loop_count;i ++ )
		{
			i1 = rand() % mList.size();
			while ( mList[i1]->instType != core_inst )
				i1 = rand() % mList.size();
			i2 = rand() % mList.size();
			while ( ( mList[i2]->instType != core_inst ) || ( i1 == i2 ) )
				i2 = rand() % mList.size();

			iPtr1 = mList[i1];
			iPtr2 = mList[i2];
			swapCells ( iPtr1, iPtr2 );
			c2 = updateTHPWL ( c1, iPtr1, iPtr2 );
			dc = (c2 - c1) + 0.01 * fabs ( iPtr1->Width - iPtr2->Width );
			if ( dc > 0 )
			{
				r = ( float ) ( ( float ) rand() / ( float ) RAND_MAX );
				if ( r > exp ( -dc / t ) )
					swapCells ( iPtr1, iPtr2 );
				else
					c1 = c2;
			}
			else
			{
				c1 = c2;
			}
		}
		if ( t > ( 0.9 * Tinit ) )
			t = t - a1 * Tinit;
		else if ( t > ( 0.1 * Tinit ) )
			t = t - a2 * Tinit;
		else
			t = t - a3 * Tinit;

		if ( ( tCntr % 100 ) == 0 )
			printf ( "\tT = %f   TWL = %f\n", t, c1 );
		tCntr ++;
	}
}
