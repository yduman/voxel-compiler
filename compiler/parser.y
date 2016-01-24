%{
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "util.h"
#include "parser.h"

extern "C" int yylex();
extern "C" int yyparse();
extern FILE *yyin;
extern char *yytext;

void yyerror(const char *s);

Expr *root;
%}

// Custom types (Arg, Args, Fn, Expr) are defined in parser.h
%union {
	int  	ival;
	char* 	sval;
	Arg* 	argval;
	Args* 	argsval;
	Fn*		fnval;
	Expr*	exprval;
}

// Task 1 - Define your tokens here
%token <ival> 	NUM
%token <ival> 	HEX
%token <sval> 	NAME
%token 			BLEFT
%token 			BRIGHT
%token 			COMMA
%token 			CONJ
%token 			DISJ
%token 			NEG

// Task 2 - define your non-terminal types here
%type <exprval> expr
%type <fnval> 	func
// >>>>>>>>>>>>>>>>> TODO :: CODE HERE !!!! <<<<<<<<<<<<<< //
%%

parser:
	expr { root = $1; }
	;

// Task 2 - define production rules here
expr:
	expr CONJ expr { $$ = new Expr(OP_CONJ, $1, $3); }
	|
	expr DISJ expr { $$ = new Expr(OP_DISJ, $1, $3); }
	|
	NEG expr { $$ = new Expr(OP_NEG, $2); }
	;
%%

#define LEXER_IMPLEMENTED

Ast parse(FILE *fp)
{
	yyin = fp;

	do {
#ifdef LEXER_IMPLEMENTED
		yyparse();
#else
		int x;
		std::cout << "Resulting tokens: ";
		while (x = yylex()) {
			std::cout << "<" << yytext << "> ";
		}
		std::cout << std::endl;
#endif

	} while (!feof(yyin));
#ifndef LEXER_IMPLEMENTED
	std::exit(EXIT_SUCCESS);
#endif

	return Ast(root);
}

void yyerror(const char *s) {
	std::cout << "Parser error: Message: " << s << std::endl;
	std::exit(EXIT_FAILURE);
}
