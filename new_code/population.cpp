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
 * Inputs:      None.
 * Outputs:     The index in the population of the selected rule.
 * Description: Selects a rule from the population using roulette wheel
 * 		selection---a fitness-based selection algorithm.
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
 * Inputs:      None.
 * Outputs:     None.
 * Description: Computes fitness1 for all rules in the population.
 ****************************************************************************/ 
void Population::evaluateFitness1() {

	// reset the fitness1 sum
	fitness1_sum_ = 0;

	// get the number of rules
	int pop_size = size();
	int num_examples = training_set_.data_points_.size();

	// a variable to store the current example
	vector<double> curr_example;

	// iterate over all the rules
	for (int i=0; i<pop_size; i++) {

		// for each input, determine whether it is a true positive,
		// true negative, false positive, or false negative for the
		// current rule
		for (int j=0; j<num_examples; j++) {
			curr_example = training_set_.data_points_[j];
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
 * Inputs:      None.
 * Outputs:     None.
 * Description: Computes fitness2 for all rules in the population.
 ****************************************************************************/ 
void Population::evaluateFitness2() {

	// the population size and the number of examples
	int pop_size = rules_.size();
	int num_examples = training_set_.num_data_points();

	// the classification of an example for the current rule (true
	// positive, false positive, true negative, or false negative)
	int example_type = 0;

	// reset the array that keeps track of which examples have already
	// been covered by a rule, as well as the fitness2 sum
	reset();

	// rank the rules by fitness1
	rankByFitness1();

	// iterate over the set of rules in rank order
	for (int i=0; i<pop_size; i++) {

		// reset TP, TN, FP, FN to 0.5 (they were already counted once
		// when we computed fitness1, so they have to be reset here
		// for the second count)
		rules_[i].resetCounts();

		// iterate over the set of examples
		for (int j=0; j<num_examples; j++) {

			// run processInput for the current example and
			// the current rule
			example_type = rules_[i].processInput(training_set_.data_points_[j]);

			// if current example is a TP for the current rule
			if (example_type == TP) {

				// if the current example has NOT already been covered by
				// another rule, indicate that the example has now been
				// covered (by this rule)
				if (training_set_.examples_covered_[j] == false) {
					training_set_.examples_covered_[j] = true;

				// if the current example HAS already been covered, we have
				// to decrement the true positive count of the current rule.
				// This is because the count was incremented when processInput
				// was called above
				} else {
					int num_tp = rules_[i].true_positives();
					rules_[i].setTruePositives(num_tp - 1);
				}
			}
		}
		
		// now that we know the counts of true and false positives for this rule,
		// we can update fitness2
		rules_[i].updateFitness2();
	}

} // evaluateFitness2

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Creates the population for the next generation
 ****************************************************************************/
void Population::select() {

	// the number of rules from the previous generation that are
	// to be preserved
	int num_elites = round(max_size_ * elitism_rate_);
	int rest_of_pop = max_size_ - num_elites;

	// by ranking by fitness2, we put the elite rules for the new generation
	// in the correct position
	rankByFitness2();

	// Perform Xover and mutation (note that this is being performed
	// on the previous generation and NOT new_pop, which is as it should
	// be)
	vector<Rule> children = crossoverAndMutate();

	// select children at random to fill the remaining slots in the
	// population
	random_shuffle(children.begin(), children.end());
	children.resize(rest_of_pop);

	// copy the selected children into the new population
	for (int i=0; i<rest_of_pop; i++)
		rules_[rest_of_pop + i] = children[i];

} // end select

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
	Rule off1 = Rule();
	Rule off2 = Rule();

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

	// make sure that the fitnesses are set to 0
	off1.setFitness1(0);
	off2.setFitness1(0);

	// return the pair
	return make_pair(off1, off2);

} // end crossover

/****************************************************************************
 * Inputs:
 * Outputs:     None.
 * Description: Applies the crossover and mutation operators to the population
 ****************************************************************************/
vector<Rule> Population::crossoverAndMutate() {

} // end crossoverAndMutate 

/****************************************************************************
 * Input:       pop_size: the number of rules to be generated.
 * 		attributes_per_rule: the number of attributes in a rule
 * Output:      A randomly generated population.
 * Description: Generates a random population of rules.
 ****************************************************************************/ 
Population Population::random1(int pop_size, int attributes_per_rule, int num_classes) {

	// initialize the population of rules
	Population p = Population(pop_size);

	// a rule variable for generating random rules
	Rule r;

	// generate an initial population of random rules
	for (int i=0; i<pop_size; i++) {

		// generate a random rule and add it to the population
		r = Rule::random(attributes_per_rule, num_classes);
		r.setID(p.id_count_);
		p.add(r);
		p.id_count_++;
	}

	return p;

} // end random

/****************************************************************************
 * Input:       
 * Output:      
 * Description: 
 ****************************************************************************/ 
Population Population::random2(int pop_size,
				int target_class,
				double elitism_rate,
				double crossover_prob,
				double mutate_prob,
				double dont_care_prob,
				double range_scalar,
				Dataset training_set, 
				Dataset test_set) {

	// initialize the population of rules
	Population p = Population(pop_size, target_class, elitism_rate, crossover_prob, 
			mutate_prob, dont_care_prob, range_scalar, training_set, test_set);

	// a rule variable for generating random rules
	Rule r;

	// the number of class attribute values
	int num_classes = p.training_set_.num_classes();

	// the number of attributes (excluding the class attribute) in an example
	int attributes_per_rule = p.training_set_.num_attributes();

	// the range of the attributes for the target class
	vector< pair<double,double> > att_ranges = p.training_set_.targetClassAttributeRanges(target_class);

	// generate an initial population of random rules
	for (int i=0; i<pop_size; i++) {

		// generate a random rule and add it to the population
		r = Rule::random(attributes_per_rule, num_classes, att_ranges, p.range_scalar(), p.dont_care_prob());
		r.setID(p.id_count_);
		r.setClass(p.target_class());
		p.add(r);
		p.id_count_++;
	}

	return p;

} // end random
