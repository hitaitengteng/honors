#include "../population.h"
#include "../dataset.h"

/****************************************************************************
 * File:        populationTest.cpp
 * Author:      William Gantt
 * Description: Implements and runs unit tests for methods of the Population
 * 		class.
 *
 * TODO:
 * 	- Fix the matchExists test.
 ****************************************************************************/ 
using namespace std;

// advance function declarations
bool testCrossover(Population *p);
bool testSubsume(Population *p);
bool testMatchExists(Population *p);
bool testClearAndEmpty(Population p);
void testDeletionSelect(Population *p);

// named constants (these should ultimately 
// be passed as parameters)
static const int MAX_POP_SIZE = 50;
static const int NUM_ATTRIBUTES = 10;

// random number generator and seed generator
mt19937 rng;
random_device rd;

// uniform distributions
uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(0,1);
uniform_int_distribution<int> int_dist2(1,25);

int main(int argc, char **argv) {

	rng.seed(rd());

	// create a population of randomly generated rules
	Rule r;
	Population *p = new Population(MAX_POP_SIZE);
	for (int i=0; i<MAX_POP_SIZE; i++) {
		r = Rule::random(NUM_ATTRIBUTES);
		p->add(r);
	}

	// test
	// testSubsume(p);
	testMatchExists(p);
	// testClearAndEmpty(*p);
	// testDeletionSelect(p);

	delete p;
	return 0;
}

/****************************************************************************
 * Inputs:      p: the population to be tested.
 * Outputs:     a boolean indicating whether all tests of the crossover
 * 		function on the given population were passed
 * Description: runs various tests of the crossover function
 ****************************************************************************/ 
bool testCrossover(Population *p) {

	// select parents randomly from the population
	int p1_index = rng() % MAX_POP_SIZE;
	int p2_index = rng() % MAX_POP_SIZE;

	// print info on parents
	printf("\nParent 1\n--------");
	p->rules_[p1_index].print();
	printf("\nParent 2\n--------");
	p->rules_[p2_index].print();

	// the offspring
	pair<Rule,Rule> offspring;

	// cross them
	offspring = p->crossover(p1_index, p2_index);
	Rule off1 = offspring.first;
	Rule off2 = offspring.second;

	// print info on offspring
	printf("\nOffspring 1\n-----------");
	off1.print();
	printf("\nOffspring 2\n-----------");
	off2.print();

	return true;
}

/****************************************************************************
 * Inputs:      the population on which the subsume operator is to be tested
 * Outputs:     a boolean indicating whether all tests of the subsume
 * 		function on the given population were passed
 * Description: runs various tests of the subsume function
 ****************************************************************************/ 
bool testSubsume(Population *p) {

	// the number of rules subsumed by the most general rule
	int num_subsumed = 0;

	// A pointer to the most general rule in the population.
	// Note that when we update most_general below, we are NOT updating
	// the rule object that is actually in the population, but rather
	// a copy of it. There are good reasons for this, which I present
	// elsewhere [give location]
	Rule most_general = p->most_general();

	// -----------------------------------------------------------------
	// TEST 1: no rules should be subsumed
	// -----------------------------------------------------------------
	
	// by setting dontCare to false and the spread to 0, we guarantee
	// that mostGeneral cannot generalize any other rule in the population
	for (int i=0; i<NUM_ATTRIBUTES; i++) {
		most_general.condition_[i].setDontCare(false);
		most_general.condition_[i].setSpread(0);
	}

	// reassign mostGeneral
	p->setMostGeneral(most_general);

	// run subsume() and evaluate results
	num_subsumed = p->subsume();
	printf("Subsumption Test 1 (most general does not"
			" generalize any rules): ");
	if (num_subsumed != 0) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}

	// -----------------------------------------------------------------
	// TEST 2: at least one rule should be subsumed
	// -----------------------------------------------------------------

	// set all of the most general rule's attributes to "don't care."
	// Once this has been done, the most general rule should subsume
	// all other rules that share its class.
	for (int i=0; i<NUM_ATTRIBUTES; i++) 
		most_general.condition_[i].setDontCare(true);

	// compute the number of rules in the population that have the
	// same class as the most general rule (so there should be at
	// least one)
	
	// the number of rules in the population with the same class
	// as the most general rule
	int tally = 0;

	// variables for classes
	int most_general_class = most_general.classification();
	int curr_class = NO_CLASS;

	// iterate over the rules and compare classes
	int pop_size = p->rules_.size();
	for (size_t i=0; i<pop_size; i++) {
		curr_class = p->rules_[i].classification();
		if (most_general_class == curr_class) {
			if (!(p->rules_[i] == most_general))
				tally++;
		}
	}

	// reassign mostGeneral
	p->setMostGeneral(most_general);

	// count how many rules were subsumed
	num_subsumed = p->subsume();

	printf("Subsumption Test 2 (most general rule has all"
			" \"don't care\" attributes): ");

	if (num_subsumed != tally) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}

	
	// -----------------------------------------------------------------
	// TEST 3: at least one rule should be subsumed
	// -----------------------------------------------------------------
	
	Rule r;
	do {
		// select a random rule from the population
		r = p->rules_[rng() % p->rules_.size()];

	} while (r == p->most_general()); // make sure it isn't the most
					    // general rule

	// the center and spread of the current
	// attribute of the randomly chosen rule
	double ri_center = 0;
	double ri_spread = 0;

	// iterate over all of the attributes
	for (int i=0; i<NUM_ATTRIBUTES; i++) {

		// get the center and spread of the current  
		// attribute for the randomly chosen rule
		ri_center = r.condition_[i].center(); 
		ri_spread = r.condition_[i].spread();

		// set the center and spread of the current attribute in
		// most_general so that it encompasses the range of
		// values for the current attribute in r
		if (r.condition_[i].dont_care() == false) {
			most_general.condition_[i].setDontCare(false);
			most_general.condition_[i].setCenter(ri_center);
			most_general.condition_[i].setSpread(ri_spread + 0.01);
		}
	}

	// make sure most_general has the same
	// class as the randomly chosene rule
	most_general.setClass(r.classification());

	// reassign most general
	p->setMostGeneral(most_general);

	// run subsume() and evaluate results
	num_subsumed = p->subsume();
	printf("Subsumption Test 3 (at least one rule should be subsumed): ");
	if (num_subsumed == 0) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}
	
	// if control reaches here, all tests have passed
	return true;

} // end testSubsume

/****************************************************************************
 * Inputs:      p: a pointer to the population to be tested.
 * Outputs:     A boolean indicating whether all tests have passed.
 * Description: Tests the matchExists function from the Population class.
 *
 * NOTE: This test is incorrect as it stands. Fix.
 ****************************************************************************/
bool testMatchExists(Population *p) {

	// generate a random data set
	Dataset d = Dataset::random(1);

	// the range scalar
	double range_scalar = 0.1;

	// get the the input
	vector<double> input = d.data_points_[0];

	// choose a random attribute
	int att = int_dist(rng) % (input.size() - 1);

	// for every rule in the population, specify the attribute selected
	// above with a value different from that of the input (to guarantee
	// that it won't match the input)
	int pop_size = p->rules_.size();
	for (int i=0; i<pop_size; i++) {
		p->rules_[i].condition_[att].setDontCare(false);
		p->rules_[i].condition_[att].setCenter(input[att] + 0.01);
		p->rules_[i].condition_[att].setSpread(0);
	}

	printf("MatchExists Test 1: ");
	// check for a match (there shouldn't be one)
	bool result1 = p->matchExists(input);
	if (result1) {
		printf("Failed. (Found a match when there wasn't one.)\n");
		return false;
	} else {
		printf("Passed.\n");
	}

	// select a random rule from the population to be specified
	// according to the input (so as to guarantee that ther *is*
	// a match)
	int index = int_dist(rng) % p->rules_.size();
	p->rules_[index].specify(input,d.attribute_ranges_,range_scalar);
	p->rules_[index].print();
	Dataset::printDataPoint(input, NUM_ATTRIBUTES);

	printf("MatchExists Test 2: ");

	// check for a match (there should be one)
	bool result2 = p->matchExists(input);
	if (!result2) {
		printf("Failed. (Did not find a match when there was one.)\n");
		return false;
	} else {
		printf("Passed.\n");
	}

	return true;
	
} // end testMatchExists

/****************************************************************************
 * Inputs:      p: the population to be tested.
 * Outputs:     A boolean indicating whether all tests have passed.
 * Description: Tests the clear and empty functions from the Population class.
 ****************************************************************************/
bool testClearAndEmpty(Population p) {

	printf("Clear Test 1: ");

	// clear the population
	p.clear();
	if (p.rules_.size() != 0) {
		printf("Failed (some rules still in population).\n");
		return false;
	} else if (p.empty() == false) {
		printf("Failed (empty function failed).\n");
	} else if (p.fitness_sum() != 0) {
		printf("Failed (fitness sum is non-zero).\n");
		return false;
	} else if (p.exp_sum() != 0) {
		printf("Failed (experience sum is non-zero).\n");
		return false;
	} else if (p.most_general().classification() != NO_CLASS) {
		printf("Failed (most general was not reset).\n");
		return false;
	}

	printf("Passed.\n");
	return true;

} // end testClear

/****************************************************************************
 * Inputs:      p: the population to be tested.
 * Outputs:     None.
 * Description: Tests the deletionSelect function from the Population class.
 ****************************************************************************/
void testDeletionSelect(Population *p) {

	// the accuracy value above which a rule may not be deleted
	double theta_acc = 1;

	// assign each rule in the population a random 
	// average niche size in the range [1,25]
	int pop_size = p->rules_.size();
	for (int i=0; i<pop_size; i++)
		p->rules_[i].setAvgNicheSize(int_dist2(rng));
	
	// print the avg niche size of every rule in the population
	printf("Rule         Avg Niche Size\n");
	printf("----         --------------\n");
	for (int i=0; i<pop_size; i++)
		printf("%d           %.3f\n", i, p->rules_[i].avg_niche_size());

	// select three rules randomly from the population
	int r1_index = p->deletionSelect(theta_acc);
	int r2_index = p->deletionSelect(theta_acc);
	int r3_index = p->deletionSelect(theta_acc);

	// get their average niche sizes
	double r1_avg_niche_size = p->rules_[r1_index].avg_niche_size();
	double r2_avg_niche_size = p->rules_[r2_index].avg_niche_size();
	double r3_avg_niche_size = p->rules_[r3_index].avg_niche_size();

	// the rank of the three rules with respect to average niche size
	int r1_rank = pop_size;
	int r2_rank = pop_size;
	int r3_rank = pop_size;

	// a rule iterator variable and the rule's average niche size
	Rule curr_rule;
	double curr_avg_niche_size;

	// iterate over the population
	for (int i=0; i<pop_size; i++) {
		curr_rule = p->rules_[i];
		curr_avg_niche_size = curr_rule.avg_niche_size();

	// the rank of each of the three rules above is increased (decremented)
	// every time we encounter a rule in the population whose
	// average niche size is greater than that of the selected rule
	if (curr_avg_niche_size < r1_avg_niche_size)
		r1_rank--;
	if (curr_avg_niche_size < r2_avg_niche_size)
		r2_rank--;
	if (curr_avg_niche_size < r3_avg_niche_size)
		r3_rank--;
	}

	// Tell user which rules were selected and their fitness ranks. The user
	// should not often expect to see rules with fitness ranks close to
	// pop_size, and should expect to frequently see rules with fitness
	// ranks closer to 1.
	printf("Selected rule %d with average niche size %.3f and rank %d\n", r1_index, r1_avg_niche_size, r1_rank);
	printf("Selected rule %d with average niche size %.3f and rank %d\n", r2_index, r2_avg_niche_size, r2_rank);
	printf("Selected rule %d with average niche size %.3f and rank %d\n", r3_index, r3_avg_niche_size, r3_rank);

} // end testDeletionSelect
