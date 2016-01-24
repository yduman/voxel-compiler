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
[ \t\r\n]      ; // ignore whitespaces and new lines
%%