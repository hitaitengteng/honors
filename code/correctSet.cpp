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
		members_.push_back(index);
		exp_sum_ += r.exp();
		fitness_sum_ += r.fitness();
		avg_niche_size_sum_ += r.avg_niche_size();
	}

} // end add

/****************************************************************************
 * Input:
 * Output:
 * Description:
 ****************************************************************************/
void CorrectSet::updateNicheInfo() {

	// the number of members in the correct set
	int num_members = members_.size();

	// the sum of the sizes of the niches that the current rule
	// has been a part of
	int curr_sum = 0;

	// the number of niches that the current rule has been a part of
	int curr_num_niches = 0;

	// iterate over all the rules in the correct set
	for (int i=0; i<num_members; i++) {

		curr_sum = p_->rules_[members_[i]].niche_sizes_sum();
		curr_num_niches = p_->rules_[members_[i]].num_niches();
		p_->rules_[members_[i]].setNicheSizesSum(curr_sum + num_members);
		p_->rules_[members_[i]].setAvgNicheSize(
				p_->rules_[members_[i]].niche_sizes_sum() / curr_num_niches);
	}

} // end updateNicheInfo



