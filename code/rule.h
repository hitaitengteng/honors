#ifndef RULE_H
#define RULE_H

#include "utilities.h"
#include "dataset.h"

/****************************************************************************
 * File:        Rule.h
 * Author:      William Gantt
 * Description: Defines the Rule class.
 *
 * TODO:
 ****************************************************************************/ 

using namespace std;

class Rule {

	public:

	// MEMBER VARIABLES
	
		// the condition under which the rule applies (NOTE: this should
		// probably be private, but I was feeling too lazy to make wrapper
		// functions)
		vector<Attribute> condition;

	// FUNCTIONS
	
		// mutates the class and attribute ranges in the condition with
		// some probability
		void mutate(double pMutate, double pDontCare, vector<pair<double,double>> ranges, 
				double rangeScalar, mt19937 &rng);

		// assigns particular values to all "don't care" attributes based
		// on the passed input vector
		void specify(vector<double> input, vector<pair<double,double>> ranges, 
				double rangeScalar, mt19937 &rng);
	
		// prints out information about a rule
		void print();
	
		// getters 
		vector<Attribute> getCondition() {return condition;}
		int getClass() {return classification;}
		int getTimeStamp() {return timeStamp;}
		int getExp() {return exp;}
		int getNumerosity() {return numerosity;}
		int getNumMatches() {return numMatches;}
		int getNumCorrect() {return numCorrect;}
		int getNumDontCare() {return numDontCare;}
		int getNumNiches() {return numNiches;}
		int getNicheSizesSum() {return nicheSizesSum;}
		double getAvgNicheSize() {return avgNicheSize;}
		double getAccuracy() {return accuracy;}
		double getFitness() {return fitness;}

		// setters
		void setCondition(vector<Attribute> condition) {this->condition = condition;}
		void setClass(int classification) {this->classification = classification;}
		void setTimeStamp(int timeStamp) {this->timeStamp = timeStamp;}
		void setExp(int exp) {this->exp = exp;}
		void setNumerosity(int numerosity) {this->numerosity = numerosity;}
		void setNumMatches(int numMatches) {this->numMatches = numMatches;}
		void setNumCorrect(int numCorrect) {this->numCorrect = numCorrect;}
		void setNumDontCare(int numDontCare) {this->numDontCare = numDontCare;}
		void setNumNiches(int numNiches) {this->numNiches = numNiches;}
		void setNicheSizesSum(int nicheSizesSum) {this->nicheSizesSum = nicheSizesSum;}
		void setAvgNicheSize(double avgNicheSize) {this->avgNicheSize = avgNicheSize;}
		void setAccuracy(double accuracy) {this->accuracy = accuracy;}
		void setFitness(double fitness) {this->fitness = fitness;}

	private:

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
