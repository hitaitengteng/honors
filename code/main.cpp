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
 *
 ****************************************************************************/ 

using namespace std;

int main(int argc, char **argv) {

	// a Marsenne Twister random number generator
	mt19937 rng; 

	// seed the RNG (random_device generates a non-deterministic seed)
	rng.seed(random_device{}());

	uniform_real_distribution<double> dist(0,10);

	Dataset d;
	d.readFromCSVFile(argv[1]);

	d.print();

	return 0;
	
}
