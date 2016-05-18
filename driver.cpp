/**	@file driver.cpp
 *
 *	@brief	class Driver implementation
 *
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#include <iostream>
#include <limits>

#include "driver.h"

// private:

/// Return the file name minus the path
std::string Driver::fileName(const std::string path) {
	std::string::size_type n = path.find_last_of('/');
	if (std::string::npos == n)
		n = path.find_last_of('\\');

	return std::string::npos != n ? path.substr(n+1, std::string::npos) : path;
}

// public:

/** Constructor
 *
 * @param	name 	The parsers name
 */
Driver::Driver(const std::string& name) : ts{*this, std::cin}, parser{*this}, progName{fileName(name)} {
	nErrors = 0;
	lineNum = 1;
}

/// Report an error and return NaN
double Driver::error(const std::string& s) {
	std::cerr << progName << ": " << s << " near line " << lineNum << std::endl;

	++nErrors;
	return std::numeric_limits<double>::quiet_NaN();
}

/// Report an error and return NaN
double Driver::error(const std::string& s, char ch) {
	return error(s + " \'" + ch + "\'");
}

/// Report an error and return NaN
double Driver::error(const std::string& s, const std::string& t) {
	return error(s + " \'" + t + "\'");
}
