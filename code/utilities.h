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
#include <chrono>
#include <math.h>

// a random number generator
extern std::mt19937 rng;

// a random device
extern std::random_device rd;

// a uniform distribution over [0,1]
extern std::uniform_real_distribution<double> real_dist;
extern std::uniform_int_distribution<int> int_dist;

// class attribute (yield change) values
static const int NO_CLASS = 0;
static const int HIGH_NEG = 1;
static const int LOW_NEG = 2;
static const int LOW_POS = 3;
static const int HIGH_POS = 4;

// population types
static const int GENERAL = 1;
static const int MATCH_SET = 2;
static const int CORRECT_SET = 3;

static const int NO_ID = -1;

// for testing
static const int NUM_CLASSES = 5;
static const int NUM_TEST_NAMES = 10;
static const int NUM_TEST_ATTRIBUTES = 10;
static const std::string TEST_NAMES[] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon",
			           "Zeta", "Eta", "Theta", "Iota", "Kappa"};
static const std::string CLASS_NAMES[] = {"NO_CLASS", "HIGH_NEG", "LOW_NEG", "LOW_POS", "HIGH_POS"};

#endif // end utilities.h
