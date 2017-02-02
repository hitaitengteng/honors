#include "population.h"

/****************************************************************************
 * File:        LCS.cpp
 * Author:      Will Gantt
 * Description:
 *
 * TODO:
 * 	- Need a separate function, maybe "evaluateInput," for testing mode.
 * 	- Get RNG working
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
	
	// createMatchAndCorrectSets();
	// (the cover operator is taken care of
	// in this function)
	
	// if (doSpecify)
	//     specify()
	
	// if (doGA)
	//     applyGA()

} // end processInput

void LCS::applyGA() {

	// rouletteWheelSelect
	
	// reproduceAndReplace (this does crossover,
	// mutation, and parent-child subsumption)
	
	// reset time stamps of rules
	
} // end applyGA

void LCS::createMatchAndCorrectSets() {

	// clear the old match and correct sets
	// NOTE: verify that the clear() function
	// works before testing this function.
	_match_set.clear();
	_correct_set.clear();
	
	// the current rule
	Rule *curr_rule;

	// iterate over all the rules in the population
	int pop_size = pop.size();
	for (size_t i=0; i<pop_size; i++) {

		// get the current rule
		curr_rule = *pop.rules[i];

		// if the current rule matches the input, add
		// the current rule to the match set
		if (pop.rules[i].matches(_curr_data_point)) {
			curr_rule->num_matches++; // this is not valid
			_match_set.add(curr_rule);

			// if the current rule's class matches
			// that of the input, add the current
			// rule to the correct set
			if (curr_rule.getClass() == input.end()) {
				curr_rule->num_correct++;
				_correct_set.add(curr_rule);
				// update correct set fitness sum
			}
		}
	}

	// if doCover: cover()

} // end createMatchAndCorrectSets

void LCS::reproduceAndReplace() {

	// p1 = p->rouletteWheelSelect();
	// p2 = p->rouletteWheelSelect();
	
	// children = pop->crossover(p1, p2);
	
	// children.first.mutate();
	// children.second.mutate();
	
	// if (doGaSubusme)
	//       gaSubsume();
	
} // end reproduceAndReplace

void LCS::cover() {

	// create a new rule
	Rule r;

	// specify it based on the value of curr_data_instance
	// r.specify(curr_data_point_, ranges, rangeScalar, <mt19937>);

	// add it to the population and the match set
	// pop_.add(r);
	// match_set_.add(r);
	
} // end cover

bool LCS::doGA() {

//	return (_correct_set.fitnessSum() > _theta_ga);

	// remove this once implemented
	return false;

} // end doGA

bool LCS::doCover() {

	// if the match set is empty or not all classes
	// are represented in the match set, return true
	// Q: How are you going to determine whether all
	// of the classes are present in the match set?
	
	// otherwise, return false
	return false;

} // end doCover

bool LCS::doSpecify() {

	// the specify operator is invoked on the correct
	// set when the rules in the correct set are "sufficiently
	// experienced" and the average error exceeds some threshold
	
	// need more details on this.
	return false;

} // end doSpecify
