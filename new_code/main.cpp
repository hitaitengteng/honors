#include "population.h"
#include <time.h>

/****************************************************************************
 * File:        main.cpp
 * Author:      William Gantt
 * Description: Runs the LCS.
 ****************************************************************************/ 
  
using namespace std;
   
// declared as external variables in utilities.h, defined here
mt19937 rng;
random_device rd;
uniform_real_distribution<double> real_dist(0,1);

/* named constants for testing
 *
 * NOTE: make sure that the following values are integers:
 * 	1. NUM_ITERS * SPEC_RATE
 * 	2. POP_SIZE * SPEC_FRAC * (1 - E_RATE)
 * 	3. E_RATE * POP_SIZE (this must be an *even* integer)
 */
const int POP_SIZE = 200;
const int NUM_ITERS = 1000;
const int DEFAULT_CLASS = -1;
const double E_RATE = 0.6;
const double SPEC_RATE = 0.05;
const double SPEC_FRAC = 0.5;
const double MUTATE_PROB = 0.25;
const double DONT_CARE_PROB = 0.3;

/*
 * Order of arguments in random function:
 * 	1. max population size
 * 	2. the number of iterations to do
 * 	3. target class
 * 	4. default class
 * 	5. elitism rate
 * 	6. specify rate
 * 	7. specify fraction
 * 	8. mutation probability
 * 	9. "don't care" probability
 * 	10. training set
 * 	11. test set
 */
int main(int argc, char **argv) {
   
	// make sure the user enters the right number of arguments
	if (argc < 5) {
		cout << stderr << "Error: too few arguments" << endl;
		exit(0);
	}

	// read in command line arguments
	string training_set_file = argv[1];
	string test_set_file = argv[2];
	string quantiles_file = argv[3];
	string output_file = argv[4];
	int target_class = atoi(argv[5]);

	// seed the random number generator
	rng.seed(rd());

	// read in the training set
	Dataset training_set;
	training_set.readFromCSVFile(training_set_file, target_class);

	// read in the quantiles
	int num_quantiles = training_set.readQuantiles(quantiles_file);
	training_set.setNumQuantiles(num_quantiles);

	// read in the test set
	Dataset test_set;
	test_set.readFromCSVFile(test_set_file, target_class);

	// generate a random population using the training and testing sets.
	// The order of the arguments is given at the top of the file.
	//
	// NOTE: make sure the target class matches the class of the quantiles file being used.
	Population p = Population::random(POP_SIZE,NUM_ITERS,target_class, DEFAULT_CLASS,
			E_RATE,SPEC_RATE,SPEC_FRAC,MUTATE_PROB,DONT_CARE_PROB,training_set,test_set);

	
	// run the LCS for a fixed number of generations 
	int num_iters = p.num_iters();
	for (p.curr_gen_=0; p.curr_gen_<num_iters; p.curr_gen_++) {
		p.applyGA();
	}

	// do a final fitness evaluation after the last iteration
	// (the LCS evaluates fitness at the beginning of an iteration)
	p.evaluateFitness1();
	p.evaluateFitness2();

	// output data for the current run
	p.writeRunData(training_set_file,
		       test_set_file,
		       quantiles_file,
		       output_file);

	// run the LCS on the test set (this will output
	// additional information to the same file as above)
	p.classify(&p.training_set_,output_file);

	return 0;
}
