#include "../rule.h"

/****************************************************************************
 * File:        ruleTest.cpp
 * Author:      William Gantt
 * Description: Implements and runs unit tests for methods of the Rule class.
 *
 * TODO:
 *
 * 	- Implement testMutate(), and testSpecify()
 * 	- Add boolean arguments to let user run specific tests
 * 	- remove rng argument from testEquality
 ****************************************************************************/ 
using namespace std;

bool testEquality(mt19937 &rng);
bool testGeneralizes();
bool testMutate();
bool testSpecify();

static const int NUM_ATTRIBUTES = 10;

int main(int argc, char **argv) {

	mt19937 rng;
	testEquality(rng);

	return 0;
}

/****************************************************************************
 * Inputs:      a Mersenne Twister random number generator
 * Outputs:     a boolean indicating whether all tests have passed
 * Description: a unit test for the Rule class equality operator ('==')
 ****************************************************************************/ 
bool testEquality(mt19937 &rng) {

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
	
	// make sure classes are the same
	r1.setClass(HIGH_POS);
	r2.setClass(HIGH_POS);

	// for selecting random doubles on the interval [0,1]
	uniform_real_distribution<double> dist(0,1);

	// variables for the center and spread of the interval
	// covered by a particular attribute
	double center = 0;
	double spread = 0;

	// iterate over the conditions of each rule
	for (int i=0; i<NUM_ATTRIBUTES; i++) {
		
		// add a random attribute to the condition
		r1.condition.push_back(Attribute::getRandom());
		r2.condition.push_back(Attribute::getRandom());

		// make sure its "don't care" variable is set to false
		// (when an attribute is generated randomly as above,
		// this value is also selected randomly)
		r1.condition[i].setDontCare(false);
		r2.condition[i].setDontCare(false);

		// get a random center and spread
		center = dist(rng);
		spread = dist(rng);

		// assign the same center and spread to the
		// current attribute in both rules
		r1.condition[i].setCenter(center);
		r1.condition[i].setSpread(spread);
		r2.condition[i].setCenter(center);
		r2.condition[i].setSpread(spread);
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
	
	for (int i=0; i<NUM_ATTRIBUTES; i++) {

		// change the spread values for the 
		// attributes in the condition of rule 2
		r2.condition[i].setSpread(dist(rng));
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
	
	for (int i=0; i<NUM_ATTRIBUTES; i++) {

		// reset the spread of the attributes in rule 2, which was
		// altered for the previous test...
		r2.condition[i].setSpread(spread);

		// ...and change the centers
		r2.condition[i].setCenter(dist(rng));
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
	
	for (int i=0; i<NUM_ATTRIBUTES; i++) {

		// reset the center of the attributes in rule 2,
		// which was altered for the previous test
		r2.condition[i].setCenter(center);
	}

	// give the rules different classes
	do {
		r2.setClass(rng() % NUM_CLASSES);

	} while (r1.getClass() == r2.getClass());

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
	r1.setClass(r2.getClass());

	// make one of the attributes in rule 1 a "don't care"
	r1.condition[rng() % NUM_ATTRIBUTES].setDontCare(true);

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

	// random number generator
	mt19937 rng;
	rng.seed(random_device{}());

	// uniform distribution over [0,1]
	uniform_real_distribution<double> dist(0,1);

	// create a random input vector
	printf("    Input\n");
	printf("-------------\n");

	for (int i=0; i<NUM_ATTRIBUTES; i++)
		printf("[ %d ] ", i);
	printf("\n");

	vector<double> input;
	vector<pair<double,double> > ranges;
	pair<double,double> range = make_pair(0,1);
	double a;
	for (int i=0; i<NUM_ATTRIBUTES; i++) {
		a = dist(rng);
		printf("%.3f ", a);
		input.push_back(a);
		ranges.push_back(range);
	}

	// random range scalar
	double range_scalar = dist(rng);

	// generate a random rule
	Rule r = Rule::getRandom(NUM_ATTRIBUTES);

	printf("\n\nOriginal Rule\n");
	printf("-------------");
	r.print();

	// run specify
	r.specify(input, ranges, range_scalar, rng);

	printf("\n  New Rule\n");
	printf("-------------");
	r.print();

	return true;

} // end testSpecify

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
bool testGeneralizes() {

	// random number generator
	mt19937 rng;
	rng.seed(random_device{}());

	// uniform distribution over [0,1]
	uniform_real_distribution<double> dist(0,1);

	// generate a random input vector, range vector, and range scalar
	double range_scalar = dist(rng);
	vector<pair<double,double> > ranges;
	pair<double,double> range = make_pair(0,1);
	vector<double> input;
	for (int i=0; i<NUM_ATTRIBUTES; i++) {
		input.push_back(dist(rng));
		ranges.push_back(range);
	}

	// generate a random rule for r1, then specify
	// r1 and assign the resulting rule to r2
	Rule r1 = Rule::getRandom(NUM_ATTRIBUTES);
	Rule temp = r1;
	Rule r2 = r1.specify(input, ranges, range_scalar, rng); 
	r1 = temp;
	r1.print();
	r2.print();

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
	r2 = Rule::getRandom(NUM_ATTRIBUTES);

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
	
	// set all of rule 1's attributes to "don't care"
	for (int i=0; i<NUM_ATTRIBUTES; i++)
		r1.condition[i].setDontCare(true);

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
}
