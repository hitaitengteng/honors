#ifndef UTILITIES_H
#define UTILITIES_H

/****************************************************************************
 * File:        utilities.h
 * Author:      Will Gantt
 * Description: Includes basic include files, named constants, and various 
 *              functions for reading and writing to files.
 ****************************************************************************/ 

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

// rule operators
static const int NO_OP = 0;
static const int EQUALS = 1;
static const int GREATER_THAN = 2;
static const int GREATER_THAN_OR_EQUAL_TO = 3;
static const int LESS_THAN = 4;
static const int LESS_THAN_OR_EQUAL_TO = 5;

#endif // end utilities.h
