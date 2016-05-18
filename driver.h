/** @file driver.h
 *
 *	@brief	class Driver
 *
 *	The calculator parser driver.
 *
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#ifndef DRIVER_H
#define DRIVER_H

#include <string>

#include "parser.h"
#include "symbol.h"
#include "token.h"

/** Calculator Parser Driver.
 *
 *	Maintains the state for, and coorinates of, the calculator parser,
 *  token-stream (scanner) and symbol table.
 */

class Driver {
	static std::string fileName (const std::string path);

public:
	SymbolTable 	table;				///< The symbol table
	TokenStream		ts;					///< The token-stream (scanner)
	Parser			parser;				///< The parser

	std::string		progName;			///< The parser drivers name
	unsigned		nErrors;			///< Number of errors seen to date
	unsigned		lineNum;			///< The current line number

	Driver(const std::string& n);

	/// Destructor
	virtual ~Driver()	{}

	/// Set the input stream to s
	void set_input(std::istream& s)			{	ts.set_input(s);	}

	/// Set the input stream to s
	void set_input(std::istream* s)			{	ts.set_input(s);	}

	double error(const std::string& s);
	double error(const std::string& s, char ch);
	double error (const std::string& s, const std::string& t);

	/// Parse input...
	unsigned parse()						{	return parser();	}
};

#endif
