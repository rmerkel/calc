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
#include <fstream>
#include <iostream>
#include <string>

#include "driver.h"

/** Write a help message to standard error
 *
 *	@param	progname	Program name
 */
static void help(std::string progName) {
	std::cerr << "Usage: " + progName + " [options] [expr...]"			<< std::endl;
	std::cerr << "Where options is zero or more of:"					<< std::endl;
	std::cerr << "\t-e expr  \tEvaluate expr"							<< std::endl;
	std::cerr << "\t-f file  \tRead and evaluate the conents of file"	<< std::endl;
	std::cerr << "\t-h,-?    \tDisplay this message and exit"			<< std::endl;
	std::cerr << "\t-V       \tDispay the version"						<< std::endl;
}

/** Parse the contents of of a string
 *
 *	@param	Driver	The parser driver
 *	@param	s		The string to parse
 *
 *	@return	the number of errors encountered.
 */
static int parseString(Driver& driver, const std::string s) {
	driver.set_input(new std::istringstream{s});
	return driver.parse();
}

/** Parse the contents of a file
 *
 *	@param	Driver	The parser drvier
 *	@param	file	The name of the file to read, or "-" if standard input should be read.
 *
 *	@return The number of errors encountered, EXIT_FAILURE if the file couldn't be open.
 */
static int parseFile(Driver& driver, const std::string file) {
	if ("-" == file)
		driver.set_input(std::cin);

	else {
		std::ifstream ifile(file);

		if (!ifile.is_open()) {
			std::cerr << driver.progName << ": error opening \'" << file << "\'" << std::endl;
			return EXIT_FAILURE;
		}


		driver.set_input(new std::ifstream{file});
	}

	return driver.parse();
}

/**************************************************************************************************
 *	main (calc/hoc)															
 **************************************************************************************************/

/// Run the calculator; read standard input or driectly from command line parameter(s)
int main(int argc, char* argv[]) {
	Driver	driver(argv[0]);

	int nerrors = 0;							// Number of errors encountered...

	if (1 == argc)
		nerrors = driver.parse();				// Read from standard input

	else {
		for (int argn = 1; argn < argc; ++argn) {
			std::string arg = argv[argn];

			if (0 == arg.size())
				continue;						// skip empty arguments...

			if ("-e" == arg) {					// -e expr - read from argument string
				if (argn == argc) {	
					std::cerr << driver.progName << ": -e expr is missing expression string!" << std::endl;
					return EXIT_FAILURE;

				} else 
					nerrors = parseString(driver, argv[++argn]);

			} else if ("-f" == arg) {		// -f file - read from a file
				if (argn == argc) {	
					std::cerr << driver.progName << ": -f file is missing file name!" << std::endl;
					return EXIT_FAILURE;

				} else 
					nerrors = parseFile(driver, argv[++argn]);

			
											// -h help - display help message and exit
			} else if ("-h" == arg || "-?" == arg) {
				help(driver.progName);
				return nerrors;

			} else if ("-V" == arg)			// -V - dispay version number
				std::cout << "version: 1.0" << std::endl;

			else 							// read from argument string
				nerrors = parseString(driver, arg);
		}
	}

	return nerrors;
};

