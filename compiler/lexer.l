%option noyywrap

%{
#ifdef _WIN32
#define YY_NO_UNISTD_H
#include "io.h"
#endif

#include <iostream>

#include "parser.h"
#include "parser.hh"
#include "util.h"

#define YY_DECL extern "C" int yylex()
%}

 // Task 1 - add your implementation in between the '%%'s
%%
[ \t\r\n]      	; 
[a-zA-Z0-9]+	{ yylval.sval = strdup(yytext); return NAME; }
[0-9]+			{ yylval.ival = atoi(yytext); return NUM; }
[x0-9]+			{ yylval.ival = strtol(yytext, NULL, 0); return HEX; }
"("				{ return BLEFT; }
")"				{ return BRIGHT; }
","				{ return COMMA; }
"|"				{ return DISJ; }
"&"				{ return CONJ; }
"!"				{ return NEG; }
%%