#ifndef MATCHSET_H
#define MATCHSET_H

#include "rule.h"
#include "population.h"
#include "utilities.h"

class MatchSet {

	public:

		// default constructor
		MatchSet() { };

		// custom constructor
		MatchSet(Population *p) : p_(p), exp_sum_(0), avg_exp_(0), 
			num_classes_represented_(0) { };

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
		void add(int index);

		// resets the match set (but maintains the pointer to the population)
		void clear() {
			exp_sum_ = 0;
			avg_exp_ = 0;
			members_.clear();
			for (int i=0; i<NUM_CLASSES; i++)
				classes_represented_[i] = false;
		}

		// indicates whether the match set is empty
		bool isEmpty() {
			return members_.empty();
		}
			
		// prints the match set
		void print() {
			printf("\nMATCH SET\n---------\n");
			int num_members = members_.size();
			for (int i=0; i<num_members; i++)
				(p_->rules_[members_[i]]).print();
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
