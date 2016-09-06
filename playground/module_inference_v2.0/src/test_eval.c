/**************************************************************************
 *
 * File:   test_eval.c 
 * Author: William Gantt
 * Date: Thu Jul 22 2016
 *
 * Description: a test function for reading in a gantt file
 *
 * Functions:
 *              -  eval: the evaluation function
 *
 * To run: "go Will_v2.0 ex1"
 *
 **************************************************************************/

#include "window.h"

double eval(char *str, int length, double *vect, int genes);

/************************************************************************
 * Function:    eval
 * Inputs:      a bit string of the solution to be evolved, the length
 *              of the bit string, an array containing the double
 *              representation of the bit string, and the number of
 *              attributes ("genes") in a solution
 * Outputs:     a double representing the fitness of the solution
 * Description: 
 ***********************************************************************/
double eval(char *str, int length, double *vect, int genes) {
  
  /* the fitness of the solution*/
  double fitness = 0.0;

  if ((Trials == 0) && (Experiment == 0)) {

    readInPPData("gantt.txt");

    exit(0);

  }     /* end if */

  return fitness;

}     /* end eval */
