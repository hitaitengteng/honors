#include "population."

void LCS::applyGA() {

} // end applyGA

void LCS::createMatchAndCorrectSets() {

	// clear the old match and correct sets
	// NOTE: this is not valid syntax
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
			}
		}
	}

} // end createMatchAndCorrectSets

void LCS::reproduceAndReplace() {

} // end reproduceAndReplace
