#include "population.h"
#include <time.h>

/****************************************************************************
 * File:        main.cpp
 * Author:      William Gantt
 * Description: 
 ****************************************************************************/ 
  
using namespace std;
   
// declared as external variables in utilities.h, defined here
mt19937 rng;
random_device rd;
uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(1,10);

/*
 * Order of arguments in random2 function:
 * 	1. max population size
 * 	2. target class
 * 	3. elitism rate
 * 	4. crossover probability
 * 	5. mutation probability
 * 	6. "don't care" probability
 * 	7. training set
 * 	8. test set
 */
int main(int argc, char **argv) {
   
	// make sure the user enters the right number of arguments
	if (argc < 2) {
		cout << stderr << "Error: too few arguments" << endl;
	}

	// get the name of the input files for
	// the training and test sets
	string training_set_file = argv[1];
	string test_set_file = argv[2];
	string quantiles_file;

	if (argc > 2)
		quantiles_file = argv[3];

	// seed the random number generator
	rng.seed(rd());

	// read in the training set
	Dataset training_set;
	training_set.readFromCSVFile(training_set_file);

	// read in the quantiles
	if (argc > 2) {
		int num_quantiles = training_set.readQuantiles(quantiles_file);
		training_set.setNumQuantiles(num_quantiles);
	}

	// read in the test set
	Dataset test_set;
	test_set.readFromCSVFile(test_set_file);

	// generate a random population using the training and testing sets.
	// The order of the arguments is given above the main function.
	Population p = Population::random2(1,0,0,0,0,0.25,training_set,test_set);
	p.rules_[0].mutate(0.5,0.25,training_set.attribute_quantiles_);

	// p.evaluateFitness1();
	// p.evaluateFitness2();
	p.printVerbose();

	return 0;
}
