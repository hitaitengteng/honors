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
 * 	- Figure out how to modify methods so that numerosity parameter
 * 	  is taken into account (or just don't use the numerosity parameter)
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
	Dataset d = Dataset::random(10);
	LCS lcs(p,d);
	for (int i=0; i<20; i++) {
		cout << lcs.curr_gen_ << endl;
		lcs.processInput(i);
	}

	return 0;
	
}
