#ifndef CORRECTSET_H
#define CORRECTSET_H

class CorrectSet {

	public:

		// constructor
		CorrectSet(Population *p) : p_(p), exp_sum_(0), fitness_sum_(0), 
			avg_niche_size_sum_(0) { }

		// the population from which the correct set is created
		Population *p_;

		// stores the indices of the rules in the general
		// population that belong to the correct set
		std::vector<int> members_;

		// adds a rule's index to the correct set and
		// updates the relevant member variables
		void add(int index) {
			if (p_) {
				Rule r = p_->rules_[index];
				p_->rules_[index].setNumCorrect(r.num_correct() + 1);
				p_->rules_[index].setNumNiches(r.num_niches() + 1);
				// need to update niche sizes sum of the rule
				members_.push_back(index);
				exp_sum_ += r.exp();
				fitness_sum_ += r.fitness();
				avg_niche_size_sum_ += r.avg_niche_size;
			}
		}

		// indicates whether the correct set is empty
		bool isEmpty() {
			return members_.empty();
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
