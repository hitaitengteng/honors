#include "matchSet.h"

/****************************************************************************
 * Inputs: 	the index in the general population of the rule to be added
 * Outputs: 	None.
 * Description: adds a rule's index to the match set and updates the
 * 		relevant member variables.
 ****************************************************************************/ 
void MatchSet::add(int index) {

	if(p_) {
		Rule r = p_->rules_[index];
		p_->rules_[index].setExp(r.exp() + 1);
		p_->rules_[index].setNumMatches(r.num_matches() + 1);
		members_.push_back(index);
		exp_sum_ += r.exp();
		avg_exp_ = exp_sum_ / members_.size();

		int r_class = r.classification();
		if (classes_represented_[r_class] == false) {
			num_classes_represented_++;
			classes_represented_[r_class] = true;
		}
	}

} // end add

