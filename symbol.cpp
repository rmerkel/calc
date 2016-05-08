/** @file symbol.cpp
 *
 *	@brief	Symbol table implemetation
 *
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#include "symbol.h"

SymbolTable table;					// The symbol table

/// Update as a defined variable
SymValue* SymValue::operator=(double value) {
	if (Kind::undefined != kind && Kind::name != kind)
		throw const_error("cannot modify constant value");

	kind = Kind::name;
	u.value = value;

	return this;
}
