/** @file parser.h
 *
 *	@brief	class Parser
 *
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#ifndef PARSER_H
#define PARSER_H

#include "symbol.h"

class Driver;
class TokenStream;

/** Calculator Parser
 *
 *	This praser is a hybred of hoc from "The UNIX Programing Enviorment" by Kernighan and Pike, and
 *	calc from TC++PL, 4th Edition by Stroustrup. The calc approch of breaking up expressions into
 *  expressions, terminal and primaries was used for it's readability.
 *
 *	@section	Grammar
 *
 *	    program:
 *		    end							- end of input
 *		    expr_list end
 *
 *	    expr_list:
 *		    assign
 *		    assign expr_list
 *		    expression print
 *		    expression print expr_list
 *
 *	    assign:
 *		    name = expression
 *
 *	    expresson:
 *		    expression + term
 *		    expression - term
 *		    term
 *
 *	    term:
 *		    term ^ prim
 *		    term * primary
 *		    term / primary
 *		    term % primary
 *		    primary
 *
 *	    primary:
 *		    number					- floating-point literal
 *		    name					- an identifier
 *		    assign
 *		    builtin ()				- built in functions
 *		    builtin1 ( expression )
 *		    builtin2 (expresson, expresson)
 *		    assign
 *		    +primary				- unary plus
 *			-primary				- unary minus
 *			( expression )
 */

class Parser {
	Driver&			driver;				///< The driver
	TokenStream&	ts;					///< The token stream (scanner)
	SymbolTable&	table;				///< The symbol table

	// The parser itself

	double prim(bool get);
	double term(bool get);
	double expr(bool get);
	void assign();
	unsigned lists();

public:
	Parser(Driver& d);
	virtual ~Parser()	{}

	unsigned operator()();
};

#endif
