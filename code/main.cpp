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
 ****************************************************************************/ 
  
using namespace std;
   
// declared as external variables in utilities.h, defined here
mt19937 rng;
random_device rd;
uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(1,10);

int main(int argc, char **argv) {
   
	// seed the random number generator
	rng.seed(rd());

	// read in the data set
	Dataset d;
	d.readFromCSVFile("../Datasets/iris3.csv");

	// create the population
	int max_pop_size = 50;
	Population p(max_pop_size);

	// create the LCS
	LCS lcs(p,d);

	// generate rules based on instances in the data set
	Rule r1 = lcs.training_set_.createRuleFromDataPoint(40,lcs.range_scalar());
	Rule r2 = lcs.training_set_.createRuleFromDataPoint(50,lcs.range_scalar());
	Rule r3 = lcs.training_set_.createRuleFromDataPoint(100,lcs.range_scalar());

	// add the rules to the population
	lcs.pop_.add(r1);
	lcs.pop_.add(r2);
	lcs.pop_.add(r3);

	// run the LCS
	for (int i=0; i<lcs.training_set_.num_data_points(); i++) {
		lcs.processInput(i);
		cout << lcs.match_set_.num_classes_represented() << endl;
	}

	// lcs.pop_.printVerbose();

	return 0;
}
