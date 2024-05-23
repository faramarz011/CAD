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
#include <part.h>
#include <stdio.h>
#include <math.h>

#include <iostream>
#include <list>
#include <vector>
#include <map>

using namespace std;
#include <atlasDB.h>
float BALANCE_RATIO = 0.1; // |Size(A) - Size(B)| <= 0.1 * Size(all)
int maxIndex = 0;
int BUCKET_SIZE = BUCKET_MAX_SIZE;
int unbalanceBound;
//-----------------------------------------------------------
// Constructors
//-----------------------------------------------------------
fmPart::fmPart()
{
	nList = new list <fmNode *>;
	eList = new list <fmEdge *>;
	moves = new list <fmMove *>;
	updatedNodes = new list <fmNode *>;
	for(int i = 0;i < BUCKET_SIZE;i ++)
		bucket[i] = new list <fmNode *>;
}

//-----------------------------------------------------------
// Destructors
//-----------------------------------------------------------
fmPart::~fmPart()
{
}

int fmPart::BiPart ( unsigned _gpn)
{
	bool endCondition = false;
	fmNode *maxGainNode = NULL;
	fmMove *lastMove;
	int i, j = 0;
// 	printNetlist();
	initPart (_gpn);
	BUCKET_SIZE = 4*findMaxNodeDegree() + 1;
	unbalanceBound  = (int)(parentPartSize(_gpn) * BALANCE_RATIO);

	printf ( "\t-------------------------------------- \n");
	printf ( "\tPartitioning Part(%d) , size = %d \n", _gpn, parentPartSize(_gpn));
	printf ( "\tInitial cutsize = %d\n", cutsize(_gpn));
	while ( endCondition == false )
	{
		printf ( "\t\tIteration %d: ", j );
		for (i = 0; i < nList->size();i ++ )
		{
			if ( i == 0 ) // First Move
				maxGainNode = computeGains(_gpn);
			else
				maxGainNode = updateGains ( _gpn, lastMove->node );
//			PrintBucket();
//			printf("max = %s, g = %d\n", maxGainNode->nodeName, maxGainNode->gain); 

			maxGainNode->pn = 1 - maxGainNode->pn;
			lastMove = new fmMove;
			lastMove->node = maxGainNode;
			lastMove->gain = maxGainNode->gain;
			moves->push_back ( lastMove );
		}
		endCondition = analyzeMoves(); // Accept Mamimum Gain Move Sequence
		moves->erase ( moves->begin(), moves->end() );
		j ++;
		printf ( " cutsize = %d \n", cutsize(_gpn) );
		unlockAll();
	}
//	printParts(_gpn);
	unsigned finalCutsize = cutsize(_gpn);
	printf ( "\tFinal cutsize = %d\n", finalCutsize );
	printf ( "\tPart(%d).size = %d, part(%d).size = %d\n", 2*_gpn+1, partSize(_gpn, 0), 2*_gpn+2, partSize(_gpn, 1));
	SetPartsIndex(_gpn);
	return 10;
}
//*****************************
// compute gain of all cells
//*****************************
fmNode *fmPart::computeGains(int _gpn)
{
	list <fmNode *>::iterator iter1, iter3;
	list <fmEdge *>::iterator iter2;
	fmNode *aNode1, *aNode2;
	fmEdge *anEdge;
	bool foundNode, hasFS, hasTE;
	int maxGain = 0x80000000;
	fmNode *maxGainNode = NULL;
	int unbalanceDegree;
	int cnt = 0;

	for(int i = 0;i < BUCKET_SIZE;i ++)
		bucket[i]->erase(bucket[i]->begin(), bucket[i]->end());

	for ( iter1 = nList->begin(); iter1 != nList->end(); iter1 ++ )
	{
		aNode1 = ( fmNode * ) ( * iter1 );
		aNode1->fs = 0;
		aNode1->te = 0;
		if ( aNode1->gpn == _gpn)
		{
			for ( iter2 = aNode1->connectedEdges->begin(); iter2 != aNode1->connectedEdges->end(); iter2 ++ )
			{
				anEdge = ( fmEdge * ) ( * iter2 );
				hasFS = true;
				hasTE = true;
				for ( iter3 = anEdge->nodes->begin(); iter3 != anEdge->nodes->end(); iter3 ++ )
				{
					aNode2 = ( fmNode * ) ( * iter3 );
					if (aNode2->gpn == _gpn)
					{
						if ( aNode2 != aNode1 )
						{
							if ( aNode2->pn == aNode1->pn )
								hasFS = false;
							if ( aNode2->pn != aNode1->pn )
								hasTE = false;
						}
					}
				}
				if ( hasFS == true )
					aNode1->fs ++;
				if ( hasTE == true )
					aNode1->te ++;
			}
		
			aNode1->gain = aNode1->fs - aNode1->te;
			unbalanceDegree = abs ( partSize (_gpn, (1 - aNode1->pn )) - partSize (_gpn, aNode1->pn) + 2) ;
	//		printf("UBD = %d UBB = %d\n", unbalanceDegree , unbalanceBound);
			if (aNode1->gain >  maxGain ) 
				maxIndex = maxGain + (BUCKET_SIZE/2); 

			if ( ( aNode1->gain >  maxGain ) && ( unbalanceDegree <=  unbalanceBound ) )
			{
				maxGain = aNode1->gain;
				maxGainNode = aNode1;
				//maxIndex = maxGain + (BUCKET_SIZE/2); 
			}
			bucket[aNode1->gain + (BUCKET_SIZE/2)]->push_back(aNode1);
			cnt++;
		}
	}
	if ( maxGainNode == NULL )
	{
		printf ( "Error: cannot find max gain node.\n" );
		exit ( 1 );
	}
	else
	{
		int index = maxGainNode->gain + (BUCKET_SIZE/2);
		for(iter1 =  bucket[index]->begin();iter1 != bucket[index]->end();iter1++)
			if ( (*iter1)->nodeID == maxGainNode->nodeID)
			{
				bucket[index]->erase(iter1);
				(*iter1)-> lock = true;
				break;
			}
	}
	return maxGainNode;
}

//************************************
// Update gain of the selected cells
//************************************
fmNode *fmPart::updateGains ( int _gpn, fmNode *movedNode )
{
	list <fmNode *>::iterator iter1, iter3, iter4;
	list <fmEdge *>::iterator iter2;
	fmNode *aNode1, *aNode2;
	fmEdge *anEdge;
	int index;
	bool foundNode, hasFS, hasTE;
	int oldGain, maxGain = 0x80000000;
	fmNode *maxGainNode = NULL;
	unsigned unbalanceDegree;
	bool balanceCondition = false;

  updatedNodes->erase(updatedNodes->begin(), updatedNodes->end());
//	printf("UpdateGain, size(0) = %d, size(1) = %d\n", partSize(_gpn, 0), partSize(_gpn, 1) );
	for ( iter1 = movedNode->neighborNodes->begin(); iter1 != movedNode->neighborNodes->end(); iter1 ++ )
	{
		aNode1 = ( fmNode * ) ( * iter1 );
		if ( (aNode1->gpn == _gpn) && (aNode2->lock == false) ) 
			updatedNodes->push_back ( aNode1 );
	}
	for ( iter1 = updatedNodes->begin(); iter1 != updatedNodes->end(); iter1 ++ )
	{
		aNode1 = ( fmNode * ) ( * iter1 );
		aNode1->fs = 0;
		aNode1->te = 0;
		for ( iter2 = aNode1->connectedEdges->begin(); iter2 != aNode1->connectedEdges->end(); iter2 ++ )
		{
			anEdge = ( fmEdge * ) ( * iter2 );
			if (aNode1->gpn == _gpn)
			{
				hasFS = true;
				hasTE = true;
				for ( iter3 = anEdge->nodes->begin(); iter3 != anEdge->nodes->end(); iter3 ++ )
				{
					aNode2 = ( fmNode * ) ( * iter3 );
					if ( aNode2->gpn == _gpn )
					{
						if ( aNode2 != aNode1 )
						{
							if ( aNode2->pn == aNode1->pn )
								hasFS = false;
							if ( aNode2->pn != aNode1->pn )
								hasTE = false;
						}
					}
				}
				if ( hasFS == true )
					aNode1->fs ++;
				if ( hasTE == true )
					aNode1->te ++;
			}
		}
		index = aNode1->gain + (BUCKET_SIZE/2);
		for(iter4 =  bucket[index]->begin();iter4 != bucket[index]->end();iter4++)
		{
			if ( (*iter4)->nodeID == aNode1->nodeID)
			{
				bucket[index]->erase(iter4);
				(*iter4) -> lock = true;
				break;
			}
		}
		aNode1->gain = aNode1->fs - aNode1->te;
		index = aNode1->gain + (BUCKET_SIZE/2);
	  printf("P1\n");
		bucket[index]->push_back(aNode1);
		if (index > maxIndex)
			maxIndex = index;
	}
if (_gpn == 2)
	while (bucket[maxIndex]->size() == 0)
	{
		maxIndex --;
	}
	maxGainNode = NULL;
	index = maxIndex;
	while( (!balanceCondition) && (index >= 0))
	{
		balanceCondition = false;
		for(iter1 = bucket[index]->begin();iter1 != bucket[index]->end();iter1 ++)
		{
			aNode1 = (fmNode *)(*iter1);
			unbalanceDegree = abs ( partSize (_gpn, (1 - aNode1->pn )) - partSize (_gpn, aNode1->pn) + 2) ;
//				printf("UBD = %d UBB = %d bucket[%d]=%d\n", unbalanceDegree, unbalanceBound, index, bucket[index]->size() );
			if (unbalanceDegree <=  unbalanceBound )
			{
				maxGainNode = aNode1;
				balanceCondition = true;
				bucket[index]->erase(iter1);
				break;
			}
		}
		if (balanceCondition == false)
		{
			index --;
			while ((bucket[index]->size() == 0) && (index >= 0))
				index--;
			if (index < 0)
			{
				printf("ERROR: Cannot find filled row.\n");
				exit(1);
			}
		}
	}
	if (maxGainNode == NULL)
	{	
		printf("Cannot find maxGainNode\n");
		exit(1);
	}
	return maxGainNode;
}


bool fmPart::analyzeMoves()
{
	list <fmMove *>::iterator iter;
	fmMove *aMove;
	int sumOfGains, maxGain = 0, maxGainMoveIndex = -1, i;

	sumOfGains = 0;
	for ( i = 0, iter = moves->begin();iter != moves->end();iter ++, i++ )
	{
		aMove = ( fmMove * ) ( *iter );
		sumOfGains += aMove->gain;
		if ( sumOfGains >= maxGain )
		{
			maxGain = sumOfGains;
			maxGainMoveIndex = i;
		}
	}
	if ( maxGain <= 0 )
		maxGainMoveIndex = -1;

	printf ( "MaxG = %d , maxGInx = %d , ", maxGain, maxGainMoveIndex );

	for ( i = 0, iter = moves->begin();iter != moves->end();iter ++, i ++ )
	{
		aMove = ( fmMove * ) ( *iter );
		if ( i > maxGainMoveIndex )
			aMove->node->pn = 1 - aMove->node->pn;
	}
	if ( maxGain > 0 )
		return false;
	else
		return true;
}

void fmPart::initPart (int _gpn)
{
	list <fmNode *>::iterator iter;
	fmNode *aNode;
	unsigned i = 0, ps, ii = 0, jj = 0;

	ps = parentPartSize(_gpn) / 2;
	for (iter = nList->begin(); iter != nList->end(); iter ++)
	{
		aNode = ( fmNode * ) ( * iter );
		if (aNode->gpn == _gpn)
		{
			if ( i < ps )
			{
				jj ++;	
				aNode->pn = 0;
			}
			else
			{
				ii ++;
				aNode->pn = 1;
			}
			i ++;
		}
	}
}


unsigned fmPart::cutsize(int _gpn)
{
	list <fmEdge *>::iterator iter1;
	list <fmNode *>::iterator iter2;
	fmNode *aNode1, *aNode2;
	fmEdge *anEdge;
	unsigned cs = 0;

	for ( iter1 = eList->begin(); iter1 != eList->end(); iter1 ++ )
	{
		anEdge = ( fmEdge * ) ( * iter1 );
		iter2 = anEdge->nodes->begin();
		aNode1 = ( fmNode * ) ( * iter2 );
		if (aNode1->gpn == _gpn)
		{
			iter2 ++;
			for ( ;iter2 != anEdge->nodes->end(); iter2 ++ )
			{
				aNode2 = ( fmNode * ) ( * iter2 );
				if (aNode1->gpn == _gpn)
				{
					if ( aNode1->pn != aNode2->pn )
					{
						cs ++;
						break;
					}
				}
			}
		}	
	}
	return cs;
}

void fmPart::printNetlist()
{
	list <fmEdge *>::iterator iter1;
	list <fmNode *>::iterator iter2, iter3;
	fmNode *aNode, *aNode2;
	fmEdge *anEdge;

	for ( iter1 = eList->begin(); iter1 != eList->end(); iter1 ++ )
	{
		anEdge = ( fmEdge * ) ( * iter1 );
		printf ( "Net %d <%s>\n", anEdge->edgeID, anEdge->edgeName );
		for ( iter2 = anEdge->nodes->begin(); iter2 != anEdge->nodes->end(); iter2 ++ )
		{
			aNode = ( fmNode * ) ( * iter2 );
			if ( aNode->type == FM_CELL )
				printf ( "\tNode %d <%s>\n", aNode->nodeID, aNode->nodeName );
			else if ( aNode->type == FM_PIN )
				printf ( "\tPIN %d <%s>\n", aNode->nodeID, aNode->nodeName );
		}
	}

	printf ( "\n\nNeighboring Nodes:\n" );
	for ( iter2 = nList->begin(); iter2 != nList->end(); iter2 ++ )
	{
		aNode = ( fmNode * ) ( * iter2 );
		printf ( "\tNode <%s> with %d neighbors:\n\t\t", aNode->nodeName, aNode->neighborNodes->size() );
		for ( iter3 = aNode->neighborNodes->begin(); iter3 != aNode->neighborNodes->end(); iter3 ++ )
		{
			aNode2 = ( fmNode * ) ( * iter3 );
			printf ( "<%s> ", aNode2->nodeName );
		}
		printf ( "\n" );
	}


	printf ( "\n\nConnected Edges:\n" );
	for ( iter2 = nList->begin(); iter2 != nList->end(); iter2 ++ )
	{
		aNode = ( fmNode * ) ( * iter2 );
		printf ( "\tNode <%s> with %d connected edges:\n\t\t", aNode->nodeName, aNode->connectedEdges->size() );
		for ( iter1 = aNode->connectedEdges->begin(); iter1 != aNode->connectedEdges->end(); iter1 ++ )
		{
			anEdge = ( fmEdge * ) ( * iter1 );
			printf ( "<%s> ", anEdge->edgeName );
		}
		printf ( "\n" );
	}
}

void fmPart::printParts(int _gpn)
{
	list <fmNode *>::iterator iter1;
	fmNode *aNode;
	int i;

	printf ( "\n\tPart 0/%d:\n\t\t", _gpn );
	for ( i = 1, iter1 = nList->begin(); iter1 != nList->end(); iter1 ++ )
	{
		aNode = ( fmNode * ) ( * iter1 );
		if (aNode->gpn == _gpn)
		{			
			if ( aNode->pn == 0 )
			{
				printf ( "%s ", aNode->nodeName);
				if ( ( i % 10 ) == 0 )
					printf ( "\n\t\t" );
				i ++;
			}
		}
	}
	printf ( "\n\tPart 1/%d:\n\t\t", aNode->gpn );
	for ( i = 1, iter1 = nList->begin(); iter1 != nList->end(); iter1 ++ )
	{
		aNode = ( fmNode * ) ( * iter1 );
		if (aNode->gpn == _gpn)
		{			
			if ( aNode->pn == 1 )
			{
				printf ( "%s ", aNode->nodeName);
				if ( ( i % 10 ) == 0 )
					printf ( "\n\t\t" );
				i ++;
			}
		}
	}
	printf ( "\n" );
}

void fmPart::SetPartsIndex(unsigned _gpn)
{
	list <fmNode *>::iterator iter1;
	fmNode *aNode;
	int i;

	for ( i = 1, iter1 = nList->begin(); iter1 != nList->end(); iter1 ++ )
	{
		aNode = ( fmNode * ) ( * iter1 );
		if (aNode->gpn == _gpn)
			aNode->gpn = 2 * aNode->gpn + aNode->pn + 1;
	}
}
// size of initial partitrion
int fmPart::parentPartSize(int _gpn)
{
	list <fmNode *>::iterator iter;
	fmNode *aNode;
	int size = 0;

	for (iter = nList->begin(); iter != nList->end(); iter ++)
	{
		aNode = ( fmNode * ) ( * iter );
		if (aNode->gpn == _gpn)
			size ++;
	}
	return size;
}

unsigned fmPart::partSize ( int _gpn, int _pn )
{
	list <fmNode *>::iterator iter1;
	fmNode *aNode;
	unsigned size = 0;

	for ( iter1 = nList->begin(); iter1 != nList->end(); iter1 ++ )
	{
		aNode = ( fmNode * ) ( * iter1 );
		if ( aNode->gpn == _gpn )
			if ( aNode->pn == _pn )
				size ++;
	}
	return size;
}
void fmPart::PrintBucket()
{
	list <fmNode *>::iterator iter;
	fmNode *aNode;
	printf("BUCKET:\n");
	for(int i = BUCKET_SIZE-1; i >= 0;i --)
	{
		if (bucket[i]->size() > 0)
		{
			printf("\tB[%d][%d]:",i - BUCKET_SIZE/2, i);
			for(iter = bucket[i]->begin(); iter != bucket[i]->end(); iter ++)
			{
				aNode = ( fmNode * ) ( * iter );
				printf("%s(%d)", aNode->nodeName, aNode->gain);
			}
			printf("\n");
		}
	}
}

int fmPart::findMaxNodeDegree()
{
	list <fmNode *>::iterator iter;
	fmNode *aNode;
	int maxDegree = 0;

	for (iter = nList->begin(); iter != nList->end(); iter ++ )
	{
		aNode = (fmNode *)(*iter);
		if ( aNode->connectedEdges->size() > maxDegree )
			maxDegree = aNode->connectedEdges->size();
	}
	return maxDegree;
}


fmNode *fmPart::getNodeByName(char *iName)
{
	list <fmNode *>::iterator iter;
	fmNode *aNode;

	for (iter = nList->begin(); iter != nList->end(); iter ++ )
	{
		aNode = ( fmNode * ) (*iter);
		if ( (strcmp(aNode->nodeName, iName)) == 0)
			return aNode;
	}
	return NULL;
}

fmNode *fmPart::getNodeByIndex(int iID)
{
	list <fmNode *>::iterator iter;
	fmNode *aNode;

	for (iter = nList->begin(); iter != nList->end(); iter ++ )
	{
		aNode = ( fmNode * ) (*iter);
		if (aNode->nodeID == iID)
			return aNode;
	}
	return NULL;
}

void fmPart::unlockAll()
{
	list <fmNode *>::iterator iter;
	fmNode *aNode;
	for(iter = nList->begin();iter != nList->end();iter++)
	{
		aNode = (fmNode *)(*iter);
		aNode->lock = true;
	}
}
