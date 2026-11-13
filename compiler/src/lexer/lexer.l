
%option noyywrap
%option yylineno

%{
#include <stdio.h>
#include "../parser/parser.tab.h"

int lexical_error_count = 0;
%}

%x COMMENT

DIGIT       [0-9]
LETTER      [A-Za-z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
FLOAT_NUM   ({DIGIT}+\.{DIGIT}*|\.{DIGIT}+)
INTEGER     {DIGIT}+

%%

"int"       { return INT; }
"float"     { return FLOAT; }
"bool"      { return BOOL; }

"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"print"     { return PRINT; }

"true"      { return TRUE; }
"false"     { return FALSE; }

{FLOAT_NUM} { return FLOAT_LITERAL; }
{INTEGER}   { return INTEGER_LITERAL; }
{IDENTIFIER} { return IDENTIFIER; }

"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MULTIPLY; }
"/"         { return DIVIDE; }
"%"         { return MODULO; }

"<="        { return LE; }
">="        { return GE; }
"=="        { return EQ; }
"!="        { return NE; }
"<"         { return LT; }
">"         { return GT; }

"&&"        { return AND; }
"||"        { return OR; }
"!"         { return NOT; }

"="         { return ASSIGN; }

"{"         { return LBRACE; }
"}"         { return RBRACE; }
"("         { return LPAREN; }
")"         { return RPAREN; }
";"         { return SEMICOLON; }

"//".*      { }

"/*"        { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\n  { }
<COMMENT>.   { }

<COMMENT><<EOF>> {
    fprintf(stderr,
            "LEXICAL ERROR: Unterminated comment at line %d\n",
            yylineno);
    lexical_error_count++;
    return INVALID;
}

[ \t\r\n]+  { }

. {
    fprintf(stderr,
            "LEXICAL ERROR: Invalid character '%s' at line %d\n",
            yytext,
            yylineno);
    lexical_error_count++;
    return INVALID;
}

%%
