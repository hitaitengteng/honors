#include "matchSet.h"

/****************************************************************************
 * Inputs: 	index: the index in the [P] of the rule to be added
 * Outputs: 	None.
 * Description: Adds a rule's index to the match set and updates the
 * 		relevant member variables.
 ****************************************************************************/ 
void MatchSet::add(int index) {

	// try to add the rule only if [P] exists
	if(p_) {

		// this is just for the sake of abbreviated expression below
		Rule r = p_->rules_[index];

		// update the rule's number of matches
		p_->rules_[index].setNumMatches(r.num_matches() + 1);

		// add the rule's index to [M]
		members_.push_back(index);

		// update the sum of the experience values 
		// and the average experience value of rules in [M]
		exp_sum_ += r.num_matches();
		avg_exp_ = exp_sum_ / members_.size();

		// if necessary, update the list of classes represented
		// by the rules in [M]
		int r_class = r.classification();
		if (classes_represented_[r_class] == false) {
			num_classes_represented_++;
			classes_represented_[r_class] = true;
		}
	}

} // end add

