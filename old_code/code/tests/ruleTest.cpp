#include "../rule.h"
#include "../dataset.h"

/****************************************************************************
 * File:        ruleTest.cpp
 * Author:      William Gantt
 * Description: Implements and runs unit tests for methods of the Rule class.
 *
 * TODO:
 *
 * 	- Add boolean arguments to let user run specific tests
 ****************************************************************************/ 
using namespace std;

bool testEquality();
bool testGeneralizes();
void testMutate();
bool testSpecify();
bool testMatches();

// a random number and seed generator
mt19937 rng;
random_device rd;

// some distributions
uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(1,10);

// named constants
static const double P_MUTATE = 0.2;
static const double P_DONT_CARE = 0.5;
static const double RANGE_SCALAR = 0.1;

/****************************************************************************
 * Input:
 * Output:
 * Description:
 ****************************************************************************/ 
int main(int argc, char **argv) {

	rng.seed(rd());

	printf("Running tests on equality operator...\n");
	testEquality();
	printf("Rule equality operator tests complete.\n\n");

	printf("Running tests on 'matches' function...\n");
	testMatches();
	printf("'Matches' function tests complete.\n\n");

	printf("Running tests on 'generalizes' function...\n");
	testGeneralizes();
	printf("'Generalizes' function tests complete.\n\n");

	printf("Running tests on 'specify' function...\n");
	testSpecify();
	printf("'Specify' function tests complete.\n\n");

	return 0;
}

/****************************************************************************
 * Inputs:      
 * Outputs:     a boolean indicating whether all tests have passed
 * Description: a unit test for the Rule class equality operator ('==')
 ****************************************************************************/ 
bool testEquality() {

	Rule r1;
	Rule r2;

	// ----------------------------------------------------------------------
	// TEST 1: condition has not been initialized; should return true
	// ----------------------------------------------------------------------

	// check equality
	printf("Rule Equality Test 1 (uninitialized conditions): ");
	if (r1 == r2) {
		printf("Passed.\n");
	} else {
		printf("Failed.\n");
		return false;
	}

	// ----------------------------------------------------------------------
	// TEST 2: same center, spread, and class; should return true
	// ----------------------------------------------------------------------
	
	// make sure ID numbers are different (otherwise, the equality check
	// will always return true)
	r1.setID(1);
	r2.setID(2);

	// make sure classes are the same
	r1.setClass(HIGH_POS);
	r2.setClass(HIGH_POS);

	// variables for the center and spread of the interval
	// covered by a particular attribute
	double center = 0;
	double spread = 0;

	// iterate over the conditions of each rule
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++) {
		
		// add a random attribute to the condition
		r1.condition_.push_back(Attribute::random());
		r2.condition_.push_back(Attribute::random());

		// make sure its "don't care" variable is set to false
		// (when an attribute is generated randomly as above,
		// this value is also selected randomly)
		r1.condition_[i].setDontCare(false);
		r2.condition_[i].setDontCare(false);

		// get a random center and spread
		center = real_dist(rng);
		spread = real_dist(rng);

		// assign the same center and spread to the
		// current attribute in both rules
		r1.condition_[i].setCenter(center);
		r1.condition_[i].setSpread(spread);
		r2.condition_[i].setCenter(center);
		r2.condition_[i].setSpread(spread);
	}

	// check equality
	printf("Rule Equality Test 2 (same center, spread, class): ");
	if (r1 == r2) {
		printf("Passed.\n");
	} else {
		printf("Failed.\n");
		return false;
	}
	
	// ----------------------------------------------------------------------
	// TEST 3: same centers and class, different spreads; should return false
	// ----------------------------------------------------------------------
	
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++) {

		// change the spread values for the 
		// attributes in the condition of rule 2
		r2.condition_[i].setSpread(real_dist(rng));
	}

	// check equality
	printf("Rule Equality Test 3 (same center and class, different spreads): ");
	if (r1 == r2) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}
	
	// ----------------------------------------------------------------------
	// TEST 4: same spreads and class, different centers; should return false
	// ----------------------------------------------------------------------
	
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++) {

		// reset the spread of the attributes in rule 2, which was
		// altered for the previous test...
		r2.condition_[i].setSpread(spread);

		// ...and change the centers
		r2.condition_[i].setCenter(real_dist(rng));
	}

	// check equality
	printf("Rule Equality Test 4 (same spread and class, different centers): ");
	if (r1 == r2) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}
	
	// ----------------------------------------------------------------------
	// TEST 5: same center and spread, different classes; should return false
	// ----------------------------------------------------------------------
	
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++) {

		// reset the center of the attributes in rule 2,
		// which was altered for the previous test
		r2.condition_[i].setCenter(center);
	}

	// give the rules different classes
	do {
		r2.setClass(rng() % NUM_CLASSES);

	} while (r1.classification() == r2.classification());

	// check equality
	printf("Rule Equality Test 5 (same center and spread, different classes): ");
	if (r1 == r2) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}

	// ---------------------------------------------------------------------
	// TEST 6: rule 1 generalizes rule 2; should return false
	// ---------------------------------------------------------------------

	// make the classes equivalent again
	r1.setClass(r2.classification());

	// make one of the attributes in rule 1 a "don't care"
	r1.condition_[rng() % NUM_TEST_ATTRIBUTES].setDontCare(true);

	// check equality
	printf("Rule Equality Test 6 (one rule generalizes the other) ");
	if (r1 == r2) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}

	// all tests passed
	return true;

} // end testEquality

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
bool testSpecify() {

	// create random input and range vectors
	vector<double> input;
	vector<pair<double,double> > ranges;
	pair<double,double> range = make_pair(0,1);
	double a;

	// create some random attribute values for the input
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++) {
		a = real_dist(rng);
		input.push_back(a);
		ranges.push_back(range);
	}

	// random range scalar
	double range_scalar = real_dist(rng);

	// generate a random rule
	Rule r = Rule::random(NUM_TEST_ATTRIBUTES);
	Rule original_r = r;

	// run specify
	Rule new_r = r.specify(input, ranges, range_scalar);

	printf("Specify Test 1: ");
	// make sure that all of the don't care attributes in the original
	// rule have been specified
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++) {
		if (new_r.condition_[i].dont_care()) {
			printf("Failed.\n");
			return false;
		}
	}
	printf("Passed.\n");
	return true;

} // end testSpecify

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
bool testGeneralizes() {

	// generate a random input vector, range vector, and range scalar
	double range_scalar = real_dist(rng);
	vector<pair<double,double> > ranges;
	pair<double,double> range = make_pair(0,1);
	vector<double> input;
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++) {
		input.push_back(real_dist(rng));
		ranges.push_back(range);
	}

	// generate a random rule for r1, then specify
	// r1 and assign the resulting rule to r2
	Rule r1 = Rule::random(NUM_TEST_ATTRIBUTES);
	r1.setID(0);
	Rule temp = r1;
	Rule r2 = r1.specify(input, ranges, range_scalar); 
	r2.setID(1);
	r1 = temp;
	r1.setClass(r2.classification());

	// ----------------------------------------------------------------------
	// TEST 1: one rule is a specified version of another; should return true
	// ----------------------------------------------------------------------
	
	// check equality
	printf("Generalization Test 1 (one rule is a specified version of"
			" the other): ");
	if (r1.generalizes(r2)) {
		printf("Passed.\n");
	} else {
		printf("Failed.\n");
		return false;
	}

	// ----------------------------------------------------------------------
	// TEST 2: two completely different rules; should return false
	// ----------------------------------------------------------------------
	
	// change r2
	r2 = Rule::random(NUM_TEST_ATTRIBUTES);

	// check equality
	printf("Generalization Test 2 (two completely different rules): ");
	if (r1.generalizes(r2)) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}

	// ----------------------------------------------------------------------
	// TEST 3: identical rules; should return false
	// ----------------------------------------------------------------------

	// set rules equal to each other
	r1 = r2;

	// check equality
	printf("Generalization Test 3 (identical rules): ");
	if (r1.generalizes(r2)) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}

	// ----------------------------------------------------------------------
	// TEST 4: first rule is all "don't cares"; should return true
	// ----------------------------------------------------------------------
	
	// make sure ID numbers are different
	r2.setID(1);

	// set all of rule 1's attributes to "don't care"
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++)
		r1.condition_[i].setDontCare(true);

	// make sure the rules have the same class
	r1.setClass(r2.classification());

	// check equality
	printf("Generalization Test 4 (first rule is all don't cares): ");
	if (r1.generalizes(r2)) {
		printf("Passed.\n");
	} else {
		printf("Failed.\n");
		return false;
	}

	// ----------------------------------------------------------------------
	// TEST 5: empty rules; should return false
	// ----------------------------------------------------------------------
	
	// reinitialize rules
	r1 = Rule();
	r2 = Rule();

	// check equality
	printf("Generalization Test 5 (empty rules): ");
	if (r1.generalizes(r2)) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}

	return true;

} // end testGeneralizes

/****************************************************************************
 * Input:
 * Output:
 * Description:
 ****************************************************************************/ 
bool testMatches() {

	// a variable for the results of the tests
	bool is_a_match = false;

	// the data instance
	vector<double> data_instance = Dataset::randomDataPoint(NUM_TEST_ATTRIBUTES);

	// -----------------------------------------------------------------------
	// TEST 1: Rule's condition covers the data instance
	// -----------------------------------------------------------------------

	// generate the rule by specifying attribute values based
	// on the data point
	Rule r = Rule::random(NUM_TEST_ATTRIBUTES);
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++)
		r.condition_[i].setCenter(data_instance[i]);

	// evaluate
	printf("Matches Test 1 (should return true): ");
	is_a_match = r.matches(data_instance);
	if (is_a_match) {
		printf("Passed.\n");
	} else {
		return false;
		printf("Failed.\n");
	}

	// -----------------------------------------------------------------------
	// TEST 2: Rule's condition does not cover the data instance
	// -----------------------------------------------------------------------

	// here, we are guaranteeing that the value of the first attribute
	// in the data instance is *not* covered by the rule
	double first_att_spread = r.condition_[0].spread();
	r.condition_[0].setCenter(data_instance[0] - first_att_spread - 0.01);
	r.condition_[0].setDontCare(false);

	// evaluate
	printf("Matches Test 2 (should return false): ");
	is_a_match = r.matches(data_instance);
	if (is_a_match) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}

	return true;

} // end testMatches

/****************************************************************************
 * Input:
 * Output:
 * Description:
 ****************************************************************************/ 
void testMutate() {

	// generate a random rule
	Rule r = Rule::random(NUM_TEST_ATTRIBUTES);

	// create a vector of ranges
	vector<pair<double,double> > ranges;
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++)
		ranges.push_back(make_pair(0,1));
	
	// original rule
	printf("\nOriginal Rule:\n");
	r.print();

	// first mutation
	r.mutate(P_MUTATE, P_DONT_CARE, ranges, RANGE_SCALAR);
	
	printf("\nFirst Mutation:\n");
	r.print();

	// second mutation
	r.mutate(P_MUTATE, P_DONT_CARE, ranges, RANGE_SCALAR);
	
	printf("\nFirst Mutation:\n");
	r.print();

} // end testMutate
