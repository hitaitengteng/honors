#ifndef LCS_H
#define LCS_H

#include "population.h"
#include "dataset.h"
#include "matchSet.h"
#include "correctSet.h"

/****************************************************************************
 * TODO:
 * 	- Write print function
 ****************************************************************************/
class LCS {

	public:
		LCS(Population p, Dataset training_set, Dataset test_set): 
				   curr_gen_(0), fitness_exponent_(1), 
				   p_crossover_(0.8), p_mutate_(0.04), 
				   p_dont_care_(0.33), theta_acc_(0.99), 
				   theta_sub_(0), theta_mna_(3), theta_ga_(25), 
				   range_scalar_(0.5), 
				   do_ga_subsumption_(true), 
				   do_correct_set_subsumption_(false)	{

			pop_ = p;
			training_set_ = training_set;
			test_set_ = test_set;
			match_set_ = MatchSet(&pop_, training_set_.num_classes());
			correct_set_ = CorrectSet(&pop_);
		}

	// MEMBER FUNCTIONS 
	
		// execute a single iteration of the genetic algorithm
		void applyGA();

		// the match set consists of all the rules in the population
		// matching a given input. The correct set consists of all
		// the rules in the match set that correctly classify the
		// input.
		void createMatchAndCorrectSets();

		// selects a rule from the correct set using roulette wheel selection
		int rouletteWheelSelect();

		// when no rule in the population matches an input, this function
		// creates one that does
		void cover();

		// update the accuracy and fitness of the population
		void fitnessUpdate();

		// eliminates superfluous rules
		void gaSubsume(int p1_index, int p2_index, Rule first_child, Rule second_child);

		// determines whether the GA should be invoked
		bool doGA(); 

		// determines whether the covering operator should be invoked
		bool doCover();
		
		// determines whether the specify operator should be invoked
		bool doSpecify();

		// processes a single input from the dataset
		void processInput(int i);

		// once the LCS has evolved a population of rules, this function
		// then classifies new inputs using those rules
		std::pair<int,int> *classifyInputs();

		void print();
	
	// MEMBER VARIABLES (these should probably be private)

		// Keeps track of the number of generations that have passed.
		// Note that this should never exceed the number of entries
		// in the data set, for obvious reasons
		int curr_gen_;

		// the population of rules
		Population pop_;

		// the set of all rules matching a given input
		MatchSet match_set_;

		// the set of all rules that both match a given input AND
		// correctly classify it
		CorrectSet correct_set_;

		// the data set used to train the LCS
		Dataset training_set_;

		// the data set used to test the LCS
		Dataset test_set_;

		// getters
		std::vector<double> curr_data_point() {return curr_data_point_;}
		int theta_mna() {return theta_mna_;}
		double range_scalar() {return range_scalar_;}

		// setters
		void setCurrDataPoint(std::vector<double> curr_data_point) {
			curr_data_point_ = curr_data_point;
		}
		void setThetaMNA(int theta_mna) {theta_mna_ = theta_mna;}

	private:

		// the exponent to which accuracy is raised in order to
		// compute fitness
		int fitness_exponent_;

		// the probability that crossover will occur on an iteration
		// of the GA
		double p_crossover_;

		// the probability of a single attribute in a rule being
		// mutated
		double p_mutate_;

		// if an attribute is to be mutated, the probability that
		// it is mutated to a "don't care" value
		double p_dont_care_;

		// the threshold accuracy value for a rule to be considered 
		// accurate
		double theta_acc_;

		// a rule's experience must exceed this value in order to
		// be able to subsume another rule
		double theta_sub_;

		// the number of actions represented in the match set below
		// which the covering operator may be invoked
		double theta_mna_;

		// if the average number of iterations that have passed since a
		// rule in [C] was last involved in a GA exceeds this value,
		// then the GA is invoked.
		double theta_ga_;

		// a scalar that determines the range from which random attribute
		// values may be selected
		double range_scalar_;

		// the current datum being processed
		std::vector<double> curr_data_point_;

		// indicates whether the GA subsumption operator should be used
		bool do_ga_subsumption_;

		// indicates whether the correct set subsumption operator should
		// be used
		bool do_correct_set_subsumption_;
};

#endif
