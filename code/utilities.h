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

using namespace std;

// a random number generator
extern mt19937 rng;

// a random device
extern random_device rd;

// a uniform distribution over [0,1]
extern uniform_real_distribution<double> real_dist;
extern uniform_int_distribution<int> int_dist;

// class attribute (yield change) values
static const int NO_CLASS = 0;
static const int HIGH_NEG = 1;
static const int LOW_NEG = 2;
static const int LOW_POS = 3;
static const int HIGH_POS = 4;

static const int NUM_CLASSES = 5;
static const int NUM_TEST_NAMES = 10;

// for testing
static const string test_names[] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon",
			           "Zeta", "Eta", "Theta", "Iota", "Kappa"};
static const string class_names[] = {"NO_CLASS", "HIGH_NEG", "LOW_NEG", "LOW_POS", "HIGH_POS"};

#endif // end utilities.h
