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

	LCS lcs(25);
	lcs.pop_ = Population::random(25,10);
	lcs.pop_.setMaxSize(30);
	lcs.training_set_ = Dataset::random(25);
	lcs.setCurrDataPoint(Dataset::randomDataPoint(10));
	lcs.reproduceAndReplace();

	return 0;
	
}
