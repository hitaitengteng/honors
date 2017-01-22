#include "../population.h"

/****************************************************************************
 * File:        populationTest.cpp
 * Author:      William Gantt
 * Description: Implements and runs unit tests for methods of the Population
 * 		class.
 *
 * TODO:
 *
 * 	- Tell user what the crossover point is in testCrossover and
 * 	  testSubsume.
 * 	- Fix test subsume
 ****************************************************************************/ 
using namespace std;

bool testCrossover(Population *p);
bool testRouletteWheelSelect(Population *p);
bool testSubsume(Population *p);

// named constants
static const int MAX_POP_SIZE = 50;
static const int NUM_ATTRIBUTES = 10;

// random number generator
mt19937 rng;

int main(int argc, char **argv) {

	// seed the random number generator
	rng.seed(random_device{}());

	// create a population of randomly generated rules
	Rule r;
	Population *p = new Population(MAX_POP_SIZE);
	for (int i=0; i<MAX_POP_SIZE; i++) {
		r = Rule::getRandom(NUM_ATTRIBUTES);
		p->add(r);
	}

	testSubsume(p);
	delete p;
	exit(0);
	return 0;
}

bool testCrossover(Population *p) {

	// select parents randomly from the population
	int p1_index = rng() % MAX_POP_SIZE;
	int p2_index = rng() % MAX_POP_SIZE;

	// print info on parents
	printf("\nParent 1\n--------");
	p->rules[p1_index].print();
	printf("\nParent 2\n--------");
	p->rules[p2_index].print();

	// the offspring
	pair<Rule,Rule> offspring;

	// cross them
	offspring = p->crossover(p1_index, p2_index, rng);
	Rule off1 = offspring.first;
	Rule off2 = offspring.second;

	// print info on offspring
	printf("\nOffspring 1\n-----------");
	off1.print();
	printf("\nOffspring 2\n-----------");
	off2.print();

	return true;
}

bool testRouletteWheelSelect(Population *p) {

	// print info on the fitnesses of all the rules
	// in the population
	printf("Rule         Fitness\n");
	printf("----         -------\n");
	for (int i=0; i<MAX_POP_SIZE; i++)
		printf("%d           %.3f\n", i, p->rules[i].getFitness());

	// select three rules randomly from the population
	int r1_index = p->rouletteWheelSelect(rng);
	int r2_index = p->rouletteWheelSelect(rng);
	int r3_index = p->rouletteWheelSelect(rng);

	// get their fitnesses
	double r1_fitness = p->rules[r1_index].getFitness();
	double r2_fitness = p->rules[r2_index].getFitness();
	double r3_fitness = p->rules[r3_index].getFitness();

	// the fitness rank of each of the three rules
	int r1FitnessRank = MAX_POP_SIZE;
	int r2FitnessRank = MAX_POP_SIZE;
	int r3FitnessRank = MAX_POP_SIZE;

	// a rule iterator variable and the rule's fitness
	Rule curr_rule;
	double curr_fitness;

	// iterate over the population
	for (int i=0; i<MAX_POP_SIZE; i++) {
		curr_rule = p->rules[i];
		curr_fitness = curr_rule.getFitness();

	// the fitness rank of each of the three rules
	// above is decremented every time we encounter
	// a rule in the population whose fitness is less
	// than that of the selected rule (in other words,
	// the higher a rule's fitness, the smaller the
	// value of its fitness rank)
	if (curr_fitness < r1_fitness)
		r1FitnessRank--;
	if (curr_fitness < r2_fitness)
		r2FitnessRank--;
	if (curr_fitness < r3_fitness)
		r3FitnessRank--;
	}

	// Tell user which rules were selected and their fitness ranks. The user
	// should not often expect to see rules with fitness ranks close to
	// MAX_POP_SIZE, and should expect to frequently see rules with fitness
	// ranks closer to 1.
	printf("Selected rule %d with fitness rank %d\n",r1_index, r1FitnessRank);
	printf("Selected rule %d with fitness rank %d\n",r2_index, r2FitnessRank);
	printf("Selected rule %d with fitness rank %d\n",r3_index, r3FitnessRank);

	return true;

} // end testRouletteWheelSelect

bool testSubsume(Population *p) {

	// the number of rules subsumed by the most general rule
	int num_subsumed = 0;

	// a pointer to the most general rule in the population
	Rule *most_general = p->getMostGeneral();

	// -----------------------------------------------------------------
	// TEST 1: no rules should be subsumed
	// -----------------------------------------------------------------
	
	// by setting dontCare to false and the spread to 0, we guarantee
	// that mostGeneral cannot generalize any other rule in the population
	for (int i=0; i<NUM_ATTRIBUTES; i++) {
		most_general->condition[i].setDontCare(false);
		most_general->condition[i].setSpread(0);
	}

	(*most_general).print();
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
		most_general->condition[i].setDontCare(true);

	(*most_general).print();
	// count how many rules were subsumed
	num_subsumed = p->subsume();

	// now compute the number of rules in the population that have
	// the same class as the most general rule (so there should be
	// at least one)
	
	// the number of rules in the population with the same class
	// as the most general rule
	int tally = 0;

	// variables for classes
	int most_general_class = most_general->getClass();
	int curr_class = NO_CLASS;

	// iterate over the rules and compare classes
	for (int i=0; i<p->rules.size(); i++) {
		curr_class = p->rules[i].getClass();
		if (most_general_class == curr_class)
			tally++;
	}

	cout << "tally: " << tally << endl;
	cout << "num_subsumed: " << num_subsumed << endl;
	printf("Subsumption Test 2 (most general rule has all"
			" \"don't care\" attributes): ");
	if (num_subsumed != tally) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}
	
	exit(0);
	// -----------------------------------------------------------------
	// TEST 3: at least one rule should be subsumed
	// -----------------------------------------------------------------
	
	// select a random rule from the population
	Rule r = p->rules[rng() % MAX_POP_SIZE];

	// the upper and lower bounds of an attribute
	double upperBound = 0;
	double lowerBound = 0;

	for (int i=0; i<NUM_ATTRIBUTES; i++) {
		upperBound = r.condition[i].getCenter() + r.condition[i].getSpread();
		lowerBound = r.condition[i].getCenter() - r.condition[i].getSpread();

		// generalize the rule
		if (r.condition[i].getDontCare() == false) {
			most_general->condition[i].setDontCare(false);
			most_general->condition[i].setCenter(r.condition[i].getCenter());
			most_general->condition[i].setSpread(r.condition[i].getSpread() + 0.1);
		}
	}

	// run subsume() and evaluate results
	num_subsumed = p->subsume();
	printf("Subsumption Test 3 (at least one rule should be subsumed): ");
	if (num_subsumed == 0) {
		printf("Failed.\n");
		return false;
	} else {
		printf("Passed.\n");
	}
	
	return true;

} // end testSubsume
