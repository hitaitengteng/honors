#ifndef POPULATION_H
#define POPULATION_H

#include "rule.h"
/****************************************************************************
 * File:        population.h
 * Author:      William Gantt
 * Description: Defines the Population class.
 *
 * TODO:
 * 	- Implement constructors
 * 	- Implement getters and setters
 ****************************************************************************/ 

using namespace std;

class Population {

	public:

	// MEMBER VARIABLES
		vector<Rule> rules; // the set of rules

	// FUNCTIONS
	
		// default constructor
		Population() { }

		// custom constructor
		Population(int maxSize) : maxSize(maxSize) { 

			mostGeneral = new Rule();
		
		}

		// custom destructor
		~Population() {

			delete mostGeneral;
		}

		// function for adding a rule to the population
		void add(Rule r);

		// deletes superfluous rules
		int subsume();

		// selects a rule from the population using roulette
		// wheel selection and returns its index
		int rouletteWheelSelect(mt19937 &rng);

		// creates two new rules using single-point crossover
		pair<Rule,Rule> crossover(int i, int j, mt19937 &rng);

		// prints all the rules in the population
		void inline print() {
			for (int i=0; i<rules.size(); i++) {
				printf("\nRule %d\n--------", i);
				rules[i].print();	
			}
		}

		// getters
		int getSize() const {return rules.size();}
		double getFitnessSum() const {return fitnessSum;}
		int getMaxSize() const {return maxSize;}
		Rule *getMostGeneral() {return mostGeneral;}

		// setters
		void setMaxSize(int maxSize) {this->maxSize = maxSize;}
		void setFitnessSum(int fitnessSum) {this->fitnessSum = fitnessSum;}
		void setMostGeneral(Rule *mostGeneral) {this->mostGeneral = mostGeneral;}

	private:
		int maxSize;        // the maximum number of rules allowed
		double fitnessSum;  // the sum of all the fitnesses
		Rule *mostGeneral;  // the most general rule in the population
				    // (the one with the most "don't care"
				    // attributes
};

#endif
