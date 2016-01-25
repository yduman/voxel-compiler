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
%token 			COMMA	// noetig (?)
%token 			CONJ
%token 			DISJ
%token 			NEG

// Task 2 - define your non-terminal types here
%type <exprval> expr
%type <fnval> 	fn
%type <argsval> args 
%type <argval> 	arg
%type 			op
// >>>>>>>>>>>>>>>>> TODO :: CODE HERE !!!! <<<<<<<<<<<<<< //
%%

parser:
	expr { root = $1; }
	;

// Task 2 - define production rules here
expr:
	expr op expr 				{ $$ = new Expr($2, $1, $3); }
	|
	op expr 					{ $$ = new Expr($1, $2); }
	|
	fn op 						{ $$ = new Expr($2, $1); }
	|
	fn 							{ ; }
	;
fn: 
	NAME BLEFT args BRIGHT 		{ $$ = new Fn($1, $3); }
	;
args:
	arg arg arg arg arg 		{ $$ = new Args(); } 
	| 
	arg arg arg arg arg arg arg { $$ = new Args(); }
	;
arg: 
	NUM 						{ $$ = new Arg($1); }
	|
	HEX 						{ $$ = new Arg($1); }
	;
op:
	CONJ 						{ $$ = OP_CONJ; } 
	|
	DISJ 						{ $$ = OP_DISJ; }
	|
	NEG 						{ $$ = OP_NEG; }
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
