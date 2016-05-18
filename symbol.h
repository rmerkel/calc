/**	@file	symbol.h
 *
 *	@brief class SymValue and SymbolTable
 *	
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <map>
#include <string>
#include <stdexcept>

#include "token.h"

/******************************************************************************
 *	Exceptions (temp location)
 ******************************************************************************/

/// Attempt to modify a constant variable
class const_error : public std::runtime_error {
public:
	/// Construct a const_error with the message 'what' as in "what happened?"
	explicit const_error (const std::string& what) : std::runtime_error(what) {}

	/// Construct a const_error with the message 'what' as in "what happened?"
	explicit const_error (const char* what) : std::runtime_error(what) {}
};

/******************************************************************************
 *	Symbol Table
 ******************************************************************************/

/** @brief Symbol table entry
 *
 *	Each symblal has a type, or 'kind:'
 *	- name		- a variable whos value has been set or defined.
 *	- undefined	- a variable whos value has never been set/defined.
 *  -  constant	- a unmutable name
 *	- builtin	- a function pointer that takes no parameters
 *	- builtin1 	- a function pointer that takes one parameter
 *  - builtin2	- a function pointer that takes two paramerts
 *
 *  And a corresponding value; double, function pointer or 'undefined'. 
 *
 *	Variables start out as undefined symbols, but become defined when their
 *  value is first set. Constant's start out defined, but attempts to modify
 *  their value results in a const_error exception being thrown.
 *
 *	Likewise, builtin's are unmutable function pointers: attempts to modify
 *	them results in a const_error exception.
 */
struct SymValue {
	Kind		kind; 					///< name, constant builtin or undefined?
	union {
		double	value;					///< identifier; name or constant
		double	(*func)();				///< builtin (no parameters)
		double	(*func1)(double);		///< builtin1 (one parameters)
		double 	(*func2)(double, double); ///< builtin2 (two parameters)
	} u;								///< Symbol table value

	/// Default constructor results in an undefined symbol
	SymValue() : kind (Kind::undefined) {
		u.value = 0.0;
	}

	/// Construct a defined symbol with the given kind/value
	SymValue(double value, Kind kind = Kind::constant) : kind(kind) {
		u.value = value;
	}

	/// Construct a builtin
	SymValue(double (*func)()) : kind(Kind::builtin) {
		u.func = func;
	}
	
	/// Construct a builtin1
	SymValue(double (*func)(double)) : kind(Kind::builtin1) {
		u.func1 = func;
	}

	/// Construct a builtin2
	SymValue(double (*func)(double, double)) : kind(Kind::builtin2) {
		u.func2 = func;
	}

	/// Update and define a symbol value. Throws an const_error if not mutable
	SymValue* operator=(double value);

	/// Return my double value
	operator double() const { return u.value;	}
};

/// Alias for std::map<std::string, SymValue> 
typedef std::map<std::string, SymValue> SymbolTable;

#endif
