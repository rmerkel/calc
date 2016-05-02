//
//  utils.h
//  calc
//
//  Created by Randy Merkel on 5/19/13.
//  Copyright (c) 2013 Randy Merkel. All rights reserved.
//

#ifndef calc_utils_h
#define calc_utils_h

#include <string>

extern	std::string	ProgName;			/// Name of this program
extern	unsigned	NumErrors;			/// # of errors seen
extern	unsigned	LineNum;			/// Current input line number

/// Report an error
double error(const std::string& s);

/// Report an error
double error(const std::string& s, char ch);

/// Report an error
double error (const std::string& s, const std::string& t);

/// Return the file name the path
std::string fileName (const std::string path);

#endif
