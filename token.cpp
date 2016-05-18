/**	@file	token.cpp
 *
 *	@brief	TokenStream implementation.
 *
 *	Created by Randy Merkel on 6/7/2013.
 *  Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#include "token.h"
#include "driver.h"
#include "symbol.h"

/************************************************************************************************
 *	Token Stream																				*
 ************************************************************************************************/

// public:

/// Read and return the next token
Token TokenStream::get() {
	if (ct.kind == Kind::none)
		get_next();						// get the first token into nt
	else if (nt.kind == Kind::none)
		get_next();						// get the next token into nt
	
	ct = nt;							// next is now current...
	
	if (ct.kind == Kind::end)
		nt = ct;						// nothing follows the end token
	else
		nt.kind = Kind::none;
	
	return ct;
}

Token& TokenStream::next()	{
	if (nt.kind == Kind::none)
		get_next();

	return nt;
}

// private:

/// Read and return the next token
Token TokenStream::get_next() {
	char ch = 0;

	do {								// skip whitespace except '\n'
		if (!ip->get(ch))
			return nt = { Kind::end };

	} while (ch != '\n' && std::isspace(ch));

	switch (ch) {
		case '\n':
			++driver.lineNum;

		case ';':
			return nt = { Kind::eos };

		case '*':
		case '/':
		case '+':
		case '-':
		case '(':
		case ')':
		case '=':
		case '^':
		case ',':
			return nt = { static_cast<Kind>(ch) };

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case '.':
			ip->putback(ch);
			*ip >> nt.number_value;
			nt.kind = Kind::number;
			return nt;

		default:							// name, name = or error
			if (std::isalpha(ch)) {
				nt.string_value = ch;
				while (ip->get(ch) && std::isalnum(ch))
					nt.string_value += ch;

				ip->putback(ch);
				SymValue v = driver.table[nt.string_value];
				if (v.kind == Kind::undefined || v.kind == Kind::constant)
					nt.kind = Kind::name;	// possible undefined or constant identifier
				else
					nt.kind = v.kind;
				return nt;
			}

			driver.error("bad token", ch);
			return nt = { Kind::eos };
	}
}
