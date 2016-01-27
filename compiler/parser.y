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
	int   ival;
	char* sval;
	Arg*  argval;
	Args* argsval;
	Fn*   fnval;
	Expr* exprval;
}

// Task 1 - Define your tokens here
%token <ival> NUM
%token <ival> HEX
%token <sval> NAME
%token BLEFT
%token BRIGHT
%token COMMA
%token CONJ
%token DISJ
%token NEG

// Task 2 - define your non-terminal types here
%type <exprval> expr
%type <argsval> args
%type <argval> arg
%type <fnval> fncall
%%

parser:
	expr { root = $1; }
	;

// Task 2 - define production rules here
expr:
	// ------------------------------------------ //
	// ------ nicht geklammerte Ausdruecke ------ //
	// ------------------------------------------ //
	expr CONJ expr { $$ = new Expr(OP_CONJ, $1, $3); }
	|
	expr DISJ expr { $$ = new Expr(OP_DISJ, $1, $3); }
	|
	NEG expr { $$ = new Expr(OP_NEG, $2); }
	|
	fncall { $$ = new Expr(OP_SPHERE, $1); } // TODO :: TESTING JUST WITH SPHERES 4 NOW
	|
	// ------------------------------------------ //
	// --------- geklammerte Ausdruecke --------- //
	// ------------------------------------------ //
	/*
	BLEFT expr CONJ expr BRIGHT { $$ = new Expr(OP_CONJ, $2, $4); }
	|
	BLEFT expr DISJ expr BRIGHT { $$ = new Expr(OP_DISJ, $2, $4); }
	|
	BLEFT NEG expr BRIGHT { $$ = new Expr(OP_NEG, $3); }
	|
	BLEFT fncall BRIGHT { $$ = new Expr(OP_SPHERE, $2); } // TODO :: TESTING JUST WITH SPHERES 4 NOW
	*/
	;

fncall:
	// Eine Funktion sieht so aus: funktionsNAME ( ARGS )
	NAME BLEFT args BRIGHT { $$ = new Fn($1, $3); }
	;

args:
	// ARGS besteht aus Argumenten (5x arg bei "heart" und "sphere", 7x bei "box")
	arg COMMA args { $$ = new Args(); $$->add($1); }
	|
	arg { $$ = new Args(); $$->add($1); }
	;

arg:
	// Ein Argument ist entweder eine ganze Zahl oder eine Hexadezimalzahl
	NUM { $$ = new Arg($1); }
	|
	HEX { $$ = new Arg($1); }
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
