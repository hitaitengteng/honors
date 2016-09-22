/**************************************************************************
 *
 * File: window.h
 * Author: William Gantt
 * Date: Mon Jul 11 2016
 *
 * Description: Type declaration and function prototypes for the window
 * 		data type. 
 *
 *              A note on terminology: In several places in this code, I
 *              use the terms "window" and "module" seemingly
 *              interchangeably. The window is merely the program's
 *              representation of a module. Given the way the program is
 *              written, however, some of the motifs in the module may
 *              in fact fall outside the boundaries of the window. These
 *              motifs will be identified as part of the module in the
 *              motifs array (that is, the index corresponding to those
 *              motifs will be marked with a '1') in the window struct,
 *              even though they may fall partially outside the beginning
 *              or ending indices.
 *
 **************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include "motif.h"

/* TYPE DECLARATIONS */

/* the window datatype */
typedef struct window {

  /* the beginning index of the window */
  int begin;

  /* the size of the window */
  int size;

  /* the overall score of the window */
  double score;

  /* the total number of motifs in the library being used */
  int totalNumMotifs;

  /* 
   * an array containing the IDs of these motifs. Each
   * index in the array corresponds to a single motif.
   */
  int *motifs;

} window;

/* FUNCTIONS */
/* (see window.c for documentation) */

window *newWindow(int begin, int size, double score, 
                  int totalNumMotifs, int *motifs);

window *initWindow(void);

window *copyWindow(window *source, window *dest);

void freeWindow(window *w);

void freeWindowArray(window *wArray, int length);

int numBasesInWindow(int loc, int windowBegin, int windowEnd);

window **findBestModules(motif *motArray, int numMotifs, int numBest,
			 int motifLength, int windowStep, int windowSize,
			 int numSequences, int seqLength);

void printBestModules(window **bestWindowsBySequence, 
                      int numSequences, int numBest);

#endif     /* WINDOW_H */
