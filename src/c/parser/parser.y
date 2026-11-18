
%{
#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../semantic/semantic.h"
#include "../symbol_table/symbol_table.h"
#include "../tac/tac.h"

int yylex(void);
void yyerror(const char *message);

ASTNode *ast_root = NULL;
%}

%error-verbose

%union {
    char *text;
    struct ASTNode *node;
}

%token INT FLOAT_TYPE DOUBLE CHAR VOID
%token SHORT LONG SIGNED UNSIGNED

%token IF ELSE WHILE FOR DO
%token SWITCH CASE DEFAULT BREAK CONTINUE RETURN

%token PRINTF SCANF
%token TRUE FALSE

%token <text> INTEGER_LITERAL FLOAT_LITERAL
%token <text> STRING_LITERAL CHAR_LITERAL IDENTIFIER_TOKEN

%token EQUAL NOT_EQUAL LESS_EQUAL GREATER_EQUAL
%token LOGICAL_AND LOGICAL_OR
%token INCREMENT DECREMENT
%token PLUS_ASSIGN MINUS_ASSIGN MULTIPLY_ASSIGN DIVIDE_ASSIGN

%token PLUS MINUS MULTIPLY DIVIDE MODULO
%token ASSIGN LESS GREATER LOGICAL_NOT

%token SEMICOLON COMMA
%token LPAREN RPAREN LBRACE RBRACE
%token LBRACKET RBRACKET

%type <node> program statement_list statement
%type <node> declaration declarator_list declarator
%type <node> type_specifier assignment expression
%type <node> expression_statement print_statement
%type <node> input_statement print_arguments argument_list
%type <node> if_statement while_statement block

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
    {
        $$ = ast_create(AST_PROGRAM, "Program");

        if ($1 != NULL)
            ast_add_child($$, $1);

        ast_root = $$;
    }
    ;

statement_list:
    /* empty */
    {
        $$ = NULL;
    }
    | statement_list statement
    {
        if ($1 == NULL)
            $$ = ast_create(AST_BLOCK, "Statements");
        else
            $$ = $1;

        ast_add_child($$, $2);
    }
    ;

statement:
      declaration          { $$ = $1; }
    | assignment           { $$ = $1; }
    | expression_statement { $$ = $1; }
    | print_statement      { $$ = $1; }
    | input_statement      { $$ = $1; }
    | if_statement         { $$ = $1; }
    | while_statement      { $$ = $1; }
    | block                { $$ = $1; }
    | RETURN expression SEMICOLON
      {
          $$ = ast_create(AST_RETURN, "return");
          ast_add_child($$, $2);
      }
    | RETURN SEMICOLON
      {
          $$ = ast_create(AST_RETURN, "return");
      }
    ;

declaration:
    type_specifier declarator_list SEMICOLON
    {
        $$ = ast_create(AST_DECLARATION, "Declaration");
        ast_add_child($$, $1);
        ast_add_child($$, $2);
    }
    ;

declarator_list:
    declarator
    {
        $$ = ast_create(AST_BLOCK, "Variables");
        ast_add_child($$, $1);
    }
    | declarator_list COMMA declarator
    {
        $$ = $1;
        ast_add_child($$, $3);
    }
    ;

declarator:
    IDENTIFIER_TOKEN
    {
        $$ = ast_create(AST_VARIABLE, $1);
    }
    | IDENTIFIER_TOKEN ASSIGN expression
    {
        $$ = ast_create(AST_VARIABLE, $1);
        ast_add_child($$, $3);
    }
    ;

type_specifier:
      INT       { $$ = ast_create(AST_TYPE, "int"); }
    | FLOAT_TYPE { $$ = ast_create(AST_TYPE, "float"); }
    | DOUBLE    { $$ = ast_create(AST_TYPE, "double"); }
    | CHAR      { $$ = ast_create(AST_TYPE, "char"); }
    | VOID      { $$ = ast_create(AST_TYPE, "void"); }
    | SHORT     { $$ = ast_create(AST_TYPE, "short"); }
    | LONG      { $$ = ast_create(AST_TYPE, "long"); }
    | SIGNED    { $$ = ast_create(AST_TYPE, "signed"); }
    | UNSIGNED  { $$ = ast_create(AST_TYPE, "unsigned"); }
    ;

assignment:
      IDENTIFIER_TOKEN ASSIGN expression SEMICOLON
      {
          $$ = ast_create(AST_ASSIGNMENT, "=");
          ast_add_child($$, ast_create(AST_IDENTIFIER, $1));
          ast_add_child($$, $3);
      }
    | IDENTIFIER_TOKEN PLUS_ASSIGN expression SEMICOLON
      {
          $$ = ast_create(AST_ASSIGNMENT, "+=");
          ast_add_child($$, ast_create(AST_IDENTIFIER, $1));
          ast_add_child($$, $3);
      }
    | IDENTIFIER_TOKEN MINUS_ASSIGN expression SEMICOLON
      {
          $$ = ast_create(AST_ASSIGNMENT, "-=");
          ast_add_child($$, ast_create(AST_IDENTIFIER, $1));
          ast_add_child($$, $3);
      }
    | IDENTIFIER_TOKEN MULTIPLY_ASSIGN expression SEMICOLON
      {
          $$ = ast_create(AST_ASSIGNMENT, "*=");
          ast_add_child($$, ast_create(AST_IDENTIFIER, $1));
          ast_add_child($$, $3);
      }
    | IDENTIFIER_TOKEN DIVIDE_ASSIGN expression SEMICOLON
      {
          $$ = ast_create(AST_ASSIGNMENT, "/=");
          ast_add_child($$, ast_create(AST_IDENTIFIER, $1));
          ast_add_child($$, $3);
      }
    ;

expression_statement:
    expression SEMICOLON
    {
        $$ = ast_create(AST_EXPRESSION_STATEMENT, "Expression");
        ast_add_child($$, $1);
    }
    ;

print_statement:
    PRINTF LPAREN STRING_LITERAL print_arguments RPAREN SEMICOLON
    {
        $$ = ast_create(AST_PRINT, "printf");
        ast_add_child($$, ast_create(AST_STRING, $3));
        ast_add_child($$, $4);
    }
    ;

print_arguments:
    /* empty */
    {
        $$ = ast_create(AST_BLOCK, "Arguments");
    }
    | COMMA argument_list
    {
        $$ = $2;
    }
    ;

input_statement:
    SCANF LPAREN STRING_LITERAL COMMA argument_list RPAREN SEMICOLON
    {
        $$ = ast_create(AST_PRINT, "scanf");
        ast_add_child($$, ast_create(AST_STRING, $3));
        ast_add_child($$, $5);
    }
    ;

argument_list:
    expression
    {
        $$ = ast_create(AST_BLOCK, "Arguments");
        ast_add_child($$, $1);
    }
    | argument_list COMMA expression
    {
        $$ = $1;
        ast_add_child($$, $3);
    }
    ;

if_statement:
    IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE
    {
        $$ = ast_create(AST_IF, "if");
        ast_add_child($$, $3);
        ast_add_child($$, $5);
    }
    | IF LPAREN expression RPAREN statement ELSE statement
    {
        $$ = ast_create(AST_IF, "if-else");
        ast_add_child($$, $3);
        ast_add_child($$, $5);
        ast_add_child($$, $7);
    }
    ;

while_statement:
    WHILE LPAREN expression RPAREN statement
    {
        $$ = ast_create(AST_WHILE, "while");
        ast_add_child($$, $3);
        ast_add_child($$, $5);
    }
    ;

block:
    LBRACE statement_list RBRACE
    {
        $$ = ast_create(AST_BLOCK, "Block");

        if ($2 != NULL)
            ast_add_child($$, $2);
    }
    ;

expression:
      INTEGER_LITERAL
      { $$ = ast_create(AST_INTEGER, $1); }
    | FLOAT_LITERAL
      { $$ = ast_create(AST_FLOAT, $1); }
    | STRING_LITERAL
      { $$ = ast_create(AST_STRING, $1); }
    | CHAR_LITERAL
      { $$ = ast_create(AST_CHAR, $1); }
    | TRUE
      { $$ = ast_create(AST_IDENTIFIER, "true"); }
    | FALSE
      { $$ = ast_create(AST_IDENTIFIER, "false"); }
    | IDENTIFIER_TOKEN
      { $$ = ast_create(AST_IDENTIFIER, $1); }
    | LPAREN expression RPAREN
      { $$ = $2; }
    | expression PLUS expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "+");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression MINUS expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "-");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression MULTIPLY expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "*");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression DIVIDE expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "/");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression MODULO expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "%");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression LESS expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "<");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression GREATER expression
      {
          $$ = ast_create(AST_BINARY_EXPR, ">");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression LESS_EQUAL expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "<=");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression GREATER_EQUAL expression
      {
          $$ = ast_create(AST_BINARY_EXPR, ">=");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression EQUAL expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "==");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression NOT_EQUAL expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "!=");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression LOGICAL_AND expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "&&");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression LOGICAL_OR expression
      {
          $$ = ast_create(AST_BINARY_EXPR, "||");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | LOGICAL_NOT expression
      {
          $$ = ast_create(AST_UNARY_EXPR, "!");
          ast_add_child($$, $2);
      }
    | MINUS expression %prec UMINUS
      {
          $$ = ast_create(AST_UNARY_EXPR, "-");
          ast_add_child($$, $2);
      }
    | IDENTIFIER_TOKEN INCREMENT
      {
          $$ = ast_create(AST_UNARY_EXPR, "++");
          ast_add_child($$, ast_create(AST_IDENTIFIER, $1));
      }
    | IDENTIFIER_TOKEN DECREMENT
      {
          $$ = ast_create(AST_UNARY_EXPR, "--");
          ast_add_child($$, ast_create(AST_IDENTIFIER, $1));
      }
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
        printf("\nSyntax Analysis Successful!\n");

        printf("\n========== ABSTRACT SYNTAX TREE ==========\n");
        ast_print(ast_root);

        SymbolTable table;

        printf("\nStarting Semantic Analysis...\n");

        semantic_analyze(ast_root, &table);

        symbol_table_print(&table);

        printf("\nStarting TAC Generation...\n");

tac_generate(ast_root);
tac_print();
tac_free();

        ast_free(ast_root);
    }
    else
    {
        printf("\nSyntax Analysis Failed!\n");
    }

    return 0;
}
