/* -*- indented-text -*- */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "functions.h"


extern "C" int yylex(void);
#define YY_DECL extern "C" int yylex();

void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 }

%}

%union {
       char *string;
}


%start cmd_line
%token <string> EXIT PIPE INPUT_REDIR OUTPUT_REDIR STRING NL BACKGROUND


%%

cmd_line    :
        | EXIT

        {
        print_cmds();
        exit(0);
        }

        | pipeline back_ground
        ;

back_ground : BACKGROUND        {printf("background"); }
        |                       {  }
        ;

simple      : command redir
        ;

command     : command STRING
                {
                add_argument($2);
                printf($2);
                }
        | STRING
                {
                add_command($1);
                printf($1);
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