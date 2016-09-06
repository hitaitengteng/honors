/*************************************************************************
 *
 * File: utilities.c 
 * Author: William Gantt
 * Date: Mon Jul 11 2016
 *
 * Description: Source code for basic functions for the rest of the
 * 		module comparison program.
 *
 * Functions:
 *              - emalloc: malloc with error checking
 *              - erealloc: realloc with error checking
 *              - ecalloc: calloc with error checking
 *              - isFree: Given the beginning index of a space (an interval 
 *                the length of a motif) in a sequence of bases, isFree 
 *                determines whether a motif can be implanted there. 
 *              - findFreeSpace: Given an array of integers representing the 
 *                indices of a genomic sequence, findFreeSpace checks to see 
 *                whether there is an interval of length MOTIF_LENGTH in the 
 *                sequence that is not already occupied by an implanted motif 
 *                instance.
 *
 **************************************************************************/

#include "utilities.h"

/***************************************************************************
 * Function:    emalloc
 * Inputs:      The size of the block of memory needed (size).
 * Returns:     A pointer to the block of memory. 
 * Description: Calls malloc, but checks for failure.
 **************************************************************************/
void *emalloc(long size) {
  
  /* request memory */
  void *p = (void *) malloc(size);   
  
  /* quit if memory unavailable */
  if (p == NULL) {
    printf("Unable to initalize...out of memory.");
    exit(0);
  }

  return p;

}      /* end emalloc */

/***************************************************************************
 * Function:    erealloc
 * Inputs:      A pointer to the block of memory needed (ptr) and the size
 * 		of the block (size).
 * Returns:     A pointer to the block of memory.
 * Description: Calls realloc, but checks for failure.
 **************************************************************************/
void *erealloc(void *ptr, long size) {

  /* request memory */
  ptr = realloc(ptr, size);

  /* quit if memory unavailable */
  if (ptr == NULL) {
    printf("Unable to initialize...out of memory.");
    exit(0);
  }

  return ptr;

}     /* end erealloc */

/***************************************************************************
 * Function:    ecalloc
 * Inputs:      The number of elements to allocate (num) and the size of each
 * 		element (size).
 * Returns:     A pointer to the block of allocated memory.
 * Description: Calls calloc, but checks for failure.
 **************************************************************************/
void *ecalloc(int num, long size) {

  /* request memory */
  void *p = (void *) calloc(num, size);

  /* quit if memory unavailable */
  if (p == NULL) {
    printf("Unable to initialize...out of memory.");
    exit(0);
  }

  return p;

}     /* end calloc */

/**************************************************************************
 * Function:    isFree
 * Inputs:      The beginning index of the space to check (index), and 
 *              the array of integers representing the indices of the 
 *              genomic sequence (sequenceIndices).
 * Outputs:     A boolean value (an integer) specifying whether the space
 *              is free or not.
 * Description: Given the beginning index of a space (an interval of the
 *              length of a motif) in a sequence of bases, isFree 
 *              determines whether a motif can be implanted there. If a 
 *              motif has already been implanted at a location that overlaps
 *              with the specified interval, the motif cannot be implanted 
 *              there.
 *
 *              NOTE 1: GAMI runs almost always find instances of motifs
 *                      that overlap with one another. Given the way the
 *                      random data generation functions are set up here,
 *                      however, this will never happen. I have, as yet,
 *                      given little thought to the problem of overlaps
 *                      (most importantly, in my design of the evaluation
 *                      function), which is a significant oversight. This
 *                      problem will have to be addressed in future
 *                      iterations of the program.
 *
 *              NOTE 2: This function should probably be modified to take
 *                      the motif length as a parameter. On the whole,
 *                      I think I have made gratuitous use of named
 *                      constants.
 *
 **************************************************************************/
int isFree(int index, int *sequenceIndices) {

 /* 
  * check the first and last indices of the space specified to determine
  * whether the space is free. We need only check these indices because
  * when a motif is implanted, we mark every index it occupies as "used."
  * Consequently, we know that if indices n and (n + MOTIF_LENGTH - 1) are
  * used, all indices between them must be used as well.
  */
  if ((sequenceIndices[index] == EMPTY) && 
      (sequenceIndices[index + (MOTIF_LENGTH - 1)] == 0))
    return TRUE;
  else
    return FALSE;

}     /* end isFree */

/**************************************************************************
 * Function:    findFreeSpace
 * Inputs:      An array of integers representing the indices of a genomic
 * 		sequence, (sequenceIndices) and the length of the array
 * 		(seqLength).
 * Outputs:     If there is a free space available, returns the beginning
 * 		index of the first free space that it finds. If no more
 * 		free spaces are available, returns -1 (NO_FREE_SPACE).
 * Description: Given an array of integers representing the indices of a
 *              genomic sequence, findFreeSpace checks to see whether there
 *              is an interval of length MOTIF_LENGTH in the sequence that
 *              is not already occupied by an implanted motif instance.
 *		Note that this function will always return the first free 
 *		space available. That means that if there is a free space, 
 *		it will return the one with the smallest index. Thus, for 
 *		implanting motif instances at *random* locations within a 
 *		sequence (in the randInstanceSet function), random indices
 *		are generated separately and checked for availability using 
 *		the isFree function. findFreeSpace is used merely to 
 *		determine *whether* there is any free space remaining.
 *
 *		NOTE: See comments in description of isFree function
 *		      regarding the use of named constants.
 **************************************************************************/
int findFreeSpace(int *sequenceIndices, int seqLength) {

  /* for counting the number of consecutive free indices */
  int count = 0;

  /* 
   * a variable for the first free index discovered (set to
   * a sentinel value initially)
   */
  int freeIndex = NO_FREE_SPACE;

  /* 
   * look for free spaces in the sequence in which to
   * implant a motif
   */
  for (int i=0; i<seqLength; i++) {

    /* 
     * every time an empty index is found, the counter is
     * incremented. If a non-empty space is found, the
     * counter is reset to 0.
     */ 
    if (sequenceIndices[i] == EMPTY)
      count++;
    else
      count = 0;

    /* 
     * If we have found an interval of length MOTIF_LENGTH,
     * then we have found a free space. Return the beginning
     * interval of this space.
     */
    if (count == MOTIF_LENGTH) {
      freeIndex = (i - MOTIF_LENGTH) + 1;
      return freeIndex;
    }
  }     /* end for */

   /* if control has reached the end of the for-loop, then
    * no free spaces were found, so we return the sentinel
    * value set at the beginning of the function (NO_FREE_SPACE).
    */
   return freeIndex;

}     /* end findFreeSpace */

/********************************************************* end of file ****/
