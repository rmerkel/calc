//
//  math.cpp
//  calc
//
//  Created by Randy Merkel on 5/19/13.
//  Copyright (c) 2013 Randy Merkel. All rights reserved.
//

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <random>
#include <string>

#include "math.h"
#include "utils.h"

/// Check result of standard library call
static double errcheck(double d, const std::string& s) {
	if (EDOM == errno) {
		errno = 0;
		error(s, "argument out of domain");

	} else if (ERANGE == errno) {
		errno = 0;
		error(s, "result out of range");

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
	return errcheck(std::log(x), "log");
}

double Log10(double x) {
	return errcheck(std::log10(x), "log");
}

double Exp(double x) {
	return errcheck(std::exp(x), "exp");
}

double Sqrt(double x) {
	return errcheck(std::sqrt(x), "sqrt");
}

double Pow(double x, double y) {
	return errcheck(std::pow(x, y), "exponentiation");
}

double integer(double x) {
	return static_cast<double> (int(x));
}