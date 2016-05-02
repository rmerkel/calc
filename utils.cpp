//
//  utils.cpp
//  calc
//
//  Created by Randy Merkel on 5/19/13.
//  Copyright (c) 2013 Randy Merkel. All rights reserved.
//

#include <iostream>

#include "utils.h"


std::string	ProgName 	= "calc";
unsigned	NumErrors	= 0;
unsigned	LineNum 	= 1;

double error(const std::string& s) {
	std::cerr << ProgName << ": " << s << " near line " << LineNum << std::endl;

	++NumErrors;
	return 1;
}

double error(const std::string& s, char ch) {
	std::cerr << ProgName << ": " << s << " \'" << ch << "\'"
		<< " near line " << LineNum << std::endl;

	++NumErrors;
	return 1;
}

double error(const std::string& s, const std::string& t) {
	std::cerr << ProgName << ": " << s << " \'" << t << "\'"
		<< " near line " << LineNum << std::endl;

	++NumErrors;
	return 1;
}

std::string fileName (const std::string path) {
	std::string::size_type n = path.find_last_of('/');
	if (std::string::npos == n)
		n = path.find_last_of('\\');

	return std::string::npos != n ? path.substr(n+1, std::string::npos) : path;
}

