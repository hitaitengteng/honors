/*************************************************************************
 *
 * File: motif.c 
 * Author: William Gantt
 * Date: Mon Jul 11 2016
 *
 * Description: The source file for the motif type and related functions
 *
 * Functions:
 *
 * 	- initMotif: Creates a new motif struct and zeroes/NULLs all of its
 * 	  values.
 * 	- newMotif: Creates a new motif struct with the specified values.
 * 	- randMotif: Creates a new motif struct with random values.
 * 	- randMotifArray: Creates an array of randomly generated motifs
 * 	- copyMotif: Deep-copies one motif into another.
 * 	- freeMotif: Frees a motif struct and the data structures it
 * 	  contains.
 * 	- freeMotifArray: Frees an array of motifs.
 * 	- randomData: Like randMotifArray, but prints out information
 * 		      about the data generated.
 *      - printMotifArrayByMotif: prints information about motifs in an
 *      			  array to the console, organized by motif.
 *      - printMotifArrayBySequence: prints information about motifs in an
 *      			     array to the console, organized by
 *      			     sequence.
 *      - readInPPData: reads in data from the gami post-processor in
 *      		Gantt file format.
 *
 **************************************************************************/

#include "motif.h"

/***************************************************************************
 * Function:    initMotif 
 * Inputs:      N/A      
 * Returns:     A pointer to the newly created motif.
 * Description: Creates a new motif struct and zeroes all of its values,
 * 		except the pointer to the array of instanceSets, which
 * 		it sets to null.
 **************************************************************************/
motif *initMotif(void) {

  /* allocate memory for the new motif struct */
  motif *newMot = emalloc(sizeof(motif));

  /* zero/NULL all values */
  newMot->motifID = 0;
  newMot->numInstanceSets = 0;
  newMot->allInstanceSets = NULL;

  return newMot;

}     /* end initMotif */

/***************************************************************************
 * Function:    newMotif
 * Inputs:      The ID number of the motif (motifID), the number of genomic 
 *              sequences in which the motif occurs (numInstanceSets), and
 *              an array containing instanceSet structs (instanceSetArray), 
 *              one for each sequence in which an instance of the motif 
 *              appears.
 * Returns:     A pointer to the newly created motif.
 * Description: Creates a new motif data structure. This contains the ID
 * 		number of the motif, along with an array of instanceSets. 
 * 		Each entry in the array contains all of the instances of 
 * 		the motif in a particular genomic sequence.
 **************************************************************************/
motif *newMotif(int motifID, int numInstanceSets, instanceSet *instanceSetArray) {

  /* allocate memory for the motif */
  motif *newMot = emalloc(sizeof(motif));

  /* allocate memory for the array of instance structs within the motif struct */
  newMot->allInstanceSets = emalloc(sizeof(instanceSet) * numInstanceSets);

  /* 
   * copy the passed motif ID and value and
   * number of instanceSets into the struct
   */
  newMot->motifID = motifID;
  newMot->numInstanceSets = numInstanceSets;

  /* a pointer to a new instanceSets struct (see below) */
  instanceSet *newInstSet = NULL;

  /*
   * copy the instanceSet from the passed array 
   * into the new array in the struct
   */
  for (int i=0; i<numInstanceSets; i++) {
    
    /* 
     * initialize a new instanceSet struct at the current array location.
     * Note that newInstSet is technically shallow-copied here. This
     * is okay because the initInstSet function doesn't actually malloc
     * the instanceArray struct within the instanceSets struct that it
     * creates. So, in a sense, there isn't really anything *to* deep-copy
     */
    newInstSet = initInstanceSet();
    newMot->allInstanceSets[i] = *newInstSet;
    
    /*
     * since newInstSet has been copied into the 
     * instance sets array, we can free it
     */
    freeInstSet(newInstSet);

    /* 
     * deep-copy the instanceSet from the passed array into the
     * motif instanceSet array
     */
    copyInstSet(&(instanceSetArray[i]), &(newMot->allInstanceSets[i]));

  }     /* end for */

  return newMot;

}     /* end newMotif */

/***************************************************************************
 * Function:    randMotif
 * Inputs:      The motif's ID number (motifID), the number of sequences in 
 *              which the motif occurs (numInstanceSets), the number of 
 *              instances in each sequence (numInstances), and the lengths 
 *              of the sequences (seqLength).
 * Returns:     A pointer to the random motif.
 * Description: Generates a random motif data structure.
 *
 * 		NOTE: In the current iteration of the program, this
 * 		      function is not used. Before using this function,
 * 		      it should probably be modified so that seqLength
 * 		      is an *array* of integers. This would allow the
 * 		      various sequences in which the motif occurs to be
 * 		      of different lengths.
 **************************************************************************/
motif *randMotif(int motifID, int numInstanceSets, 
                 int numInstances, int seqLength) {

  /* an array containing all the instanceSets */
  instanceSet *instSetArray = emalloc(sizeof(instanceSet) * numInstanceSets);

  /* pointers to arrays of instanceSets */
  instanceSet *randInstSet = NULL;
  instanceSet *nullInstSet = NULL;

  /*
   * an array to keep track of indices where
   * motif instances have been implanted
   */
  int *seqIndices = ecalloc(seqLength, sizeof(int));

  /* randomly generate the instanceSets and put them in the array */
  for (int i=0; i<numInstanceSets; i++) {

    /* initialize an instanceSet struct at the current location in instSetArray */
    nullInstSet = initInstanceSet();
    instSetArray[i] = *nullInstSet;

    /* generate a random second occurrences struct */
    randInstSet = randInstanceSet(i, seqLength, seqIndices,
                                  numInstances, MIN_SCORE, MAX_SCORE);
    
    /* deep copy */
    copyInstSet(randInstSet, &(instSetArray[i]));

    /* 
     * since copyInstSet deep-copies an instanceSet
     * struct, we can free randInstSet and nullInstSet
     */
    freeInstSet(randInstSet);
    freeInstSet(nullInstSet);

  }  /* end for */

  /* now create the actual motif */
  motif *randMot = newMotif(motifID, numInstanceSets, instSetArray);

  /* free the array of instanceSets */
  freeInstSetArray(instSetArray, numInstanceSets);

  return randMot;

}     /* end randMotif */

/***************************************************************************
 * Function:    randMotifArray
 * Inputs:      The number of motifs to be created (numMotifs), the number 
 *              of sequences in which the motif occurs (numInstanceSets),
 * 		the number of instances per sequence (numInstances), and
 * 		the length of each sequence (seqLength).
 * Returns:     A pointer to the random motif array.
 * Description: Generates an array of random motifs.
 *
 * 		NOTE: in the current iteration of the program, this
 * 		      function is not used. The randomData function
 * 		      performs what is essentially the same task, but
 * 		      does it better. Before using this function, it
 * 		      should probably be modified so that seqLength
 * 		      is an *array* of integers. This would allow the
 * 		      various sequences in which the motifs occur to
 * 		      be of different lengths.
 **************************************************************************/
motif *randMotifArray(int numMotifs, int numInstanceSets, 
                      int numInstances, int seqLength) {

  /* allocate memory for the array */
  motif *motArray = emalloc(sizeof(motif) * numMotifs);

  motif *nullMot = NULL;
  motif *randMot = NULL;

  /* randomly generate the motifs and put them in the array */
  for (int i=0; i<numMotifs; i++) {

    /* initialize the motifs in the array */
    nullMot = initMotif();
    motArray[i] = *nullMot;
       
    /* generate a random motif to be copied into the array */
    randMot = randMotif(i, numInstanceSets, numInstances, seqLength);

    /* copy it into the array */
    copyMotif(randMot, &(motArray[i]));

    /* 
     * since copyMotif deep-copies a motif struct, we can free
     * randMot and nullMot
     */
    freeMotif(randMot);
    freeMotif(nullMot);

  }     /* end for */

  return motArray;

}     /* end randMotifArray */

/***************************************************************************
 * Function:    copyMotif
 * Inputs:      A pointer to the motif structure to be copied (source),
 *              and a pointer to a second motif structure that the
 *              first will be copied into (dest).
 * Returns:     A pointer to the "dest" motif struct.
 * Description: Deep-copies a motif struct to another motif struct.
 **************************************************************************/
motif *copyMotif(motif *source, motif *dest) {

  /* copy motif ID, and the number of occurrences structs */
  dest->motifID = source->motifID;
  dest->numInstanceSets = source->numInstanceSets;

  /* 
   * resize block of memory allocated for the destination
   * struct's instanceSet array just in case it isn't big enough
   */
  dest->allInstanceSets = realloc(dest->allInstanceSets, 
                                 (sizeof(instanceSet) * source->numInstanceSets));

  instanceSet *nullInstSet = NULL;

  /* deep copy the array of instanceSets */
  for (int i=0; i<source->numInstanceSets; i++) {
    
    /* initialize an empty instanceSet struct to this location in the array */
    nullInstSet = initInstanceSet();
    dest->allInstanceSets[i] = *nullInstSet;

    /* deep-copy */    
    copyInstSet(&(source->allInstanceSets[i]), &(dest->allInstanceSets[i]));

    freeInstSet(nullInstSet);

  }     /* end for */ 

  return dest;

}     /* end copyMotif */

/***************************************************************************
 * Function:    freeMotif
 * Inputs:      A pointer to the motif struct to be freed (mot).
 * Returns:     N/A
 * Description: Frees a motif struct.
 **************************************************************************/
void freeMotif(motif *mot) {

  instance *currInstanceArray = NULL;

  /* 
   * free the arrays of instances for each
   * instanceSet in the array of instanceSets 
   */
  for (int i=0; i<mot->numInstanceSets; i++) {
    currInstanceArray = mot->allInstanceSets[i].instanceArray;
    free(currInstanceArray);
  }
  
  currInstanceArray = NULL;

  /* 
   * now that the instance array has been freed for each occurrence 
   * in the array, we can free the array itself
   */
  free(mot->allInstanceSets);
  
  /* free the rest of the struct */
  free(mot);
  mot = NULL;

}     /* end freeMotif */

/***************************************************************************
 * Function:    freeMotifArray
 * Inputs:      A pointer to the beginning of the motif array to be freed,
 *              and the length of the array.
 * Returns:     N/A
 * Description: Frees an array of motifs.
 **************************************************************************/
void freeMotifArray(motif *motArray, int length) {

  /* a pointer to the current motif in the array */
  motif *currMotif = NULL; 

  /* for each motif... */
  for (int i=0; i<length; i++) {
    
    /* (get the current motif) */
    currMotif = &(motArray[i]);

    /* free the occurrences array for the current motif */
    freeInstSetArray(currMotif->allInstanceSets, currMotif->numInstanceSets);
 
  }     /* end for */

  /* free the array of motifs itself, and the currMotif pointer */
  free(motArray);
  motArray = NULL;
  currMotif = NULL;    

}     /* end freeMotifArray */

/***************************************************************************
 * Function:    randomData
 * Inputs:      The number of motifs in the library to be used (numMotifs),
 * 		the maximum number of instances of a motif in a sequence
 * 		(maxInstances), the number of sequences for which data is
 * 		to be generated (numSequences), and an array specifying
 * 		the length of each sequence (seqLength).      
 * Returns:     A pointer to the array of motif structs generated.
 * Description: Creates a set of motif structs and prints out information
 * 		about them (this is a somewhat fancier version of the
 * 		randMotifArray function).
 **************************************************************************/
motif *randomData(int numMotifs, int maxInstances,
                   int numSequences, int *seqLength) {

  /* allocate memory for the array of motifs */
  motif *motArray = emalloc(sizeof(motif) * numMotifs);

  /* for iterating through a motif array */
  motif *currMot = NULL;

  /* 
   * for each sequence, we have to maintain an array that
   * indicates where motifs have been implanted (so that
   * we don't implant motifs on top of one another). This
   * is an array of such arrays.
   */
  int **allSeqIndices = emalloc(sizeof(int*) * numSequences);

  /*
   * for each sequence, create an array of indices and put
   * each of them in the array of arrays
   */
  for (int i=0; i<numSequences; i++) {

    allSeqIndices[i] = ecalloc(seqLength[i], sizeof(int));

  }     /* end for (i) */

  /* generate an initial list of motifs */
  for (int i=0; i<numMotifs; i++) {

    /* initialize the motif at this index */
    motif *newMot = initMotif();
    motArray[i] = *newMot;

    /* free this, since it was copied into the array */
    freeMotif(newMot);

    /* get the current motif */
    currMot = &(motArray[i]);

    /* an array of instanceSets for this motif, one per sequence */
    instanceSet *instSetArray = emalloc(sizeof(instanceSet) * numSequences);

    /* generate numSequences random instanceSets for the motif. */
    for (int j=0; j<numSequences; j++) {

      /* initialize the current entry in the instanceSet array */
      instanceSet *nullInstSet = initInstanceSet();
      instSetArray[j] = *nullInstSet;
      freeInstSet(nullInstSet);

      /*
       * a randomly selected number of instances for the current
       * motif on the current sequence, between 1 and maxInstances
       */
      int r = (rand() % maxInstances) + 1;

      /* generate the random instances */
      instanceSet *randInstSet = 
        randInstanceSet(j,seqLength[j],allSeqIndices[j],r,MIN_SCORE, MAX_SCORE);
      
      /* 
       * put the new instanceSet in the array
       * of all instanceSets for this motif
       */
      copyInstSet(randInstSet, &(instSetArray[j]));
       
      /*
       * since the instanceSet has been copied into the instSetArray,
       * we can free the original copy
       */
      freeInstSet(randInstSet);

    }     /* end for (j) */

     
    /* generate the new motif using the array of instances created above */
    newMot = newMotif(i, numSequences, instSetArray);
    copyMotif(newMot, currMot);

    /*
     * free newMot, since it has been copied into the array of all
     * motifs (motArray), and free instSetArray, since it was copied
     * into newMot
     */
    free(newMot);
    freeInstSetArray(instSetArray, numSequences);

  }     /* end for (i) */

   /* print out information about the motifs and their instances */
   printMotifArrayBySequence(motArray, numMotifs, numSequences); 
  
   currMot = NULL;

   /* free the array of arrays of sequence indices */
   for (int i=0; i<numSequences; i++) {
     free(allSeqIndices[i]);
   }
   free(allSeqIndices);

   return motArray;

}     /* end randomData */

/***************************************************************************
 * Function:    printMotifArrayByMotif
 * Inputs:      The motif array to be printed (motArray), the length of the 
 *              array (length), and the number of genomic sequences in which 
 *              the motifs occur (numSequences).
 * Returns:     N/A
 * Description: Prints information about the occurrences of motifs in
 * 		a sequence, organized by motif.
 *
 * 		NOTE: Fix formatting of print statement so that it fits
 * 		      80 columns to a line.
 **************************************************************************/
void printMotifArrayByMotif(motif *motArray, int length, int numSequences) {

  /* pointers to the current motif and instance */
  motif *currMot = NULL;
  instance *currInstance = NULL;

  /* iterate over the motifs */
  for (int i=0; i<length; i++) {

    /* get the current motif */
    currMot = &(motArray[i]);

    printf("\nMotif %d ------------------------------------------------------------\n", 
           currMot->motifID);

    /* iterate over the sequences */
    for (int j=0; j<numSequences; j++) {
      printf("\n   Sequence %d\n", currMot->allInstanceSets[j].seqID);

      /* get the number of motif instances for the current motif */
      int numInstances = currMot->allInstanceSets[j].numInstances;

      /* iterate through the list of instances for the current sequence */
      for (int k=0; k<numInstances; k++) {

        /* get the current instance */
        currInstance = &(currMot->allInstanceSets[j].instanceArray[k]);

        /* print out the instance number, location, and score */
        printf("      Instance %d:\n         Location: %d\n         Score: %f\n",
               k, currInstance->loc, currInstance->score);

      }     /* end for (k) */
    }     /* end for (j) */
  }     /* end for (i) */

  /* NULL the pointers */
  currMot = NULL;
  currInstance = NULL;

}     /* end printMotifArrayByMotif */


/***************************************************************************
 * Function:    printMotifArrayBySequence
 * Inputs:      The motif array to be printed (motArray), the length of the 
 *              array (length), and the number of genomic sequences in which 
 *              the motifs occur (numSequences).
 * Returns:     N/A
 * Description: Prints information about the occurrences of motifs in
 * 		a sequence, organized by sequence.
 *
 * 		NOTE: Fix formatting of print statement so that it fits
 * 		      80 columns to a line.
 **************************************************************************/
void printMotifArrayBySequence(motif *motArray, int length, int numSequences) {

  /* pointers to the current motif and instance */
  motif *currMot = NULL;
  instance *currInstance = NULL;

  /* iterate over the sequences */
  for (int i=0; i<numSequences; i++) {
    printf("\nSequence %d ------------------------------------------------------------\n", i);

    /* iterate over the motifs */
    for (int j=0; j<length; j++) {
      printf("\n   Motif %d\n", j);

      /* get the current motif */
      currMot = &(motArray[j]);

      /*
       * get the number of instances for the
       * current motif on the current sequence
       */
      int numInstances = currMot->allInstanceSets[i].numInstances;
      /* 
       * iterate over the instances of the 
       * current motif in the current sequence
       */ 
      for (int k=0; k<numInstances; k++) {

        /* get the current instance */
        currInstance = &(currMot->allInstanceSets[i].instanceArray[k]);

        /* print out the instance number, location, and score */
        printf("      Instance %d:\n         Location: %d\n         Score %f\n",
                k, currInstance->loc, currInstance->score);

      }     /* end for (k) */
    }     /* end for (j) */
  }     /* end for (i) */

  /* NULL the pointers */
  currMot = NULL;
  currInstance = NULL;

}     /* end printMotifArrayBySequence */

/***************************************************************************
 * Function:    readInPPData
 * Inputs:      The name of the file to be read in.
 * Returns:     N/A
 * Description: reads in actual genetic data from the GAMI post-processor.
 **************************************************************************/
void readInPPData(char *fileName) {

  /* the number of sequences used */
  int numSequences;

  /* the number of motifs */
  int numMotifs;

  /* the length of the motifs */
  int motifLength;

  /* the current line to be read in */
  char line[100];

  /* a string describing the type of file to be read in */
  char *fileType = emalloc(sizeof(GANTT_FILE));

  /* open the file */
  FILE *inFile = fopen(fileName, "r");

  /* the name of a sequence to be read in */
  char seqName[20];

  /* the length of the sequence to be read in */
  int seqLength;

  /* check that the file opened correctly */
  if (inFile == NULL) {
    fprintf(stderr, "***ERROR: Unable to open file %s\n", fileName);
    exit(0);
  }

  /* get the first line of the file */
  fgets(line, sizeof(line), inFile);
  sscanf(line, "%s", fileType);

  /* check that the file is the right kind of file */
  if (strcmp(fileType, GANTT_FILE) != 0) {
    fprintf(stderr, "***ERROR: %s is not a gantt file\n", fileName);
    exit(0);
  }
  
  /* skip two lines */
  fgets(line, sizeof(line), inFile);
  fgets(line, sizeof(line), inFile);

  /*
   * get the number of species (sequences), the number of
   * motifs, and the length of the motifs
   */
  fscanf(inFile, "%*s %*s %*s %d", &numSequences);  
  fscanf(inFile, "%*s %*s %*s %d", &numMotifs);
  fscanf(inFile, "%*s %*s %*s %d", &motifLength);

  /* create the array of motifs */
  motif *motifArray = emalloc(sizeof(motif) * numMotifs);
 
  /* create array to store info about all the sequences */
  sequence *allSeqs = emalloc(sizeof(sequence) * numSequences);

  /* skip four lines */
  for (int i=0; i<4; i++)
    fgets(line, sizeof(line), inFile);

  /* read in information about the sequences */
  for (int i=0; i<numSequences; i++) {

    /*
     * get the name and length of the
     * sequence from the current line
     */
    fscanf(inFile, "%s %*s %d", seqName, &seqLength);
    allSeqs[i].name = emalloc(sizeof(seqName));
 
    /* copy info into the sequences array */
    strcpy(allSeqs[i].name, seqName);
    allSeqs[i].idNum = i;
    allSeqs[i].length = seqLength;

  }     /* end for (i) */ 

  /* skip four lines */
  for (int i=0; i<4; i++)
    fgets(line, sizeof(line), inFile);

  /* a pointer to the current instance */
  instance *currInst = emalloc(sizeof(instance));

  /* read in the instances */
  for (int i=0; i<numMotifs; i++) {

    do {
      
      fgets(line, sizeof(line), inFile);
      sscanf(line, "%*d %*s %d %*s %d", currInst->loc, currInst->score);

    } while (line[0] != '\n')

  }     /* end for (i) */

  /* free allocated memory */
  free(fileType);
  free(motifArray);

  /* free the sequences array */
  for (int i=0; i<numSequences; i++)
    free(allSeqs[i].name);
  free(allSeqs);

  /* close the file */
  fclose(inFile);

}     /* end readInPPData */

/************************************************ end of file ****/
