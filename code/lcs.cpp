#include "population.h"
#include "lcs.h"

/****************************************************************************
 * File:        LCS.cpp
 * Author:      Will Gantt
 * Description:
 *
 * TODO:
 * 	- Need a separate function, maybe "evaluateInput," for testing mode.
 ****************************************************************************/

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
			curr_rule.num_matches_++; // this is not valid
			match_set_.add(curr_rule);

			// if the current rule's class matches
			// that of the input, add the current
			// rule to the correct set
			if (curr_rule.classification() == curr_data_point_.back()) {
				curr_rule.num_correct_++;
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

	// p1 = p->rouletteWheelSelect();
	// p2 = p->rouletteWheelSelect();
	
	// children = pop->crossover(p1, p2);
	
	// children.first.mutate();
	// children.second.mutate();
	
	// if (doGaSubusme)
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
