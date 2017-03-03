#include "lcs.h"
#include <time.h>

/****************************************************************************
 * File:        main.cpp
 * Author:      William Gantt
 * Description: 
 *
 * TODO:
 * 	- make member variables private that should be private
 * 	- Update Crisp_LCS_Schema.txt
 * 	- Should population size increase when a child is subsumed?
 * 	- Figure out how to modify methods so that numerosity parameter
 * 	  is taken into account (or just don't use the numerosity parameter)
 * 	- Fix bug that causes centers to have negative values
 * 	- Figure out why there are so many rules with accuracy and fitness 0
 ****************************************************************************/ 
  
using namespace std;
   
// declared as external variables in utilities.h, defined here
mt19937 rng;
random_device rd;
uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(1,10);

int main(int argc, char **argv) {
   
	if (argc < 2) {
		cout << stderr << "Error: too few arguments" << endl;
	}

	// get the name of the input files for
	// the training and test sets
	string training_set_file = argv[1];
	string test_set_file = argv[2];

	// seed the random number generator
	rng.seed(rd());

	// read in the training set
	Dataset training_set;
	training_set.readFromCSVFile(training_set_file);

	// read in the test set
	Dataset test_set;
	test_set.readFromCSVFile(test_set_file);

	// create the population
	int max_pop_size = 30;
	Population p(max_pop_size);

	// create the LCS
	LCS lcs(p,training_set,test_set);

	// run the LCS
	for (int i=0; i<lcs.training_set_.num_data_points(); i++) {
		lcs.processInput(i);
		// lcs.print();
	}

	// lcs.pop_.printVerbose();
	pair<int,int>* classifications = lcs.classifyInputs();
	free(classifications);

	return 0;
}
