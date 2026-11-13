%option noyywrap
%option yylineno

%{
#include <stdio.h>

int lexical_error_count = 0;

#define PRINT_TOKEN(type) \
    printf("%-18s : %-12s (line %d)\n", type, yytext, yylineno)
%}

%x COMMENT

DIGIT       [0-9]
LETTER      [A-Za-z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
FLOAT       ({DIGIT}+\.{DIGIT}*|\.{DIGIT}+)
INTEGER     {DIGIT}+

%%

"int"       { PRINT_TOKEN("KEYWORD"); }
"float"     { PRINT_TOKEN("KEYWORD"); }
"bool"      { PRINT_TOKEN("KEYWORD"); }
"if"        { PRINT_TOKEN("KEYWORD"); }
"else"      { PRINT_TOKEN("KEYWORD"); }
"while"     { PRINT_TOKEN("KEYWORD"); }
"print"     { PRINT_TOKEN("KEYWORD"); }

"true"      { PRINT_TOKEN("BOOL_LITERAL"); }
"false"     { PRINT_TOKEN("BOOL_LITERAL"); }

{FLOAT}     { PRINT_TOKEN("FLOAT_LITERAL"); }
{INTEGER}   { PRINT_TOKEN("INTEGER_LITERAL"); }

{IDENTIFIER} { PRINT_TOKEN("IDENTIFIER"); }

 /* Arithmetic operators */
"+"         { PRINT_TOKEN("ARITHMETIC_OPERATOR"); }
"-"         { PRINT_TOKEN("ARITHMETIC_OPERATOR"); }
"*"         { PRINT_TOKEN("ARITHMETIC_OPERATOR"); }
"/"         { PRINT_TOKEN("ARITHMETIC_OPERATOR"); }
"%"         { PRINT_TOKEN("ARITHMETIC_OPERATOR"); }

 /* Relational and equality operators */
"<="        { PRINT_TOKEN("RELATIONAL_OPERATOR"); }
">="        { PRINT_TOKEN("RELATIONAL_OPERATOR"); }
"=="        { PRINT_TOKEN("RELATIONAL_OPERATOR"); }
"!="        { PRINT_TOKEN("RELATIONAL_OPERATOR"); }
"<"         { PRINT_TOKEN("RELATIONAL_OPERATOR"); }
">"         { PRINT_TOKEN("RELATIONAL_OPERATOR"); }

 /* Logical operators */
"&&"        { PRINT_TOKEN("LOGICAL_OPERATOR"); }
"||"        { PRINT_TOKEN("LOGICAL_OPERATOR"); }
"!"         { PRINT_TOKEN("LOGICAL_OPERATOR"); }

 /* Assignment operator */
"="         { PRINT_TOKEN("ASSIGNMENT_OPERATOR"); }

 /* Delimiters — using [{] / [}] to avoid flex "{...}" ambiguity */
[{]         { PRINT_TOKEN("DELIMITER"); }
[}]         { PRINT_TOKEN("DELIMITER"); }
"("         { PRINT_TOKEN("DELIMITER"); }
")"         { PRINT_TOKEN("DELIMITER"); }
";"         { PRINT_TOKEN("DELIMITER"); }

 /* Single-line comments */
"//".*      { /* ignore */ }

 /* Multi-line comments */
"/*"        { BEGIN(COMMENT); }
<COMMENT>"*/" { BEGIN(INITIAL); }
<COMMENT>\n  { /* ignore */ }
<COMMENT>.   { /* ignore */ }

<COMMENT><<EOF>> {
    printf("LEXICAL ERROR: Unterminated comment at line %d\n", yylineno);
    lexical_error_count++;
    return 0;
}

 /* Ignore whitespace (also strips \r from CRLF input files) */
[ \t\r\n]+  { /* ignore */ }

 /* Invalid characters */
. {
    printf("LEXICAL ERROR: Invalid character '%s' at line %d\n",
           yytext, yylineno);
    lexical_error_count++;
}

%%

int main(int argc, char **argv)
{
    printf("========== LEXICAL ANALYSIS ==========\n\n");

    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("fopen");
            return 2;
        }
        yyin = f;
    }

    yylex();

    printf("\n======================================\n");

    if (lexical_error_count == 0) {
        printf("Lexical analysis completed successfully.\n");
        printf("No lexical errors found.\n");
    } else {
        printf("Lexical analysis completed with %d error(s).\n",
               lexical_error_count);
    }

    return lexical_error_count == 0 ? 0 : 1;
}