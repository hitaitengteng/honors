#include "rule.h"

/****************************************************************************
 * File:        Rule.cpp
 * Author:      William Gantt
 * Description: Implements functions for the Rule class.
 *
 * TODO:
 *
 * 	- write function descriptions
 * 	- improve efficiency of generalizes function (eliminate equality
 * 	  check at end)
 ****************************************************************************/ 
using namespace std;

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description: Two rules are defined to be equivalent if their condition
 * 		and class are matching. The values of other member variables
 * 		are considered irrelevant.
 ****************************************************************************/ 
bool Rule::operator==(const Rule &rule) const {

	// check the classes of both rules
	if (classification != rule.getClass())
		return false;

	// check the conditions
	for (int i=0; i<condition.size(); i++) {

		// if one of the rules has the "don't care" variable set for
		// the current attribute and the other doesn't, the rules
		// are not equivalent
		if (condition[i].getDontCare() != rule.condition[i].getDontCare())
			return false;

		// if control reaches here, we know thaat both rules have
		// the same value for the "don't care" variable. If that
		// value is false, we must then check their center and
		// spread variables for equality
		if (condition[i].getDontCare() == false) {
			if (condition[i].getCenter() != 
			    rule.condition[i].getCenter())
				return false;
			if (condition[i].getSpread() != 
			    rule.condition[i].getSpread())
				return false;
		}
	}
	return true;

} // end operator==

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
void Rule::mutate(double pMutate, double pDontCare, 
		vector<pair<double,double> > ranges, 
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
Rule Rule::specify(vector<double> input, vector<pair<double,double> > ranges, 
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

	return (*this);

} // end specify

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 *
 * NOTE: In looking through the code below, one may wonder why a boolean is
 *       used in checking for the equivalence of the rules instead of the
 *       '==' operator. The reason for this is that a boolean saves a lot
 *       of time; using the '==' operator would mean doing a second iteration
 *       through the conditions of both rules.
 ****************************************************************************/ 
bool Rule::generalizes(const Rule &rule) const {

	// immediately return false if the rules' classes are different
	if (classification != rule.getClass())
		return false;

	// a boolean for checking the equivalence of the two rules
	bool areEquivalent = true;

	// iterate over the conditions of both rules
	for (int i=0; i<condition.size(); i++) {

		if ((condition[i].getDontCare() == true) &&
		     (rule.condition[i].getDontCare() == false))
			     areEquivalent = false;

		// only want to check attributes that aren't "don't cares"
		// (if this rule has the "don't care" variable set for a
		// particular attribute, it is at least as general as the
		// other rule with respect to that attribute).
		if (condition[i].getDontCare() == false) {

			// if this rule has a specific value for an
			// attribute for which the other rule has the
			// "don't care" variable set, then the former
			// cannot be more general than the latter
			if (rule.condition[i].getDontCare() == true)
				return false;

			// determine the intervals over the current attribute
			// that each rule covers
			double r1LowerBound = condition[i].getCenter() -
				condition[i].getSpread();
			double r1UpperBound = condition[i].getCenter() +
				condition[i].getSpread();
			double r2LowerBound = rule.condition[i].getCenter() -
				rule.condition[i].getSpread();
			double r2UpperBound = rule.condition[i].getCenter() +
				rule.condition[i].getSpread();

			// return false if the interval described by rule 2
			// does not fall within that described by rule 1
			if ((r2LowerBound < r1LowerBound) ||
			    (r2UpperBound > r1UpperBound)) {
				return false;

			  // if control reaches here, then we know the interval
			  // described by rule 2 falls within that described by
			  // rule 1. This else if statement checks to see whether
			  // at least one of the bounds of rule 2 is not equivalent
			  // to the corresponding bound of rule 1. If this is so,
			  // then the rules cannot be equivalent.
			} else if ((r2LowerBound > r1LowerBound) ||
			           (r2UpperBound < r1UpperBound)) {
				areEquivalent = false;
			}
		}
	}
	
	// as mentioned in the description, if two rules are equivalent to
	// each other, neither is considered a generalization of the other
	if (areEquivalent)
		return false;

	return true;

} // end generalizes

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
Rule Rule::getRandom(int num_attributes) {

	Rule r;

	// random number generator
	mt19937 rng;
	rng.seed(random_device{}());

	// random class
	r.setClass(rng() % NUM_CLASSES);

	// random attributes
	Attribute a("");
	for (int i=0; i<num_attributes; i++) {
		a = Attribute::getRandom();
		if (a.getDontCare() == true)
			r.numDontCare++;
		r.condition.push_back(a);
	}

	// a uniform distribution over [0,1]
	uniform_real_distribution<double> dist(0,1);

	// random accuracy and fitness
	r.setAccuracy(dist(rng));
	r.setFitness(dist(rng));

	return r;

} // end getRandom

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
void Rule::print() {

	printf("\nAttribute:  ");
	for (int i=0; i<condition.size(); i++)
		printf("[ %d ] ", i);
	printf("\nDon't Care: ");
	for (int i=0; i<condition.size(); i++)
		printf("[ %d ] ", (int) condition[i].getDontCare());
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
