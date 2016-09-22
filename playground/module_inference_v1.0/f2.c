/************************************************  file f1.c  ****/

#include "extern.h"

extern int Gen;
extern int Trials;

/* function prototypes */
double eval(char *str, int length, double *vect, int genes);

double eval(char *str, int length, double *vect, int genes)
{
  register int i;
  double sum;
  static double bestSoFar;

  if (Trials == 0)
    bestSoFar = 0;

  if (Trials == 99999)
    printf("Final generation: %d", Gen);

  sum = 0.0;
  for (i=0; i<genes; i++)
    sum += vect[i] * vect[i];
  
  if (sum > bestSoFar) {
    bestSoFar = sum;
    printf("New best found in generation %d; Fitness is %.4f\n", Gen, sum);
    printf(" bitstring:  %s\n", str);
    printf(" genes:     %.2f  %.2f  %.2f  %.2f  %.2f\n", vect[0], vect[1], vect[2], vect[3], vect[4]);
  }
  
  return (sum);
}

/************************************************ end of file ****/
