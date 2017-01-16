#include "rule.h"
#include "dataset.h"

/****************************************************************************
 * File:        main.cpp
 * Author:      William Gantt
 * Description: 
 *
 * TODO:
 * 	- Determine default spread values for all attributes
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
