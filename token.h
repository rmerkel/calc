/**	@file	token.h
 *
 *	@brief	enum Kind, class Token and TokenStream.
 *
 *	Token "kinds" and streams
 *
 *	Created by Randy Merkel on 6/7/2013.
 *  Copyright (c) 2016 Randy Merkel. All rights reserved.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <sstream>

class Driver;

/************************************************************************************************
 *	Token "kinds" (types)																		*
 ************************************************************************************************/

/// Token kinds are representated by the integer value of its character
enum class Kind : char {
	none,								///< Placeholder for a 'real' token
	name,								///< An identifier
	constant,							///< A constant identifier
	undefined,							///< An undefined identifier
	builtin,							///< Builtin functin with no paramers
	builtin1,							///< Builtin function with one parameter
	builtin2,							///< Builtin function with two parameters
	number,								///< floating-point literal
	end,								///< End of input

	// End of non-printing character codes for ASCII and UNICODE

	plus	= '+',						///< Addition
	minus	= '-',						///< Subtraction
	mul		= '*',						///< Multiplication
	div 	= '/',						///< Division
	mod 	= '%',						///< Modulus (remainder)
	expo	= '^',						///< Expnentiation

	eos		= ';',						///< End of statement
	assign	= '=',						///< Assignment

	comma	= ',',						///< Comma
	
	lp		= '(',						///< Opening parentheses
	rp		= ')'						///< Closing parentheses
};

/************************************************************************************************
 *	Token Stream																				*
 ************************************************************************************************/

/// A token kind/value pair
struct Token {
	Kind		kind;					///< Token type
	std::string	string_value;			///< kind == name
	double		number_value;			///< Kind == number

	/// Construct a token of type k, stirng value "", number value 0.
    Token(Kind k) : kind{k}, number_value{0} {}
};

/// A stream of tokens... with look-ahead
class TokenStream {
public:
	/// Initialize with an input stream which this does not own
	TokenStream(Driver& drv, std::istream& s) : driver{drv}, ip{&s}, owns{false} {}

	/// Initialize with an input stream which this does own
	TokenStream(Driver& drv, std::istream* s) : driver{drv}, ip{s}, owns{true} {}

	~TokenStream()						{	close();	}

	Token get();

	/// The current token
	Token& current() 					{	return ct;	}
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
	Driver&			driver;				///< The parser driver
	std::istream*	ip;					///< Poiner to an input stream
	bool			owns;				///< Does the TokenStream own this istream?
	Token 			ct { Kind::none };	///< Current token
	Token 			nt { Kind::none };	///< Next token

	void close()						{ if (owns) delete ip;	}
	Token get_next();
};

#endif
