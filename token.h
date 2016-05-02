/**	@file	token.h
 *
 *	@brief	Token "kinds" and streams
 *
 *	Created by Randy Merkel on 6/7/13.
 *  Copyright (c) 2013 Randy Merkel. All rights reserved.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <sstream>

/************************************************************************************************
 *	Token "kinds" (types)																		*
 ************************************************************************************************/

/// Token kinds are representated by the integer value of its character
enum class Kind : char {
	none,								///< Placeholder for a 'real' token;
	
	name,								///< An identifier
	constant,							///< A constant identifier
	undefined,							///< An undefined identifier
	buildin,							///< Builtin functin with no paramers
	builtin1,							///< Builtin function with one parameter
	builtin2,							///< Builtin function with two parameters
	number,								///< floating-point literal
	end,								///< End of input

	// End of non-printing character codes for ASCII and UNICODE

	plus	= '+', minus	= '-',
	mul		= '*', div 		= '/',
	mod 	= '%',						///< Modulus (remainder)
	expo	= '^',						///< Expnentiation

	eos		= ';',						///< End of statement
	assign	= '=',

	comma	= ',',
	
	lp		= '(',
	rp		= ')'
};

/************************************************************************************************
 *	Token Stream																				*
 ************************************************************************************************/

/// A token kind/value pair
struct Token {
	Kind		kind;					///< Token type
	std::string	string_value;			///< kind == name
	double		number_value;			///< Kind == number

    Token(Kind k) : kind{k}, number_value{0} {}
};

/// A stream of tokens... with look-ahead
class Token_stream {
public:
	/// Initialize with an input stream which this does not own
	Token_stream(std::istream& s) : ip(&s), owns(false) {}

	/// Initialize with an input stream which this does own
	Token_stream(std::istream* p) :ip(p), owns(true) {}

	~Token_stream()						{	close();	}

	Token get();
	Token& current() 	{ return ct; }	///< The current token
	Token& next();						///< Next token (look-ahead)

	/// Set the input stream to s
	void set_input(std::istream& s) {
		close();
		ip = &s;
		owns = false;
	}

	/// Set the input stream to s
	void set_input(std::istream* p) {
		close();
		ip = p;
		owns = true;
	}

private:
	void close()						{ if (owns) delete ip;	}
	Token get_next();

	std::istream*	ip;					///< Poiner to an input stream
	bool			owns;				///< Does the Token_stream own this istream?
	Token 			ct { Kind::none };	///< Current token
	Token 			nt { Kind::none };	///< Next token
};

extern Token_stream ts;

#endif
