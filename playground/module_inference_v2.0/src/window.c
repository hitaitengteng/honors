 /**************************************************************************
 *
 * File: window.c
 * Author: William Gantt
 * Date: Thu Jun 23 09:14 2016
 *
 * Description: Functions for the window data type.
 *
 * Functions:
 *
 * 	- newWindow: Creates a new window with the specified values.
 * 	- initWindow: Creates a new window and zeroes all values except
 * 	  the pointer to the motifs array, which it sets to NULL.
 * 	- copyWindow: Deep-copies one window struct into another 
 * 	- freeWindow: Completely frees a window struct.
 * 	- freeWindowArray: Completely frees an array of window structs
 * 	- numBasesInWindow: Given a motif and the beginning and ending
 * 	  indices of a window, determines how many of the motif's bases
 * 	  fall within the window.
 * 	- findBestModules: Given a library of motif structs (which include
 * 	  instances of those motifs on some number of sequences), finds the
 * 	  best candidate modules in each sequence.
 *      - printBestModules: Prints the best modules found by the
 *        findBestModules function.
 *
 **************************************************************************/ 

#include "window.h"

/***************************************************************************
 * Function:    newWindow
 * Inputs:      The beginning index of the window (begin), the size of the
 * 		window in base pairs (size), the total score of the window
 * 		(score), the total number of motifs in the library being
 * 		used (totalNumMotifs) and an array containing the IDs of 
 * 		those motifs.
 * Returns:     A pointer to the newly created window.
 * Description: Creates a new window struct with the values specified.
 **************************************************************************/
window *newWindow(int begin, int size, double score, 
                  int totalNumMotifs, int *motifs) {

  /* allocate memory for the struct */
  window *w = emalloc(sizeof(window));

  /* allocate memory for the array of motif IDs */
  w->motifs = emalloc(sizeof(int) * totalNumMotifs);

  /* copy values into the struct */
  w->begin = begin;
  w->size = size;
  w->score = score;
  w->totalNumMotifs = totalNumMotifs;

  /* copy values from the array of motif IDs into the struct */
  for (int i=0; i<totalNumMotifs; i++)
    w->motifs[i] = motifs[i];

  return w;

}     /* end newWindow */

/***************************************************************************
 * Function:    initWindow
 * Inputs:      N/A
 * Returns:     A pointer to the newly created window.
 * Description: Creates a new window struct, and sets all attributes to
 * 		either 0 or NULL, as appropriate.
 **************************************************************************/
window *initWindow(void) {

  /* allocate memory for the struct */
  window *w = emalloc(sizeof(window));

  /* zero and NULL all values */
  w->begin = 0;
  w->size = 0;
  w->score = 0;
  w->totalNumMotifs = 0;
  w->motifs = NULL;

  return w;

}     /* end initWindow */

/***************************************************************************
 * Function:    copyWindow
 * Inputs:      A pointer to the window to be copied from (source), and
 * 		a pointer to the window to be copied to (dest).
 * Returns:     A pointer to the "dest" window.
 * Description: Deep-copies one window into another.
 **************************************************************************/
window *copyWindow(window *source, window *dest) {

  /* copy values from source to dest */
  dest->begin = source->begin;
  dest->size = source->size;
  dest->score = source->score;
  dest->totalNumMotifs = source->totalNumMotifs;

  dest->motifs = erealloc(dest->motifs, sizeof(int) * source->totalNumMotifs);
  /* copy the array values from source to dest */
  for (int i=0; i<source->totalNumMotifs; i++)
    dest->motifs[i] = source->motifs[i];

  return dest; 

}     /* end copyWindow */

/***************************************************************************
 * Function:    freeWindow
 * Inputs:      A pointer to the window struct to be freed (w).
 * Returns:     N/A
 * Description: Completely frees a window struct.
 **************************************************************************/
void freeWindow(window *w) {

  /* first, free the array of motifs */
  free(w->motifs);

  /* free the actual struct and set the pointer to NULL */
  free(w);
  w = NULL;

}     /* end freeWindow */

/***************************************************************************
 * Function:    freeWindowArray
 * Inputs:      A pointer to the array of window structs to be freed (w),
 *              and the length of the array (length).
 * Returns:     N/A
 * Description: Completely frees a window struct.
 **************************************************************************/
void freeWindowArray(window *wArray, int length) {

  /*
   * first, iterate through each window in the array and free its
   * individual motif array
   */
  for (int i=0; i<length; i++)
    free(wArray[i].motifs);

  /* finally, free the remainder of the array */
  free(wArray);

}     /* end freeWindowArray */

/***************************************************************************
 * Function:    numBasesInWindow
 * Inputs:      The location within the genomic sequence of the motif 
 *              instance to be checked (loc), the beginning index of the 
 *              window (windowBegin), and the ending index of the window
 *              (windowEnd).
 * Returns:     The number of the motif instance's bases that lie within the
 *              window.
 * Description: Given a window along a genomic sequence and the location of
 *              a motif instance on that sequence, determines the number of
 *              the instance's base pairs that lie within the window.
 *
 *              NOTE: motif length should probably be passed as a
 *                    parameter.
 **************************************************************************/
int numBasesInWindow(int loc, int windowBegin, int windowEnd) {

  /* the beginning and ending indices of the motif instance */
  int instanceBegin = loc;
  int instanceEnd = loc + (MOTIF_LENGTH - 1);

  /* case 1: the instance begins to the left of the window */
  if (instanceBegin < windowBegin) {
    
    /* 
     * if the instance begins MOTIF_LENGTH base pairs or more to the
     * left of the window, then the instance has no bases inside
     * the window, so we return 0
     */
    if ((windowBegin - instanceBegin) >= MOTIF_LENGTH)
      return 0;

    /* otherwise, return the number of bases in the window */
    else
      return (MOTIF_LENGTH - (windowBegin - instanceBegin));

  }     /* end if */

  /* case 2: the instance ends to the right of the window */ 
  else if (instanceEnd > windowEnd) {
    
    /* 
     * if the instance ends MOTIF_LENGTH base pairs or more to the
     * right of the window, then the instance has no bases inside
     * the window, so we return 0
     */
    if ((instanceEnd - windowEnd) >= MOTIF_LENGTH)
      return 0;

    /* otherwise, return the number of bases in the window */
    else
      return (MOTIF_LENGTH - (instanceEnd - windowEnd));

  }     /* end else if */

  /* 
   * case 3: the instance lies entirely within the window (if neither
   * of the above cases is satisfied, then this must be true)
   */
  else
    return MOTIF_LENGTH;

}     /* end numBasesInWindow */

/***************************************************************************
 * Function:    findBestModules
 * Inputs:      A library of motifs from which to construct modules
 * 		(motArray), the number of motifs in the library (numMotifs),
 * 		the number of best candidate modules to keep for each
 * 		sequence (numBest), the length of the motifs (motifLength),
 * 		the number of indices by which the window is to be
 * 		incremented at each iteration (windowStep), the length of
 * 		the window in base pairs (windowSize), the number of
 * 		sequences in which to search for modules (numSequences),
 * 		and the length of the sequences.
 * Returns:     An array of the best windows in each sequence.
 * Description: Given a library of motifs, iterates over numSequences
 * 		genomic sequences and finds the numBest candidate modules
 * 		(represented by a window struct) in each sequence.
 *
 * 		NOTE: seqLength should be expanded into an array to allow
 * 		      for variable sequence lengths.
 **************************************************************************/
window **findBestModules(motif *motArray, int numMotifs, int numBest,
                      int motifLength, int windowStep, int windowSize,
                      int numSequences, int seqLength) {

  /* an array to hold all of the best windows in all of the sequences */
  window **bestWindowsBySequence = emalloc(sizeof(window*) * numSequences);

  /*
   * a variable to keep track of the numBest
   * best windows in the current sequence
   */
  window *bestWindows = NULL;

  /* an array to keep track of the motifs that occur within a given window */
  int *motifsInWindow = ecalloc(numMotifs, sizeof(int));

  /* iterate over all sequences */
  for (int s=0; s<numSequences; s++) {

    /* an array to keep track of the best windows in this sequence */
    bestWindows = emalloc(sizeof(window) * numBest);

    /* initialize the elements of the array */
    for (int i=0; i<numBest; i++) {

      window *w = initWindow();
      bestWindows[i] = *w;
      freeWindow(w);

    }     /* end for (i) */

    /* the number of times the window will be moved */
    int totalWindowIters = seqLength / windowStep;

    /*
     * for determining the numBest windows in a sequence, it is
     * important to maintain the index of the worst window
     * among the numBest, so as to know when we have found a
     * window that scores high enough to replace one of them
     */
    int minWindowIndex = 0;

    /* a variable for iterating through the bestWindows array */
    int bestWindowsIndex = 0;

    /* a pointer to the current window */
    window *currWindow = NULL;

    /* move the window */
    for (int w=0; w<totalWindowIters; w++) {

      /* create a new window struct */
      currWindow = 
        newWindow(w * windowStep, windowSize, 0.0, numMotifs, motifsInWindow);

      /* get the beginning and ending indices of the current window */
      int windowBegin = currWindow->begin;
      int windowEnd = windowBegin + (currWindow->size - 1);

      /* a pointer to the current motif */
      motif *currMotif = NULL;

      /* iterate over all motifs */
      for (int m=0; m<numMotifs; m++) {

        /* get the current motif */
        currMotif = &(motArray[m]);

        /* the score of the best instance of this motif in this window */
        double bestInstanceScore = 0.0;

        /* 
         * the number of instances of the current
         * motif in the entire sequence
         */
        int numInstances = currMotif->allInstanceSets[s].numInstances;

        /* a pointer to the current instance */
        instance *currInstance = NULL;

        /* iterate over the instances */
        for (int i=0; i<numInstances; i++) {

          /* get the current instance */
          currInstance = &(currMotif->allInstanceSets[s].instanceArray[i]);

          /*
           * determine the number of the instance's bases
           * that fall within the window
           */
          int basesInWindow = 
            numBasesInWindow(currInstance->loc, windowBegin, windowEnd);

          /* compute the score of the current instance */
          double instanceScore = 
            ((double) basesInWindow / (double) motifLength) * currInstance->score;

          /*
           * if the current motif has an instance with a score greater than
           * 0, then we know that there must be an instance of that motif
           * in the current window. Consequently, we update the motifsInWindow
           * array.
           */ 
          if (instanceScore > 0) {

            currWindow->motifs[m] = 1;

            /* update the best instance score, if necessary */
            if (instanceScore > bestInstanceScore)
              bestInstanceScore = instanceScore;

          }     /* end if */

        }     /* end for (i) */

        /* add the best instance score to the total score for the window */
        currWindow->score += bestInstanceScore;

      }     /* end for (m) */

      /* 
       * if we have not already collected numBestWindows for the
       * current sequence, then add the current window to the
       * bestWindows array.
       */
      if (bestWindowsIndex < numBest) { 

        copyWindow(currWindow, &(bestWindows[bestWindowsIndex]));
       
        /* if necessary, update the index of the lowest-scoring window */
        if (currWindow->score <= bestWindows[minWindowIndex].score)
          minWindowIndex = bestWindowsIndex;

        bestWindowsIndex++;

      }     /* end if */

      /*
       * otherwise, we have already found numBest windows and need
       * to check whether the current window's score is higher than
       * that of the lowest-scoring window among the numBest
       */ 
      else if (currWindow->score > bestWindows[minWindowIndex].score) {

        copyWindow(currWindow, &(bestWindows[minWindowIndex]));

        /*
         * if the current window score is greater than the minimum window
         * score, then we need to check whether it is the new minimum
         * (since the new minimum could also be one of the numBest windows
         * that were not replaced).
         */
        for (int n=0; n<numBest; n++) {

          if (bestWindows[n].score < bestWindows[minWindowIndex].score)
            minWindowIndex = n;

        }     /* end for (n) */
 
      }     /* end else if */

      /*
       * free the current window, since, if we needed it,
       * it has already been copied into the array of
       * best windows
       */
      freeWindow(currWindow);

    }     /* end for (w) */

    /*
     * add the bestWindows array for the current sequence
     * to the array of bestWindows for all sequences
     */
    bestWindowsBySequence[s] = bestWindows;

  }     /* end for (s) */

  /* print out information about the modules that were found */
  printBestModules(bestWindowsBySequence, numSequences, numBest);

  return bestWindowsBySequence;

}     /* end findBestModules */

/***************************************************************************
 * Function:    printBestModules
 * Inputs:      An array of pointers to arrays of best windows in a
 * 		sequence, the number of sequences, and the number of
 * 		best windows per sequence.
 * Returns:     N/A
 * Description: Prints out information about the best modules found
 * 		in a set of sequences.
 *
 * 		NOTE: Fix formatting of print statements so that they fit
 * 		      80 columns to a line.
 **************************************************************************/
void printBestModules(window **bestWindowsBySequence,
                      int numSequences, int numBest) {

  /* iterate over the sequences */
  for (int i=0; i<numSequences; i++) {

    printf("\nSequence %d -----------------------------------------------------------\n", i);

    /* get the array of best windows in the current sequence */
    window *bestWindows = bestWindowsBySequence[i];
 
    /* iterate over the best windows in the current sequence */
    for (int j=0; j<numBest; j++) {

      /* print out information about the current best window */
      printf("   Best module %d | location: %d | score: %f | bits: ",
             j, bestWindows[j].begin, bestWindows[j].score);

      /* get the number of motifs in the library */
      int numMotifs = bestWindows[j].totalNumMotifs;
      /*
       * print out the bits in the motifs
       * array of the current window
       */
      for (int k=0; k<numMotifs; k++) {
        printf("%d", bestWindows[j].motifs[k]);
      }

      printf("\n");

    }     /* end for (j) */

      printf("----------------------------------------------------------------------\n");
  }     /* end for (i) */

}     /* end printBestModules */


/************************************************ end of file ****/
