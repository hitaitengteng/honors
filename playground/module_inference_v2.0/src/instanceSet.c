/*************************************************************************
 *
 * File: instanceSet.c 
 * Author: William Gantt
 * Date: Mon Jul 11 2016
 *
 * Description: Functions for the instanceSet data type
 *
 * Functions:
 *              -  initInstanceSet: Creates a new instanceSet struct with
 *                 zeroed values and the instaceArray pointer set to NULL.
 *              -  newInstanceSet: Creates a new instanceSet struct with
 *                 the specified values.
 *              -  randInstanceSet: Creates a new instanceSet struct with
 *                 randomly generated values.
 *              -  copyInstSet: Deep-copies one instanceSet structure into
 *                 another.
 *              -  freeInstSet: Completely frees an instanceSet struct.
 *              -  freeInstSetArray: Completely frees an array of instanceSet
 *                 structs.
 *
 **************************************************************************/

#include "instanceSet.h"

/***************************************************************************
 * Function:    initInstanceSet
 * Inputs:      N/A      
 * Returns:     A pointer to the newly created instanceSet struct.
 * Description: Creates a new instanceSet struct and zeroes all of its
 * 		values except the instanceArray pointer, which is set
 * 		to NULL.
 **************************************************************************/
instanceSet *initInstanceSet(void) {

  /* allocate memory for the new instanceSet struct */
  instanceSet *newInstSet = emalloc(sizeof(instanceSet));

  /* zero/NULL all values */
  newInstSet->seqID = 0;
  newInstSet->seqLength = 0;
  newInstSet->numInstances = 0;
  newInstSet->instanceArray = NULL;

  return newInstSet;

}     /* end initInstanceSet */

/***************************************************************************
 * Function:    newInstanceSet
 * Inputs:      The ID (seqID) and length (seqLength) of the genomic 
 *              sequence, the number of instances of the motif that occur 
 *              in the sequence (numInstances), and a pointer to the array 
 *              containing those instances (instanceArray).
 * Returns:     A pointer to the newly created set of instances.
 * Description: Creates a new instanceSet data structure. This contains all of
 * 		the instances of a single motif in a single genomic sequence.
 **************************************************************************/
instanceSet *newInstanceSet(int seqID, int seqLength,
                        int numInstances, instance *instanceArray) { 

  /* allocate memory for the instanceSet struct */
  instanceSet *newInstSet = emalloc(sizeof(instanceSet));

  /* allocate memory for the array of instances within the struct */
  newInstSet->instanceArray = emalloc(sizeof(instance) * numInstances);

  /* 
   * copy the sequence ID, sequence length, and the
   * number of instances into the new data structure.
   */
  newInstSet->seqID = seqID;
  newInstSet->seqLength = seqLength;
  newInstSet->numInstances = numInstances;

  /* copy the instances from the passed array into the new array */
  for (int i=0; i<numInstances; i++) {
    newInstSet->instanceArray[i] = instanceArray[i];
  }

  return newInstSet;

}     /* end newInstanceSet */

/***************************************************************************
 * Function:    randInstanceSet
 * Inputs:      The ID number of the genomic sequence (seqID), the length 
 *              of the genomic sequence (seqLength), an array to keep track 
 *              of the indices where instances have been implanted
 *              (seqIndices), the number of instances in the sequence
 *              (numInstances),  and the minimum and maximum score of the 
 * 		instances (minScore and maxScore).
 * Returns:     A pointer to the newly created instanceSet data structure.
 * Description: Generates a random instanceSet data structure. 
 **************************************************************************/
instanceSet *randInstanceSet(int seqID, int seqLength, 
                             int *seqIndices, int numInstances, 
                             double minScore, double maxScore) {

  /* allocate memory for the array of instances */
  instance *instanceArray = emalloc(sizeof(instance) * numInstances);

  /* a random instance */
  instance *randInstance = NULL;

  /* a random index for a motif instance */
  int loc;

  /* a random score for the motif instance */
  double score;

    /* generate the random instances and put them in the array */
  for (int i=0; i<numInstances; i++) {
    
    /* while there is space left to implant a motif... */
    if (findFreeSpace(seqIndices, seqLength) != NO_FREE_SPACE) {

      /* 
       * generate random indices in the sequence until we find
       * one where we can implant a motif
       */
       do { 
        loc = rand() % (seqLength - MOTIF_LENGTH);
      } while (isFree(loc, seqIndices) == FALSE); 

      /*
       * once we have found such an index, we need to update
       * the sequenceIndices array to let the function know
       * that we have used the indices where we are implanting
       * the motif
       */
      for (int j=0; j<MOTIF_LENGTH; j++) {
        seqIndices[loc+j] = USED;
      }

      /* generate a random score between minScore and maxScore */
      score = minScore + (rand() % (int)((maxScore - minScore) + 1));

      /* create the motif instance */
      randInstance = newInstance(loc, score);

      /* put the motif instance in the array */
      instanceArray[i] = *randInstance;

      /* 
       * now that the data has been copied into
       * the array, we can free the variable
       */
      freeInstance(randInstance);

    }     /* end if */

    /* 
     * otherwise, we've run out of space, 
     * so we break out of the for loop
     */
    else 
      break;

  }     /* end for */ 

  /* create the actual instanceSet data structure */
  instanceSet *randInstSet = 
    newInstanceSet(seqID, seqLength, numInstances, instanceArray);

  /* 
   * when we create a new instanceSet struct, the array of instances that gets
   * passed to the newInstanceSet function actually gets copied into the struct,
   * so we can free instanceArray now that the instanceSet struct has been created
   */
  free(instanceArray);
  instanceArray = NULL;

  return randInstSet;

}     /* end randInstanceSet */

/***************************************************************************
 * Function:    copyInstSet
 * Inputs:      A pointer to the instanceSet structure to be copied (source),
 *              and a pointer to a second instanceSet structure that the
 *              first will be copied into (dest).
 * Returns:     A pointer to the "dest" instanceSet struct.
 * Description: Deep-copies an instanceSet struct to another instanceSet
 *              struct.
 **************************************************************************/
instanceSet *copyInstSet(instanceSet *source, instanceSet *dest) {

  /* copy sequence ID, sequence length, and the number of instances */
  dest->seqID = source->seqID;
  dest->seqLength = source->seqLength;
  dest->numInstances = source->numInstances;

  /* 
   * resize block of memory allocated for the destination
   * struct's instance array just in case it isn't big enough
   */
  dest->instanceArray = realloc(dest->instanceArray,
                            (sizeof(instance) * source->numInstances));

  /* deep-copy the array of instances */
  for (int i=0; i<source->numInstances; i++) {
    dest->instanceArray[i] = source->instanceArray[i];
  }

  return dest;

}     /* end copyInstSet */  

/***************************************************************************
 * Function:    freeInstSet
 * Inputs:      A pointer to the instanceSet struct to be freed (instSet).
 * Returns:     N/A
 * Description: Frees an instanceSet struct.
 **************************************************************************/
void freeInstSet(instanceSet *instSet) {

  /* 
   * have to free instanceArray before the rest of the struct,
   * since it is malloc'ed separately
   */
  free(instSet->instanceArray);
  free(instSet);
  instSet = NULL;

}     /* end freeInstSet */

/***************************************************************************
 * Function:    freeInstSetArray
 * Inputs:      A pointer to the array of instanceSets to be freed
 *              (instSetArray), and the length of the array (length).
 * Returns:     N/A
 * Description: Frees an array of instanceSet structs. One may well ask what
 *              the point is in having a function like this when we already
 *              have freeInstSet. The problem is that, since C prohibits you
 *              from freeing individual array elements, a function that 
 *              allows you to free an atomic struct (like freeInstSet) is 
 *              useless when trying to free an array of those structs. Hence 
 *              this separate function. 
 **************************************************************************/
void freeInstSetArray(instanceSet *instSetArray, int length) {

  /* 
   * free the array of instances for each
   * instanceSet struct in the array
   */
  for (int i=0; i<length; i++) {
    free(instSetArray[i].instanceArray);
  }

  /* free the array of occurrences itself and set the pointer to null */
  free(instSetArray);
  instSetArray = NULL;

}     /* end freeInstSetArray */

/************************************************ end of file ****/
