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
	// NOTE: verify that the clear() function
	// works before testing this function.
	match_set_.clear();
	correct_set_.clear();
	
	// the current rule
	Rule curr_rule;

	// iterate over all the rules in the population
	int pop_size = pop_.size();
	for (size_t i=0; i<pop_size; i++) {

		// get the current rule
		curr_rule = pop_.rules_[i];

		// if the current rule matches the input, add
		// the current rule to the match set
		if (pop_.rules_[i].matches(curr_data_point_)) {
			int num_matches = curr_rule.num_matches();
			curr_rule.setNumMatches(num_matches + 1);
			match_set_.add(curr_rule);

			// if the current rule's class matches
			// that of the input, add the current
			// rule to the correct set
			if (curr_rule.classification() == curr_data_point_.back()) {
				int num_correct = curr_rule.num_correct();
				curr_rule.setNumCorrect(num_correct + 1);
				correct_set_.add(curr_rule);
				// update correct set fitness sum
			}
		}
	}

	// if doCover: cover()

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
	
	// temporary
	vector<pair<double,double> > ranges;
	for (int i=0; i<NUM_TEST_ATTRIBUTES; i++)
		ranges.push_back(make_pair(0,1));

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

	// specify it based on the value of curr_data_instance
	// r.specify(curr_data_point_, ranges, range_scalar_); 

	// add it to the population and the match set
	pop_.add(r);
	match_set_.add(r);
	
} // end cover

/****************************************************************************
 * Inputs:      
 * Outputs:    
 * Description:
 ****************************************************************************/
void LCS::gaSubsume(int p1_index, int p2_index, Rule first_child, Rule second_child) {

	// bool subsume_first = false;
	// bool subsume_second = false;
	
	// get the fitter of the two parents (p)
	
	// if ((p.exp > theta_sub) && (p.accuracy > theta_acc))
	// 	if (p.generalizes(first_child))
	// 		[increase numerosity of p]
	// 		subsume_first = true;
	// 	else
	// 		
	// 	
	// 	if (p.generalizes(second_child)
	// 		[increase numerosity of p]
	// 		subsume_second = true;
	
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
	
	// otherwise, return false
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
