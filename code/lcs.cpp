#include "population.h"
#include "lcs.h"

/****************************************************************************
 * File:        LCS.cpp
 * Author:      Will Gantt
 * Description:
 *
 * TODO:
 * 	- Need a separate function, maybe "evaluateInput," for testing mode.
 * 	- Implement gaSubsume function (first write out pseudocode)
 * 	- Figure out how to update niche_sizes_sum variable for rules
 ****************************************************************************/

using namespace std;

/****************************************************************************
 * Inputs:      The current input's index in the vector of all inputs
 * Outputs:     None.
 * Description: [For training set only; should eventually be called in a
 * 		 for-loop in main.cpp]
 ****************************************************************************/
void LCS::processInput(int i) {

	// get a data instance from the data set
	// and assign it to curr_data_point_
	curr_data_point_ = training_set_.data_points_[i];

	createMatchAndCorrectSets();

	if (doSpecify()) {
		// select a random rule from the population
		//
		// specify it
	}
	
	if (doGA())
		applyGA();

} // end processInput

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Runs the genetic algorithm.
 ****************************************************************************/
void LCS::applyGA() {

	// rouletteWheelSelect
	correct_set_.rouletteWheelSelect();

	// reproduceAndReplace (this does crossover,
	// mutation, and parent-child subsumption)
	reproduceAndReplace();

	// reset time stamps of rules
	correct_set_.resetTimeStamps();

} // end applyGA

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Creates the match set [M] and the correct set [C] from the
 * 		rules in the general population that (a) match the current
 * 		input and (b) correctly classify it, respectively.
 *
 * TODO:
 * 		Mark which classes are represented in the match set
 ****************************************************************************/
void LCS::createMatchAndCorrectSets() {

	// clear the old match and correct sets
	match_set_.clear();
	correct_set_.clear();
	
	// these keep track of what rules belong
	// in the match and correct sets
	vector<int> rules_in_match_set;
	vector<int> rules_in_correct_set;

	// the current rule
	Rule r;

	// iterate over all the rules in the population
	int pop_size = pop_.rules_.size();
	for (int i=0; i<pop_size; i++) {

		// get the current rule
		r = pop_.rules_[i];

		// if the current rule matches the input, then
		// the rule belongs in the match set
		if (r.matches(curr_data_point_)) {

			// update the number of data instances this rule matches
			r.setNumMatches(r.num_matches() + 1);

			// update which classes are represented in the match set
			classes_in_match_set_[r.classification()] = true;

			// update the rule's experience
			r.setExp(r.exp() + 1);
			rules_in_match_set.push_back(i);

			// if the current rule's class matches that of the input,
			// then the rule also belongs in the correct set
			if (r.classification() == curr_data_point_.back()) {

				// update the number of data instances this rule
				// correctly identifies
				r.setNumCorrect(r.num_correct() + 1);

				// update the number of niches
				// to which this rule belongs
				r.setNumNiches(r.num_niches() + 1);
				rules_in_correct_set.push_back(i);
			}

			// update the rule's accuracy and fitness and
			// copy it back into the population vector
			r.updateAccuracyAndFitness(0);
		}
	}

	// get the sizes of the match and correct sets
	int c_size = rules_in_correct_set.size();
	int m_size = rules_in_match_set.size();

	// the index of the current rule
	int r_i = 0;

	// a counter for the rules_in_correct_set vector
	int c = 0;

	// the sum of the sizes of the niches to which a rule belongs
	int ns_sum;

	// iterate over all the rules that were found to match the input
	for (int i=0; i<m_size; i++) {

		// get the index of the current rule in the general population
		r_i = rules_in_match_set[i];

		// if the current rule is also in the correct set, update niche information
		if ((c < c_size) && (r_i == rules_in_correct_set[c])) {

			ns_sum = pop_.rules_[r_i].niche_sizes_sum();
			pop_.rules_[r_i].setNicheSizesSum(ns_sum + c_size);
			pop_.rules_[r_i].updateAvgNicheSize();

			correct_set_.add(pop_.rules_[r_i]);
			c++;
		}

		// add the rule to the match set
		match_set_.add(pop_.rules_[r_i]);
	}

	// if the match set is empty or if not all of the classes are
	// represented in the match set, a new rule is created and
	// added to both the population and the match set (and the
	// correct set, if applicable)
	if (doCover())
		cover();

} // end createMatchAndCorrectSets

/****************************************************************************
 * Inputs:      
 * Outputs:    
 * Description:
 ****************************************************************************/
void LCS::reproduceAndReplace() {

	// select the first parent
	int p1_index = pop_.rouletteWheelSelect();

	// make sure a different rule is selected for the second parent
	int p2_index;
	do {
		p2_index = pop_.rouletteWheelSelect();
	} while (p1_index == p2_index);

	// generate a pair of offspring
	pair<Rule,Rule> children = pop_.crossover(p1_index, p2_index);
	
	// mutate the offspring 
	children.first.mutate(p_mutate_, p_dont_care_, 
			training_set_.attribute_ranges_, range_scalar_);
	children.second.mutate(p_mutate_, p_dont_care_, 
			training_set_.attribute_ranges_, range_scalar_);

	// REMOVE AFTER DEBUGGING
	pop_.rules_[p1_index].setExp(30);
	pop_.rules_[p2_index].setExp(30);

	if (do_ga_subsumption_)
		gaSubsume(p1_index, p2_index, children.first, children.second);
	
} // end reproduceAndReplace

/****************************************************************************
 * Inputs:       None.
 * Outputs:      None.
 * Description: 
 *
 * NOTE: the correct set and the rules in the correct set will not update
 * properly when the covering operator is invoked. This needs to be fixed.
 *
 * NOTE 2: what if the population is already at capacity? Is the covering
 * operator still invoked, and if so, is a rule selected for deletion in
 * the same manner as in GA subsumption?
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

	// add it to the population and the match set
	pop_.add(r);
	match_set_.add(r);

	// check if it belongs in the correct set as well
	if (r.classification() == curr_data_point_.back())
		correct_set_.add(r);

} // end cover

/****************************************************************************
 * Inputs:      
 * Outputs:    
 * Description:
 *
 * TODO: fix so that child is subsumed more often?
 ****************************************************************************/
void LCS::gaSubsume(int p1_index, int p2_index, Rule first_child, Rule second_child) {

	// booleans to indicate which of the 
	// offspring rules are to be subsumed
	bool subsume_first = false;
	bool subsume_second = false;
	
	// if an offspring rule is not subsumed, another rule from the
	// population must be selected for deletion. These variables
	// store the indices of those rules
	int rule_to_remove1 = -1;
	int rule_to_remove2 = -1;
	
	// determine which of the two parents is fitter
	int fitter;
	if (pop_.rules_[p1_index].fitness() > pop_.rules_[p2_index].fitness())
		fitter = p1_index;
	else
		fitter = p2_index;

	// A parent may subsume a child rule only if its experience exceeds a threshold
	// value theta_sub, and only if its accuracy exceeds a threshold value theta_acc.
	// If this is not the case, then nothing happens.
	if ((pop_.rules_[fitter].exp() > theta_sub_) && 
			(pop_.rules_[fitter].accuracy() > theta_acc_)) {

		// if the fitter parent meets both of the above criteria, it will
		// subsume a child rule only if it generalizes the child rule.
	 	if (pop_.rules_[fitter].generalizes(first_child)) {
			pop_.rules_[fitter].setNumerosity(pop_.rules_[fitter].numerosity() + 1);
			subsume_first = true;
		} 
	 	if (pop_.rules_[fitter].generalizes(second_child)) {
			pop_.rules_[fitter].setNumerosity(pop_.rules_[fitter].numerosity() + 1);
			subsume_second = true;
		}
	
		// if a child rule was NOT subsumed, it is added to the population. In order
		// to maintain a constant population size, another rule must be selected for
		// deletion (NOTE: the rule(s) selected for deletion may be the parent(s)).
		if (!subsume_first) {
			rule_to_remove1 = pop_.deletionSelect(theta_fit_);
	 		pop_.rules_[rule_to_remove1] = first_child;
		}
		if (!subsume_second) {
			do {
				rule_to_remove2 = pop_.deletionSelect(theta_fit_);
	 			pop_.rules_[rule_to_remove2] = second_child;
			} while (rule_to_remove2 == rule_to_remove1);
		}
	}
	
} // end gaSubsume

/****************************************************************************
 * Inputs:      None.
 * Outputs:     A boolean indicating whether the covering operator should
 * 		be invoked.
 * Description: Decides whether the covering operator should be invoked.
 * 		Returns true only if one of the following is true:
 * 			1. the match set is empty
 * 			2. Not all of the possible classes are represented
 * 			   in the match set.
 ****************************************************************************/
bool LCS::doCover() {

	if (match_set_.empty())
		return true;

	int num_classes_represented = 0;
	for (int i=0; i<NUM_CLASSES; i++) {
		if (classes_in_match_set_[i] == true)
			num_classes_represented++;
	}

	if (num_classes_represented < theta_mna_)
		return true;

	return false;

} // end doCover

/****************************************************************************
 * Inputs:      
 * Outputs:    
 * Description:
 ****************************************************************************/
bool LCS::doSpecify() {

	// the specify operator is invoked on the correct
	// set when the rules in the correct set are "sufficiently
	// experienced" and the average error exceeds some threshold
	
	// need more details on this.
	return false;

} // end doSpecify
