/** @file math.cpp
 *
 *	@brief	Math built-in implementation
 *
 *	Implementation of the math utilites; see errcheck(d,s).
 *
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#include <cerrno>
#include <cmath>
#include <limits>
#include <random>

#include "math.h"

/// Check result of standard library call
static double errcheck(double d) {
	if (0 != errno) {
		errno = 0;
		return std::numeric_limits<double>::quiet_NaN();
	}

	return d;
}

// Random numbers - should use <chrono> for the seed.
static	std::default_random_engine generator (static_cast<double> (clock()));
static	std::uniform_real_distribution<double> distribution(0,1);

double Rand() {	// return psudo random value n the range 0-1
	return distribution(generator);
}

double Log(double x) {
	return errcheck(std::log(x));
}

double Log10(double x) {
	return errcheck(std::log10(x));
}

double Exp(double x) {
	return errcheck(std::exp(x));
}

double Sqrt(double x) {
	return errcheck(std::sqrt(x));
}

double Pow(double x, double y) {
	return errcheck(std::pow(x, y));
}

double Integer(double x) {
	return static_cast<double> (int(x));
}
