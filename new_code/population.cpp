#include "population.h"

/****************************************************************************
 * File:        population.cpp
 * Author:      William Gantt
 * Description: Method implementations for the Population class
 *
 * TODO:
 * 	- deletionSelect needs to be rewritten
 * 	- random needs to be rewritten
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
	int curr_index;
	for (int i=0; i<num_rules; i++) {
		random -= rules_[i].fitness2();
		if (random <= 0) {
			return i;
		}
	}

	// control should never reach here
	return rules_[num_rules - 1];

} // end rouletteWheelSelect

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
 * Inputs:      None.
 * Outputs:     The index in [P] of the rule to be deleted.
 * Description: Randomly selects a rule in [P] for deletion. A rule's
 * 		likelihood of being selected is directly proportional to
 * 		its average niche size.
 *
 * NOTE: If this function is giving you trouble, the first thing you should
 * check is whether theta_acc is set to an appropriate value.
 *
 * TODO:
 * 	Horribly inefficient---Fix. Also: the metrics used to
 * 	determine which rule is to be deleted should be more
 * 	sophisticated.
 ****************************************************************************/ 
/* int Population::deletionSelect(double theta_acc) {

	// sum the average niche sizes of all the rules
	double avg_niche_size_sum = 0;

	// the maximum avg niche size across all rules in the population
	double max_avg_niche_size = 0;

	// the index of the rule with the maximum average niche size
	int max_index = 0;

	// ensures that the do-while loop doesn't get stuck
	int while_counter = 0;

	// calculate the sum of the average niche sizes of all the rules
	// and find the maximum across all rules
	int pop_size = rules_.size();
	for (int i=0; i<pop_size; i++) {
		avg_niche_size_sum += rules_[i].avg_niche_size();
		if (rules_[i].avg_niche_size() > max_avg_niche_size) {
			max_avg_niche_size = rules_[i].avg_niche_size();
			max_index = i;
		}
	}

	// select a random rule for deletion. A rule may not be deleted if its
	// fitness exceeds a threshold value theta_acc, hence the do-while loop
	double random;
	int to_delete;
	do {
		// generate a random value in the range [0,avg_niche_size_sum]
		random = real_dist(rng) * avg_niche_size_sum;

		// using the random value generated above, this loop selects a rule
		// for deletion from the population. A rule's chance of being
		// selected is directly proportional to its average niche size.
		for (int i=0; i<pop_size; i++) {
			random -= rules_[i].avg_niche_size();
			if (random <= 0) {
				to_delete = i;
				break;
			}
			if (while_counter > 5) {
				return max_index;
			}
		}
		while_counter++;
	} while ((rules_[to_delete].accuracy() > theta_acc));

	return to_delete;

} // end deletionSelect
*/

/****************************************************************************
 * Input:       pop_size: the number of rules to be generated.
 * 		attributes_per_rule: the number of attributes in a rule
 * Output:      A randomly generated population.
 * Description: Generates a random population of rules.
 ****************************************************************************/ 
Population Population::random(int pop_size, int attributes_per_rule) {

	return NULL;

} // end random
