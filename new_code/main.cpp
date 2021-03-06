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
uniform_int_distribution<int> int_dist(1,10);

// named constants for testing
//
// NOTE: E_RATE * POP_SIZE must be an integer value, and for
// crossover to work properly, I think it has to be an *even*
// integer value
//
// NOTE 2: Should add parameter for specify operator frequency
const int POP_SIZE = 300;
const int NUM_ITERS = 2500;
const int DEFAULT_CLASS = -1;
const double E_RATE = 0.6;
const double MUTATE_PROB = 0.25;
const double DONT_CARE_PROB = 0.3;

/*
 * Order of arguments in random function:
 * 	1. max population size
 * 	2. the number of iterations to do
 * 	3. target class
 * 	4. elitism rate
 * 	6. mutation probability
 * 	7. "don't care" probability
 * 	8. training set
 * 	9. test set
 */
int main(int argc, char **argv) {
   
	// make sure the user enters the right number of arguments
	if (argc < 5) {
		cout << stderr << "Error: too few arguments" << endl;
	}

	// get the name of the input files for
	// the training and test sets
	string training_set_file = argv[1];
	string test_set_file = argv[2];
	string quantiles_file = argv[3];
	string output_file = argv[4];
	int target_class = atoi(argv[5]);

	// seed the random number generator
	rng.seed(rd());

	// read in the training set
	Dataset training_set;
	training_set.readFromCSVFile(training_set_file);

	// read in the quantiles
	int num_quantiles = training_set.readQuantiles(quantiles_file);
	training_set.setNumQuantiles(num_quantiles);

	// read in the test set
	Dataset test_set;
	test_set.readFromCSVFile(test_set_file);

	// generate a random population using the training and testing sets.
	// The order of the arguments is given at the top of the file.
	//
	// NOTE: make sure the target class matches the class of the quantiles file being used.
	
	Population p = Population::random(POP_SIZE,NUM_ITERS,target_class, DEFAULT_CLASS,
			E_RATE,MUTATE_PROB,DONT_CARE_PROB,training_set,test_set);
	int num_iters = p.num_iters();
	for (int i=0; i<num_iters; i++)
		p.applyGA();

	p.evaluateFitness1();
	p.evaluateFitness2();

	p.writeRunData(training_set_file,
		       test_set_file,
		       quantiles_file,
		       output_file);
	p.classify(&p.training_set_,output_file);
	p.classify(&p.test_set_,output_file);


	return 0;
}
