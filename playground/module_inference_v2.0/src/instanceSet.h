/**************************************************************************
 *
 * File: instanceSet.h
 * Author: William Gantt
 * Date: Mon Jul 11 2016
 *
 * Description: Type declaration and function prototypes for the
 * 		instanceSet data type.
 *
 **************************************************************************/

#ifndef INSTANCESET_H
#define INSTANCESET_H

#include "instance.h"

/* TYPE DEFINITIONS */

/* 
 * a structure containing all of the instances of a
 * particular motif for a particular genomic sequence
 */
typedef struct instanceSet {

  /* the ID number for the DNA sequence */
  int seqID;

  /* the length of the sequence */
  int seqLength;

  /* the number of instances in the sequence */
  int numInstances;

  /* an array of instances for this sequence */
  instance *instanceArray;

} instanceSet;

/*
 * a structure containing information about a
 * particular sequence
 */
typedef struct sequence {

  /* the ID number of the sequence (may be irrelevant) */
  int idNum;

  /* the name of the sequence (usually the species name) */
  char *name;

  /* the length of the sequence */
  int length;

} sequence;

/* FUNCTION PROTOTYPES */
/* (see instanceSet.c for documentation) */

instanceSet *initInstanceSet(void);

instanceSet *newInstanceSet(int seqID, int seqLength,
                            int numInstances, instance *instanceArray);

instanceSet *randInstanceSet(int seqID, int seqLength, 
                             int *seqIndices, int numInstances,
                             double minScore, double maxScore);

instanceSet *copyInstSet(instanceSet *source, instanceSet *dest);

void freeInstSet(instanceSet *instSet);

void freeInstSetArray(instanceSet *instSetArray, int length);

#endif     /* INSTANCESET_H */
