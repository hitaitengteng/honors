#include "../lcs.h"

/****************************************************************************
 * File:        lcsTest.cpp
 * Author:      Will Gantt
 * Description: Implements and runs various unit tests for functions of the
 * 		LCS class.
 ****************************************************************************/ 

using namespace std;

bool testDoGA(LCS lcs);
bool testDoCover(LCS lcs);
bool testDoSpecify(LCS lcs);
bool testCover(LCS lcs);
void testRouletteWheelSelect(LCS lcs);
bool testGaSubsume(LCS lcs);
bool testCreateMatchAndCorrectSets(LCS lcs);
bool testApplyGA(LCS lcs);
bool testProcessInput(LCS lcs);

// INITIALIZE GLOBALS HERE

// named constants (these should ultimately 
// be passed as parameters)
static const int MAX_POP_SIZE = 50;
static const int NUM_ATTRIBUTES = 10;
static const int NUM_DATA_POINTS = 10;

// random number generator and seed generator
mt19937 rng;
random_device rd;

// uniform distributions
uniform_real_distribution<double> real_dist(0,1);
uniform_int_distribution<int> int_dist(0,1);
uniform_int_distribution<int> int_dist2(1,25);

int main(int argc, char **argv) {

	// create a random population and data set and
	// generate the LCS
	Population p = Population::random(MAX_POP_SIZE);
	Dataset d = Dataset::random(NUM_DATA_POINTS);
	LCS lcs = LCS(p,d);
	
	// run the tests

	return 0;
}

bool testDoGA(LCS lcs) {

	return true;

} // end testDoGA

/****************************************************************************
 * Inputs:      the population on which the roulette wheel selection operator
 * 		is to be tested
 * Outputs:     a boolean indicating whether all of the tests of the
 * 		roulette wheel selection on the given population were passed
 * Description: runs various tests of the roulette wheel selection operator
 ****************************************************************************/ 
void testRouletteWheelSelect(LCS lcs) {

	// print info on the fitnesses of all the rules
	// in the population
	printf("Rule         Fitness\n");
	printf("----         -------\n");
	for (int i=0; i<MAX_POP_SIZE; i++)
		printf("%d           %.3f\n", i, lcs.pop_.rules_[i].fitness());

	// select three rules randomly from the population
	int r1_index = lcs.rouletteWheelSelect();
	int r2_index = lcs.rouletteWheelSelect();
	int r3_index = lcs.rouletteWheelSelect();

	// get their fitnesses
	double r1_fitness = lcs.pop_.rules_[r1_index].fitness();
	double r2_fitness = lcs.pop_.rules_[r2_index].fitness();
	double r3_fitness = lcs.pop_.rules_[r3_index].fitness();

	// the fitness rank of each of the three rules
	int r1_fitness_rank = MAX_POP_SIZE;
	int r2_fitness_rank = MAX_POP_SIZE;
	int r3_fitness_rank = MAX_POP_SIZE;

	// a rule iterator variable and the rule's fitness
	Rule curr_rule;
	double curr_avg_niche_size;

	// iterate over the population
	for (int i=0; i<MAX_POP_SIZE; i++) {
		curr_rule = lcs.pop_.rules_[i];
		curr_avg_niche_size = curr_rule.fitness();

	// the fitness rank of each of the three rules
	// above is decremented every time we encounter
	// a rule in the population whose fitness is less
	// than that of the selected rule (in other words,
	// the higher a rule's fitness, the smaller the
	// value of its fitness rank)
	if (curr_avg_niche_size < r1_fitness)
		r1_fitness_rank--;
	if (curr_avg_niche_size < r2_fitness)
		r2_fitness_rank--;
	if (curr_avg_niche_size < r3_fitness)
		r3_fitness_rank--;
	}

	// Tell user which rules were selected and their fitness ranks. The user
	// should not often expect to see rules with fitness ranks close to
	// MAX_POP_SIZE, and should expect to frequently see rules with fitness
	// ranks closer to 1.
	printf("Selected rule %d with fitness rank %d\n",r1_index, r1_fitness_rank);
	printf("Selected rule %d with fitness rank %d\n",r2_index, r2_fitness_rank);
	printf("Selected rule %d with fitness rank %d\n",r3_index, r3_fitness_rank);

} // end testRouletteWheelSelect

