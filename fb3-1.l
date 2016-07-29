%option noyywrap nodefault yylineno
%{
#include "fb3-1.tab.h"
YY_BUFFER_STATE bp;
%}

EXP ([Ee][+-]?[0-9])

%%
"+" |
"-" |
"*" |
"/" |
"|" |
"(" |
")" { /*printf("** %c **\n", yytext[0]);*/return yytext[0]; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { /*printf("%s*\n", yytext);*/yylval.d = atof(yytext); return NUMBER; }

\n { return EOL; }
"//".*
[ \t]
. { printf("Mystery character %c\n", *yytext); }
%%

void yinput(char * s, char * len)
{
	bp = yy_scan_buffer(s, len);
	return ;
}

void yfree()
{
	yy_delete_buffer(bp);
	return ;
}


