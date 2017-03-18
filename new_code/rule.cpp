#include "rule.h"

/****************************************************************************
 * File:        Rule.cpp
 * Author:      William Gantt
 * Description: Implements functions for the Rule class.
 *
 * TODO:
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
	if (classification_ != rule.classification())
		return false;

	// if the rules are in fact the same object, return true immediately
	if (id_ == rule.id())
		return true;

	// check the conditions
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++) {

		// if one of the rules has the "don't care" variable set for
		// the current attribute and the other doesn't, the rules
		// are not equivalent
		if (condition_[i].dont_care() != rule.condition_[i].dont_care())
			return false;

		// if control reaches here, we know that both rules have
		// the same value for the "don't care" variable. If that
		// value is false, we must then check their center and
		// spread variables for equality
		if (condition_[i].dont_care() == false) {
			if (condition_[i].center() != 
			    rule.condition_[i].center())
				return false;
			if (condition_[i].spread() != 
			    rule.condition_[i].spread())
				return false;
		}
	}
	return true;

} // end operator ==

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
int Rule::processInput(std::vector<double> &input) {

	// get the input's class
	int classification = input.back();

	// if the rule's condition matches the input...
	if (matches(input)) {

		// ...and if the rule correctly classifies the input,
		// then the input counts as a true positive for this rule
		if (classification_ == classification) {
			true_positives_++;
			return TP;
		}

		// if the rule doesn't correctly classify the input,
		// it's a false positive
		else {
			false_positives_++;
			return FP;
		}

	// if the rule's condition DOES NOT match the input...
	} else {

		// ...but the class of the rule matches the class of
		// the input, then the input counts as a false negative
		// for this rule
		if (classification_ == classification) {
			false_negatives_++;
			return FN;
		}

		// if the rule doesn't correctly classify the input,
		// it's a true negative
		else {
			true_negatives_++;
			return TN;
		}
	}

} // end processInput 

/****************************************************************************
 * Inputs:      p_mutate: the probability that the value of a single
 * 			 attribute is mutated.
 * 		p_dont_care: given that an attribute is to be mutated, the
 * 			   probability that it will be mutated to a "don't
 * 			   care" value
 * 		ranges: the acceptable values for each attribute
 * 		range_scalar: used in determining the size of the new range
 * 			     of a mutated attribute.
 * 		rng: a random number generator
 * Outputs:     None.
 * Description: Mutates a rule. This involves probabilistically altering the
 * 		values of the attributes in its condition.
 ****************************************************************************/ 
void Rule::mutate(double p_mutate, double p_dont_care, 
		vector<pair<double,double> > ranges, 
		double range_scalar) {

	// a random values on [0,1]
	double result1;
	double result2;

	// iterate over all attributes in the condition
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++) {

		// get a random value between 0 and 1
		result1 = real_dist(rng);

		// if the value is less than or equal to p_mutate, we mutate
		// the current attribute. The attribute may be mutated by 
		// either adjusting its "center" variable, or by adjusting
		// its "don't care" value
		if (result1 <= p_mutate) {

			// determine whether to change the "don't care" value
			// or to move the center
			result2 = real_dist(rng);

			if (result2 <= p_dont_care) { // change to "don't care"

				if (condition_[i].dont_care() == true) {
					condition_[i].setDontCare(false);
					num_dont_care_--;
				} else {
					condition_[i].setDontCare(true);
					num_dont_care_++;
				}

			} else {                      // move center

				// get the range of values for the current attribute
				double range = ranges[i].second - ranges[i].first;

				// scale the range by range_scalar
				double maxVal = range * range_scalar;

				// select a random value in the interval [0,maxVal] 
				// by which the center is to be adjusted
				uniform_real_distribution<double> real_dist2(0,maxVal);
				double centerAdjust = real_dist2(rng);

				// get the current center value
				double oldCenter = condition_[i].center();

				// adjust the center (whether to add centerAdjust or
				// substract centerAdjust is determined by whether
				// rng is odd or even)
				if (rng() % 2 == 0)
					condition_[i].setCenter(oldCenter + centerAdjust);
				else
					condition_[i].setCenter(oldCenter - centerAdjust);
			}
		}
	}
} // end mutate

/****************************************************************************
 * Inputs:      input: the data instance that will be used to specify the
 * 		       rule.
 * 		ranges: the acceptable ranges of values for each attribute
 * 		range_scalar: used to scale the spread of an attribute
 * 		rng: a random number generator
 * Outputs:     a specified version of the rule
 * Description: Specifies a rule based on an input vector. For each attribute
 * 		in the rule's condition that is set to "don't care," we
 * 		assign it a particular range of values, using the value of
 * 		the corresponding attribute in the input as the center of
 * 		that range.
 ****************************************************************************/ 
void Rule::specify(vector<double> input, vector<pair<double,double> > ranges, 
		double range_scalar) {

	// set the class attribute value of the rule to that of the input
	setClass(input.back());

	// iterate over all attributes in the condition
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++) {

		// if the current attribute is a "don't care" attribute...
		if (condition_[i].dont_care() == true) {

			// specify its value 
			condition_[i].setDontCare(false);
			condition_[i].setCenter(input[i]);
			num_dont_care_--;

			// and specify its spread (generate a random spread
			// within a particular range)
			double range = ranges[i].second - ranges[i].first;
			double maxVal = range * range_scalar;
			uniform_real_distribution<double> real_dist2(0, maxVal);
			double spread = real_dist2(rng);
			condition_[i].setSpread(spread);
		}
	}
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
	if ((classification_ != rule.classification()) || (id_ == rule.id()))
		return false;

	// iterate over the conditions of both rules
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++) {

		// only want to check attributes that aren't "don't cares"
		// (if this rule has the "don't care" variable set for a
		// particular attribute, it is at least as general as the
		// other rule with respect to that attribute).
		if (condition_[i].dont_care() == false) {

			// if this rule has a specific value for an
			// attribute for which the other rule has the
			// "don't care" variable set, then the former
			// cannot be more general than the latter
			if (rule.condition_[i].dont_care() == true)
				return false;

			// determine the intervals over the current attribute
			// that each rule covers
			double r1LowerBound = condition_[i].center() -
				condition_[i].spread();
			double r1UpperBound = condition_[i].center() +
				condition_[i].spread();
			double r2LowerBound = rule.condition_[i].center() -
				rule.condition_[i].spread();
			double r2UpperBound = rule.condition_[i].center() +
				rule.condition_[i].spread();

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
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++) {

		// evaluate only if the current attribute is not set to
		// "don't care" in the condition
		if (condition_[i].dont_care() == false) {

			// get the center and spread of the current 
			// attribute in the condition
			center = condition_[i].center();
			spread = condition_[i].spread();

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
 * Inputs:      
 * Outputs:     
 * Description: 
 ****************************************************************************/ 
Rule Rule::random(int num_attributes, int num_classes, 
		vector<pair<double,double> > attribute_ranges,
		double range_scalar, double dont_care_prob) {

	Rule r;

	// random class (the '+1' is an artifice that should eventually
	// be removed)
	r.setClass((rng() % num_classes) + 1);

	// random attributes
	Attribute a;
	for (int i=0; i<num_attributes; i++) {
		a = Attribute::random(attribute_ranges[i], range_scalar, dont_care_prob);
		if (a.dont_care() == true)
			r.num_dont_care_++;
		r.condition_.push_back(a);
	}

	return r;
}

/****************************************************************************
 * Inputs:      num_attributes: the number of attributes that the rule is
 * 				is to have in its condition
 * Outputs:     A randomly generated rule.
 * Description: Generates a random rule with a condition of a specified
 * 		length.
 ****************************************************************************/ 
Rule Rule::random(int num_attributes, int num_classes) {

	Rule r;

	// random class
	r.setClass(rng() % num_classes);

	// random attributes
	Attribute a;
	for (int i=0; i<num_attributes; i++) {
		a = Attribute::random();
		if (a.dont_care() == true)
			r.num_dont_care_++;
		r.condition_.push_back(a);
	}

	return r;

} // end random

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Prints important information about a rule.
 ****************************************************************************/ 
void Rule::print() {

	char dc[] = "[DC]";
	char space[] = " ";

	printf("\nRule %d\n--------\n", id_);
	printf("\nAttribute:  ");
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++)
		printf("%-7d", (int) i);
	printf("\nDon't Care: ");
	for (size_t i=0; i<condition_length; i++) {
		if (condition_[i].dont_care())
			printf("%-7s",dc);
		else
			printf("%-7s", space);
	}
	printf("\nCenter:     ");
	for (size_t i=0; i<condition_length; i++) {
		if (!condition_[i].dont_care())
			printf("%-7.3f", condition_[i].center()); 
		else 
			printf("%-7s", space);
	}
	printf("\nSpread:     ");
	for (size_t i=0; i<condition_length; i++) {
		if (!condition_[i].dont_care())
			printf("%-7.3f", condition_[i].spread()); 
		else 
			printf("%-7s", space);
	}

	printf("\n");
	printf("Fitness1:         %.3f\n", fitness1_);
	printf("Fitness2:         %.3f\n", fitness2_);
	printf("Class:           %d\n", classification_);

} // end printRule


/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Prints important information about a rule.
 ****************************************************************************/ 
void Rule::printVerbose() {

	char dc[] = "[DC]";
	char space[] = " ";

	printf("\nRule %d\n--------\n", id_);
	printf("\nAttribute:  ");
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++)
		printf("%-7d", (int) i);
	printf("\nDon't Care: ");
	for (size_t i=0; i<condition_length; i++) {
		if (condition_[i].dont_care())
			printf("%-7s",dc);
		else
			printf("%-7s", space);
	}
	printf("\nCenter:     ");
	for (size_t i=0; i<condition_length; i++) {
		if (!condition_[i].dont_care())
			printf("%-7.3f", condition_[i].center()); 
		else 
			printf("%-7s", space);
	}
	printf("\nSpread:     ");
	for (size_t i=0; i<condition_length; i++) {
		if (!condition_[i].dont_care())
			printf("%-7.3f", condition_[i].spread()); 
		else 
			printf("%-7s", space);
	}

	printf("\n\n");
	printf("Class:           %d\n", classification_);
	printf("# Don't Care:    %d\n", num_dont_care_);
	printf("Fitness1:        %.3f\n", fitness1_);
	printf("Fitness2:        %.3f\n", fitness2_);
	printf("True Pos:        %d\n", true_positives_);
	printf("False Pos:       %d\n", false_positives_);
	printf("True Neg:        %d\n", true_negatives_);
	printf("False Neg:       %d\n", false_negatives_);
	printf("\n");

} // end printRule

// end rule.cpp
