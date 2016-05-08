/** @file math.h
 *
 *	@brief	Built in math functions.
 *
 *	Built in math functiions, most with error checking: returns NaN if the 
 *	underlying library function returns EDOM or ERANGE.
 *
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#ifndef calc_math_h
#define calc_math_h

/// return psudo random value n the range 0-1
double Rand();

/// Returns the natural logarithm of x, or NaN in case of error
double Log(double x);

/// Returns the common logarithm of x, or NaN in case of error
double Log10(double x);

/// Returns teh Exponential value of x, or NaN in case of error
double Exp(double);

/// Returns the Square-Root of x, or NaN in case of error
double Sqrt(double);

/// Returns x based to the y power, or Nan in case of error
double Pow(double x, double y);

///  Returns x as an integer, with out rounding
double Integer(double x);

#endif
