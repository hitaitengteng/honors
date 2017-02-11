#include "lcs.h"
#include <time.h>

/****************************************************************************
 * File:        main.cpp
 * Author:      William Gantt
 * Description: 
 *
 * TODO:
 * 	- make member variables private that should be private
 * 	- add comments to the population and LCS files (esp function 
 * 	  descriptions)
 * 	- Update Crisp_LCS_Schema.txt
 * 	- Should population size increase when a child is subsumed?
 *
 ****************************************************************************/ 
  
using namespace std;
   
// declared as external variables in utilities.h, defined here
mt19937 rng;
random_device rd;
uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(1,10);

/****************************************************************************
 *
 *
 *
 ****************************************************************************/ 
int main(int argc, char **argv) {
   
	rng.seed(rd());

	Population p = Population::random(20,10);
	p.setMaxSize(30);
	LCS lcs(p);
	// lcs.pop_.printVerbose();
	lcs.training_set_ = Dataset::random(25);
	lcs.setCurrDataPoint(Dataset::randomDataPoint(10));
	lcs.createMatchAndCorrectSets();
	if (lcs.correct_set_.members_.size() >= 2)
		lcs.reproduceAndReplace();

	return 0;
	
}
