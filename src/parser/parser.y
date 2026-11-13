
%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);

extern int line_num;
%}

/* Tokens received from Flex */
%token T_INT T_FLOAT T_BOOL
%token T_IF T_ELSE T_WHILE T_PRINT
%token T_TRUE T_FALSE

%token ID INT_CONST FLOAT_CONST

%token LE GE EQ NE AND OR

/* Operator precedence */
%left OR
%left AND
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/' '%'
%right '!' UMINUS

/* Solve the dangling else problem */
%nonassoc LOWER_THAN_ELSE
%nonassoc T_ELSE

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
    | assignment ';'
    | T_PRINT expression ';'
    | T_IF '(' expression ')' block %prec LOWER_THAN_ELSE
    | T_IF '(' expression ')' block T_ELSE block
    | T_WHILE '(' expression ')' block
    | block
    ;

declaration:
    type ID ';'
    ;

type:
    T_INT
    | T_FLOAT
    | T_BOOL
    ;

assignment:
    ID '=' expression
    ;

block:
    '{' statement_list '}'
    ;

expression:
    expression OR expression
    | expression AND expression
    | expression EQ expression
    | expression NE expression
    | expression '<' expression
    | expression '>' expression
    | expression LE expression
    | expression GE expression
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression '%' expression
    | '!' expression
    | '-' expression %prec UMINUS
    | ID
    | INT_CONST
    | FLOAT_CONST
    | T_TRUE
    | T_FALSE
    | '(' expression ')'
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Syntax error at line %d: %s\n", line_num, s);
}

int main(void)
{
    printf("Enter your program (Ctrl+Z then Enter to finish):\n");

    if (yyparse() == 0)
        printf("Syntax is valid!\n");

    return 0;
}
