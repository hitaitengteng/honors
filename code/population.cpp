#include "population.h"

/****************************************************************************
 * File:        population.cpp
 * Author:      William Gantt
 * Description: Method implementations for the Population class
 *
 * TODO:
 * 	- method descriptions
 * 	- complete subsume implementation
 * 	- unit test all functions
 ****************************************************************************/ 

using namespace std;

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
pair<Rule,Rule> Population::crossover(int i, int j, mt19937 &rng) {
	
	// get the parents
	Rule p1 = rules[i];
	Rule p2 = rules[j];

	// select a point in [1,condition.size()] for 1-pt crossover
	int crossPoint = (rng() % p1.condition.size()) + 1;

	// create the offspring
	Rule off1;
	Rule off2;

	// copy parent conditions into offspring
	for (int i=0; i<crossPoint; i++) {
		off1.condition.push_back(p1.condition[i]);
		off2.condition.push_back(p2.condition[i]);
	}
	for (int i=crossPoint; i<p1.condition.size(); i++) {
		off1.condition.push_back(p2.condition[i]);
		off2.condition.push_back(p1.condition[i]);
	}

	// copy classes
	off1.setClass(p1.getClass());
	off2.setClass(p2.getClass());

	// return the pair
	return make_pair(off1,off2);

} // end crossover

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
int Population::rouletteWheelSelect(mt19937 &rng) {

	// sum the individual fitnesses
	double sumFitnesses = 0;
	for (int i=0; i<rules.size(); i++) {
		sumFitnesses += rules[i].getFitness();
	}

	// select a random number in the range [0,sumFitnesses]
	uniform_real_distribution<double> dist(0,1);
	double random = dist(rng) * sumFitnesses;

	// we determine the individual selected by subtracting
	// the individual fitnesses from the random value generated
	// above until that value falls to or below 0. In this way,
	// an individual's likelihood of being selected is directly
	// proportional to its fitness. 
	for (int i=0; i<rules.size(); i++) {
		random -= rules[i].getFitness();
		if (random <= 0)
			return i;
	}

	// if there are somehow rounding errors
	// we still return the last rule in the population
	return rules.size() - 1;

} // end rouletteWheelSelect

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
void Population::subsume() { // [INCOMPLETE]

	// a vector to store the indices of any superfluous rules
	vector<int> toDelete;

	// for each rule in the population, check whether the most general
	// rule generalizes it
	for (int i=0; i<rules.size(); i++) {
		// make sure we're not checking the most general rule itself
			if (mostGeneral.generalizes(rules[i]))
				toDelete.push_back(i);
	}

	// delete all the superfluous rules
	for (int i=0; i<toDelete.size(); i++) {
		rules.erase(rules.begin() + toDelete[i]);
	}

} // end subsume
