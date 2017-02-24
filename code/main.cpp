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
   
	rng.seed(rd());

	// read in the data set
	Dataset d;
	d.readFromCSVFile("../Datasets/iris2.csv");

	// create the population
	int max_pop_size = 50;
	Population p(max_pop_size);

	// create the LCS
	LCS lcs(p,d);

	// generate rules based on instances in the data set
	Rule r(d.num_attributes());
	for (int i=0; i<max_pop_size; i++) {
		r.specify(d.data_points_[i], d.attribute_ranges_, lcs.range_scalar());
		lcs.pop_.add(r);
	}

	lcs.pop_.print();

	return 0;
	
}
