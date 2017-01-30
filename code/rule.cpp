#include "rule.h"

/****************************************************************************
 * File:        Rule.cpp
 * Author:      William Gantt
 * Description: Implements functions for the Rule class.
 ****************************************************************************/ 
using namespace std;

/****************************************************************************
 * Inputs:      rule: the rule to be checked for equality.
 * Outputs:     a boolean indicating whether two rules are equivalent.
 * Description: Two rules are defined to be equivalent if their condition
 * 		and class are matching. The values of other member variables
 * 		are considered irrelevant.
 *
 * 		NOTE: ID number is considered irrelevant because there
 * 		should never be duplicates of these. If one needs to
 * 		determine whether two rules are literally the same object,
 * 		then the comparison is easily made using the getID function.
 ****************************************************************************/ 
bool Rule::operator==(const Rule &rule) const {

	// check the classes of both rules
	if (classification != rule.getClass())
		return false;

	// if the rules are in fact the same object, return true immediately
	if (id == rule.getID())
		return true;

	// check the conditions
	int condition_length = condition_length;
	for (size_t i=0; i<condition_length; i++) {

		// if one of the rules has the "don't care" variable set for
		// the current attribute and the other doesn't, the rules
		// are not equivalent
		if (condition[i].getDontCare() != rule.condition[i].getDontCare())
			return false;

		// if control reaches here, we know that both rules have
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

} // end operator ==

/****************************************************************************
 * Inputs:      rule: the rule to be checked for inequality
 * Outputs:     A boolean indicating whether two rules are NOT equal to
 * 		each other.
 * Description: See above.
 ****************************************************************************/ 
bool Rule::operator!=(const Rule &rule) const {

	return !((*this) == rule);

} // end operator != 

/****************************************************************************
 * Inputs:      pMutate: the probability that the value of a single
 * 			 attribute is mutated.
 * 		pDontCare: given that an attribute is to be mutated, the
 * 			   probability that it will be mutated to a "don't
 * 			   care" value
 * 		ranges: the acceptable values for each attribute
 * 		rangeScalar: used in determining the size of the new range
 * 			     of a mutated attribute.
 * 		rng: a random number generator
 * Outputs:     None.
 * Description: Mutates a rule. This involves probabilistically altering the
 * 		values of the attributes in its condition.
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
	int condition_length = condition_length;
	for (size_t i=0; i<condition_length; i++) {

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
 * Inputs:      input: the data instance that will be used to specify the
 * 		       rule.
 * 		ranges: the acceptable ranges of values for each attribute
 * 		rangeScalar: used to scale the spread of an attribute
 * 		rng: a random number generator
 * Outputs:     a specified version of the rule
 * Description: Specifies a rule based on an input vector. For each attribute
 * 		in the rule's condition that is set to "don't care," we
 * 		assign it a particular range of values, using the value of
 * 		the corresponding attribute in the input as the center of
 * 		that range.
 ****************************************************************************/ 
Rule Rule::specify(vector<double> input, vector<pair<double,double> > ranges, 
		double range_scalar, mt19937 &rng) {

	// iterate over all attributes in the condition
	int condition_length = condition_length;
	for (size_t i=0; i<condition_length; i++) {

		// if the current attribute is a "don't care" attribute...
		if (condition[i].getDontCare() == true) {

			// specify its value 
			condition[i].setDontCare(false);
			condition[i].setCenter(input[i]);

			// and specify its spread (generate a random spread
			// within a particular range)
			double range = ranges[i].second - ranges[i].first;
			double maxVal = range * range_scalar;
			uniform_real_distribution<double> dist(0, maxVal);
			double spread = dist(rng);
			condition[i].setSpread(spread);
		}
	}
	return (*this);

} // end specify

/****************************************************************************
 * Inputs:      rule: the rule to be checked for generalization.
 * Outputs:     a boolean indicating whether one rule generalizes another.
 * Description: Determines whether one rule generalizes another. This occurs
 * 		when the following criteria are met:
 * 			(1) both rules have the same class value.
 * 			(2) the range of values for each attribute in one
 * 			    of the rules's condition is greater than the 
 * 			    range of values for the corresponding attribute
 * 			    in the other rule.
 *
 * NOTE: In looking through the code below, one may wonder why a boolean is
 *       used in checking for the equivalence of the rules instead of the
 *       '==' operator. The reason for this is that a boolean saves a lot
 *       of time; using the '==' operator would mean doing a second iteration
 *       through the conditions of both rules.
 ****************************************************************************/ 
bool Rule::generalizes(Rule &rule) const {

	// immediately return false if the rules' classes are different or
	// if they have the same ID number (that is, if they are *literally*
	// the same rule)
	if ((classification != rule.getClass()) || (id == rule.getID()))
		return false;

	// iterate over the conditions of both rules
	int condition_length = condition_length;
	for (size_t i=0; i<condition_length; i++) {

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
			    (r2UpperBound > r1UpperBound))
				return false;
		}
	}
	return true;

} // end generalizes

/****************************************************************************
 * Inputs:      input: the data instance to be checked for matching.
 * Outputs:     A boolean indicating whether the rule matches the data
 * 		instance.
 * Description: Determines whether the condition of a rule matches a given
 * 		input vector. A rule is said to match the input when all of
 * 		the input's attribute values fall within the range of the
 * 		corresponding attribute in the condition.
 ****************************************************************************/ 
bool Rule::matches(vector<double> &input) const {

	// the center and spread of the current attribute
	double center = 0;
	double spread = 0;

	// iterate over the attributes of the rule's condition
	int condition_length = condition_length;
	for (size_t i=0; i<condition_length; i++) {

		// evaluate only if the current attribute is not set to
		// "don't care" in the condition
		if (condition[i].getDontCare() == false) {

			// get the center and spread of the current 
			// attribute in the condition
			center = condition[i].getCenter();
			spread = condition[i].getSpread();

			// check whether the input value for the current
			// attribute falls within the range [center - spread,
			// center + spread]. If not, return false.
			if ((input[i] < (center - spread)) ||
			    (input[i] > (center + spread)))
				return false;
		}
	} 
	return true;
	
} // end matches

/****************************************************************************
 * Inputs:      num_attributes: the number of attributes that the rule is
 * 				is to have in its condition
 * Outputs:     A randomly generated rule.
 * Description: Generates a random rule with a condition of a specified
 * 		length.
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
 * Inputs:      None.
 * Outputs:     None.
 * Description: Prints important information about a rule.
 ****************************************************************************/ 
void Rule::print() {

	printf("\nRule %d\n--------\n", id);
	printf("\nAttribute:  ");
	int condition_length = condition_length;
	for (size_t i=0; i<condition_length; i++)
		printf("[ %d ] ", (int) i);
	printf("\nDon't Care: ");
	for (size_t i=0; i<condition_length; i++)
		printf("[ %d ] ", (int) condition[i].getDontCare());
	printf("\n  Center:   ");
	for (size_t i=0; i<condition_length; i++)
		printf("%.3f ", condition[i].getCenter()); 
	printf("\n  Spread:   ");
	for (size_t i=0; i<condition_length; i++)
		printf("%.3f ", condition[i].getSpread());

	printf("\n");

	printf("Class:      %d\n", classification);
	printf("Experience: %d\n", exp);
	printf("Numerosity: %d\n", numerosity);
	printf("Accuracy:   %.3f\n", accuracy);
	printf("Fitness:    %.3f\n", fitness);

} // end printRule

// end rule.cpp
