#ifndef RULE_H
#define RULE_H

#include "attribute.h"

/****************************************************************************
 * File:        rule.h
 * Author:      Will Gantt
 * Description: Defines the Rule class and declares related functions
 *
 * 		A rule is the basic evolutionary unit in a Michigan-style
 * 		learning classifier system (like this one), and it has
 * 		two fundamental components: a condition and a predicted class.
 *
 * 		The condition specifies the features an example (input vector)
 * 		must have in order for the rule to match it. These 'features'
 * 		are just ranges of values for particular attributes. Thus,
 * 		the data structure here used to represent a condition is
 * 		a vector of Attribute objects.
 *
 * 		The rule's class determines how examples matching the rule
 * 		will be classified. A given rule has just a single class.
 * 		In its current iteration, this program tries to describe the
 * 		features of just one class at a time. Therefore, all of the
 * 		rules in a Population have the same class. This could easily
 * 		be changed in future experiments.
 *
 * 		The Rule class also has several other member variables. The
 * 		ID (id_) is an integer value that uniquely identifies a rule.
 * 		'num_dont_care_' indicates the number of attributes in the
 * 		rule's condition that have their 'dont_care_' variables set
 * 		to true.
 *
 * 		Additionally, rules keep track of the number of true positive,
 * 		false positive, true negative, and false negative examples.
 * 		The meanings of these terms are explained in the comments 
 * 		below.
 * 		
 * 		Finally, rules have two fitness values. The first is computed
 * 		from the number of true and false positives and negatives and
 * 		is used to establish a preliminary ranking of all the rules
 * 		in the Population. The second fitness is also computed from
 * 		the number of true and false positives and negatives, but
 * 		takes into account the ranking established by the first
 * 		fitness. The fitness sytem is explained in greater detail
 * 		below.
 ****************************************************************************/ 

class Rule {

	public:

	// MEMBER VARIABLES
	
		// A vector of attributes that determine the features of
		// the examples that the rule will match. Ideally, this 
		// would be a private member variable, but writing wrappers 
		// for all the relevant functions in the vector class is 
		// a pain.
		std::vector<Attribute> condition_;

	// FUNCTIONS
	
		// first constructor (default)
		Rule() { 

			 id_ = NO_ID;
			 classification_ = 0;
			 num_dont_care_ = 0;
			 true_positives_ = 0.5;
			 false_positives_ = 0.5;
			 true_negatives_ = 0.5;
			 false_negatives_ = 0.5;
		}

		// second constructor (initializes the condition vector)
		Rule(int num_attributes) { 

			 id_ = NO_ID;
			 classification_ = 0;
			 num_dont_care_ = 0;
			 true_positives_ = 0.5;
			 false_positives_ = 0.5;
			 true_negatives_ = 0.5;
			 false_negatives_ = 0.5;	 

			 // initialize the condition vector
			 Attribute a;
			 for (int i=0; i<num_attributes; i++)
				 condition_.push_back(a);
		}

		// equality operator
		bool operator==(const Rule &rule) const;

		// computes fitness1 using odds ratio. Odds ratio is
		// just the product of the number of true positives and 
		// true negatives divided by the product of the number
		// of the false positives and false negatives:
		//
		// 	(|TP| x |TN|) / (|FP| x |FN|)
		//
		void updateFitness1() {

			fitness1_ = (true_positives_ * true_negatives_) /
				(false_positives_ * false_negatives_);

			// for each "don't care" value that the rule has, 
			// it receives a slight fitness boost
			fitness1_ += num_dont_care_ * FITNESS_BOOST;
		}

		// computes fitness2 using odds ratio
		void updateFitness2() {

			fitness2_ = (true_positives_ * true_negatives_) /
					(false_positives_ * false_negatives_);

			// for each "don't care" value that the rule has,
			// it receives a slight fitness boost
			fitness2_ += num_dont_care_ * FITNESS_BOOST;
		}


		// resets the counts of true positives, false positives, 
		// true negatives, and false negatives
		void resetCounts() {

			true_positives_ = 0.5;
			false_positives_ = 0.5;
			true_negatives_ = 0.5;
			false_negatives_ = 0.5;

		}

		// determines whether a given input should count as a true positive,
		// true negative, false positive, or false negative for the rule
		int processInput(std::vector<double> &input);

		// mutates the class and attribute ranges in the condition with
		// some probability
		void mutate(double p_mutate, 
			    double p_dont_care, 
			    std::vector< std::vector<double> > quantiles);

		// assigns particular values to all "don't care" attributes based
		// on the passed input vector
		void specify(std::vector<double> input, 
			     std::vector< std::vector<double> > quantiles);
	
		// checks whether this rule is a more general version of another
		bool generalizes(Rule &rule) const;

		// checks whether this rule matches an input datum
		bool matches(std::vector<double> input);

		// generate a random rule, supposing you already have some 
		// information about the dataset that you're working with
		static Rule random(int num_classes, 
				std::vector<std::vector<double> > quantiles,
				double dont_care_prob);

		// prints out some information about a rule
		void print();

		// prints out complete information about a rule
		void printVerbose();
	
		//
		// getters 
		//

		int id() const {return id_;}
		int classification() const {return classification_;}
		double true_positives() const {return true_positives_;}
		double false_positives() const {return false_positives_;}
		double true_negatives() const {return true_negatives_;}
		double false_negatives() const {return false_negatives_;}
		int num_dont_care() const {return num_dont_care_;}
		double fitness1() const {return fitness1_;}
		double fitness2() const {return fitness2_;}

		//
		// setters
		//

		void setID(int id) {
			id_ = id;
		}
		void setClass(int classification) {
			classification_ = classification;
		}
		void setTruePositives(double true_positives) {
			true_positives_ = true_positives;
		}
		void setFalsePositives(double false_positives) {
			false_positives_ = false_positives;
		}
		void setTrueNegatives(double true_negatives) {
			true_negatives_ = true_negatives;
		}
		void setFalseNegatives(double false_negatives) {
			false_negatives_ = false_negatives;
		}
		void setNumDontCare(int num_dont_care) {
			num_dont_care_ = num_dont_care;
		}
		void setFitness1(double fitness1) {
			fitness1_ = fitness1;
		}
		void setFitness2(double fitness2) 
		{fitness2_ = fitness2;
		}

	private:

		/* 
		 * Note that the variable type for the number of TPs, 
		 * TNs, FPs, and FNs is double. This is so that they
		 * may be initialized with a value of 0.5, which is a
		 * way to avoid having to check for division by 0 when 
		 * computing the odds ratio.
		 */

		// a number that uniquely identifies the rule
		int id_;

		// the class into which any input matching the condition is
		// to be categorized, according to the rule
		int classification_;

		// the number of examples correctly classified by the rule
		double true_positives_;

		// the number of examples that the rule matches but does
		// not correctly classify
		double false_positives_;

		// the number of examples that the rule neither matches
		// nor correctly classifies
		double true_negatives_;

		// the number of examples that the rule does not match,
		// but that have the same class as the rule
		double false_negatives_;

		// the number of attributes in the condition of this rule 
		// whose dont_care_ variable is set to true
		int num_dont_care_;

		// the first fitness is used to establish a preliminary
		// ranking of the rules used to compute the 2nd fitness,
		// which drives the GA. This first fitness is calculated
		// using odds ratio
		double fitness1_;

		// the second fitness is also calculated using odds ratio,
		// but in computing the 2nd fitness, we count an example
		// as a true positive only for the fittest rule that covers
		// it
		double fitness2_;

};

#endif
