
%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *message);
%}

/* Generate the shared parser header */

%error-verbose

%token INT FLOAT_TYPE DOUBLE CHAR VOID
%token SHORT LONG SIGNED UNSIGNED

%token IF ELSE WHILE FOR DO
%token SWITCH CASE DEFAULT BREAK CONTINUE RETURN

%token PRINTF SCANF
%token TRUE FALSE

%token INTEGER_LITERAL FLOAT_LITERAL
%token STRING_LITERAL CHAR_LITERAL IDENTIFIER_TOKEN

%token EQUAL NOT_EQUAL LESS_EQUAL GREATER_EQUAL
%token LOGICAL_AND LOGICAL_OR
%token INCREMENT DECREMENT
%token PLUS_ASSIGN MINUS_ASSIGN MULTIPLY_ASSIGN DIVIDE_ASSIGN

%token PLUS MINUS MULTIPLY DIVIDE MODULO
%token ASSIGN LESS GREATER LOGICAL_NOT

%token SEMICOLON COMMA
%token LPAREN RPAREN LBRACE RBRACE
%token LBRACKET RBRACKET

%left LOGICAL_OR
%left LOGICAL_AND
%left EQUAL NOT_EQUAL
%left LESS GREATER LESS_EQUAL GREATER_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%right LOGICAL_NOT
%right UMINUS

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

program:
    statement_list
    ;

statement_list:
    /* empty */
    | statement_list statement
    ;

statement:
      declaration
    | assignment
    | expression_statement
    | print_statement
    | input_statement
    | if_statement
    | while_statement
    | block
    | RETURN expression SEMICOLON
    | RETURN SEMICOLON
    ;

declaration:
      type_specifier declarator_list SEMICOLON
    ;

declarator_list:
      declarator
    | declarator_list COMMA declarator
    ;

declarator:
      IDENTIFIER_TOKEN
    | IDENTIFIER_TOKEN ASSIGN expression
    ;

type_specifier:
      INT
    | FLOAT_TYPE
    | DOUBLE
    | CHAR
    | VOID
    | SHORT
    | LONG
    | SIGNED
    | UNSIGNED
    ;

assignment:
      IDENTIFIER_TOKEN ASSIGN expression SEMICOLON
    | IDENTIFIER_TOKEN PLUS_ASSIGN expression SEMICOLON
    | IDENTIFIER_TOKEN MINUS_ASSIGN expression SEMICOLON
    | IDENTIFIER_TOKEN MULTIPLY_ASSIGN expression SEMICOLON
    | IDENTIFIER_TOKEN DIVIDE_ASSIGN expression SEMICOLON
    ;

expression_statement:
      expression SEMICOLON
    ;

print_statement:
      PRINTF LPAREN STRING_LITERAL print_arguments RPAREN SEMICOLON
    ;

print_arguments:
      /* empty */
    | COMMA argument_list
    ;

input_statement:
      SCANF LPAREN STRING_LITERAL COMMA argument_list RPAREN SEMICOLON
    ;

argument_list:
      expression
    | argument_list COMMA expression
    ;

if_statement:
      IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE
    | IF LPAREN expression RPAREN statement ELSE statement
    ;

while_statement:
      WHILE LPAREN expression RPAREN statement
    ;

block:
      LBRACE statement_list RBRACE
    ;

expression:
      INTEGER_LITERAL
    | FLOAT_LITERAL
    | STRING_LITERAL
    | CHAR_LITERAL
    | TRUE
    | FALSE
    | IDENTIFIER_TOKEN
    | LPAREN expression RPAREN
    | expression PLUS expression
    | expression MINUS expression
    | expression MULTIPLY expression
    | expression DIVIDE expression
    | expression MODULO expression
    | expression LESS expression
    | expression GREATER expression
    | expression LESS_EQUAL expression
    | expression GREATER_EQUAL expression
    | expression EQUAL expression
    | expression NOT_EQUAL expression
    | expression LOGICAL_AND expression
    | expression LOGICAL_OR expression
    | LOGICAL_NOT expression
    | MINUS expression %prec UMINUS
    | IDENTIFIER_TOKEN INCREMENT
    | IDENTIFIER_TOKEN DECREMENT
    ;

%%

void yyerror(const char *message)
{
    fprintf(stderr, "Syntax Error: %s\n", message);
}

int main(void)
{
    printf("C Compiler Parser Started\n");

    if (yyparse() == 0)
    {
        printf("Syntax Analysis Successful!\n");
        return 0;
    }

    printf("Syntax Analysis Failed!\n");
    return 1;
}
