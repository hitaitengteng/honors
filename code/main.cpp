#include "rule.h"

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

	// create input
	vector<double> input;
	for (int i=0; i<10; i++) {
		input.push_back(dist(rng));
	}

	// create ranges
	vector<pair<double,double>> ranges;
	for (int i=0; i<10; i++) {
		ranges.push_back(make_pair(2,5));
	}

	Rule rule;
	Attribute a("nuts");
	int dontCare;
	for (int i=0; i<10; i++) {
		a.setCenter(dist(rng));
		dontCare = rng() % 2;
		if (dontCare == 1)
			a.setDontCare(true);
		else
			a.setDontCare(false);
		rule.condition.push_back(a);
		printf("center: %f\nspread: %f\ndontCare: %d\n\n", a.getCenter(), a.getSpread(), dontCare);
	}

	double rangeScalar = 0.1;
	double pMutate = 0.1;
	double pDontCare = 0.2;
	rule.specify(input, ranges, rangeScalar, rng);
	rule.print();

	return 0;
	
}
