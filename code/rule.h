#ifndef RULE_H
#define RULE_H

#include "utilities.h"
#include "attribute.h"

/****************************************************************************
 * File: Rule.h
 * Author:
 * Description: Provides the class definition for the Rule class
 ****************************************************************************/ 

class Rule {

	public:

		// GETTERS AND SETTERS

	private:

		// the condition under which the rule applies
		AttributeVector condition;

		// the class into which any input matching the condition is
		// to be categorized, according to the rule
		int classification;

		// the number of times this rule's parameters have been updated
		int exp;

		// the number of copies of this rule in the population set [P]
		int num;

		// the rule's accuracy in correctly classifying inputs
		float acc;

		// the rule's fitness (used in the GA; based on accuracy)
		float fitness;

};

#endif
