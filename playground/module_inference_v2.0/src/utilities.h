/**************************************************************************
 *
 * File: utilities.h
 * Author: William Gantt
 * Date: Mon Jul 11 2016
 *
 * Description: Contains includes, named constants, and basic function
 * 		prototypes for the rest of the module comparison program.
 *
 **************************************************************************/

#ifndef UTILITIES_H
#define UTILITIES_H

/* INCLUDES */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "extern.h"


/* NAMED CONSTANTS */

/* 
 * NOTE: all variables pertaining to motifs, instances of motifs,
 * sequences, and scores, are used primarily for generating random
 * data
 */

/* The number of motifs to generate */
#define NUM_MOTIFS 10

/* The number of bases in a motif */
#define MOTIF_LENGTH 10

/*
 * The maximum number of instances of a given motif that may
 * occur in a single genomic sequence
 */
#define MAX_INSTANCES 3

/* The maximum and minimum possible scores of a motif */
#define MAX_SCORE 10
#define MIN_SCORE 8

/* The number of genomic sequences to use (when creating random data) */
#define NUM_SEQUENCES 6

/* The length of a genomic sequence */
#define SEQ_LENGTH 1000

/*
 * When identifying modules, the number of best modules to report
 * for a given genomic sequence
 */
#define N_BEST 5

/*
 * The size of the window used to identify modules within a
 * genomic sequence
 */
#define WINDOW_SIZE 200

/*
 * The number of base pairs by which a window is incremented
 * when searching a genomic sequence for modules
 */
#define WINDOW_STEP 25

/* Boolean values */
#define TRUE 1
#define FALSE 0

/* 
 * For keeping track of which indices in a sequence
 * are not taken up by motifs
 */
#define USED 1
#define EMPTY 0
#define NO_FREE_SPACE -1

/* the gantt file type */
#define GANTT_FILE "GANTT_FILE"

/* FUNCTION PROTOTYPES */
/* (see utilities.c for documentation) */

void *emalloc(long size);
void *erealloc(void *ptr, long size);
void *ecalloc(int num, long size);

int isFree(int index, int *sequenceIndices);
int findFreeSpace(int *sequenceIndicies, int seqLength);

#endif     /* UTILITIES_H */
