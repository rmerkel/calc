/**	@file	calc.cpp
 *
 *	@brief	calc(ulator), a hoc port
 *
 *	@mainpage calc - a hoc port, implemented as recursive descent parser
 *
 *	A hybred of 'hoc' from The UNIX Programing Enviorment by Kernighan and Pike,
 *	and 'calc' from TC++PL, 4th Edition by Stroustrup. The calc approch of
 *	breaking up expressions intoexpressions, terminal and primaries was used for
 *	it's readability.
 *
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
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

/******************************************************************************
 *	main (calc/hoc)															
 ******************************************************************************/

/// Run the calculator; read standard input or the 1st command line parameter
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

