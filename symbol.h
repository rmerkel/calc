/**	@file	symbol.h
 *
 *	@brief Symbal Table entry type
 *
 *	Created by Randy Merkel on 6/7/13.
 *	Copyright (c) 2013 Randy Merkel. All rights reserved.
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <map>
#include <string>
#include <stdexcept>

#include "token.h"

/************************************************************************************************
 *	Exceptions (temp location)																	*
 ************************************************************************************************/

/// Attempt to modify a constant variable
class const_error : public std::runtime_error {
public:
	explicit const_error (const std::string& what) : std::runtime_error(what) {}
	explicit const_error (const char* what) : std::runtime_error(what) {}
};

/************************************************************************************************
 *	Symbol Table																				*
 ************************************************************************************************/

/// Symbol table entry
struct SymValue {
	Kind		kind; 					///< name, builtin or undefined?
	union {
		double	value;					// identifier; name or constant
		double	(*func)();				// builtin
		double	(*func1)(double);		// builtin1
		double 	(*func2)(double, double);	// buildin2
	} u;

	/// Default constructor results in an undefined variable
	SymValue() : kind (Kind::undefined) {
		u.value = 0.0;
	}

	/// Construct a new, defined, variable
	SymValue(double value, Kind kind = Kind::constant) : kind(kind) {
		u.value = value;
	}

	/// Construct a builtin
	SymValue(double (*func)()) : kind(Kind::buildin) {
		u.func = func;
	}
	
	/// Construct a builtin1
	SymValue(double (*func)(double)) : kind(Kind::builtin1) {
		u.func1 = func;
	}

	/// Construct a buildin2
	SymValue(double (*func)(double, double)) : kind(Kind::builtin2) {
		u.func2 = func;
	}

	SymValue* operator=(double value);

	/// Return my double value
	operator double() const { return u.value;	}
};

typedef std::map<std::string, SymValue> SymbolTable;

/// A symbol table; doubles keyed by a string
extern SymbolTable table;

#endif
