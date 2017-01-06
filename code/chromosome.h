#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include "utilities.h"

/****************************************************************************
 * File:        chromosome.h
 * Author:      Will Gantt
 * Description: provides the Chromosome class definition, with functions for
 *              reading in chromosome data from a file and for manipulating
 *              chromosomes.
 ****************************************************************************/ 
class Chromosome {

	public:
		Chromosome(int numAttributes);
		~Chromosome();
		void readChromosomesFromTxtFile(string fileName);
		
		setValue(int index, int val);
		getValue(int index) {return attVect.at(i);}

	private:

		vector<float> attVect;

}; // end class definition

#endif // end chromosome.h
