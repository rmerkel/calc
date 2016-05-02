//
//  symbol.cpp
//  calc
//
//  Created by Randy Merkel on 6/7/13.
//  Copyright (c) 2013 Randy Merkel. All rights reserved.
//

#include "symbol.h"

SymbolTable table;					// The symbol table

/// Update as a defined variable
SymValue* SymValue::operator=(double value) {
	if (Kind::constant == kind) throw const_error("cannot modify constant value");
	kind = Kind::name;
	u.value = value;
	return this;
}
