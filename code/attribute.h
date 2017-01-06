#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "utilities.h"

/****************************************************************************
 * File:        attribute.h
 * Author:      Will Gantt
 * Description: Attribute class definition
 ****************************************************************************/ 
class Attribute {

	public:
		Attribute(std::string name): 
			name(name), val(0), op(NO_OP) {
		}

		Attribute(std::string name, float val): 
			name(name), val(val), op(NO_OP)	{
		}

		Attribute(std::string name, float val, int op):
			name(name), val(val), op(op)	{
		}

		~Attribute() { }

		// getters 
		std::string getName() const {return name;}
		float getVal() const {return val;}
		int getOperator() const {return op;}

		// setters
		void setVal(float val) {this->val = val;}
		void setOperator(int op) {this->op = op;}

	private:
		std::string name; // the name of the attribute
		float val;       // the value of the attribute
		int op;      // (for rules only) the operator (<,<=,>,>=,==)
		             // used to specify a rule
};

#endif
