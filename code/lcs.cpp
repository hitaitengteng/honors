#include "population.h"
#include "lcs.h"

/****************************************************************************
 * File:        LCS.cpp
 * Author:      Will Gantt
 * Description:
 *
 * TODO:
 * 	- Need a separate function, maybe "evaluateInput," for testing mode.
 * 	- Figure out how to update niche_sizes_sum variable for rules
 * 	- Implement specify operator
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

	// determine which rules match the input and, of these, which
	// correctly classify it
	createMatchAndCorrectSets();

	if (doSpecify()) {
		// select a random rule from the population
		//
		// specify it
	}
	
	// the GA is invoked on the correct set only if the average
	// number of iterations that have passed since a rule in the
	// correct set participated in a GA call exceeds a threshold
	// value theta_ga_
	if (doGA())
		applyGA();

} // end processInput

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Runs the genetic algorithm.
 ****************************************************************************/
void LCS::applyGA() {

	// rouletteWheelSelect [What's this about?]
	// rouletteWheelSelect();

	// reproduceAndReplace (this does crossover,
	// mutation, and parent-child subsumption)
	reproduceAndReplace();

	// reset time stamps of rules
	// correct_set_.resetTimeStamps();

} // end applyGA

/****************************************************************************
 * Inputs:      None.
 * Outputs:     None.
 * Description: Creates the match set [M] and the correct set [C] from the
 * 		rules in the general population that (a) match the current
 * 		input and (b) correctly classify it, respectively.
 ****************************************************************************/
void LCS::createMatchAndCorrectSets() {

	// clear the old match and correct sets
	match_set_.clear();
	correct_set_.clear();
	
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
			match_set_.add(i);

			// if the current rule's class matches that of the input,
			// then the rule also belongs in the correct set
			if (r.classification() == curr_data_point_.back()) {
				correct_set_.add(i);

			}

			// update the rule's accuracy and fitness and
			// copy it back into the population vector
			r.updateAccuracyAndFitness(0);

		}
	}

	match_set_.print();

	// There is certain information about the correct set (such as
	// the number of members it contains) that each rule must have,
	// but which cannot be known until after the correct set has
	// already been created. That's the information that gets
	// updated here.
	correct_set_.updateNicheInfo();

	correct_set_.print();

	// if the match set is empty or if not all of the classes are
	// represented in the match set, a new rule is created and
	// added to both the population and the match set (and the
	// correct set, if applicable)
	if (doCover()) {
		cover();
	}

} // end createMatchAndCorrectSets

/****************************************************************************
 * Inputs:      
 * Outputs:    
 * Description:
 *
 * TODO: fix so that it operates on the correct set. This entails:
 *
 * 	1. Fixing gaSubsume so that it adds children to [C]. What if the
 * 	   child doesn't belong in [C] (i.e. it doesn't match the input)?
 ****************************************************************************/
void LCS::reproduceAndReplace() {

	// select the first parent
	int p1_index = rouletteWheelSelect();

	// make sure a different rule is selected for the second parent
	int p2_index;
	do {
		p2_index = rouletteWheelSelect();
	} while (p1_index == p2_index);

	// generate a pair of offspring
	pair<Rule,Rule> children = pop_.crossover(p1_index, p2_index);
	
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
		pop_.remove(pop_.deletionSelect(theta_fit_));
		pop_.remove(pop_.deletionSelect(theta_fit_));
		pop_.add(children.first);
		pop_.add(children.second);
	}
	
} // end reproduceAndReplace

/****************************************************************************
 * Inputs:
 * Outputs:
 * Description:
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

	// if there are somehow rounding errors, we
	// return the last rule in the correct set
	return correct_set_.members_[num_rules - 1];

} // end rouletteWheelSelect

/****************************************************************************
 * Inputs:       None.
 * Outputs:      None.
 * Description: 
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

} // end cover

/****************************************************************************
 * Inputs:      
 * Outputs:    
 * Description:
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
	if ((pop_.rules_[fitter].exp() >= theta_sub_) && 
			(pop_.rules_[fitter].accuracy() >= theta_acc_)) {

		// if the fitter parent meets both of the above criteria, it will
		// subsume a child rule only if it generalizes the child rule.
	 	if (pop_.rules_[fitter].generalizes(first_child)) {
			pop_.rules_[fitter].setNumerosity(pop_.rules_[fitter].numerosity() + 1);
			subsume_first = true;
			pop_.rules_[fitter].printVerbose();
			cout << "First child was subsumed" << endl;
		} 
	 	if (pop_.rules_[fitter].generalizes(second_child)) {
			pop_.rules_[fitter].setNumerosity(pop_.rules_[fitter].numerosity() + 1);
			subsume_second = true;
			pop_.rules_[fitter].printVerbose();
			cout << "Second child was subsumed" << endl;
		}
	
		// if a child rule was NOT subsumed, it is added to the population. In order
		// to maintain a constant population size, another rule must be selected for
		// deletion (NOTE: the rule(s) selected for deletion may be the parent(s)).
		if (!subsume_first) {
			rule_to_remove1 = pop_.deletionSelect(theta_fit_);
			first_child.setID(pop_.id_count_);
			pop_.id_count_++;
	 		pop_.rules_[rule_to_remove1] = first_child;
			cout << "First child was added to the population" << endl;
		}
		if (!subsume_second) {
			do {
				rule_to_remove2 = pop_.deletionSelect(theta_fit_);
	 			pop_.rules_[rule_to_remove2] = second_child;
			} while (rule_to_remove2 == rule_to_remove1);
			second_child.setID(pop_.id_count_);
			pop_.id_count_++;
			cout << "Second child was added to the population" << endl;
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

	if (match_set_.isEmpty())
		return true;

	if (match_set_.num_classes_represented() < theta_mna_)
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
