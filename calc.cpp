/**
 *	@file	main.cpp
 *
 *	calc - a Hoc port, implemented as recursive descent parser 
 *
 *	Created by Randy Merkel on 5/10/13.
 *	Copyright (c) 2013 Randy Merkel. All rights reserved.
 *
 *	Grammar
 *
 *	program:	end				- end of input
 *				expr_list end
 *
 *	expr_list:	expression print
 *				expression print expr_list
 *
 *	expresson:	expression + term
 *				expression - term
 *				term
 *
 *	term:		term * primary
 *				term / primary
 *				prim % primary
 *				primary
 *
 *	primary:	number			- floating-point literal
 *				name			- an identifier
 *				name = expression
 *				+primary
 *				-primary
 *				( expression )
 */

#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

unsigned no_of_errors = 0;

/// write a diagnositic message
double error(const std::string& s) {
	++no_of_errors;
	std::cerr << "error: " << s << '\n';
	return 1;
}

/// Token kinds are representated by the integer value of its character 
enum class Kind : char {
	name,								///< An identifier
	number,								///< floating-point literal
	end,								///< End of input
	
	plus	= '+', minus	= '-',
	mul		= '*', div 		= '/',
	mod 	= '%',						// 	modulus (remainder)
	
	print	= ';',						// 	end of statement; print result
	assign	= '=',
	
	lp		= '(',
	rp		= ')'
};

/// A token kind/value pair
struct Token {
	Kind		kind;					///< Token type
	std::string	string_value;			///< kind == name
	double		number_value;			///< Kind == number
};

/// A stream of tokens
class Token_stream {
public:
	/// Initialize with an input stream which this does not own
	Token_stream(std::istream& s) : ip(&s), owns(false) {}

	/// Initialize with an input stream wich this does own
	Token_stream(std::istream* p) :ip(p), owns(true) {}

	~Token_stream()						{	close();	}

	Token get();
	Token& current() { return ct; }		///< Most reciently read token

	void set_input(std::istream& s) {
		close();
		ip = &s;
		owns = false;
	}

	void set_input(std::istream* p) {
		close();
		ip = p;
		owns = true;
	}

private:
	void close()						{ if (owns) delete ip;	}

	std::istream*	ip;					///< Poiner to an input stream
	bool			owns;				///< Does the Token_stream own this istream?
	Token 			ct { Kind::end };	///< Current token
};

/// Read and return next token
Token Token_stream::get() {
	char ch = 0;

	do {								// skip whitespace except '\n'
		if (!ip->get(ch))
			return ct = { Kind::end };
	} while (ch != '\n' && std::isspace(ch));


	switch (ch) {
	case ';':
	case '\n':
		return ct = { Kind::print };

	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		return ct = {static_cast<Kind>(ch)};

	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case '.':
		ip->putback(ch);
		*ip >> ct.number_value;
		ct.kind = Kind::number;
		return ct;

	default:							// name, name = or error
		if (std::isalpha(ch)) {
			ct.string_value = ch;
			while (ip->get(ch))
				if (std::isalnum(ch))
					ct.string_value += ch;
				else {
					ip->putback(ch);
					break;
				}
			ct.kind = Kind::name;
			return ct;
		}
		
		error("bad token");
		return ct = { Kind::print };
	}
}

/// A symbol table; doubles keyed by a string
std::map<std::string, double> table;

Token_stream ts (std::cin);

double expr(bool);

/// Handle primary expressions @param get get a new token if true
double prim(bool get) {

	if (get)
		ts.get();						// read next token

	switch(ts.current().kind) {
	case Kind::number: {				// floating-pont constant
		double v = ts.current().number_value;
		ts.get();
		return v;
	}
			
	case Kind::name: {					// identifier
		double& v = table[ts.current().string_value];
		if (ts.get().kind == Kind::assign)
			v = expr(true);				// assignment
		return v;
	}

	case Kind::minus:					// unary minus
		return -prim(true);

	case Kind::plus:					// unary plus
		return prim(true);

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

/// multiple and divide. @param get get a new token if true
double term(bool get) {
	double left = prim(get);

	for (;;) {
		switch(ts.current().kind) {
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
				left = remainder(left, d);
				break;
			}

			return error("divide by 0");
			
		default:
			return left;
		}
	}
}

/// add and subtract. @param get get a new token if true
double expr(bool get) {
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

/// driver...
void calculator() {
	for (;;) {
		ts.get();
		if (ts.current().kind == Kind::end)
			break;

		if (ts.current().kind == Kind::print)
			continue;

		std::cout << expr(false) << '\n';
	}
}

int main(int argc, char* argv[]) {
	switch (argc) {
	case 1:										// read from standard input
		break;

	case 2:										// read from argument string
		ts.set_input(new std::istringstream{argv[1]});
		break;

	default:
		error("too many command line arguments");
		return EXIT_FAILURE;
	}
		
	table["pi"]		=  3.14159265358979323846;
	table["e"]		=  2.71828182845904523536;
	table["gamma"]	=  0.57721566490153286060;	// Euler
	table["deg"]	= 57.29577951308232087680;  // deg/radian
	table["phi"]	=  1.61803398874989484820;  // golden ratio

	calculator();

	return no_of_errors;
};

