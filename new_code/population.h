#ifndef POPULATION_H
#define POPULATION_H

#include "rule.h"
#include "dataset.h"
/****************************************************************************
 * File:        population.h
 * Author:      William Gantt
 * Description: Defines the Population class.
 ****************************************************************************/ 

class Population {

	public:

	// MEMBER VARIABLES (these should definitely be private)

		std::vector<Rule> rules_; // the set of rules
		int id_count_;            // counts the number of IDs
				          // that have been used

	// FUNCTIONS
	
		// default constructor
		Population() : id_count_(0), fitness1_sum_(0) {  }

		// custom constructor
		Population(int max_size) : 
			id_count_(0), max_size_(max_size), fitness1_sum_(0) { }

		// adds a rule to the population
		void add(Rule r);

		// removes a rule from the population
		void remove(int index);

		// resets the population
		void clear() {
			rules_.clear();
			fitness1_sum_ = 0;
		}

		// the comparison function used to rank the population by fitness1
		static bool ranking_function(Rule r1, Rule r2) {
			return r1.fitness1() > r2.fitness1();
		}

		// order the population by fitness1
		void rank() {
			std::sort(rules_.begin(), rules_.end(), ranking_function);
		}

		// select a rule from the population using roulette wheel selection
		int rouletteWheelSelect();

		// go through all the inputs and determine which rules cover which
		// inputs
		void evaluate();

		// returns the size of the population
		int size() {return rules_.size();}

		// indicates whether the population is empty
		bool empty() {return (rules_.size() == 0);}

		// selects a rule from the population using roulette
		// wheel selection based on average niche size
		int deletionSelect(double theta_acc);

		// creates two new rules using single-point crossover
		std::pair<Rule,Rule> crossover(int i, int j);

		// generates a random population
		static Population random(int pop_size, int attributes_per_rule);

		// prints all the rules in the population
		void print() {
			int num_rules = rules_.size();
			for (size_t i=0; i<num_rules; i++) {
				rules_[i].print();	
			}
		}

		// prints all the rules in the population with additional detail
		void printVerbose() {
			int num_rules = rules_.size();
			for (size_t i=0; i<num_rules; i++) {
				rules_[i].printVerbose();	
			}
		}

		// getters
		int size() const {return rules_.size();}
		double fitness1_sum() const {return fitness1_sum_;}
		double fitness2_sum() const {return fitness2_sum_;}
		int max_size() const {return max_size_;}

		// setters
		void setMaxSize(int max_size) {max_size_ = max_size;}
		void setFitness1Sum(double fitness1_sum) {fitness1_sum_ = fitness1_sum;}
		void setFitness2Sum(double fitness2_sum) {fitness2_sum_ = fitness2_sum;}

	private:
		int max_size_;         		// the maximum number of rules allowed
		double fitness1_sum_;   	// the sum of all the fitnesses
		double fitness2_sum_;
};

#endif
