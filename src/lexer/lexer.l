
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.tab.h"

int line_num = 1;
%}

DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z0-9_]*

%%

"//".*                  { /* Ignore comments */ }

[ \t\r]+                { /* Ignore spaces and tabs */ }
\n                      { line_num++; }

"int"                   { return T_INT; }
"float"                 { return T_FLOAT; }
"bool"                  { return T_BOOL; }
"if"                    { return T_IF; }
"else"                  { return T_ELSE; }
"while"                 { return T_WHILE; }
"print"                 { return T_PRINT; }
"true"                  { return T_TRUE; }
"false"                 { return T_FALSE; }

"=="                    { return EQ; }
"!="                    { return NE; }
"<="                    { return LE; }
">="                    { return GE; }
"&&"                    { return AND; }
"||"                    { return OR; }

{DIGIT}+"."{DIGIT}+     {
                            yylval.float_value = atof(yytext);
                            return FLOAT_CONST;
                        }

{DIGIT}+                {
                            yylval.int_value = atoi(yytext);
                            return INT_CONST;
                        }

{ID}                    {
                            yylval.string = strdup(yytext);
                            return ID;
                        }

"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
"%"                     { return '%'; }
"="                     { return '='; }
"<"                     { return '<'; }
">"                     { return '>'; }
"!"                     { return '!'; }

"("                     { return '('; }
")"                     { return ')'; }
"{"                     { return '{'; }
"}"                     { return '}'; }
";"                     { return ';'; }
","                     { return ','; }

.                       {
                            printf("Lexical error at line %d: unexpected character '%s'\n",
                                   line_num, yytext);
                            return yytext[0];
                        }

%%

int yywrap(void)
{
    return 1;
}
