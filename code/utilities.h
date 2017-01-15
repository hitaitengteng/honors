#ifndef UTILITIES_H
#define UTILITIES_H

/****************************************************************************
 * File:        utilities.h
 * Author:      Will Gantt
 * Description: Includes basic include files, named constants, and various 
 *              functions for reading and writing to files.
 ****************************************************************************/ 

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>

// class attribute (yield change) values
static const int NO_CLASS = 0;
static const int HIGH_NEG = 1;
static const int LOW_NEG = 2;
static const int LOW_POS = 3;
static const int HIGH_POS = 4;

#endif // end utilities.h
