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
	int  ival;
	char *sval;
	Arg  *argval;
	Args *argsval;
	Fn   *fnval;
	Expr *exprval;
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
	expr CONJ expr { $$ = new Expr(OP_CONJ, $1, $3); }
	|
	expr DISJ expr { $$ = new Expr(OP_DISJ, $1, $3); }
	|
	NEG expr { $$ = new Expr(OP_NEG, $2); }
	|
	BLEFT expr CONJ expr BRIGHT { $$ = new Expr(OP_CONJ, $2, $4); }
	|
	BLEFT expr DISJ expr BRIGHT { $$ = new Expr(OP_DISJ, $2, $4); }
	|
	BLEFT NEG expr BRIGHT { $$ = new Expr(OP_NEG, $3); }
	|
	fncall { ; }
	|
	BLEFT fncall BRIGHT { ; }
	;
fncall: 
	NAME BLEFT args BRIGHT { $$ = new Fn($1, $3); }
	;
args:
	arg COMMA arg COMMA arg COMMA arg COMMA arg { $$ = new Args(); 
		$$->add($1); $$->add($3); $$->add($5); $$->add($7); $$->add($9); }
	|
	arg COMMA arg COMMA arg COMMA arg COMMA arg COMMA arg COMMA arg { $$ = new Args(); 
		$$->add($1); $$->add($3); $$->add($5); $$->add($7); 
		$$->add($9); $$->add($11); $$->add($13); }
	;
arg: 
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
