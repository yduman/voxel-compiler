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

using namespace std;
#define YY_DECL extern "C" int yylex()
%}

 // Task 1 - add your implementation in between the '%%'s
%%
[ \t\r\n]      ;
[0-9]+			{ yylval.ival = atoi(yytext); return NUM; }
0x[0-9a-fA-F]+		{ yylval.ival = strtol(yytext, NULL, 0); return HEX; }
[a-zA-Z0-9]+	{ yylval.sval = strdup(yytext); return NAME; }
"," { return COMMA; }
"("	{ return BLEFT; }
")"	{ return BRIGHT; }
"|"	{ return DISJ; }
"&"	{ return CONJ; }
"!"	{ return NEG; }
%%
