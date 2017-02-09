#ifndef MATCHSET_H
#define MATCHSET_H

#include "utilities.h"

class MatchSet {

	public:

		// constructor
		MatchSet(Population *p) : exp_sum_(0), avg_exp_(0), 
			num_classes_represented_(0) p_(p) { }

		// the population from which the match set is created
		Population *p_;
		
		// stores the indices of the rules in the
		// general population that belong to the match set
		std::vector<int> members_;

		// indicates which class attribute values are
		// represented in the match set
		bool classes_represented_[NUM_CLASSES];

		// adds a rule's index to the match set and
		// updates the relevant member variables
		void add(int index) {
			if (p_) {
				Rule r = p_->rules_[index];
				p_->rules_[index].setExp(r.exp() + 1);
				p_->rules_[index].setNumMatches(r.num_matches() + 1);
				members_.push_back(index);
				exp_sum_ += r.exp();
				avg_exp_ = exp_sum_ / members_.size();

				int r_class = r.classification();
				if (classes_represented_[r_class] == false) {
					num_classes_represented_++;
					classes_represented_[r_class]
			}
		}

		// resets the match set
		void reset() {
			exp_sum_ = 0;
			avg_exp_ = 0;
			members_.clear();
			for (int i=0; i<NUM_CLASSES; i++)
				classes_in_match_set_[i] = false;
		}

		// indicates whether the match set is empty
		bool isEmpty() {
			return members_.empty();
		}
			
		// getters
		double exp_sum() {return exp_sum_;}
		double avg_exp() {return avg_exp_;}
		double num_classes_represented() {return num_classes_represented_;}

		// setters
		void setExpSum(double exp_sum) {
			exp_sum_ = exp_sum;
		}
		void setAvgExp(double avg_exp) {
			avg_exp_ = avg_exp;
		}

	private:

		double exp_sum_;
		double avg_exp_;
		int num_classes_represented_;
};

#endif
