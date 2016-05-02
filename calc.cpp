/**
 *	@file	calc.cpp
 *
 *	calc - a Hoc port, implemented as recursive descent parser
 *
 *	Created by Randy Merkel on 5/10/13.
 *	Copyright (c) 2013 Randy Merkel. All rights reserved.
 *
 */

#include <cassert>
#include <cctype>
#include <iostream>
#include <string>

#include "utils.h"
#include "math.h"
#include "parser.h"
#include "symbol.h"
#include "token.h"

/************************************************************************************************
 *	main (calc/hoc)																				*
 ************************************************************************************************/

/// Run the calculator; read from standard input or the 1st command line parameter
int main(int argc, char* argv[]) {
	ProgName = fileName(argv[0]);

	switch (argc) {
	case 1:										// read from standard input
		break;

	case 2:										// read from argument string
		ts.set_input(new std::istringstream{argv[1]});
		break;

	default:
		error("too many command line arguments");
		return EXIT_FAILURE;
	}

	return calculator();
};

