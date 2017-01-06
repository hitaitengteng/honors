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

	private:
		int classification;
		AttributeVector antecedent;
		float fitness;


};

#endif
