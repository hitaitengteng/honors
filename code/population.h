#ifndef POPULATION_H
#define POPULATION_H

#include "rule.h"
/****************************************************************************
 * File:        population.h
 * Author:      William Gantt
 * Description: Defines the Population class.
 *
 * TODO:
 * 	- make id_count private
 ****************************************************************************/ 

class Population {

	public:

	// MEMBER VARIABLES (these should definitely be private)

		std::vector<Rule> rules_; // the set of rules
		int id_count_;       // counts the number of IDs
				    // that have been used

	// FUNCTIONS
	
		// default constructor
		Population() : id_count_(0), fitness_sum_(0), exp_sum_(0) {  }

		// custom constructor
		Population(int max_size) : 
			id_count_(0), max_size_(max_size), fitness_sum_(0), exp_sum_(0) {  }

		// adds a rule to the population
		void add(Rule r);

		// removes a rule from the population
		void remove(int index);

		// deletes superfluous rules
		int subsume();

		// resets the population
		void clear() {
			rules_.clear();
			fitness_sum_ = 0;
			exp_sum_ = 0;
			most_general_ = Rule();
		}

		// indicates whether the population is empty
		bool empty() {
			return (rules_.size() == 0);
		}

		// selects a rule from the population using roulette
		// wheel selection and returns its index
		int rouletteWheelSelect();

		// selects a rule from the population using roulette
		// wheel selection based on average niche size
		int deletionSelect(double theta_fit);

		// indicates whether at least one rule in the population
		// matches a given input
		bool matchExists(std::vector<double> &input) const;

		// creates two new rules using single-point crossover
		std::pair<Rule,Rule> crossover(int i, int j);

		// generates a random population
		static Population random(int pop_size, int attributes_per_rule);

		// set the time stamps of all rules back to 0
		void resetTimeStamps() {
			int num_rules = rules_.size();
			for (size_t i=0; i<num_rules; i++) {
				rules_[i].setTimeStamp(0);
			}
		}

		// prints all the rules in the population
		void print() {
			int num_rules = rules_.size();
			for (size_t i=0; i<num_rules; i++) {
				rules_[i].print();	
			}
		}

		// getters
		int size() const {return rules_.size();}
		double fitness_sum() const {return fitness_sum_;}
		double exp_sum() const {return exp_sum_;}
		int max_size() const {return max_size_;}
		Rule most_general() {return most_general_;}

		// setters
		void setMaxSize(int max_size) {max_size_ = max_size;}
		void setFitnessSum(int fitness_sum) {fitness_sum_ = fitness_sum;}
		void setExpSum(double exp_sum) {exp_sum_ = exp_sum;}
		void setMostGeneral(Rule most_general) {most_general_ = most_general;}

	private:
		int type_;             // the type of population (general population,
				       // match set, or correct set);
		int max_size_;         // the maximum number of rules allowed
		double fitness_sum_;   // the sum of all the fitnesses
		double exp_sum_;       // the sum of all the experience values
		Rule most_general_;    // the most general rule in the population
				       // (the one with the most "don't care" attributes)
};

#endif
