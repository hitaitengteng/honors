#ifndef UTILITIES_H
#define UTILITIES_H

/****************************************************************************
 * File:        utilities.h
 * Author:      Will Gantt
 * Description: Contains all includes from external libraries
 ****************************************************************************/ 

#include <algorithm>     // sorting algorithms for vectors
#include <cfloat>        // constants for minimum and maximum float values
#include <chrono>        // for timing
#include <fstream>       // file I/O
#include <iostream>      // file I/O
#include <random>        // random number generation
#include <unordered_map> // hash maps
#include <vector>        // C++ vector class

// a random number generator
extern std::mt19937 rng;

// a random device
extern std::random_device rd;

// a uniform distribution over [0,1]
extern std::uniform_real_distribution<double> real_dist;
extern std::uniform_int_distribution<int> int_dist;

// class attribute (yield change) values
static const int NO_CLASS = -1;
static const int L = 0;
static const int M = 1;
static const int H = 2;

// named constants for true positives, false positives,
// true negatives, and false negatives
static const int TP = 1;
static const int TN = 2;
static const int FP = 3;
static const int FN = 4;

// fitness boost for each "don't care" value
// possessed by a rule
static const double FITNESS_BOOST = 0.01;

// the default ID value for a rule
static const int NO_ID = -1;

#endif // end utilities.h
