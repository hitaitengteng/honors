#include "population.h"
#include "lcs.h"

/****************************************************************************
 * File:        LCS.cpp
 * Author:      Will Gantt
 * Description:
 *
 * TODO:
 * 	- Need a separate function, maybe "evaluateInput," for testing mode.
 * 	- Implement specify operator
 ****************************************************************************/

using namespace std;

/****************************************************************************
 * Inputs:      The current input's index in the training set.
 * Outputs:     None.
 * Description: Presents a single data instance to the LCS for processing.
 ****************************************************************************/
void LCS::processInput(int i) {

	// get a data instance from the data set
	// and assign it to curr_data_point_
	curr_data_point_ = training_set_.data_points_[i];

	// determine which rules match the input and, of these, which
	// correctly classify it
	createMatchAndCorrectSets();

	// specify

	// the GA is invoked on the correct set only if the average
	// number of iterations that have passed since a rule in the
	// correct set participated in a GA call exceeds a threshold
	// value theta_ga_
	
	if (doGA()) {
		printf("GA ran on iteration %d\n", curr_gen_);
		applyGA();
	}

} // end processInput

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Creates the match set [M] and the correct set [C] from the
 * 		rules in the general population that (a) match the current
 * 		input and (b) correctly classify it, respectively.
 ****************************************************************************/
void LCS::createMatchAndCorrectSets() {

	// increment the current generation
	curr_gen_++;

	// clear the old match and correct sets
	match_set_.clear();
	correct_set_.clear();
	
	// iterate over all the rules in the population
	int pop_size = pop_.rules_.size();
	for (int i=0; i<pop_size; i++) {

		// if the current rule matches the input, then
		// the rule belongs in the match set
		if (pop_.rules_[i].matches(curr_data_point_)) {
			match_set_.add(i);

			// if the current rule's class matches that of the input,
			// then the rule also belongs in the correct set
			if (pop_.rules_[i].classification() == curr_data_point_.back()) {
				correct_set_.add(i);

			}

			// update the rule's accuracy and fitness and
			// copy it back into the population vector
			pop_.rules_[i].updateAccuracyAndFitness(fitness_exponent_);

		}
	}

	// There is certain information about the correct set (such as
	// the number of members it contains) that each rule must have,
	// but which cannot be known until after the correct set has
	// already been created. That's the information that gets
	// updated here.
	correct_set_.updateNicheInfo();

	// match_set_.print();
	// correct_set_.print();

	// if the match set is empty or if not all of the classes are
	// represented in the match set, a new rule is created and
	// added to both the population and the match set (and the
	// correct set, if applicable)
	if (doCover()) {
		cover();
	}

} // end createMatchAndCorrectSets

/****************************************************************************
 * Inputs:      None. 
 * Outputs:     None.
 * Description: Executes a single iteration of the genetic algorithm.
 *
 * TODO:
 *      - fix most_general_
 * 	- ga_subsume needs to be fixed.
 * 		- figure out why it is so rarely the case that the child
 * 		  is subsumed by the parent and whether that's a problem.
 * 	- figure out where experience needs to be incorporated.
 * 	- figure out how you're going to deal with numerosity
 * 	- write a function for breaking up the data into training and testing
 * 	  sets
 *
 * 	Secondary:
 * 		- Need a way of mapping class names to numbers
 * 		- update Makefile dependencies
 ****************************************************************************/
void LCS::applyGA() {

	// the index of the parents in the general population
	int p1_index;
	int p2_index;

	// if the fitness sum is less than 2, rouletteWheelSelect can get stuck
	// in a loop, because it will keep selecting the same single parent for
	// crossover. The code in this if-statement prevents that.
	if (correct_set_.fitness_sum() < 2) {
		p1_index = correct_set_.members_[0];
		p2_index = correct_set_.members_[1];

	} else {

		// select the first parent
		p1_index = rouletteWheelSelect();

		// make sure a different rule is selected for the second parent
		do {
			p2_index = rouletteWheelSelect();
		} while (p1_index == p2_index);
	}

	// the offspring
	pair<Rule,Rule> children;

	// determine whether to apply crossover
	double do_crossover = real_dist(rng);

	// do crossover
	if (do_crossover <= p_crossover_) {

		// generate a pair of offspring
		children = pop_.crossover(p1_index, p2_index);
	
	// don't do crossover
	} else {
		// otherwise, just create a copy of the parents, but make sure their
		// IDs are reset
		children = make_pair(pop_.rules_[p1_index], pop_.rules_[p2_index]);
		children.first.setID(NO_ID);
		children.second.setID(NO_ID);
	}
	
	// mutate the offspring 
	children.first.mutate(p_mutate_, p_dont_care_, 
		training_set_.attribute_ranges_, range_scalar_);
	children.second.mutate(p_mutate_, p_dont_care_, 
		training_set_.attribute_ranges_, range_scalar_);

	// see documentation for gaSubsume below. If gaSubsume is not used,
	// then two existing rules are deleted and the two offspring are
	// added to the population. A rule's likelihood of being chosen
	// for deletion is proportional to its average niche size.
	if (do_ga_subsumption_) {
		gaSubsume(p1_index, p2_index, children.first, children.second);
	} else {
		pop_.remove(pop_.deletionSelect(theta_acc_));
		pop_.remove(pop_.deletionSelect(theta_acc_));
		pop_.add(children.first);
		pop_.add(children.second);
	}

	// update the time stamps of the rules in [C]
	correct_set_.updateTimeStamps(curr_gen_);
	
} // end applyGA

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Randomly selects a rule from [C]. A rule's likelihood of
 * 		being selected is directly proportional to its fitness.
 *
 * NOTE: If this function is giving you trouble (esp. if the whole program
 * fails to terminate b/c of it, you should make sure that the fitness sum
 * is being set correctly.
 ****************************************************************************/ 
int LCS::rouletteWheelSelect() {

	// select a random number in the range [0,correct_set_.fitness_sum()]
	double random = real_dist(rng) * correct_set_.fitness_sum();

	/*
	 * we determine the individual selected by subtracting
	 * the individual fitnesses from the random value generated
	 * above until that value falls to or below 0. In this way,
	 * an individual's likelihood of being selected is directly
	 * proportional to its fitness
	 */
	int num_rules = correct_set_.members_.size();
	int curr_index;
	for (int i=0; i<num_rules; i++) {
		curr_index = correct_set_.members_[i];
		random -= pop_.rules_[curr_index].fitness();
		if (random <= 0) {
			return i;
		}
	}

	// control should never reach here
	return correct_set_.members_[num_rules - 1];

} // end rouletteWheelSelect

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: If [C] is empty on a given iteration of the LCS, or if not
 * 		all of the classes are represented in [M], this function is
 * 		called. Cover creates a new rule whose attribute values are
 * 		centered at the corresponding attribute values of the input,
 * 		and whose class is the same as the input's class. The new
 * 		rule is then added to the general population, and to [M] and
 * 		[C].
 ****************************************************************************/
void LCS::cover() {

	// create a new rule
	Rule r;

	// set the class
	r.setClass(curr_data_point_.back());

	Attribute a;
	int cond_length = curr_data_point_.size() - 1;
	for (int i=0; i<cond_length; i++) {

		// set the center value of the attribute using the value of
		// the current element in the data point
		a.setCenter(curr_data_point_[i]);

		// set the spread and "don't care" values
		double curr_att_range = training_set_.attribute_ranges_[i].second - 
			training_set_.attribute_ranges_[i].first;
		double spread = curr_att_range * range_scalar_ * real_dist(rng);
		a.setSpread(spread);
		a.setDontCare(false);

		// add the attribute to the vector
		r.condition_.push_back(a);
	}

	// add it to the population, match, and correct sets 
	pop_.add(r);
	match_set_.add(pop_.size() - 1);
	correct_set_.add(pop_.size() - 1);

	// update the accuracy and fitness of the rule, as well as
	// niche information
	pop_.rules_[pop_.size() - 1].updateAccuracyAndFitness(fitness_exponent_);
	correct_set_.updateNicheInfo();

} // end cover

/****************************************************************************
 * Inputs:     
 * 	p1_index:    the index of the first parent rule in the general
 * 		     population.
 * 	p2_index:    the index of the second parent rule in the general
 * 		     population.
 * 	first_child: the first of the two offspring rules.
 * 	second_child: the second of the two offspring rules. 
 *
 * Outputs:     None.
 * Description: After crossover and mutation occur, and if do_ga_subsume is
 * 		set to true, it must be determined whether the offspring
 * 		rules are to be added to the population. If it is found that
 * 		the fitter of the two parent rules is more general than one
 * 		of the offspring, that offspring is not added to the
 * 		population, and the parent's numerosity parameter is
 * 		incremented. Otherwise, the offspring is added to
 * 		the population and a rule is selected from the general
 * 		population at random for deletion.
 ****************************************************************************/
void LCS::gaSubsume(int p1_index, int p2_index, Rule first_child, Rule second_child) {

	// booleans to indicate which of the offspring rules are to be subsumed
	bool subsume_first = false;
	bool subsume_second = false;
	
	// if an offspring rule is not subsumed, another rule from the
	// population must be selected for deletion. These variables
	// store the indices of those rules
	int rule_to_remove1;
	int rule_to_remove2;
	
	// determine which of the two parents is fitter (TODO: fix this so that the
	// second parent is not automatically selected if they have equal fitness)
	int fitter; 
	if (pop_.rules_[p1_index].fitness() > pop_.rules_[p2_index].fitness())
		fitter = p1_index;
	else
		fitter = p2_index;

	// A parent may subsume a child rule only if its experience exceeds a threshold
	// value theta_sub, and only if its accuracy exceeds a threshold value theta_acc.
	// If this is not the case, then nothing happens.
	if ((pop_.rules_[fitter].exp() >= theta_sub_) && 
			(pop_.rules_[fitter].accuracy() >= theta_acc_)) {

		// if the fitter parent meets both of the above criteria, it will
		// subsume a child rule only if it generalizes the child rule.
	 	if (pop_.rules_[fitter].generalizes(first_child)) {
			pop_.rules_[fitter].setNumerosity(pop_.rules_[fitter].numerosity() + 1);
			cout << "first child subsumed" << endl;
			subsume_first = true;
		} 
	 	if (pop_.rules_[fitter].generalizes(second_child)) {
			pop_.rules_[fitter].setNumerosity(pop_.rules_[fitter].numerosity() + 1);
			cout << "second child subsumed" << endl;
			subsume_second = true;
		}
	
		// if a child rule was NOT subsumed, it is added to the population. If
		// the population has already reached capacity, another rule must be
		// deleted. (NOTE: The parents may be selected for deletion.)
		if (!subsume_first) {

			// set the ID of the child and update the rule ID count
			// of the population
			first_child.setID(pop_.id_count_);
			pop_.id_count_++;

			// if the population is already at capacity, a rule has to 
			// be deleted
			if (pop_.rules_.size() == pop_.max_size()) {
				rule_to_remove1 = pop_.deletionSelect(theta_acc_);
				pop_.remove(rule_to_remove1);
			}

			// add the child to the population
			pop_.add(first_child);
			cout << "first child added to the population" << endl;
		}
		if (!subsume_second) {
			second_child.setID(pop_.id_count_);
			pop_.id_count_++;

			if (pop_.rules_.size() == pop_.max_size()) {

				// If the population is at capacity and the first
				// child was subsumed, AND the second child is also
				// to be subsumed, we want to make sure we don't select
				// the first child for deletion. If it has been added,
				// the first child will be at index (pop_size - 1), so
				// we don't want to pick that index. Hence the do-while loop.
				do {
					rule_to_remove2 = pop_.deletionSelect(theta_acc_);
				} while (rule_to_remove2 == (pop_.size() - 1));
				pop_.remove(rule_to_remove2);
			}
			pop_.add(second_child);
			cout << "second child added to the population" << endl;
		}
	} 
	
} // end gaSubsume

/****************************************************************************
 * Inputs:      None.
 * Outputs:     A boolean indicating whether the covering operator should
 * 		be invoked.
 * Description: Decides whether the covering operator should be invoked.
 * 		Returns true only if the population size limit has not been
 * 		reached and if one of the following is true:
 * 			1. the correct set is empty
 * 			2. Not all of the possible classes are represented
 * 			   in the match set.
 *
 * 		NOTE: You may want to change the condition that prevents the
 * 		cover operator from executing when the population size limit
 * 		has been reached. If you do, you will have to invoke
 * 		deletionSelect to remove a rule from the population.
 ****************************************************************************/
bool LCS::doCover() {

	// if the population size limit has already been reached, cover
	// should not execute
	if (pop_.rules_.size() >= pop_.max_size())
		return false;

	return ((correct_set_.isEmpty()) || 
			(match_set_.num_classes_represented() < theta_mna_));

} // end doCover

/****************************************************************************
 * Inputs:      None.
 * Outputs:     A boolean indicating whether the GA should be invoked.
 * Description: At each iteration of the LCS, determines whether the GA
 * 		should be invoked. The GA is invoked if the average number
 * 		of iterations since the rules in [C] last participated in
 * 		a run of the GA exceeds some threshold value theta_ga.
 ****************************************************************************/
bool LCS::doGA() {

	int correct_set_size = correct_set_.members_.size();

	// if there are fewer than two rules in [C], the GA cannot be executed.
	// Also: since selection in the GA is fitness-based, it makes little
	// sense to run the GA if all of the candidate rules' fitness values are 0.
	if ((correct_set_size < 2) || (correct_set_.fitness_sum() < 2))
		return false;

	// the sum of the number of iterations since each rule last
	// participated in a correct set to which the GA was applied
	int sum_iters_since_last_ga = 0;

	// the index of the current rule in the general population
	int r_i;

	// iterate over all the rules in the correct set
	for (int i=0; i<correct_set_size; i++) {

		r_i = correct_set_.members_[i];
		sum_iters_since_last_ga += 
			curr_gen_ - pop_.rules_[r_i].time_stamp();
	}

	return (((double)sum_iters_since_last_ga) / 
			((double) correct_set_size)) > theta_ga_;

} // end doGA

/****************************************************************************
 * Inputs:      None.
 * Outputs:     A boolean indicating whether the specify operator should be
 * 		invoked.
 * Description: Determines whether the specify operator should be invoked
 * 		on [C].
 ****************************************************************************/
bool LCS::doSpecify() {

	// in XCS, the specify operator is invoked on the correct
	// set when the rules in the correct set are "sufficiently
	// experienced" and the average error exceeds some threshold
	
	// need more details on this.
	return false;

} // end doSpecify
