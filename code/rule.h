#ifndef RULE_H
#define RULE_H

#include "utilities.h"
#include "attribute.h"

/****************************************************************************
 * File:        Rule.h
 * Author:      William Gantt
 * Description: Defines the Rule class.
 *
 * TODO:
 * 	- Implement constructors
 * 	- Implement getters and setters
 * 	- Implement vector wrapper functions
 ****************************************************************************/ 

using namespace std;

class Rule {

	public:

		// CONSTRUCTORS
		// DESTRUCTOR
		// mutates the class and attribute ranges in the condition with
		// some probability
		void mutate(double pMutate, double pDontCare, double rangeScalar);

		// assigns particular values to all "don't care" attributes based
		// on the passed input vector
		void specify(vector<double> input);

		// GETTERS AND SETTERS

	private:

		// the condition under which the rule applies
		vector<Attribute> condition;

		// the class into which any input matching the condition is
		// to be categorized, according to the rule
		int classification;

		// the last generation on which this rule participated in an
		// iteration of the GA
		int timeStamp;

		// the number of times this rule's parameters have been updated
		int exp;

		// the number of copies of this rule in the population set [P]
		int numerosity;

		// the number of inputs that this rule matches
		int numMatches;

		// the number of inputs that this rule both matches AND correctly
		// classifies
		int numCorrect;

		// the number of attributes in the condition of this rule whose
		// dontCare variable is set to true
		int numDontCare;

		// the number of distinct correct sets to which this rule has
		// belonged
		int numNiches;

		// the sum of the sizes of all niches to which this rule has
		// belonged
		int nicheSizesSum;

		// the average size of the correct sets to which this rule has
		// belonged
		double avgNicheSize;

		// the rule's accuracy in correctly classifying inputs
		double accuracy;

		// the rule's fitness (used in the GA; based on accuracy)
		double fitness;

};

#endif
