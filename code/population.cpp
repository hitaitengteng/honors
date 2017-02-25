#include "population.h"

/****************************************************************************
 * File:        population.cpp
 * Author:      William Gantt
 * Description: Method implementations for the Population class
 *
 * TODO:
 * 	- method descriptions
 * 	- unit test all functions
 * 	- resolve problems with most_general
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
 * Inputs:      index: the index in the general population of the rule to
 * 		be removed.
 * Outputs:     None.
 * Description: Removes a rule from the population.
 *
 * TODO: take care of the case where the rule to be removed is the most
 * 	 general rule
 ****************************************************************************/ 
void Population::remove(int index) {

	// if the population is empty, return an error
	if (rules_.size() == 0) {
		printf("Population is empty; rule could not be deleted\n");
		return;
	}

	// decrement the fitness and experience sums of the population
	fitness_sum_ -= rules_[index].fitness();
	exp_sum_ -= rules_[index].exp();

	// Delete the rule. Note that we do not have to delete it from
	// the match or correct sets because those are reset at every
	// iteration anyway. 
	rules_.erase(rules_.begin() + index);

} // end remove

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

	// copy parent conditions into offspring (up to break point)
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

	// copy parent conditions into offspring (after break point)
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
int Population::deletionSelect(double theta_acc) {

	// sum the average niche sizes of all the rules
	double avg_niche_size_sum = 0;
	int pop_size = rules_.size();
	for (int i=0; i<pop_size; i++) {
		avg_niche_size_sum += rules_[i].avg_niche_size();
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
		}
	} while (rules_[to_delete].accuracy() > theta_acc);

	return to_delete;

} // end deletionSelect

/****************************************************************************
 * Inputs:      None.
 * Outputs:     The number of rules subsumed
 * Description: Checks the most general rule against every other rule in [P].
 * 		If the most general rule generalizes another rule R, R is
 * 		deleted and the numerosity of the most general rule is
 * 		increased.
 *
 * TODO:
 *
 * 	- This needs to be updated to accommodate the addition of matchSet
 * 	  and correctSet classes (should be implemented in either
 * 	  correctSet.cpp or lcs.cpp)
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
 * Input:       pop_size: the number of rules to be generated.
 * 		attributes_per_rule: the number of attributes in a rule
 * Output:      A randomly generated population.
 * Description: Generates a random population of rules.
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
