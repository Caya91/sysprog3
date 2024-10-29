/* -*- indented-text -*- */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>


extern "C" int yylex(void);
#define YY_DECL extern "C" int yylex();

void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 }




%}

// Define tokens
%token HELLO
%token ERROR

%%

// Define the grammar rules
input:
    | input statement
    ;

statement:
    HELLO { printf("hello to you\n"); }
    | ERROR { printf("Error: unrecognized input\n"); }
    ;

%%

