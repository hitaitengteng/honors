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

/****************************************************************************
 *
 *
 *
 ****************************************************************************/ 
int main(int argc, char **argv) {
   
	rng.seed(rd());

	Dataset d;
	d.readFromCSVFile("../Datasets/iris2.csv");
	d.printDataset();

	return 0;
	
}
