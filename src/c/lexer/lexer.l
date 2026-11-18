
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

int line_number = 1;
%}

%option noyywrap
%option yylineno

DIGIT       [0-9]
LETTER      [a-zA-Z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)?
WHITESPACE  [ \t\r]+

%%

"int"           { return INT; }
"float"         { return FLOAT_TYPE; }
"double"        { return DOUBLE; }
"char"          { return CHAR; }
"void"          { return VOID; }
"short"         { return SHORT; }
"long"          { return LONG; }
"signed"        { return SIGNED; }
"unsigned"      { return UNSIGNED; }

"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"for"           { return FOR; }
"do"            { return DO; }
"switch"        { return SWITCH; }
"case"          { return CASE; }
"default"       { return DEFAULT; }
"break"         { return BREAK; }
"continue"      { return CONTINUE; }
"return"        { return RETURN; }

"printf"        { return PRINTF; }
"scanf"         { return SCANF; }

"true"          { return TRUE; }
"false"         { return FALSE; }

{FLOAT}         { return FLOAT_LITERAL; }
{INTEGER}       { return INTEGER_LITERAL; }

{IDENTIFIER}    { return IDENTIFIER_TOKEN; }

"=="            { return EQUAL; }
"!="            { return NOT_EQUAL; }
"<="            { return LESS_EQUAL; }
">="            { return GREATER_EQUAL; }
"&&"            { return LOGICAL_AND; }
"||"            { return LOGICAL_OR; }
"++"            { return INCREMENT; }
"--"            { return DECREMENT; }
"+="            { return PLUS_ASSIGN; }
"-="            { return MINUS_ASSIGN; }
"*="            { return MULTIPLY_ASSIGN; }
"/="            { return DIVIDE_ASSIGN; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULTIPLY; }
"/"             { return DIVIDE; }
"%"             { return MODULO; }
"="             { return ASSIGN; }
"<"             { return LESS; }
">"             { return GREATER; }
"!"             { return LOGICAL_NOT; }

";"             { return SEMICOLON; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }

\"([^\\\"\n]|\\.)*\" { return STRING_LITERAL; }
\'([^\\\'\n]|\\.)*\' { return CHAR_LITERAL; }

"//".*          { /* Ignore single-line comments */ }

"/*"([^*]|\*+[^*/])*\*+"/" { /* Ignore block comments */ }

{WHITESPACE}    { /* Ignore spaces and tabs */ }
\n              { line_number++; }

.               {
                    fprintf(stderr,
                    "Lexical Error: Unexpected character '%s' at line %d\n",
                    yytext, line_number);
                }

%%
