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
		Population(int size, int maxSize) : size(size), maxSize(maxSize) { }

		// deletes superfluous rules
		void subsume();

		// selects a rule from the population using roulette
		// wheel selection and returns its index
		int rouletteWheelSelect(mt19937 &rng);

		// creates two new rules using single-point crossover
		pair<Rule,Rule> crossover(int i, int j, mt19937 &rng);

		// getters
		int getSize() {return size;}
		int getMaxSize() {return maxSize;}
		Rule getMostGeneral() {return mostGeneral;}

		// setters
		void setSize(int size) {this->size = size;}
		void setMaxSize(int maxSize) {this->maxSize = maxSize;}
		void setMostGeneral(Rule mostGeneral) {
			this->mostGeneral = mostGeneral;
		}

	private:
		int size;           // the number of rules in the population
		int maxSize;        // the maximum number of rules allowed
		Rule mostGeneral;   // the most general rule in the population
				    // (the one with the most "don't care"
				    // attributes
};

#endif
