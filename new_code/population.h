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
			num_iters_ = 0;
			target_class_ = 0;
			default_class_ = 0;
			fitness1_sum_ = 0;
			fitness2_sum_ = 0;
			elitism_rate_ = 0;
			mutate_prob_ = 0;
			dont_care_prob_ = 0;
		}

		// custom constructor
		Population(int max_size) {

			id_count_ = 0;
			max_size_ = max_size;
			num_iters_ = 0;
			target_class_ = 0;
			default_class_ = 0;
			fitness1_sum_ = 0;
			fitness2_sum_ = 0;
			elitism_rate_ = 0;
			mutate_prob_ = 0;
			dont_care_prob_ = 0;
		}

		// custom constructor
		Population(int max_size, 
				int num_iters,
				int target_class,
				int default_class,
				double elitism_rate,
				double mutate_prob,
				double dont_care_prob,
				Dataset training_set, 
				Dataset test_set
			) {

		      	id_count_ = 0;
		      	fitness1_sum_ = 0;
		      	fitness2_sum_ = 0;

       	       		max_size_ = max_size;	       
			num_iters_ = num_iters;
			target_class_ = target_class;
			default_class_ = default_class;
			elitism_rate_ = elitism_rate;
			mutate_prob_ = mutate_prob;
			dont_care_prob_ = dont_care_prob;

		       	training_set_ = training_set;
		       	test_set_ = test_set;

		}

		// adds a rule to the population
		void add(Rule r);

		// removes a rule from the population
		void remove(int index);

		// resets the population
		void reset() {

			// the fitness sums must be recomputed at each iteration
			fitness2_sum_ = 0;

			// reset the array that keeps track of the covered examples
			int num_examples = training_set_.num_data_points();
			if (!training_set_.empty()) {
				for (int i=0; i<num_examples; i++)
					training_set_.examples_covered_[i] = false;
			}
		}

		// the comparison function used to rank the population by fitness1
		static bool ranking_function1(Rule r1, Rule r2) {
			return r1.fitness1() > r2.fitness1();
		}

		// the comparison function used to rank the population by fitness1
		static bool ranking_function2(Rule r1, Rule r2) {
			return r1.fitness2() > r2.fitness2();
		}
		// order the population by fitness1
		void rankByFitness1() {
			std::sort(rules_.begin(), rules_.end(), ranking_function1);
		}

		// order the population by fitness2
		void rankByFitness2() {
			std::sort(rules_.begin(), rules_.end(), ranking_function2);
		}

		// select a rule from the population using roulette wheel selection
		int rouletteWheelSelect();

		// calculate fitness1 for all rules
		void evaluateFitness1();

		// calculate fitness2 for all rules
		void evaluateFitness2();
		
		// choose parents for crossover using stochastic universal sampling
		std::vector<int> sus(int num_to_select);

		// creates two new rules using single-point crossover
		std::pair<Rule,Rule> crossover(int i, int j);

		// applies both genetic operators
		std::vector<Rule> crossoverAndMutate(std::vector<int> selected);

		// executes a single run of the genetic algorithm
		void applyGA();

		// generate a random population from a data set
		static Population random(int pop_size,
						int num_iters,
						int target_class,
						int default_class,
						double elitism_rate,
						double mutate_prob,
						double dont_care_prob,
						Dataset training_set, 
						Dataset test_set);

		// categorizes inputs in the test set based on the rules
		// in the current population
		double classify(Dataset* d, std::string output_file);

		// returns the size of the population
		int size() {return rules_.size();}

		// indicates whether the population is empty
		bool empty() {return (rules_.size() == 0);}

		// prints all the rules in the population
		void print() {
			int num_rules = rules_.size();
			for (int i=0; i<num_rules; i++) {
				rules_[i].print();	
			}
		}

		// prints all the rules in the population with additional details
		void printVerbose() {
			int num_rules = rules_.size();
			for (int i=0; i<num_rules; i++) {
				rules_[i].printVerbose();	
			}
		}

		// print elites
		void printElites() {
			rankByFitness2();
			int num_elites = 0;
			for (int i=0; rules_[i].true_positives() > 0.5; i++)
				num_elites++;
				
			for (int i=0; i<num_elites; i++)
				rules_[i].printVerbose();
		}

		// output data from a complete run of the LCS to a file
		void writeRunData(std::string training_file,
				  std::string testing_file,
				  std::string quantiles_file,
				  std::string output_file);

		// getters
		int size() const {return rules_.size();}
		int max_size() const {return max_size_;}
		int num_iters() const {return num_iters_;}
		int target_class() const {return target_class_;}
		int default_class() const {return default_class_;}
		double fitness1_sum() const {return fitness1_sum_;}
		double fitness2_sum() const {return fitness2_sum_;}
		double mutate_prob() const {return mutate_prob_;}
		double elitism_rate() const {return elitism_rate_;}
		double dont_care_prob() const {return dont_care_prob_;}

		// setters
		void setMaxSize(int max_size) {max_size_ = max_size;}
		void setNumIters(int num_iters) {num_iters_ = num_iters;}
		void setTargetClass(int target_class) {target_class_ = target_class;}
		void setDefualtClass(int default_class) {default_class_ = default_class;}
		void setFitness1Sum(double fitness1_sum) {fitness1_sum_ = fitness1_sum;}
		void setFitness2Sum(double fitness2_sum) {fitness2_sum_ = fitness2_sum;}
		void setMutateProb(double mutate_prob) {mutate_prob_ = mutate_prob;}
		void setElitismRate(double elitism_rate) {elitism_rate_ = elitism_rate;}
		void setDontCareProb(double dont_care_prob) {dont_care_prob_ = dont_care_prob;}

	private:
		int max_size_;         		
		int num_iters_;
		int target_class_;
		int default_class_;
		double fitness1_sum_;   
		double fitness2_sum_;
		double mutate_prob_;
		double elitism_rate_;
		double dont_care_prob_;
};

#endif
