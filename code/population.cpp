#include "population.h"

/****************************************************************************
 * File:        population.cpp
 * Author:      William Gantt
 * Description: Method implementations for the Population class
 *
 * TODO:
 * 	- method descriptions
 * 	- unit test all functions
 * 	- remove rng from arguments
 * 	- implement matchExists function
 ****************************************************************************/ 

using namespace std;

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
void Population::add(Rule r) {

	// do not add the rule if the population limit has been reached
	if (rules.size() == maxSize) {
		cout << "Unable to add rule: population limit reached." << endl;
		return;
	}

	// give the rule an ID number
	r.setID(id_count);
	id_count++;

	// if this is either the first rule added to the population,
	// or else is more general than the current most general
	// rule, update 'mostGeneral'
	if (rules.empty() || (r.getNumDontCare() > mostGeneral.getNumDontCare())) {
		mostGeneral = r;
	} else if ((r.getNumDontCare() == mostGeneral.getNumDontCare()) &&
			(r.generalizes(mostGeneral))) {
		mostGeneral = r;
	}

	// add the rule to the vector of all rules
	rules.push_back(r);

	// increment the fitness sum of the population
	fitnessSum += r.getFitness();

} // end add

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
	for (size_t i=crossPoint; i<p1.condition.size(); i++) {
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

	// select a random number in the range [0,fitnessSum]
	uniform_real_distribution<double> dist(0,1);
	double random = dist(rng) * fitnessSum;

	// we determine the individual selected by subtracting
	// the individual fitnesses from the random value generated
	// above until that value falls to or below 0. In this way,
	// an individual's likelihood of being selected is directly
	// proportional to its fitness. 
	int num_rules = rules.size();
	for (size_t i=0; i<num_rules; i++) {
		random -= rules[i].getFitness();
		if (random <= 0)
			return i;
	}

	// if there are somehow rounding errors
	// we still return the last rule in the population
	return rules.size() - 1;

} // end rouletteWheelSelect

/****************************************************************************
 * Inputs:      none
 * Outputs:     the number of rules subsumed
 * Description: 
 ****************************************************************************/ 
int Population::subsume() {

	// the number of rules subsumed
	int num_subsumed = 0;

	// for each rule in the population, check whether the most general
	// rule generalizes it; if so, remove it from the vector of rules
	for (auto it = rules.begin(); it != rules.end(); ) {
		if (mostGeneral.generalizes(*it)) {
			rules.erase(it);
			++num_subsumed;
		} else {
			it++;
		}
	}

	return num_subsumed;

} // end subsume

/****************************************************************************
 * Inputs:      input: the input to be checked for a match.
 * Outputs:     A boolean indicating whether one of the rules in the
 * 		population matches the input.
 * Description: See above.
 ****************************************************************************/ 
bool Population::matchExists(vector<double> &input) const {

	int num_rules = rules.size();
	for (size_t i=0; i<num_rules; i++) {
		if (rules[i].matches(input))
			return true;
	}
		
	return false;

} // end matchExists
