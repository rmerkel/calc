/**	@file parser.cpp
 *
 *	@brief	Parser implementation
 *
 *	This praser is a hybred of hoc from The UNIX Programing Enviorment by Kernighan and Pike, and
 *	calc from TC++PL, 4th Edition by Stroustrup. The calc approch of breaking up expressions into
 *  expressions, terminal and primaries was used for it's readability.
 *
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#include <iostream>
#include <cmath>

#include "parser.h"
#include "driver.h"
#include "math.h"
#include "symbol.h"

/** Handle primary expressions
 *
 *	@param get get a new token if true
 *
 *	@return primary expression value
 */
double Parser::prim(bool get) {
	if (get)
		ts.get();						// read next token

	switch(ts.current().kind) {
	case Kind::number: {				// floating-pont constant
		double v = ts.current().number_value;
		ts.get();
		return v;
	}

	case Kind::name: {					// identifier
		const std::string name = ts.current().string_value;
		SymValue &v = table[name];

		if (ts.get().kind == Kind::assign) {
			if (v.kind == Kind::constant)
				return driver.error("can not modify constant variable", name);
			v = expr(true);

		} else if (v.kind == Kind::undefined)
			return driver.error("undefined variable", name);

		return v;
	}

	case Kind::minus:					// unary minus
		return -prim(true);

	case Kind::plus:					// unary plus
		return prim(true);

	case Kind::builtin: {				// func ()
		SymValue& v = table[ts.current().string_value];
		if (ts.get().kind != Kind::lp)
		 	return driver.error("'(' expected");
		else if (ts.get().kind != Kind::rp)
			return driver.error("')' expected");
		else {
			ts.get();					// eat ')'
			return v.u.func();
		}
	}
			
	case Kind::builtin1: {				// func ( expression )
		std::string name = ts.current().string_value;
		if (ts.get().kind != Kind::lp)
			return driver.error("'(' expected");

		else {
			SymValue& v = table[name];
			double e = expr(true);
				
			if (ts.current().kind != Kind::rp)
				return driver.error("')' expected");
				
			ts.get();					// eat ')'
			return v.u.func1(e);
		}
	}
			
	case Kind::builtin2: {				// func ( expression, expression )
		std::string name = ts.current().string_value;
		if (ts.get().kind == Kind::lp) {
			SymValue& v = table[name];

			double e1 = expr(true);
			if (ts.current().kind != Kind::comma)
				return driver.error("',' expected");

			double e2 = expr(true);
			if (ts.current().kind != Kind::rp)
				return driver.error("')' expected");
				
			ts.get();				// eat ')'
			return v.u.func2(e1, e2);

		} else
			return driver.error("'(' expected");
	}

	case Kind::lp: {					// ( expression )
		auto e = expr(true);
		if (ts.current().kind != Kind::rp)
			return driver.error("')' expected");

		ts.get();						// eat ')'
		return e;
	}

	default:
		return driver.error ("primary expected");
	}
}

/**	Terminal expressions, such as multiply and divide, or primaries
 *
 *	@param get get a new token if true
 *
 *	@return	Terminal expression value
 */
double Parser::term(bool get) {
	double left = prim(get);

	for (;;) {
		switch(ts.current().kind) {
			case Kind::mul:					// term * prim
				left *= prim(true);
				break;

			case Kind::div:					// term / prim
				if (auto d = prim(true))
					left /= d;
				else
					return driver.error("divide by 0");
				break;

			case Kind::expo:				// term ^ prim
				left = Pow(left, prim(true));
				break;

			case Kind::mod:					// term % prim
				if (auto d = prim(true)) 
					left = std::remainder(left, d);
				else
					return driver.error("divide by 0");
				break;

			default:
				return left;
		}
	}
}

/** Expressions such as add, subtract, terminals or primaries
 *
 *	@param get get a new token if true
 *
 *	@return	expression value
 */
double Parser::expr(bool get) {
	double left = term(get);

	for (;;) {
		switch (ts.current().kind) {
			case Kind::plus:				// expr + term
				left += term(true);
				break;

			case Kind::minus:				// expr - term
				left -= term(true);
				break;

			default:
				return left;
		}
	}
}

/// Assignment statement
void Parser::assign() {
	const std::string name = ts.current().string_value;
	SymValue& v = table[name];

	ts.get();							// consume identifier
	if (v.kind == Kind::constant)
		driver.error("can not modify constant variable", name);
	else
		v = expr(true);
}

/**	Lists of expressions and assignments...
 *
 *	@return The number of errors encountered.
 */
unsigned Parser::lists() {
	for (;;) {
		ts.get();
		if (ts.current().kind == Kind::end)
			break;

		if (ts.current().kind == Kind::eos)
			continue;

		if (ts.current().kind == Kind::name && ts.next().kind == Kind::assign) {
			assign();
			continue;							// Don't print assigned values
		}

		// Print and save last result in "last"

		double& v = (table["last"] = expr(false))->u.value;
		std::cout << '\t' << v << '\n';
	}

	return driver.nErrors;
}

/** Construct a parser...
 *
 *	Results in a parser reading from standard input.
 *
 *	@param	drv		The parser driver
 */
Parser::Parser(Driver& drv) : driver{drv}, ts{drv.ts}, table{drv.table} {
	if (!table.empty()) return;					// Install constants, built-ins just once
	
	table["pi"]		= SymValue( 3.14159265358979323846);
	table["e"]		= SymValue( 2.71828182845904523536);	// Base of natural logarithms
	table["gamma"]	= SymValue( 0.57721566490153286060);	// Euler-Mascheroni constant
	table["deg"]	= SymValue(57.29577951308232087680);	// Degress/radian
	table["phi"]	= SymValue( 1.61803398874989484820);	// The Golden ratio
	
	table["rand"]	= SymValue(Rand);

	table["sin"]	= SymValue(sin);
	table["cos"]	= SymValue(cos);
	table["atan"]	= SymValue(atan);
	table["log"]	= SymValue(Log);			// checks argument
	table["log10"]	= SymValue(Log10);			// checks argument
	table["exp"]	= SymValue(Exp);			// checks argument
	table["sqrt"]	= SymValue(Sqrt);			// checks argument
	table["int"]	= SymValue(Integer);		// checks argument
	table["abs"]	= SymValue(fabs);			// checks argument

	table["atan2"]	= SymValue(atan2);
}

/**	Parse calculator/hoc program(s)...
 *
 *	@return The number of errors encountered.
 */
unsigned Parser::operator()() {
	return lists();
}
