/* -*- indented-text -*- */
%{
#include "test.tab.h"
#include <string.h>

extern "C" int yylex (void);

#ifdef DOESNT_HAVE_STRDUP
#warning DOESNT_HAVE_STRDUP
char *strdup(const char *s);
#endif

#define ECHO
%}

%%


"hello" { return HELLO; }


[ \t\n]   ;


.         { return ERROR; }

%%
int yywrap (void) {return 1;}