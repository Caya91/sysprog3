/* -*- indented-text -*- */
%{
#include "test.tab.h"
#include <string.h>
#include <FlexLexer.h>

extern "C" int yylex (void);

#ifdef DOESNT_HAVE_STRDUP
#warning DOESNT_HAVE_STRDUP
char *strdup(const char *s);
#endif

#define ECHO
%}

NL              [\n]
PIPE            [|]
OUTPUT_REDIR    [>]
INPUT_REDIR     [<]
BG              [&]
CHAR            [0-9a-zA-Z_/\-\*\.]
SPACE           [\032]

%%


exit            { ECHO; return EXIT; }
{NL}            { ECHO; return 0; }
{BG}            { ECHO; return BACKGROUND; }
{PIPE}          { ECHO; return PIPE;}
{OUTPUT_REDIR}  { ECHO; return OUTPUT_REDIR; }
{INPUT_REDIR}   { ECHO; return INPUT_REDIR; }
{CHAR}+         { ECHO; yylval.string = strdup(yytext); return STRING; }
.               ;
%%
int yywrap (void) {return 1;}