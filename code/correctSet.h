#ifndef CORRECTSET_H
#define CORRECTSET_H

#include "utilities.h"
#include "population.h"
#include "rule.h"

class CorrectSet {

	public:

		// default constructor
		CorrectSet() { };

		// custom constructor
		CorrectSet(Population *p) : p_(p), fitness_sum_(0), 
			avg_niche_size_sum_(0), exp_sum_(0) { }

		// the population from which the correct set is created
		Population *p_;

		// stores the indices of the rules in the general
		// population that belong to the correct set
		std::vector<int> members_;

		// adds a rule's index to the correct set and
		// updates the relevant member variables
		void add(int index); 

		// indicates whether the correct set is empty
		bool isEmpty() {
			return members_.empty();
		}

		// resets the correct set (but maintains the pointer to the population
		void clear() {
			members_.clear();
			exp_sum_ = 0;
			fitness_sum_ = 0;
			avg_niche_size_sum_ = 0;
		}

		// prints the correct set
		void print() {
			int num_members = members_.size();
			printf("\nCORRECT SET\n-----------\n");
			for (int i=0; i<num_members; i++) {
				(p_->rules_[members_[i]]).print();
			}
		}

		// getters
		double fitness_sum() {return fitness_sum_;}
		double avg_niche_size_sum() {return avg_niche_size_sum_;}
		double exp_sum() {return exp_sum_;}

		// setters
		void setFitnessSum(double fitness_sum) {
			fitness_sum_ = fitness_sum;
		}
		void setAvgNicheSizeSum(double avg_niche_size_sum) {
			avg_niche_size_sum_ = avg_niche_size_sum;
		}
		void setExpSum(double exp_sum) {
			exp_sum_ = exp_sum;
		}

	private:

		// the sum of the fitnesses of all 
		// the rules in the correct set
		double fitness_sum_;

		// the sum of the average niche sizes
		// of all the rules in the correct set
		double avg_niche_size_sum_;

		// the sum of the experience levels
		// of all the rules in the correct set
		double exp_sum_;
};

#endif
