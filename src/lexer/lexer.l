
%{
#include <stdio.h>

int line_num = 1;
%}

DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z0-9_]*
NUMBER  {DIGIT}+(\.{DIGIT}+)?

%%

"//".*                  { /* Ignore comments */ }

[ \t\r]+                { /* Ignore spaces and tabs */ }
\n                      { line_num++; }

"int"                   { printf("KEYWORD(int)\n"); }
"float"                 { printf("KEYWORD(float)\n"); }
"bool"                  { printf("KEYWORD(bool)\n"); }
"if"                    { printf("KEYWORD(if)\n"); }
"else"                  { printf("KEYWORD(else)\n"); }
"while"                 { printf("KEYWORD(while)\n"); }
"print"                 { printf("KEYWORD(print)\n"); }
"true"                  { printf("KEYWORD(true)\n"); }
"false"                 { printf("KEYWORD(false)\n"); }

"=="                    { printf("OP(==)\n"); }
"!="                    { printf("OP(!=)\n"); }
"<="                    { printf("OP(<=)\n"); }
">="                    { printf("OP(>=)\n"); }
"&&"                    { printf("OP(&&)\n"); }
"||"                    { printf("OP(||)\n"); }

"+"                     { printf("OP(+)\n"); }
"-"                     { printf("OP(-)\n"); }
"*"                     { printf("OP(*)\n"); }
"/"                     { printf("OP(/)\n"); }
"%"                     { printf("OP(%%)\n"); }
"="                     { printf("OP(=)\n"); }
"<"                     { printf("OP(<)\n"); }
">"                     { printf("OP(>)\n"); }
"!"                     { printf("OP(!)\n"); }

"("                     { printf("LPAREN\n"); }
")"                     { printf("RPAREN\n"); }
"{"                     { printf("LBRACE\n"); }
"}"                     { printf("RBRACE\n"); }
";"                     { printf("SEMI\n"); }
","                     { printf("COMMA\n"); }

{NUMBER}                { printf("NUMBER(%s)\n", yytext); }
{ID}                    { printf("ID(%s)\n", yytext); }

.                       { printf("LEXICAL ERROR line %d: unexpected character '%s'\n", line_num, yytext); }

%%

int yywrap(void)
{
    return 1;
}

int main(void)
{
    printf("Enter your program (Ctrl+Z then Enter to finish):\n");
    yylex();
    return 0;
}
