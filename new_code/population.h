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
		Dataset training_set_;    // the training set
		Dataset test_set_;        // the test set
		int curr_gen_;		  // the current generation of the GA
		int id_count_;            // counts the number of IDs
				          // that have been used

	// FUNCTIONS
	
		// default constructor
		Population() { 

			id_count_ = 0;
			max_size_ = 0;
			target_class_ = 0;
			fitness1_sum_ = 0;
			fitness2_sum_ = 0;
			elitism_rate_ = 0;
			crossover_prob_ = 0;
			mutate_prob_ = 0;
			dont_care_prob_ = 0;
			range_scalar_ = 0;
		}

		// custom constructor
		Population(int max_size) {

			id_count_ = 0;
			max_size_ = max_size;
			target_class_ = 0;
			fitness1_sum_ = 0;
			fitness2_sum_ = 0;
			elitism_rate_ = 0;
			crossover_prob_ = 0;
			mutate_prob_ = 0;
			dont_care_prob_ = 0;
			range_scalar_ = 0;
		}

		// custom constructor
		Population(int max_size, 
				int target_class,
				double elitism_rate,
				double crossover_prob,
				double mutate_prob,
				double dont_care_prob,
				double range_scalar,
				Dataset training_set, 
				Dataset test_set
				) {

		      	id_count_ = 0;
		      	fitness1_sum_ = 0;
		      	fitness2_sum_ = 0;

       	       		max_size_ = max_size;	       
			target_class_ = target_class;
			elitism_rate_ = elitism_rate;
			crossover_prob_ = crossover_prob;
			mutate_prob_ = mutate_prob;
			dont_care_prob_ = dont_care_prob;
			range_scalar_ = range_scalar;
		       	training_set_ = training_set;
		       	test_set_ = test_set;

		}

		// adds a rule to the population
		void add(Rule r);

		// removes a rule from the population
		void remove(int index);

		// resets the population
		void clear() {
			rules_.clear();
			fitness1_sum_ = 0;
			fitness2_sum_ = 0;
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

		// calculate fitness1 for all rules
		void evaluateFitness1();

		// calculate fitness2 for all rules
		void evaluateFitness2();

		// returns the size of the population
		int size() {return rules_.size();}

		// indicates whether the population is empty
		bool empty() {return (rules_.size() == 0);}

		// selects the rules that will breed to produce the next generation
		void select();

		// executes the genetic operators on the appropriate rules
		void crossoverAndMutate();

		// creates two new rules using single-point crossover
		std::pair<Rule,Rule> crossover(int i, int j);

		// generate a random population
		static Population random1(int pop_size, int attributes_per_rule, int num_classes);

		// generate a random population from a data set
		static Population random2(int pop_size,
						int target_class,
						double elitism_rate,
						double crossover_prob,
						double mutate_prob,
						double dont_care_prob,
						double range_scalar, 
						Dataset training_set, 
						Dataset test_set);

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
		int max_size() const {return max_size_;}
		int target_class() const {return target_class_;}
		double fitness1_sum() const {return fitness1_sum_;}
		double fitness2_sum() const {return fitness2_sum_;}
		double mutate_prob() const {return mutate_prob_;}
		double crossover_prob() const {return crossover_prob_;}
		double elitism_rate() const {return elitism_rate_;}
		double dont_care_prob() const {return dont_care_prob_;}
		double range_scalar() const   {return range_scalar_;}

		// setters
		void setMaxSize(int max_size) {max_size_ = max_size;}
		void setTargetClass(int target_class) {target_class_ = target_class;}
		void setFitness1Sum(double fitness1_sum) {fitness1_sum_ = fitness1_sum;}
		void setFitness2Sum(double fitness2_sum) {fitness2_sum_ = fitness2_sum;}
		void setMutateProb(double mutate_prob) {mutate_prob_ = mutate_prob;}
		void setCrossoverProb(double crossover_prob) {crossover_prob_ = crossover_prob;}
		void setElitismRate(double elitism_rate) {elitism_rate_ = elitism_rate;}
		void setDontCareProb(double dont_care_prob) {dont_care_prob_ = dont_care_prob;}
		void setRangeScalar(double range_scalar) {range_scalar_ = range_scalar;}

	private:
		int max_size_;         		// the maximum number of rules allowed
		int target_class_;
		double fitness1_sum_;   	// the sum of all the fitnesses
		double fitness2_sum_;
		double mutate_prob_;
		double crossover_prob_;
		double elitism_rate_;
		double dont_care_prob_;
		double range_scalar_;
};

#endif