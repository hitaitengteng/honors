#include "rule.h"

/****************************************************************************
 * File:        Rule.cpp
 * Author:      William Gantt
 * Description: Implements functions for the Rule class.
 *
 * TODO:
 *
 * 	- implement printRule
 * 	- write unit test for mutate and specify functions
 * 	- write function descriptions
 ****************************************************************************/ 
using namespace std;

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
void Rule::mutate(double pMutate, double pDontCare, 
		vector<pair<double,double>> ranges, 
		double rangeScalar, 
		mt19937 &rng) {

	// a uniform distribution over [0,1]
	uniform_real_distribution<double> dist(0,1);

	// a random values on [0,1]
	double result1;
	double result2;

	// iterate over all attributes in the condition
	for (int i=0; i<condition.size(); i++) {

		// get a random value between 0 and 1
		result1 = dist(rng);

		// if the value is less than or equal to pMutate, we mutate
		// the current attribute. The attribute may be mutated by 
		// either adjusting its "center" variable, or by adjusting
		// its "don't care" value
		if (result1 <= pMutate) {

			// determine whether to change the "don't care" value
			// or to move the center
			result2 = dist(rng);

			if (result2 <= pDontCare) { // change to "don't care"

				if (condition[i].getDontCare() == true)
					condition[i].setDontCare(false);
				else
					condition[i].setDontCare(true);

			} else {                    // move center

				// get the range of values for the current attribute
				double range = ranges[i].second - ranges[i].first;

				// scale the range by rangeScalar
				double maxVal = range * rangeScalar;

				// select a random value in the interval [0,maxVal] 
				// by which the center is to be adjusted
				uniform_real_distribution<double> dist2(0,maxVal);
				double centerAdjust = dist2(rng);

				// get the current center value
				double oldCenter = condition[i].getCenter();

				// adjust the center (whether to add centerAdjust or
				// substract centerAdjust is determined by whether
				// rng is odd or even)
				if (rng() % 2 == 0)
					condition[i].setCenter(oldCenter + centerAdjust);
				else
					condition[i].setCenter(oldCenter - centerAdjust);
			}

		}
	}

} // end mutate

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
void Rule::specify(vector<double> input, vector<pair<double,double>> ranges, 
		double rangeScalar, mt19937 &rng) {

	// iterate over all attributes in the condition
	for (int i=0; i<condition.size(); i++) {

		// if the current attribute is a "don't care" attribute...
		if (condition[i].getDontCare() == true) {

			// specify its value 
			condition[i].setDontCare(false);
			condition[i].setCenter(input[i]);

			// and specify its spread (generate a random spread
			// within a particular range)
			double range = ranges[i].second - ranges[i].first;
			double maxVal = range * rangeScalar;
			uniform_real_distribution<double> dist(0, maxVal);
			double spread = dist(rng);
			condition[i].setSpread(spread);
		}

	}

} // end specify

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
void Rule::print() {

	printf("\nAttribute:  ");
	for (int i=0; i<condition.size(); i++)
		printf("[ %d ] ", i);
	printf("\n  Center:   ");
	for (int i=0; i<condition.size(); i++)
		printf("%.3f ", condition[i].getCenter()); 
	printf("\n  Spread:   ");
	for (int i=0; i<condition.size(); i++)
		printf("%.3f ", condition[i].getSpread());

	printf("\n");

	printf("Class:      %d\n", classification);
	printf("Experience: %d\n", exp);
	printf("Numerosity: %d\n", numerosity);
	printf("Accuracy:   %.3f\n", accuracy);
	printf("Fitness:    %.3f\n", fitness);

} // end printRule

// end rule.cpp
