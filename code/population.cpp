#include "population.h"

/****************************************************************************
 * File:        population.cpp
 * Author:      William Gantt
 * Description: Method implementations for the Population class
 *
 * TODO:
 * 	- method descriptions
 * 	- unit test all functions
 ****************************************************************************/ 

using namespace std;

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
void Population::add(Rule r) {

	// do not add the rule if the population limit has been reached
	if (rules_.size() == max_size_) {
		cout << "Unable to add rule: population limit reached." << endl;
		return;
	}

	// give the rule an ID number (if it doesn't already have one)
	if (r.id() == NO_ID) {
		r.setID(id_count_);
		id_count_++;
	}

	// if this is either the first rule added to the population,
	// or else is more general than the current most general
	// rule, update 'most_general_'
	if (rules_.empty() || (r.num_dont_care() > most_general_.num_dont_care())) {
		most_general_ = r;
	} else if ((r.num_dont_care() == most_general_.num_dont_care()) &&
			(r.generalizes(most_general_))) {
		most_general_ = r;
	}

	// add the rule to the vector of all rules
	rules_.push_back(r);

	// increment the fitness and experience sums of the population
	fitness_sum_ += r.fitness();
	exp_sum_ += r.exp();

} // end add

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
pair<Rule,Rule> Population::crossover(int i, int j) {
	
	// get the parents
	Rule p1 = rules_[i];
	Rule p2 = rules_[j];

	// select a point in [1,condition.size()] for 1-pt crossover
	int cross_point = (rng() % p1.condition_.size()) + 1;

	// DELETE AFTER DEBUGGING
	printf("Crossover Point: %d\n", cross_point);

	// create the offspring
	Rule off1;
	Rule off2;

	// copy parent conditions into offspring
	for (int i=0; i<cross_point; i++) {

		// add the appropriate attribute values to the rules
		off1.condition_.push_back(p1.condition_[i]);
		off2.condition_.push_back(p2.condition_[i]);

		// update the number of "don't cares," if necessary
		if (p1.condition_[i].dont_care())
			off1.setNumDontCare(off1.num_dont_care() + 1);
		if (p2.condition_[i].dont_care())
			off2.setNumDontCare(off2.num_dont_care() + 1);
	}

	for (size_t i=cross_point; i<p1.condition_.size(); i++) {

		off1.condition_.push_back(p2.condition_[i]);
		off2.condition_.push_back(p1.condition_[i]);

		if (p1.condition_[i].dont_care())
			off2.setNumDontCare(off2.num_dont_care() + 1);
		if (p2.condition_[i].dont_care())
			off1.setNumDontCare(off1.num_dont_care() + 1);
	}

	// copy classes
	off1.setClass(p1.classification());
	off2.setClass(p2.classification());

	// return the pair
	return make_pair(off1, off2);

} // end crossover

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
int Population::rouletteWheelSelect() {

	// select a random number in the range [0,fitness_sum_]
	double random = real_dist(rng) * fitness_sum_;

	// we determine the individual selected by subtracting
	// the individual fitnesses from the random value generated
	// above until that value falls to or below 0. In this way,
	// an individual's likelihood of being selected is directly
	// proportional to its fitness. 
	int num_rules = rules_.size();
	for (size_t i=0; i<num_rules; i++) {
		random -= rules_[i].fitness();
		if (random <= 0) {
			return i;
		}
	}

	// if there are somehow rounding errors
	// we still return the last rule in the population
	return rules_.size() - 1;

} // end rouletteWheelSelect

/****************************************************************************
 * Inputs:      none
 * Outputs:     
 * Description: 
 ****************************************************************************/ 
int Population::subsumptionSelect() {

	// if there are somehow rounding errors,
	// we still return the last rule in the population
	return rules_.size() - 1;

} // end subsumptionSelect

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
	for (auto it = rules_.begin(); it != rules_.end(); ) {
		if (most_general_.generalizes(*it)) {
			rules_.erase(it);
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

	int num_rules = rules_.size();
	for (size_t i=0; i<num_rules; i++) {
		if (rules_[i].matches(input))
			return true;
	}
		
	return false;

} // end matchExists

/****************************************************************************
 * Input:
 * Output:
 * Description:
 ****************************************************************************/ 
Population Population::random(int pop_size, int attributes_per_rule) {

	// initialize a population with a maximum size of 'size'
	Population p(pop_size);

	// generate a random set of rules
	for (size_t i=0; i<pop_size; i++) {
		p.add(Rule::random(attributes_per_rule));

		// update the most general rule in the population
		// (if necessary)
		if (i==0) {
			p.most_general_ = p.rules_[i];
		} else if (p.rules_[i].generalizes(p.most_general_)) {
			p.most_general_ = p.rules_[i];
		}
	}

	return p;

} // end random
