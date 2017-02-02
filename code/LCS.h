#ifndef LCS_H
#define LCS_H

#include "population.h"
#include "dataset.h"

class LCS {

	public:
		// default constructor
		// complete constructor
		// getters and setters

	// MEMBER FUNCTIONS 
	
		// execute a single iteration of the genetic algorithm
		void applyGA();

		// the match set consists of all the rules in the population
		// matching a given input. The correct set consists of all
		// the rules in the match set that correctly classify the
		// input.
		void createMatchAndCorrectSets();

		// runs the crossover, mutation, and subsumption operators
		// as appropriate
		void reproduceAndReplace();

		// when no rule in the population matches an input, this function
		// creates one that does
		void cover();

		// eliminates superfluous rules
		void gaSubsume();

		// determines whether the GA should be invoked
		bool doGA();

		// determines whether the covering operator should be invoked
		bool doCover();
		
		// determines whether the specify operator should be invoked
		bool doSpecify();

		// processes a single input from the dataset
		void processInput(int i);

		// GETTERS AND SETTERS
		vector<double> getCurrDataPoint() {return _curr_data_point;}
		void setCurrDataPoint(vector<double> curr_data_point) {
			_curr_data_point = curr_data_point;
		}
	
	// MEMBER VARIABLES (these should probably be private)

		// the population of rules
		Population pop_;

		// the set of all rules matching a given input
		Population match_set_;

		// the set of all rules that both match a given input AND
		// correctly classify it
		Population correct_set_;

		// the data set used to train the LCS
		Dataset training_set_;

		// the data set used to test the LCS
		Dataset testing_set_;

	private:
		// the maximum allowable number of rules in the population
		int maxPopSize;

		// the exponent to which accuracy is raised in order to
		// compute fitness
		int fitnessExponent;

		// the probability that crossover will occur on an iteration
		// of the GA
		double pCrossover;

		// the probability of a single attribute in a rule being
		// mutated
		double pMutate;

		// if an attribute is to be mutated, the probability that
		// it is mutated to a "don't care" value
		double pDontCare;

		// the threshold accuracy value for a rule to be considered accurate
		double thetaAcc;

		// a rule's experience must exceed this value in order to
		// be able to subsume another rule
		double thetaSub;

		// NEEDS A DESCRIPTION
		double thetaDel;

		// the fitness value above which a rule may not be deleted
		double thetaFit;

		// the number of actions represented in the match set below
		// which the covering operator may be invoked
		double thetaMna;

		// the maximum number of time steps that may pass before the
		// GA is invoked
		double _theta_ga;

		// a scalar that determines the range from which random attribute
		// values may be selected
		double rangeScalar;

		// the current datum being processed
		vector<double> curr_data_point_;

		// indicates whether the GA subsumption operator should be used
		bool doGASubsumption;

		// indicates whether the correct set subsumption operator should
		// be used
		bool doCorrectSetSubsumption;
};

#endif
