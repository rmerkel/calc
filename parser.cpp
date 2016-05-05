/**	@file parser.cpp
 *
 *	@brief	Parser implementation for a Calculator
 */

#include <iostream>
#include <cmath>
#include <ctime>

#include "math.h"
#include "parser.h"
#include "symbol.h"
#include "utils.h"

/************************************************************************************************
 *	Primary expressions																			*
 ************************************************************************************************/

static double expr(bool);

/** Handle primary expressions
 *
 *	@param get get a new token if true
 *
 *	@return primary expression value
 */
static double prim(bool get) {
	if (get)
		ts.get();						// read next token

	switch(ts.current().kind) {
	case Kind::number: {				// floating-pont constant
		double v = ts.current().number_value;
		ts.get();
		return v;
	}

	case Kind::name: {					// identifier
		std::string name = ts.current().string_value;
		SymValue &v = table[name];
		ts.get();						// consume the identifier
		if (v.kind == Kind::undefined)
			return error("undefined variable", name);

		return v;
	}

	case Kind::minus:					// unary minus
		return -prim(true);

	case Kind::plus:					// unary plus
		return prim(true);

	case Kind::buildin: {				// func ()
		SymValue& v = table[ts.current().string_value];
		if (ts.get().kind != Kind::lp)
		 	return error("'(' expected");
		else if (ts.get().kind != Kind::rp)
			return error("')' expected");
		else {
			ts.get();					// eat ')'
			return v.u.func();
		}
	}
			
	case Kind::builtin1: {				// func ( expression )
		std::string name = ts.current().string_value;
		if (ts.get().kind != Kind::lp)
			return error("'(' expected");

		else {
			SymValue& v = table[name];
			double e = expr(true);
				
			if (ts.current().kind != Kind::rp)
				return error("')' expected");
				
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
				return error("',' expected");

			double e2 = expr(true);
			if (ts.current().kind != Kind::rp)
				return error("')' expected");
				
			ts.get();					// eat ')'
			return v.u.func2(e1, e2);

		} else
			return error("'(' expected");
	}

	case Kind::lp: {					// ( expression )
		auto e = expr(true);
		if (ts.current().kind != Kind::rp)
			return error("')' expected");

		ts.get();						// eat ')'
		return e;
	}

	default:
		return error ("primary expected");
	}
}

/************************************************************************************************
 *	Terminal expressions																		*
 ************************************************************************************************/

/**	Terminal expressions, such as multiply and divide.
 *
 *	@param get get a new token if true
 *
 *	@return	Terminal expression value
 */
static double term(bool get) {
	double left = prim(get);

	for (;;) {
		switch(ts.current().kind) {
			case Kind::expo:				// term ^ prim
				left = Pow(left, expr(true));
				break;

			case Kind::mul:					// term * prim
				left *= prim(true);
				break;

			case Kind::div:					// term / prim
				if (auto d = prim(true)) {
					left /= d;
					break;
				}

				return error("divide by 0");

			case Kind::mod:					// term % prim
				if (auto d = prim(true)) {
					left = std::remainder(left, d);
					break;
				}

				return error("divide by 0");

			default:
				return left;
		}
	}
}

/************************************************************************************************
 *	Expressons																					*
 ************************************************************************************************/

/** Expressions such as add, subtract
 *
 *	@param get get a new token if true
 *
 *	@return	expression value
 */
static double expr(bool get) {
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

/************************************************************************************************
 *	Statements																					*
 ************************************************************************************************/

/** Assignment
 *
 *	Handle attempt assignment to constants.
 *
 *	@param	get	get a new token if true
 *
 *	@return	assigned value.
 */
double assign(bool get) {
	std::string name = ts.current().string_value;

	if (ts.get().kind == Kind::assign) {
		SymValue& v = table[name];

		if (v.kind == Kind::constant)
			return error("can not modify constant variable", name);

		v = expr(get);
		return v;

	} else
		return error("syntax error - bad assignment");
}

/************************************************************************************************
 *	Driver																						*
 ************************************************************************************************/

/// Initialize the parser...
static void init() {
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
	table["int"]	= SymValue(integer);		// checks argument
	table["abs"]	= SymValue(fabs);			// checks argument

	table["atan2"]	= SymValue(atan2);
}

/**	Driver... lists of expressions, statemnts  and assignments
 *
 *	@return The number of errors encountered.
 */
unsigned calculator() {
	init();
	
	for (;;) {
		ts.get();
		if (ts.current().kind == Kind::end)
			break;

		if (ts.current().kind == Kind::eos)
			continue;

		if (ts.current().kind == Kind::name && ts.next().kind == Kind::assign) {
			assign(true);
			continue;							// Don't print the assigned value
		}

		// Print and save last result in "last"
		double& v = (table["last"] = expr(false))->u.value;
		std::cout << v << '\n';
	}

	return NumErrors;
}

