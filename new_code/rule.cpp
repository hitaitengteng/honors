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
			if (condition_[i].l_bound() != 
			    rule.condition_[i].l_bound())
				return false;
			if (condition_[i].u_bound() != 
			    rule.condition_[i].u_bound())
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
void Rule::mutate(double p_mutate, double p_dont_care, vector<vector<double> > quantiles) {

	// a random values on [0,1]
	double result1;
	double result2;

	// the quantile of the current attribute
	int curr_quantile = -1;

	// the number of quantiles
	int num_quantiles = quantiles.size() - 1;

	// iterate over all attributes in the condition
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++) {

		// get a random value between 0 and 1
		result1 = real_dist(rng);

		// if the value is less than or equal to p_mutate, we mutate
		// the current attribute. The attribute may be mutated in one
		// of two ways:
		// 	1. Change of quantiles (of upper and lower bounds)
		// 	2. Change of "don't care" value
		if (result1 <= p_mutate) {

			// determine whether to change the "don't care" value
			// or to move the center
			result2 = real_dist(rng);

			if (result2 <= p_dont_care) { // change "don't care"

				if (condition_[i].dont_care() == true) {
					condition_[i].setDontCare(false);
					num_dont_care_--;
				} else {
					condition_[i].setDontCare(true);
					num_dont_care_++;
				}

			} else {                      // change quantile

				// get the quantile of the current attribute
				curr_quantile = condition_[i].quantile();

				// randomly select a different quantile
				int new_quantile = -1;
				do {
					new_quantile = rng() % num_quantiles;
				} while (new_quantile == curr_quantile);

				// update the attribute's quantile
				condition_[i].setQuantile(new_quantile);
				cout << new_quantile << endl;

				// and update the upper and lower bounds
				condition_[i].setLowerBound(quantiles[i][new_quantile]);
				condition_[i].setUpperBound(quantiles[i][new_quantile+1]);
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
void Rule::specify(vector<double> input, vector<vector<double> > quantiles) {

/*
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
*/
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

			// return false if the interval described by rule 2
			// does not fall within that described by rule 1
			if ((rule.condition_[i].l_bound() < condition_[i].l_bound()) ||
			    (rule.condition_[i].u_bound() > condition_[i].u_bound()))
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
bool Rule::matches(vector<double> input) {

	// the center and spread of the current attribute
	double l_bound = 0;
	double u_bound = 0;

	// iterate over the attributes of the rule's condition
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++) {

		// evaluate only if the current attribute is not set to
		// "don't care" in the condition
		if (condition_[i].dont_care() == false) {

			// get the center and spread of the current 
			// attribute in the condition
			l_bound = condition_[i].l_bound();
			u_bound = condition_[i].u_bound();

			// check whether the input value for the current
			// attribute falls within the range [center - spread,
			// center + spread]. If not, return false.
			if ((input[i] < l_bound) || (input[i] > u_bound))
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
Rule Rule::random(int num_classes, vector<vector<double> > quantiles, double dont_care_prob) {

	Rule r;

	// random class 
	r.setClass((rng() % num_classes) + 1);

	// the number of quantiles into which the attributes are divided
	int num_quantiles = quantiles[0].size();

	// a random quantile
	int quantile = -1;

	// random attributes
	Attribute a;
	int num_attributes = quantiles.size() - 1;
	for (int i=0; i<num_attributes; i++) {

		// generate a random quantile
		quantile = rng() % (num_quantiles - 1);

		// generate the attribute
		a = Attribute::random(quantiles[i], quantile, dont_care_prob);

		// count the number of don't cares
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
	printf("\nAttribute:       ");
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++)
		printf("%-7d", (int) i);
	printf("\nDon't Care:      ");
	for (size_t i=0; i<condition_length; i++) {
		if (condition_[i].dont_care())
			printf("%-7s",dc);
		else
			printf("%-7s", space);
	}
	printf("\nLower Bound:     ");
	for (size_t i=0; i<condition_length; i++) {
		if (!condition_[i].dont_care())
			printf("%-7.3f", condition_[i].l_bound()); 
		else 
			printf("%-7s", space);
	}
	printf("\nUpper Bound:     ");
	for (size_t i=0; i<condition_length; i++) {
		if (!condition_[i].dont_care())
			printf("%-7.3f", condition_[i].u_bound()); 
		else 
			printf("%-7s", space);
	}
	printf("\nQuantile:        ");
	for (size_t i=0; i<condition_length; i++) {
		if (!condition_[i].dont_care())
			printf("%-7d", condition_[i].quantile());
		else
			printf("%-7s", space);
	}

	printf("\n");
	printf("Fitness1:         %.3f\n", fitness1_);
	printf("Fitness2:         %.3f\n", fitness2_);
	printf("Class:           %d\n", classification_);

} // end print

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Prints important information about a rule.
 ****************************************************************************/ 
void Rule::printVerbose() {

	char dc[] = "[DC]";
	char space[] = " ";

	printf("\nRule %d\n--------\n", id_);
	printf("\nAttribute:        ");
	int condition_length = condition_.size();
	for (size_t i=0; i<condition_length; i++)
		printf("%-7d", (int) i);
	printf("\nDon't Care:      ");
	for (size_t i=0; i<condition_length; i++) {
		if (condition_[i].dont_care())
			printf("%-7s",dc);
		else
			printf("%-7s", space);
	}
	printf("\nLower Bound:     ");
	for (size_t i=0; i<condition_length; i++) {
		if (!condition_[i].dont_care())
			printf("%-7.3f", condition_[i].l_bound()); 
		else 
			printf("%-7s", space);
	}
	printf("\nUpper Bound:     ");
	for (size_t i=0; i<condition_length; i++) {
		if (!condition_[i].dont_care())
			printf("%-7.3f", condition_[i].u_bound()); 
		else 
			printf("%-7s", space);
	}
	printf("\nQuantile:        ");
	for (size_t i=0; i<condition_length; i++) {
		if (!condition_[i].dont_care())
			printf("%-7d", condition_[i].quantile());
		else
			printf("%-7s", space);
	}

	printf("\n\n");
	printf("Class:           %d\n", classification_);
	printf("# Don't Care:    %d\n", num_dont_care_);
	printf("Fitness1:        %.3f\n", fitness1_);
	printf("Fitness2:        %.3f\n", fitness2_);
	printf("True Pos:        %.1f\n", true_positives_);
	printf("False Pos:       %.1f\n", false_positives_);
	printf("True Neg:        %.1f\n", true_negatives_);
	printf("False Neg:       %.1f\n", false_negatives_);
	printf("\n");

} // end printVerbose

// end rule.cpp
