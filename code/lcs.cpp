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
 * 	- How are you going to keep track of niches?
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
 * Inputs:      
 * Outputs:    
 * Description:
 ****************************************************************************/
void LCS::createMatchAndCorrectSets() {

	// clear the old match and correct sets
	match_set_.clear();
	correct_set_.clear();
	
	vector<int> rules_in_match_set;
	vector<int> rules_in_correct_set;

	// iterate over all the rules in the population
	for (int i=0; i<pop_size; i++) {

		// if the current rule matches the input, then
		// the rule belongs in the match set
		if (pop_.rules_[i].matches(curr_data_point_)) {

			int num_matches = pop.rules_[i].num_matches();
			pop.rules_[i].setNumMatches(num_matches + 1);

			int exp = pop.rules_[i].exp();
			pop.rules_[i].setExp(exp + 1);

			// if the current rule's class matches that of the input,
			// then the rule also belongs in the correct set
			if (curr_rule.classification() == curr_data_point_.back()) {

				int num_correct = pop.rules_[i].num_correct();
				pop.rules_[i].setNumCorrect(num_correct + 1);

				int num_niches = pop.rules_[i].num_niches();
				pop.rules_[i].setNumNiches(num_niches + 1);
			}

			// update the rule's accuracy and fitness
			pop.rules_[i].updateAccuracyAndFitness(fitness_exponent_);
		}
	}

	// get the sizes of the match and correct sets
	int correct_set_size = rules_in_correct_set.size();
	int match_set_size = rules_in_match_set.size();

	// the index of the current rule
	int curr_rule = 0;

	// a counter for the rules_in_correct_set vector
	int correct_counter = 0;

	// the sum of the sizes of the 
	int niche_sizes_sum;

	// iterate over all the rules that were found to match the input
	for (int i=0; i<match_set_size; i++) {

		// get the index of the current rule in the general population
		curr_rule = rules_in_match_set[i];

		// if the current rule is also in the correct set, update niche information
		if (curr_rule == rules_in_correct_set[correct_counter]) {
			niche_sizes_sum = pop.rules_[curr_rule].niche_sizes_sum();
			pop.rules_[curr_rule].setNicheSizesSum(niche_sizes_sum + correct_set_size);
			pop.rules_[curr_rule].updateAvgNicheSize;
			correct_set_.add(pop.rules_[curr_rule]);
			correct_counter++;
		}

		// add the rule to the match set
		match_set_.add(pop.rules_[curr_rule]);
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

	pop_.rules_[p1_index].print();
	pop_.rules_[p2_index].print();
	
	// generate a pair of offspring
	pair<Rule,Rule> children = pop_.crossover(p1_index, p2_index);
	
	// TEMPORARY!!!!!!!!!
	vector<pair<double,double> > ranges;
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++)
		ranges.push_back(make_pair(0,0.5));

	// mutate the offspring (NOTE: ultimately, we want 'ranges' to be
	// d.attribute_ranges_ (where d is the member dataset)
	children.first.mutate(p_mutate_, p_dont_care_, ranges, range_scalar_);
	children.second.mutate(p_mutate_, p_dont_care_, ranges, range_scalar_);

	// if (do_ga_subsumption_)
	//       gaSubsume();
	
} // end reproduceAndReplace

/****************************************************************************
 * Inputs:      
 * Outputs:    
 * Description:
 ****************************************************************************/
void LCS::cover() {

	// create a new rule
	Rule r;

	// set the class
	r.setClass(curr_data_point_.back());

	int cond_length = curr_data_point_.size() - 1;
	for (int i=0; i<cond_length; i++)
		r.condition_.push_back(curr_data_point_[i]);

	// add it to the population and the match set
	pop_.add(r);
	match_set_.add(r);

	// check if it belongs in the correct set as well
	
} // end cover

/****************************************************************************
 * Inputs:      
 * Outputs:    
 * Description:
 *
 * NOTE: the the likelihood that a rule is to be replaced by one of the
 * offspring rules should be inversely proportional to its average niche
 * size. This means wherever we see a "rouletteWheelSelect" below, we actually
 * need a different selection function.
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
	if (pop_[p1_index].fitness() > pop_[p2].fitness)
		fitter = p1_index;
	else
		fitter = p2_index;

	// a parent may subsume a child rule only if its experience exceeds a threshold
	// value theta_sub, and only if its accuracy exceeds a threshold value theta_acc
	if ((pop_[fitter].exp() > theta_sub) && (pop_[fitter].accuracy() > theta_acc)) {

		// if the fitter parent meets both of the above criteria, it will
		// subsume a child rule only if it generalizes the child rule.
	 	if (pop_[fitter].generalizes(first_child)) {
			pop_[fitter].setNumerosity(pop_[fitter].numerosity() + 1);
			subsume_first = true;
		} 
	 	if (pop_[fitter].generalizes(second_child)) {
			pop_[fitter].setNumerosity(pop_[fitter].numerosity() + 1);
			subsume_second = true;
		}
	}
	
	// if a child rule was NOT subsumed, it is added to the population. In order
	// to maintain a constant population size, another rule must be selected for
	// deletion (NOTE: the rule(s) selected for deletion may be the parent(s)
	if (!subsume_first) {
		rule_to_remove1 = pop_.subsumptionSelect();
	 	pop_[rule_to_remove1] = first_child;
	}
	if (!subsume_second) {
		rule_to_remove2 = pop_subsumptionSelect();
	 	pop_[rule_to_remove2] = second_child;
	}
	
} // end gaSubsume

/****************************************************************************
 * Inputs:      
 * Outputs:    
 * Description:
 ****************************************************************************/
bool LCS::doCover() {

	// if the match set is empty or not all classes
	// are represented in the match set, return true
	// Q: How are you going to determine whether all
	// of the classes are present in the match set?
	return (match_set_.empty() || /* all classes present */);

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
