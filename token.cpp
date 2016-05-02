/**	@file	token.cpp
 *
 *	@brief	Token "kinds" and streams
 *
 *	Created by Randy Merkel on 6/7/13.
 *  Copyright (c) 2013 Randy Merkel. All rights reserved.
 */

#include <iostream>

#include "symbol.h"
#include "token.h"
#include "utils.h"

/************************************************************************************************
 *	Token Stream																				*
 ************************************************************************************************/

Token_stream ts (std::cin);				///< The token-stream; by default standard input

// public:

/// Read and return the next token
Token Token_stream::get() {
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

Token& Token_stream::next()	{
	if (nt.kind == Kind::none)
		get_next();

	return nt;
}

// private:

/// Read and return the next token
Token Token_stream::get_next() {
	char ch = 0;

	do {								// skip whitespace except '\n'
		if (!ip->get(ch))
			return nt = { Kind::end };

	} while (ch != '\n' && std::isspace(ch));

	switch (ch) {
		case '\n':
			++LineNum;

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
				SymValue v = table[nt.string_value];
				if (v.kind == Kind::undefined || v.kind == Kind::constant)
					nt.kind = Kind::name;	// possible undefined or constant identifier
				else
					nt.kind = v.kind;
				return nt;
			}

			error("bad token", ch);
			return nt = { Kind::eos };
	}
}
