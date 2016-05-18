/** @file symbol.cpp
 *
 *	@brief	SymbolTable implemetation
 *
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#include "symbol.h"

/// Update as a defined variable
SymValue* SymValue::operator=(double value) {
	if (Kind::undefined != kind && Kind::name != kind)
		throw const_error("cannot modify constant value");

	kind = Kind::name;
	u.value = value;

	return this;
}
