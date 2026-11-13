
%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex(void);
extern int yylineno;
extern int lexical_error_count;

int syntax_error_count = 0;

void yyerror(const char *message);
%}

/* Token definitions */
%token INT FLOAT BOOL
%token IF ELSE WHILE PRINT
%token TRUE FALSE

%token IDENTIFIER
%token INTEGER_LITERAL FLOAT_LITERAL

%token PLUS MINUS MULTIPLY DIVIDE MODULO
%token LT GT LE GE EQ NE
%token AND OR NOT
%token ASSIGN

%token LBRACE RBRACE
%token LPAREN RPAREN
%token SEMICOLON
%token INVALID

/* Operator precedence */
%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%right NOT UMINUS

/* Resolve the if-else ambiguity */
%nonassoc IF_WITHOUT_ELSE
%nonassoc ELSE

%start program

%%

program
    : statement_list
    ;

statement_list
    : /* empty */
    | statement_list statement
    ;

statement
    : declaration
    | assignment
    | print_statement
    | if_statement
    | while_statement
    | block
    | error SEMICOLON { yyerrok; }
    ;

type
    : INT
    | FLOAT
    | BOOL
    ;

declaration
    : type IDENTIFIER SEMICOLON
    | type IDENTIFIER ASSIGN expression SEMICOLON
    ;

assignment
    : IDENTIFIER ASSIGN expression SEMICOLON
    ;

print_statement
    : PRINT LPAREN expression RPAREN SEMICOLON
    ;

if_statement
    : IF LPAREN expression RPAREN statement
      %prec IF_WITHOUT_ELSE
    | IF LPAREN expression RPAREN statement ELSE statement
    ;

while_statement
    : WHILE LPAREN expression RPAREN statement
    ;

block
    : LBRACE statement_list RBRACE
    ;

expression
    : expression PLUS expression
    | expression MINUS expression
    | expression MULTIPLY expression
    | expression DIVIDE expression
    | expression MODULO expression

    | expression LT expression
    | expression GT expression
    | expression LE expression
    | expression GE expression
    | expression EQ expression
    | expression NE expression

    | expression AND expression
    | expression OR expression

    | NOT expression
    | MINUS expression %prec UMINUS

    | LPAREN expression RPAREN

    | IDENTIFIER
    | INTEGER_LITERAL
    | FLOAT_LITERAL
    | TRUE
    | FALSE
    ;

%%

void yyerror(const char *message)
{
    syntax_error_count++;

    fprintf(stderr,
            "SYNTAX ERROR at line %d: %s\n",
            yylineno,
            message);
}

int main(void)
{
    int result;

    printf("========== SYNTAX ANALYSIS ==========\n\n");

    result = yyparse();

    printf("\n=====================================\n");

    if (result == 0 &&
        syntax_error_count == 0 &&
        lexical_error_count == 0)
    {
        printf("Syntax analysis completed successfully.\n");
        printf("No syntax errors found.\n");
        return 0;
    }

    printf("Syntax analysis completed with errors.\n");
    printf("Syntax errors: %d\n", syntax_error_count);
    printf("Lexical errors: %d\n", lexical_error_count);

    return 1;
}
