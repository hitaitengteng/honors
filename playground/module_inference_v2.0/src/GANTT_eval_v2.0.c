/**************************************************************************
 *
 * File:   Will_v2.0.c
 * Author: William Gantt
 * Date: Thu Jun 16 13:51 2016
 *
 * Description: A very primitive fitness function for Genesis. 
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

  /* the highest fitness found so far */
  static double bestSoFar;

  /* the sum of the highest-scoring modules in each sequence */
  static double maxScore;

  /* an array for the motif library */
  static motif *motArray;

  /* an array for the best modules in each sequence */
  static window **allModules;

  /*
   * an array of the scores of the
   * best modules in each sequence
   */
  static double *bestModuleScores;

/* ---------------------- BEGINNING OF RUN ----------------------- */

  /* stuff to do at the beginning of a run */
  if ((Trials == 0) && (Experiment == 0)) {
  
    bestSoFar = 0.0;

    /*
     * create an array to store the length
     * of the sequences to be used
     */ 
    int *seqLengths = emalloc(sizeof(int) * NUM_SEQUENCES);
    
    /*
     * for simplicity's sake, I have made all  
     * of the sequences the same length here 
     */
    for (int i=0; i<NUM_SEQUENCES; i++)
      seqLengths[i] = SEQ_LENGTH;
    
    /* 
     * create an array of randomly generated motifs. Note 
     * that when each motif struct is created, some number of 
     * instances of the motif are implanted in each sequence
     */
    motArray = randomData(NUM_MOTIFS, MAX_INSTANCES,
                                 NUM_SEQUENCES, seqLengths);

    /* find the best modules in every sequence */
    allModules = findBestModules(motArray, NUM_MOTIFS, N_BEST, 
                            MOTIF_LENGTH, WINDOW_STEP, WINDOW_SIZE, 
                            NUM_SEQUENCES, SEQ_LENGTH);

    /* initialize the array of best module scores */
    bestModuleScores = emalloc(sizeof(double) * NUM_SEQUENCES);

    /* 
     * compute the maximum possible score for a motif.
     * If a motif occurs in a window m, then m's
     * score is added to the motif's score
     */

    /* iterate over the sequences */
    for (int i=0; i<NUM_SEQUENCES; i++) {

      /* the maximum possible score for a motif in this sequence */
      double maxMotifScore = 0.0;

      /* iterate over the modules in this sequence */
      for (int j=0; j<N_BEST; j++) {

        /*
         * we are looking for the module (represented by a window)
         * with the highest overall score in the sequence. If, while
         * iterating over the modules in the sequence, we find one
         * with a score greater than the previous overall best,
         * update the overall best.
         */
        if (allModules[i][j].score > maxMotifScore) 
          maxMotifScore = allModules[i][j].score;
       
      }     /* end for (j) */

      /*
       * once we have found the best score for this
       * sequence, add it to the maximum total motif 
       * score and the array of maximum module scores
       */
      maxScore += maxMotifScore; 
      bestModuleScores[i] = maxMotifScore;

    }     /* end for (i) */

  }     /* end if */

/* ---------------------- MIDDLE OF EXPERIMENT ----------------------- */

  /* iterate through the bit string */
  for (int i=0; i<length; i++) {

    /*
     * we care only about indices in the
     * bit string that contain a '1'
     */
    if (str[i] == '1') {

      /*
       * the current index in the string represents a
       * particular motif. To compute the score of the
       * bit string, we first score and sum the
       * individual motifs
       */
      double motifScore = 0.0;

      /* iterate through the sequences */
      for (int s=0; s<NUM_SEQUENCES; s++) {
      
        /*
         * the score of the highest-scoring module
         * in which the current motif appears
         */
        double bestScoreInSequence = 0.0;

        /* iterate through the modules */
        for (int n=0; n<N_BEST; n++) {

          /* get the current module */
          window *currModule = &(allModules[s][n]);
          
          /* 
           * if the current motif occurs in the current
           * module and the current module's score is
           * higher than than that of any previous
           * module in which the motif appears,
           * update bestScoreInSequence.
           */
          if ((currModule->motifs[i] == 1) &&
              (currModule->score > bestScoreInSequence)) {
              bestScoreInSequence = currModule->score;
          }

          /*
           * what to do when we have reached the
           * last module in the sequence
           */ 
          if (n == N_BEST - 1) {

            /* 
             * if bestScoreInSequence is still 0 at this
             * point, then we know that the motif did
             * not appear in any of the modules in the
             * sequence, so we set the score of the motif
             * to -1 for this sequence.
             */
            if (bestScoreInSequence == 0.0) 
              motifScore += -1;

            /*
             * otherwise, we set the motif's score to
             * bestScoreInSequence -- the score of the
             * highest-scoring module in the sequence
             * in which the motif appears -- divided
             * by the highest-scoring module in the
             * sequence.
             */ 
            else
              motifScore += bestScoreInSequence / bestModuleScores[s];

          }     /* end if */

        }     /* end for (n) */

      }     /* end for (s) */

      /*
       * the overall score for a motif is the average of its score
       * in each sequence. Since a motif's score in each sequence
       * is in the range [0.0, 1.0], the overall fitness will be
       * in the range [0.0, NUM_MOTIFS].
       */
      fitness += motifScore / 3;  

    }     /* end if */

  }     /* end for (i) */
  
  /*
   * if the fitness of the current solution is higher than the best
   * fitness found so far, update the best fitness found so far
   */
  if (fitness > bestSoFar) {
    bestSoFar = fitness;

    /*
     * print out information about the new best solution,
     * including the generation it was found in and its fitness
     */
    printf("\nNew best found in generation %d; \
            Fitness is %.2f \n", Gen, fitness);

    /* print the actual solution as a bit string */
    printf("Bits: ");
    for (int i=0; i<length; i++) {
      printf("%c", (int) str[i]);
    }
    printf("\n");

  }     /* end if */

/* ---------------------- END OF EXPERIMENT  ----------------------- */


  if (Trials == 9999) {

    printf("Final generation: %d", Gen);

    /* free the array of best modules */
    for (int i=0; i<NUM_SEQUENCES; i++) {
      freeWindowArray(allModules[i], N_BEST);
    }

    /*
     * if we have reached the end of the final
     * experiment, free all data structures
     */
    if (Experiment == 0) {

      /* free the library of motifs */
      freeMotifArray(motArray, NUM_MOTIFS);

      /* free the array of the best module scores */
      free(bestModuleScores);

    }     /* end if */

  }     /* end if */

  return fitness;

}     /* end eval */

