/**************************************************************************
 *
 * File: motif.h
 * Author: William Gantt
 * Date: Mon Jul 11 2016
 *
 * Description: Type declaration and function prototypes for the motif
 * 		data type.
 *
 **************************************************************************/

#ifndef MOTIF_H
#define MOTIF_H

#include "instanceSet.h"

/* TYPE DEFINITIONS */

/* 
 * The motif data type. Contains an array of instanceSets,
 * one entry per sequence in which the motif occurs
 */
typedef struct motif {

  /* the ID number for the motif */
  int motifID;

  /* the number of instanceSet structs in the instanceSet array */
  int numInstanceSets;

  /* 
   * an array of instanceSets, one entry per sequence
   * in which the motif occurs
   */
  instanceSet *allInstanceSets;

} motif;


/* FUNCTION PROTOTYPES */
/* (see motif.c for documentation) */

motif *initMotif(void);

motif *newMotif(int motifID, int numInstanceSets, instanceSet *instanceSetArray);

motif *randMotif(int motifID, int numInstanceSets, 
                 int numInstances, int seqLength);

motif *randMotifArray(int numMotifs, int numInstanceSets,
                      int numInstances, int seqLength);

motif *randomData(int numMotifs, int maxInstances,
		  int numSequences, int *seqLength);

motif *copyMotif(motif *source, motif *dest);

void freeMotif(motif *mot);

void freeMotifArray(motif *motArray, int length);

void printMotifArrayByMotif(motif *motArray, int length, int numSequences);

void printMotifArrayBySequence(motif *motArray, int length, int numSequences);

void readInPPData(char *fileName);

#endif     /* MOTIF_H */
