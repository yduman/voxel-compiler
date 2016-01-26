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
[ \t\r\n]      ; // ignore whitespaces and new lines
[a-zA-Z0-9]+	{ yylval.sval = strdup(yytext); cout << "Found NAME: " << yytext << endl; return NAME; }
[0-9]+			{ yylval.ival = atoi(yytext); cout << "Found NUM: " << yytext << endl; return NUM; }
0x[0-9a-f]+		{ yylval.ival = strtol(yytext, NULL, 0); cout << "Found HEX: " << yytext << endl; return HEX; }
","				{ cout << "Found COMMA: " << yytext << endl; return COMMA; }
"("				{ cout << "Found BLEFT: " << yytext << endl; return BLEFT; }
")"				{ cout << "Found BRIGHT: " << yytext << endl; return BRIGHT; }
"|"				{ cout << "Found DISJ: " << yytext << endl; return DISJ; }
"&"				{ cout << "Found CONJ: " << yytext << endl; return CONJ; }
"!"				{ cout << "Found NEG: " << yytext << endl; return NEG; }
%%