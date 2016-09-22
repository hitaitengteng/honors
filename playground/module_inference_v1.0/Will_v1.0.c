/**************************************************************************
 *
 * File:   Will_v1.0.c
 * Author: William Gantt
 * Date: Thu Jun 16 13:51 2016
 *
 * Description: A very primitive fitness function for Genesis. Used to
 *              evaluate the similarity of two "modules" (arrays of motifs).
 *              The file also includes functions for generating motifs and
 *              modules to use as fake data for the GA.
 *
 * Functions:
 *              -  emalloc: malloc with error checking
 *              -  eval: the evaluation function
 *              -  newMotif: creates a new motif (a character string 
 *                 consisting of the letter A, C, T, and G).
 *              -  randMotifArray: creates an array of randomly generated
 *                 motifs
 *              -  randModules: creates an array of motif arrays (an array
 *                 of "modules")
 *
 * To run: "go Will_v1.0 ex1"
 *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "extern.h"

/* A,C,T,G */
#define NUM_BASES 4

/* The number of modules to work with */
#define NUM_MODULES 2

/* The number of motifs in a module */
#define MOTIFS_PER_MODULE 10

/* The number of motifs to use when
 * generating a random set of motifs
 */
#define NUM_RANDOM_MOTIFS 20

/* The number of bases in a motif */
#define MOTIF_LENGTH 10

/* The probability that a motif will be modified
 * in the modifyMotifArray function
 */
#define MOD_PROB 0.8

/* Boolean values */
#define TRUE 1
#define FALSE 0

/* function prototypes (see top of file and function declarations 
 * for descriptions)
 */
double eval(char *str, int length, double *vect, int genes);

void *emalloc(long size);   

char *newSequence(int length);

struct motif *newMotif(int idNum, int length);

struct motif *randMotifArray(int numMotifs, int motifLength);

struct motif *modifyMotifArray(struct motif *motifArray, double modProb);

/* The motif datatype. Every motif consists of an ID number and a
 * character string of bases (A's, C's, T's, and G's). As this program
 * gets more sophisticated, I hope to accommodate N's as well. Ideally,
 * in the end, the program will accommodate the entire IUPAC nucleic
 * alphabet. */
typedef struct motif {
  int idNum;
  char *bases;
} motif;

/************************************************************************
 * Function:    eval
 * Inputs:      a bit string of the solution to be evolved, the length
 *              of the bit string, an array containing the double
 *              representation of the bit string, and the number of
 *              attributes ("genes") in a solution
 * Outputs:     a double representing the fitness of the solution
 * Description: At the beginning of each experiment, two "modules"
 *              (motif arrays) are randomly generated. The length of the
 *              bit string used is equal to the number of motifs per
 *              module. The bit string's fitness is calculated according
 *              to the following rules:
 *              	1) If there is a '1' at index i in the bitstring
 *              	   and the motif at index i in the first module
 *              	   also occurs in the second module, the solution's
 *              	   fitness is incremented by 1.
 *              	2) If there is a '1' at index i in the bitstring
 *              	   and the motif at index i in the first module
 *              	   does *not* occur in the second module, the
 *              	   solution's fitness is decremented by 1.
 *              	3) If there is a '0' at index i in the bitstring,
 *              	   the fitness remains unchanged.
 ***********************************************************************/
double eval(char *str, int length, double *vect, int genes) {
  
  /* the highest fitness found in the current experiment */
  static double bestSoFar;
  
  /* the random modules to be compared */
  static motif* randMod1;
  static motif* randMod2;

  /* the fitness of the solution*/
  double fitness = 0;

  /* stuff to do at the beginning of a new experiment */
  if (Trials == 0) {

    /* generate two random modules at the beginning of each experiment */
    randMod1 = randMotifArray(NUM_RANDOM_MOTIFS, MOTIF_LENGTH);
    randMod2 = modifyMotifArray(randMod1, MOD_PROB);
 
    /* reset bestSoFar only if a new experiment has begun */
    bestSoFar = 0; 
    printf("\n\n\n");

    /* print the first module */
    printf("Module 1:\n");
    for (int i=0; i<MOTIFS_PER_MODULE; i++) {
      printf("   Motif %d: %s\n", randMod1[i].idNum, randMod1[i].bases);
    }
    printf("\n");

    /* print the second module */
    printf("Module 2:\n");
    for (int i=0; i<MOTIFS_PER_MODULE; i++) {
      printf("   Motif %d: %s\n", randMod2[i].idNum, randMod2[i].bases);
    }
    printf("\n");
  }

  /* evaluate the fitness of the solution */
  for (int i = 0; i < MOTIFS_PER_MODULE; i++) {
    
    /* We want to reward those bitstrings that have 1's at indices in
     * the first module that contain motifs that are in *both* modules.
     * If the first module has motif X at index 5, for example, and
     * motif X is also in the second module, then we reward those bit-
     * strings that have a '1' at index 5. We care only about the indices
     * in the bitstring that contain 1's, since the fitness is unaffected
     * by 0's.
     */
    if (str[i] == '1') {

      /* keeps track of whether an instance of a motif from the first
       * module has been found in the second module
       */
      int matchFound = FALSE;

      /* iterate through the second module to look for the motif at index
       * i in the first module
       */
      for (int j=0; j<MOTIFS_PER_MODULE; j++) {
        
        /* if a match is found, increment the fitness, set matchFound to
         * true and break out of the loop
         */ 
        if (randMod2[j].idNum == randMod1[i].idNum) {
          fitness += 1;
          matchFound = TRUE;
          break;
        }
      }
      
      /* if matchFound is still false, no match was found, so we decrement
       * the fitness
       */
       if (matchFound == FALSE)
         fitness -= 1;
    }
  }
 
  /* if the fitness of the current solution is higher than the best
   * fitness found so far, update the best fitness found so far
   */
  if (fitness > bestSoFar) {
    bestSoFar = fitness;

    /* print out information about the new best solution, including
     * the generation it was found in and its fitness
     */
      printf("New best found in generation %d; \
              Fitness is %.2f \n", Gen, fitness);

      /* print the actual solution as a bit string */
      printf("Bits: ");
      for (int i = 0; i < length; i++) {
        printf("%c", (int) str[i]);
      }
      printf("\n");
  } 

  return fitness;

}     /* end eval */

/************************************************************************
 * Function:    newSequencee
 * Inputs:      The length of the sequence to be generated
 * Outputs:     A character string containing the sequence generated
 * Description: Generates a random sequence of base pairs. Very similar
 *              to newMotif, but has its own function because of
 *                   a) concerns for clarity
 *                   b) the fact that newMotif returns a motif datatype,
 *                      which we don't necessarily want to do here
 ***********************************************************************/
char *newSequence(int length) {

  /* a random integer */
  int r;

  /* a variable to store a base */
  char base;

  /* request memory for the sequence */
  char *newSeq = emalloc(sizeof(char) * length);

  /* generate the sequence randomly */
  for (int i=0; i<length; i++) {
  
    /* get a random integer from 0 to 3 inclsuive */
    r = rand() % 4;

    /* assign the appropriate base to the current
     * index in the sequence based on the number
     * generated above
     */
     switch(r) {
       case 0:
         base = 'A';
         break;
       case 1:
         base = 'C';
         break;
       case 2:
         base = 'T';
         break;
       default:
         base = 'G';
     }
     newSeq[i] = base;
  }
  return newSeq;

}    /* end newSequence */

/************************************************************************
 * Function:    newMotif
 * Inputs:      An ID number for the motif to be created and the number of 
 *              bases in the motif
 * Outputs:     A motif struct
 * Description: Generates a motif struct with the specified ID number and
 *              with a randomly generated sequence of base pairs (a
 *              character array) of the length specified
 ***********************************************************************/
struct motif *newMotif(int idNum, int length) {

  /* a variable to store a base */
  char base;

  /* request the memory for the motif */
  motif *newMot = emalloc(sizeof(motif));

  /* request the memory for the string of bases */
  newMot->bases = emalloc(sizeof(char) * length);

  /* set the ID number of the motif */
  newMot->idNum = idNum;

  /* generate a random sequence of bases of the length specified */
  for (int i=0; i<length; i++) {

    /* generate a random integer between 0 and 3 inclusive */
    int r = rand() % NUM_BASES;
  
    /* use this random integer to select a base */
    switch(r) {
      case 0:
        base = 'A';
        break;
      case 1:
        base = 'C';
	break;
      case 2:
        base = 'T';
	break;
      default:
        base = 'G';
    }
  
    /* put this base in the string of bases */
    newMot->bases[i] = base;
  }

  return newMot;

}     /* end newMotif */

/**************************************************************************
 * Function:    randMotifArray
 * Inputs:      The number of motifs to be created and the length (in 
 *              base pairs) of the motifs to be created
 * Outputs:     An array of motif structs
 * Description: Uses the newMotif function to create an array of randomly
 *              generated motifs of the length specified
 **************************************************************************/ struct motif *randMotifArray(int numMotifs, int motifLength) {

  /* request memory for the array of motifs */
  motif *motifArray = emalloc(sizeof(motif) * numMotifs);

  /* for each index in the array, generate a new motif */
  for (int i=0; i<numMotifs; i++) {
    motifArray[i] = *(newMotif(i,motifLength));
  }

  return motifArray;

}     /* end randMotifArray */

/***************************************************************************
 * Function:    motifyMotifArray
 * Inputs:      The array of motifs to be modified and the probability that
 *              a given motif in the array will be modified
 * Returns:     A pointer to the modified array
 * Description: The reason this function does not return void is that it is
 *              not literally modifying an existing array, but rather
 *              creating a new array that uses the old array as its basis.
 *              Of course, the closer the value of modProb is to 1, the
 *              more changed the new array is likely to be. 
 **************************************************************************/
struct motif *modifyMotifArray(motif *motifArray, double modProb) {

  /* request memory for the modified array */
  motif* modifiedArray = emalloc(sizeof(motif) * MOTIFS_PER_MODULE);

  /* modify the array */
  for (int i=0; i<MOTIFS_PER_MODULE; i++) {
  
    /* generate a random double between 0 and 1 inclusive */
    double r = (double) rand() / (double) RAND_MAX;

    /* if the random double is less than the probability of
     * modification, generate a new motif and put it in the
     * current index in the modified array
     */
    if (r<modProb)
      modifiedArray[i] = *(newMotif(i+MOTIFS_PER_MODULE, MOTIFS_PER_MODULE));

    /* otherwise, we simply copy the motif at the index in the old array
     * into the index in the modified array
     */
    else
      modifiedArray[i] = motifArray[i];
  }
  return modifiedArray;

}     /* end motifyMotifArray */

/***************************************************************************
 * Function:    emalloc
 * Inputs:      The size of the block of memory needed
 * Returns:     A pointer to the block of memory
 * Description: Calls malloc, but checks for failure
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

}     /* end emalloc */

/************************************************ end of file ****/
