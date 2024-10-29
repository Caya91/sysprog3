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

char* args;
char* output;
char* input;


%}

%union {
       char *string;
}


%start cmd_line
%token <string> EXIT PIPE INPUT_REDIR OUTPUT_REDIR STRING NL BACKGROUND
%token TEST


%%

cmd_line    :
        | EXIT             { }
        | pipeline back_ground
        ;

back_ground : BACKGROUND        {  }
        |                       {  }
        ;

simple      : command redir
        ;

command     : command STRING
                {
                }
        | STRING
                {
                }
        ;

redir       : input_redir output_redir
        ;

output_redir:    OUTPUT_REDIR STRING
                {
                }
        |        /* empty */
				{
				}
        ;

input_redir:    INPUT_REDIR STRING
                {
                }
        |       /* empty */
                {
				}
        ;

pipeline    : pipeline PIPE simple
                {
                }
        | simple
                {
                }
        ;
%%

/* Declarations */
void set_input_string(const char* in);
void end_lexical_scan(void);

/* This function parses a string */
int parse_string(const char* in) {
  set_input_string(in);
  int rv = yyparse();
  end_lexical_scan();
  return rv;
}

