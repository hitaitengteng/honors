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
 * 	- Implement vector wrapper functions
 ****************************************************************************/ 

using namespace std;

class Population {

	public:
		// CONSTRUCTORS & DESTRUCTORS
		// VECTOR WRAPPERS

		// deletes superfluous rules
		void subsume();

		// finds the most general rule in the population
		int findMostGeneral();

		// selects a rule from the population using roulette
		// wheel selection and returns its index
		int rouletteWheelSelect();

		// creates two new rules using single-point crossover
		pair<Rule,Rule> crossover(int i, int j);

	private:
		vector<Rule> rules; // the set of rules
		int size;           // the number of rules in the population
		int maxSize;        // the maximum number of rules allowed
};

#endif
