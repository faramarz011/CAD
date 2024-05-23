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
// Calculate the delay of a wire
//-------------------------------------------------------
double ASTA::calculateWireDelay ( staNode *srcNode, staNode *snkNode )
{
  staNode *aTerm;
  double cin = 0, l, ro, delay, c0, r0, w;

  for ( int i = 0;i < srcNode->connections.size();i ++ )
  {
    aTerm = srcNode->connections[i];
    cin += aTerm->techInfo.cin;
  }

  ro = srcNode->techInfo.ro;
  l = fabs ( srcNode->center.x - snkNode->center.x ) + fabs ( srcNode->center.y - snkNode->center.y );
  l = l / DBU;
  delay = ro * ( C_PER_MICRON*l + cin ) + R_PER_MICRON * l * cin; // ref: cong'2001
  //printf("Delay of net l=%f = %f\n", l, delay);

return  delay;
}

//-----------------------------------------------------------------
// Calculate the delay of a wire when src is a priamry input pin
//-----------------------------------------------------------------
double ASTA::calculateWireDelayForPI ( staNode *srcNode, staNode *snkNode )
{
  staNode *aTerm;
  double cin = 0, l, delay, ro;

  ro = srcNode->techInfo.ro;

  for ( int i = 0;i < srcNode->connections.size();i ++ )
  {
    aTerm = srcNode->connections[i];
    cin += aTerm->techInfo.cin;
  }

  //l = fabs(srcNode->center.x - snkNode->center.x) + fabs(srcNode->center.y - snkNode->center.y);
  l = 0.01; // adjusted as a small length
  delay = ( ro * ( C_PER_MICRON * l + cin ) ) + R_PER_MICRON * l * cin;
  // printf("Delay of net l=%f = %f\n", l, delay);

return  delay;
}

//-------------------------------------------------------------------
// Calculate the delay of a connection or gate considering MOC nets
//-------------------------------------------------------------------
double ASTA::calculateNewDelayWeight ( staOperationType operationType, staNode *srcNode, staNode *snkNode )
{
  double pureDelay;

  pureDelay = calculatePureDelay ( operationType, srcNode, snkNode );

  return pureDelay;
}

//-------------------------------------------------------
// Calculate the delay of a connection or gate
//-------------------------------------------------------
double ASTA::calculatePureDelay ( staOperationType operationType, staNode *srcNode, staNode *snkNode )
{
	
  if ( operationType == sta_preplace_opr )
  {
    if ( ( srcNode->type == internal_pin ) || ( srcNode->type == flipflop_pin ) )
    {
      if ( srcNode->dir == sta_pin_input_dir )
      {
        return srcNode->techInfo.delay;
      }
      else // if (srcNode->dir == sta_pin_output_dir)
      {
        return 0;
      }
    }
    else // if (srcNode->type == primary_pin)
    {
      return 0;
    }
  }
  else if ( operationType == sta_postplace_opr )
  {
    if ( ( srcNode->type == internal_pin ) || ( srcNode->type == flipflop_pin ) )
    {
      if ( isInputPin ( srcNode ) == true )
      {
        return srcNode->techInfo.delay;
      }
      else if ( isOutputPin ( srcNode ) == true )
      {
        return calculateWireDelay ( srcNode, snkNode );
      }
    }
    else if ( srcNode->type == primary_pin )
    {
      if ( isInputPin ( srcNode ) == true )
      {
        //         cout<<"calculateWireDelayForPI(srcNode, snkNode) = "<<calculateWireDelayForPI(srcNode, snkNode)<<endl;
        return calculateWireDelayForPI ( srcNode, snkNode );
      }
      else
      {
        return 0;
      }
    }
    else
    {
      printf ( "Error in STA operation mode setting" );
      return 0;
    }
  }
}

