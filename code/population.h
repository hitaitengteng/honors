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

using namespace std;

class Population {

	public:

	// MEMBER VARIABLES (these should definitely be private)

		vector<Rule> rules; // the set of rules
		int id_count;

	// FUNCTIONS
	
		// default constructor
		Population() : id_count(0), fitnessSum(0) {  }

		// custom constructor
		Population(int maxSize) : id_count(0), maxSize(maxSize), fitnessSum(0) {  }

		// function for adding a rule to the population
		void add(Rule r);

		// deletes superfluous rules
		int subsume();

		// reset the population
		void clear() {
			rules.clear();
			fitnessSum = 0;
			mostGeneral = Rule();
		}

		// indicates whether the population is empty
		bool empty() {
			return (rules.size() == 0);
		}

		// selects a rule from the population using roulette
		// wheel selection and returns its index
		int rouletteWheelSelect(mt19937 &rng);

		// indicates whether at least one rule in the population
		// matches a given input
		bool matchExists(vector<double> &input) const;

		// creates two new rules using single-point crossover
		pair<Rule,Rule> crossover(int i, int j, mt19937 &rng);

		// prints all the rules in the population
		void inline print() {
			for (size_t i=0; i<rules.size(); i++) {
				rules[i].print();	
			}
		}

		// getters
		int getSize() const {return rules.size();}
		double getFitnessSum() const {return fitnessSum;}
		int getMaxSize() const {return maxSize;}
		Rule getMostGeneral() {return mostGeneral;}

		// setters
		void setMaxSize(int maxSize) {this->maxSize = maxSize;}
		void setFitnessSum(int fitnessSum) {this->fitnessSum = fitnessSum;}
		void setMostGeneral(Rule mostGeneral) {this->mostGeneral = mostGeneral;}

	private:
		int maxSize;        // the maximum number of rules allowed
		double fitnessSum;  // the sum of all the fitnesses
		Rule mostGeneral;   // the most general rule in the population
				    // (the one with the most "don't care" attributes)
};

#endif
