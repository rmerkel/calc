/** @file	utils.h
 *
 *	@brief	Varioius utilities, mostly dealing with errors. 
 *
 *	Created by Randy Merkel on 6/7/2013.
 *  Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#ifndef calc_utils_h
#define calc_utils_h

#include <string>

extern	std::string	ProgName;			///< Name of this program
extern	unsigned	NumErrors;			///< Number of errors seen to date
extern	unsigned	LineNum;			///< Current input line number

/// Report an error, returns NaN
double error(const std::string& s);

/// Report an error, returns NaN
double error(const std::string& s, char ch);

/// Report an error, returns NaN
double error (const std::string& s, const std::string& t);

/// Return the file name minus the path
std::string fileName (const std::string path);

#endif
