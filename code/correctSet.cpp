#include "correctSet.h"

/****************************************************************************
 * Inputs: 	the index in the general population of the rule to be added
 * 	   	to the correct set
 * Outputs: 	None.
 * Description: Adds a rule's index to the correct set and updates the
 * 		relevant member variables
 ****************************************************************************/ 
void CorrectSet::add(int index) {

	if (p_) {
		Rule r = p_->rules_[index];
		p_->rules_[index].setNumCorrect(r.num_correct() + 1);
		p_->rules_[index].setNumNiches(r.num_niches() + 1);
		// need to update niche sizes sum of the rule and
		// average niche size
		members_.push_back(index);
		exp_sum_ += r.exp();
		fitness_sum_ += r.fitness();
		avg_niche_size_sum_ += r.avg_niche_size();
	}

} // end add
