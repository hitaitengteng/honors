/**************************************************************************
 *
 * File: instance.h
 * Author: William Gantt
 * Date: Mon Jul 11 2016
 *
 * Description: Type definition and function prototypes for the instance
 * 		data type. 
 *
 **************************************************************************/

#ifndef INSTANCE_H
#define INSTANCE_H

#include "utilities.h"

/* TYPE DEFINITIONS */

/* 
 * A motif instance. This defines the location of
 * a particular instance of a motif in a sequence (loc)
 * and the fitness score of that instance (score).
 */
typedef struct instance {

  int loc;
  double score;

} instance;

/* FUNCTION PROTOTYPES */
/* (see instance.c for documentation */

instance *initInstance(void);

instance *newInstance(int loc, double score);

void freeInstance(instance *instance);

#endif     /* INSTANCE_H */
