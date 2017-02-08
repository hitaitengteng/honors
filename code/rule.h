#ifndef RULE_H
#define RULE_H

#include "utilities.h"
#include "dataset.h"

/****************************************************************************
 * File:        Rule.h
 * Author:      William Gantt
 * Description: Defines the Rule class.
 ****************************************************************************/ 

class Rule {

	public:

	// MEMBER VARIABLES
	
		// the condition under which the rule applies (NOTE: this should
		// probably be private, but I was feeling too lazy to make wrapper
		// functions)
		std::vector<Attribute> condition_;

	// FUNCTIONS
	
		// constructor
		Rule() : id_(NO_ID), classification_(0), time_stamp_(0), exp_(0), numerosity_(1),
		         num_matches_(0), num_correct_(0), num_dont_care_(0), num_niches_(0),
			 niche_sizes_sum_(0), avg_niche_size_(real_dist(rng)), accuracy_(0), fitness_(0) { }

		// equality operator
		bool operator==(const Rule &rule) const;

		// inequality operator
		bool operator!=(const Rule &rule) const;

		// mutates the class and attribute ranges in the condition with
		// some probability
		void mutate(double p_mutate, double p_dont_care, std::vector<std::pair<double,double> > ranges, 
				double range_scalar);

		// assigns particular values to all "don't care" attributes based
		// on the passed input vector
		Rule specify(std::vector<double> input, std::vector<std::pair<double,double> > ranges, 
				double range_scalar);
	
		// checks whether this rule is a more general version of another
		bool generalizes(Rule &rule) const;

		// checks whether this rule matches an input datum
		bool matches(std::vector<double> &input) const;

		// updates accuracy and fitness
		void updateAccuracyAndFitness(int fitness_exponent) {
			if (num_correct_ != 0) {
				accuracy_ = num_matches_ / num_correct_;
				fitness_ = pow(accuracy_, fitness_exponent);
			}
		}

		// updates average niche size
		void updateAvgNicheSize() {
			if (num_niches_ != 0) {
				avg_niche_size_ = niche_sizes_sum_ / num_niches_;
			}
		}

		// generate a random rule
		static Rule random(int num_attributes);

		// prints out some information about a rule
		void print();

		// prints out complete information about a rule
		void print_verbose();
	
		// getters 
		std::vector<Attribute> condition() const {return condition_;}
		int id() const {return id_;}
		int classification() const {return classification_;}
		int time_stamp() const {return time_stamp_;}
		int exp() const {return exp_;}
		int numerosity() const {return numerosity_;}
		int num_matches() const {return num_matches_;}
		int num_correct() const {return num_correct_;}
		int num_dont_care() const {return num_dont_care_;}
		int num_niches() const {return num_niches_;}
		int niche_sizes_sum() const {return niche_sizes_sum_;}
		double avg_niche_size() const {return avg_niche_size_;}
		double accuracy() const {return accuracy_;}
		double fitness() const {return fitness_;}

		// setters
		void setCondition(std::vector<Attribute> condition) {condition_ = condition;}
		void setClass(int classification) {classification_ = classification;}
		void setTimeStamp(int time_stamp) {time_stamp_ = time_stamp;}
		void setExp(int exp) {exp_ = exp;}
		void setNumerosity(int numerosity) {numerosity_ = numerosity;}
		void setNumMatches(int num_matches) {num_matches_ = num_matches;}
		void setNumCorrect(int num_correct) {num_correct_ = num_correct;}
		void setNumDontCare(int num_dont_care) {num_dont_care_ = num_dont_care;}
		void setNumNiches(int num_niches) {num_niches_ = num_niches;}
		void setNicheSizesSum(int niche_sizes_sum) {niche_sizes_sum_ = niche_sizes_sum;}
		void setAvgNicheSize(double avg_niche_size) {avg_niche_size_ = avg_niche_size;}
		void setAccuracy(double accuracy) {accuracy_ = accuracy;}
		void setFitness(double fitness) {fitness_ = fitness;}
		void setID(int id) {id_ = id;}

	private:

		// a number that uniquely identifies the rule
		int id_;

		// the class into which any input matching the condition is
		// to be categorized, according to the rule
		int classification_;

		// the last generation on which this rule participated in an
		// iteration of the GA
		int time_stamp_;

		// the number of times this rule's parameters have been updated
		int exp_;

		// the number of copies of this rule in the population set [P]
		int numerosity_;

		// the number of inputs that this rule matches
		int num_matches_;

		// the number of inputs that this rule both matches AND correctly
		// classifies
		int num_correct_;

		// the number of attributes in the condition of this rule whose
		// dontCare variable is set to true
		int num_dont_care_;

		// the number of distinct correct sets to which this rule has
		// belonged
		int num_niches_;

		// the sum of the sizes of all niches to which this rule has
		// belonged
		int niche_sizes_sum_;

		// the average size of the correct sets to which this rule has
		// belonged
		double avg_niche_size_;

		// the rule's accuracy in correctly classifying inputs
		double accuracy_;

		// the rule's fitness (used in the GA; based on accuracy)
		double fitness_;

};

#endif
