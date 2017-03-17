#include "population.h"
#include "dataset.h"

/****************************************************************************
 * File:        population.cpp
 * Author:      William Gantt
 * Description: Method implementations for the Population class
 ****************************************************************************/ 

using namespace std;

/****************************************************************************
 * Inputs:      r: the rule to be added to the population
 * Outputs:     None.
 * Description: Adds a rule to the population.
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

	// add the rule to the vector of all rules
	rules_.push_back(r);

	// increment the fitness and experience sums of the population
	// NOTE: currently, no rules being added to the population will
	// have nonzero fitness.
	fitness1_sum_ += r.fitness1();
	fitness2_sum_ += r.fitness2();

} // end add

/****************************************************************************
 * Inputs:      index: the index in the general population of the rule to
 * 		be removed.
 * Outputs:     None.
 * Description: Removes a rule from the population.
 ****************************************************************************/ 
void Population::remove(int index) {

	// if the population is empty, return an error
	if (rules_.size() == 0) {
		printf("Population is empty; rule could not be deleted\n");
		return;
	}

	// decrement the fitness sum of the population
	fitness1_sum_ -= rules_[index].fitness1();
	fitness2_sum_ -= rules_[index].fitness2();

	// Delete the rule. Note that we do not have to delete it from
	// the match or correct sets because those are reset at every
	// iteration anyway. 
	rules_.erase(rules_.begin() + index);

} // end remove

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/
int Population::rouletteWheelSelect() {

	// select a random number in the range [0,correct_set_.fitness_sum()]
	double random = real_dist(rng) * fitness2_sum();

	/*
	 * we determine the individual selected by subtracting
	 * the individual fitnesses from the random value generated
	 * above until that value falls to or below 0. In this way,
	 * an individual's likelihood of being selected is directly
	 * proportional to its fitness
	 */
	int num_rules = rules_.size();
	for (int i=0; i<num_rules; i++) {
		random -= rules_[i].fitness2();
		if (random <= 0) {
			return i;
		}
	}

	// control should never reach here
	return (num_rules - 1);

} // end rouletteWheelSelect

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
 ****************************************************************************/ 
void Population::evaluateFitness1(int num_examples) {

	// reset the fitness1 sum
	fitness1_sum_ = 0;

	// get the number of rules
	int pop_size = size();

	// a variable to store the current example
	vector<double> curr_example;

	// iterate over all the rules
	for (int i=0; i<pop_size; i++) {

		// for each input, determine whether it is a true positive,
		// true negative, false positive, or false negative for the
		// current rule
		for (int j=0; j<num_examples; j++) {
			//
			// get the current example
			// 

			rules_[i].processInput(curr_example);	
		}

		// once the number of true and false positives and negatives
		// has been determined, we update the fitness of the rule,
		// and the fitness1 sum for the population
		rules_[i].updateFitness1();
		fitness1_sum_ += rules_[i].fitness1();
	}

} // end evaluateFitness1

/****************************************************************************
 * Inputs:      i,j: the indices in the general population of the rules
 * 		to be crossed.
 * Outputs:     A pair of offspring rules
 * Description: Creates two new rules via one-point crossover of the rules
 * 		specified by the values i and j.
 ****************************************************************************/ 
pair<Rule,Rule> Population::crossover(int i, int j) {
	
	// get the parents
	Rule p1 = rules_[i];
	Rule p2 = rules_[j];

	// select a point in [0,condition.size() - 1] for 1-pt crossover
	int cross_point = (rng() % p1.condition_.size());

	// create the offspring
	Rule off1;
	Rule off2;

	// copy parent conditions into offspring (up to crossover point)
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

	// copy parent conditions into offspring (after crossover point)
	for (int i=cross_point; i<p1.condition_.size(); i++) {

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
 * Input:       pop_size: the number of rules to be generated.
 * 		attributes_per_rule: the number of attributes in a rule
 * Output:      A randomly generated population.
 * Description: Generates a random population of rules.
 ****************************************************************************/ 
Population Population::random(int pop_size, int attributes_per_rule) {

	// initialize the population of rules
	Population p = Population(pop_size);

	// a rule variable for generating random rules
	Rule r;

	// generate an initial population of random rules
	for (int i=0; i<pop_size; i++) {

		// generate a random rule and add it to the population
		r = Rule::random(attributes_per_rule);
		r.setID(p.id_count_);
		p.add(r);
		p.id_count_++;
	}

	return p;

} // end random
