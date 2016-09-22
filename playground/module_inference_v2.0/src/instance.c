/*************************************************************************
 *
 * File: instance.c 
 * Author: William Gantt
 * Date: Thu Jun 23 2016
 *
 * Description: Functions for the instance data type.
 *
 * Functions:
 *
 * 	- initInstance: Creates a new instance struct and zeroes all of its
 * 	  values.
 * 	- newInstace: Creates a new instance struct with the specified values.
 * 	- freeInstance: completely frees an instance struct.
 *
 **************************************************************************/

#include "instance.h"

/***************************************************************************
 * Function:    initInstance
 * Inputs:      N/A      
 * Returns:     A pointer to the newly created instance.
 * Description: Creates a new instance struct and zeroes all of
 *              its values.
 **************************************************************************/
instance *initInstance(void) {

  /* allocate memory for the new instance */
  instance *newInstance = emalloc(sizeof(instance));

  /* set values to 0 */
  newInstance->loc = 0;
  newInstance->score = 0;

  return newInstance;

}     /* end initInstance */

/***************************************************************************
 * Function:    newInstance 
 * Inputs:      The index in a genomic sequence of the motif instance (loc)
 *              and its fitness score at that location (score).
 * Returns:     A pointer to the newly created instance.
 * Description: Creates a new instance structure.
 **************************************************************************/
instance *newInstance(int loc, double score) {
  
  /* allocate memory for the new instance */
  instance *newInstance = emalloc(sizeof(instance));

  /* copy the passed values into the data structure */
  newInstance->loc = loc;
  newInstance->score = score;

  return newInstance;

}     /* end newInstance */

/***************************************************************************
 * Function:    freeInstance
 * Inputs:      A pointer to the instance struct to be freed (instance).
 * Returns:     N/A
 * Description: Frees an instance struct.
 **************************************************************************/
void freeInstance(instance *instance) {

  free(instance);
  instance = NULL;

}     /* end freeInstance */

/************************************************ end of file ****/
