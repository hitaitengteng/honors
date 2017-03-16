#ifndef RULE_H
#define RULE_H

#include "attribute.h"
/****************************************************************************
 * File:        Rule.h
 * Author:      William Gantt
 * Description: Defines the Rule class.
 *
 * TODO:
 * 	- Need some way to avoid dividing by 0 when computing odds ratio
 ****************************************************************************/ 

class Rule {

	public:

	// MEMBER VARIABLES
	
		// the condition under which the rule applies (NOTE: this should
		// probably be private, but I was feeling too lazy to make wrapper
		// functions)
		std::vector<Attribute> condition_;

	// FUNCTIONS
	
		// first constructor
		Rule() : id_(NO_ID), classification_(0), true_positives_(0), 
			 false_positives_(0), true_negatives_(0), false_negatives_(0), num_dont_care_(0) { }

		// second constructor
		Rule(int num_attributes) : id_(NO_ID), classification_(0), true_positives_(0), 
			 false_positives_(0), true_negatives_(0), false_negatives_(0), num_dont_care_(0) {
		 
				 // initialize the condition vector
				 Attribute a;
				 for (int i=0; i<num_attributes; i++)
					 condition_.push_back(a);
		}

		// equality operator
		bool operator==(const Rule &rule) const;

		// computes fitness1 using odds ratio
		void updateFitness1() {

			fitness1_ = (double) (true_positives_ + true_negatives_) /
					(double) (false_positives_ + false_negatives_);
		}

		void processInput(std::vector<double> &input);

		// mutates the class and attribute ranges in the condition with
		// some probability
		void mutate(double p_mutate, 
				double p_dont_care, 
				std::vector<std::pair<double,double> > ranges, 
				double range_scalar);

		// assigns particular values to all "don't care" attributes based
		// on the passed input vector
		void specify(std::vector<double> input, 
				std::vector<std::pair<double,double> > ranges, 
				double range_scalar);
	
		// checks whether this rule is a more general version of another
		bool generalizes(Rule &rule) const;

		// checks whether this rule matches an input datum
		bool matches(std::vector<double> &input) const;

		// generate a random rule
		static Rule random(int num_attributes);
		
		// generate a random rule, supposing you already have some information
		// about the dataset that you're working with
		static Rule random(int num_attributes, int num_classes, 
				std::vector<std::pair<double,double> > attribute_ranges, 
				double range_scalar);

		// prints out some information about a rule
		void print();

		// prints out complete information about a rule
		void printVerbose();
	
		// getters 
		int id() const {return id_;}
		int classification() const {return classification_;}
		int true_positives() const {return true_positives_;}
		int false_positives() const {return false_positives_;}
		int true_negatives() const {return true_negatives_;}
		int false_negatives() const {return false_negatives_;}
		int num_dont_care() const {return num_dont_care_;}
		double fitness1() const {return fitness1_;}
		double fitness2() const {return fitness2_;}

		// setters
		void setID(int id) {id_ = id;}
		void setClass(int classification) {classification_ = classification;}
		void setTruePositives(int true_positives) {true_positives_ = true_positives;}
		void setFalsePositives(int false_positives) {false_positives_ = false_positives;}
		void setTrueNegatives(int true_negatives) {true_negatives_ = true_negatives;}
		void setFalseNegatives(int false_negatives) {false_negatives_ = false_negatives;}
		void setNumDontCare(int num_dont_care) {num_dont_care_ = num_dont_care;}
		void setFitness1(double fitness1) {fitness1_ = fitness1;}
		void setFitness2(double fitness2) {fitness2_ = fitness2;}

	private:

		// a number that uniquely identifies the rule
		int id_;

		// the class into which any input matching the condition is
		// to be categorized, according to the rule
		int classification_;

		int true_positives_;

		int false_positives_;

		int true_negatives_;

		int false_negatives_;

		// the number of attributes in the condition of this rule whose
		// dontCare variable is set to true
		int num_dont_care_;

		// the rule's fitness (used in the GA; based on accuracy)
		double fitness1_;

		double fitness2_;

};

#endif
