#include "rule.h"
#include "dataset.h"

/****************************************************************************
 * File:        main.cpp
 * Author:      William Gantt
 * Description: 
 *
 * TODO:
 * 	- fix member variable names in all necessary files
 * 	- make member variables private that should be private
 * 	- add comments to the population and LCS files (esp function 
 * 	  descriptions)
 * 	- make it so that you're using a single random number generator for
 * 	  all functions
 * 	- Update Crisp_LCS_Schema.txt
 *
 ****************************************************************************/ 

using namespace std;

int main(int argc, char **argv) {

	rng.seed(rd());

	vector<double> d1 = Dataset::randomDataPoint(10);
	vector<double> d2 = Dataset::randomDataPoint(10);

	for (int i=0; i<10; i++) {
		printf("%.3f ", d1[i]);
	}

	printf("\n");
	for (int i=0; i<10; i++) {
		printf("%.3f ", d2[i]);
	}
	printf("\n");

	return 0;
	
}
