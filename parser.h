/** @file parser.h
 *
 *	@brief	Calculator Parser
 *
 *	Created by Randy Merkel on 6/7/2013.
 *	Copyright (c) 2016 Randy Merkel. All rights reserved.
 *
 *	@section	Grammar
 *
 *	program:
 *		end							- end of input
 *		expr_list end
 *
 *	expr_list:
 *		assign
 *		assign expr_list
 *		expression print
 *		expression print expr_list
 *
 *	assign:
 *		name = expression
 *
 *	expresson:
 *		expression + term
 *		expression - term
 *		term
 *
 *	term:
		term ^ prim
 *		term * primary
 *		term / primary
 *		term % primary
 *		primary
 *
 *	primary:
 *		number					- floating-point literal
 *		name					- an identifier
 *		buildin ()				- built in functions
 *		builtin1 ( expression )
 *		builtin2 (expresson, expresson)
 *		assign
 *		+primary				- unary plus
 *		-primary				- unary minus
 *		( expression )
 */

#ifndef PARSER_H
#define PARSER_H

#include "token.h"

// driver... lists of expressions and assignments
unsigned calculator();

#endif
