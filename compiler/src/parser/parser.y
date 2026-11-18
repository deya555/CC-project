%code requires {
#include "../ast/ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"

extern int yylex(void);
extern int yylineno;
extern int lexical_error_count;

int syntax_error_count = 0;
int symbol_error_count = 0;

ASTNode *ast_root = NULL;

SymbolTable symbol_table;

void yyerror(const char *message);
%}

%union {
    char *text;
    ASTNode *node;
}

%token INT FLOAT BOOL
%token IF ELSE WHILE PRINT
%token TRUE FALSE

%token <text> IDENTIFIER
%token <text> INTEGER_LITERAL FLOAT_LITERAL

%token PLUS MINUS MULTIPLY DIVIDE MODULO
%token LT GT LE GE EQ NE
%token AND OR NOT
%token ASSIGN

%token LBRACE RBRACE
%token LPAREN RPAREN
%token SEMICOLON
%token INVALID

%type <text> type

%type <node> program statement_list statement
%type <node> declaration assignment print_statement
%type <node> if_statement while_statement block expression

%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%right NOT UMINUS

%nonassoc IF_WITHOUT_ELSE
%nonassoc ELSE

%start program

%%

program
    : statement_list
      {
          $$ = ast_create("Program", NULL);
          ast_add_child($$, $1);
          ast_root = $$;
      }
    ;

statement_list
    : /* empty */
      {
          $$ = ast_create("StatementList", NULL);
      }
    | statement_list statement
      {
          ast_add_child($1, $2);
          $$ = $1;
      }
    ;

statement
    : declaration       { $$ = $1; }
    | assignment        { $$ = $1; }
    | print_statement   { $$ = $1; }
    | if_statement      { $$ = $1; }
    | while_statement   { $$ = $1; }
    | block             { $$ = $1; }
    | error SEMICOLON
      {
          yyerrok;
          $$ = ast_create("Error", NULL);
      }
    ;

type
    : INT   { $$ = ast_copy("int"); }
    | FLOAT { $$ = ast_copy("float"); }
    | BOOL  { $$ = ast_copy("bool"); }
    ;

declaration
    : type IDENTIFIER SEMICOLON
      {
          int declaration_line = yylineno;

          if (!insert_symbol(
                  &symbol_table,
                  $2,
                  $1,
                  declaration_line
              )) {
              fprintf(
                  stderr,
                  "SYMBOL TABLE ERROR at line %d: "
                  "Variable '%s' already declared in this scope.\n",
                  declaration_line,
                  $2
              );

              symbol_error_count++;
          }

          $$ = ast_create("VariableDeclaration", $1);

          ast_add_child(
              $$,
              ast_create("Identifier", $2)
          );

          free($1);
          free($2);
      }
    | type IDENTIFIER ASSIGN expression SEMICOLON
      {
          int declaration_line = yylineno;

          if (!insert_symbol(
                  &symbol_table,
                  $2,
                  $1,
                  declaration_line
              )) {
              fprintf(
                  stderr,
                  "SYMBOL TABLE ERROR at line %d: "
                  "Variable '%s' already declared in this scope.\n",
                  declaration_line,
                  $2
              );

              symbol_error_count++;
          }

          $$ = ast_create("VariableDeclaration", $1);

          ast_add_child(
              $$,
              ast_create("Identifier", $2)
          );

          ast_add_child($$, $4);

          free($1);
          free($2);
      }
    ;

assignment
    : IDENTIFIER ASSIGN expression SEMICOLON
      {
          $$ = ast_create("Assignment", NULL);

          ast_add_child(
              $$,
              ast_create("Identifier", $1)
          );

          ast_add_child($$, $3);

          free($1);
      }
    ;

print_statement
    : PRINT LPAREN expression RPAREN SEMICOLON
      {
          $$ = ast_create("PrintStatement", NULL);
          ast_add_child($$, $3);
      }
    ;

if_statement
    : IF LPAREN expression RPAREN statement
      %prec IF_WITHOUT_ELSE
      {
          $$ = ast_create("IfStatement", NULL);

          ast_add_child($$, $3);
          ast_add_child($$, $5);
      }
    | IF LPAREN expression RPAREN statement ELSE statement
      {
          $$ = ast_create("IfElseStatement", NULL);

          ast_add_child($$, $3);
          ast_add_child($$, $5);
          ast_add_child($$, $7);
      }
    ;

while_statement
    : WHILE LPAREN expression RPAREN statement
      {
          $$ = ast_create("WhileStatement", NULL);

          ast_add_child($$, $3);
          ast_add_child($$, $5);
      }
    ;

block
    : LBRACE
      {
          enter_scope(&symbol_table);
      }
      statement_list RBRACE
      {
          $$ = ast_create("Block", NULL);
          ast_add_child($$, $3);

          exit_scope(&symbol_table);
      }
    ;

expression
    : expression PLUS expression
      {
          $$ = ast_create("BinaryOperation", "+");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression MINUS expression
      {
          $$ = ast_create("BinaryOperation", "-");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression MULTIPLY expression
      {
          $$ = ast_create("BinaryOperation", "*");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression DIVIDE expression
      {
          $$ = ast_create("BinaryOperation", "/");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression MODULO expression
      {
          $$ = ast_create("BinaryOperation", "%");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression LT expression
      {
          $$ = ast_create("RelationalOperation", "<");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression GT expression
      {
          $$ = ast_create("RelationalOperation", ">");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression LE expression
      {
          $$ = ast_create("RelationalOperation", "<=");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression GE expression
      {
          $$ = ast_create("RelationalOperation", ">=");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression EQ expression
      {
          $$ = ast_create("RelationalOperation", "==");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression NE expression
      {
          $$ = ast_create("RelationalOperation", "!=");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression AND expression
      {
          $$ = ast_create("LogicalOperation", "&&");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | expression OR expression
      {
          $$ = ast_create("LogicalOperation", "||");
          ast_add_child($$, $1);
          ast_add_child($$, $3);
      }
    | NOT expression
      {
          $$ = ast_create("UnaryOperation", "!");
          ast_add_child($$, $2);
      }
    | MINUS expression %prec UMINUS
      {
          $$ = ast_create("UnaryOperation", "-");
          ast_add_child($$, $2);
      }
    | LPAREN expression RPAREN
      {
          $$ = $2;
      }
    | IDENTIFIER
      {
          $$ = ast_create("Identifier", $1);
          free($1);
      }
    | INTEGER_LITERAL
      {
          $$ = ast_create("IntegerLiteral", $1);
          free($1);
      }
    | FLOAT_LITERAL
      {
          $$ = ast_create("FloatLiteral", $1);
          free($1);
      }
    | TRUE
      {
          $$ = ast_create("BooleanLiteral", "true");
      }
    | FALSE
      {
          $$ = ast_create("BooleanLiteral", "false");
      }
    ;

%%

void yyerror(const char *message)
{
    syntax_error_count++;

    fprintf(
        stderr,
        "SYNTAX ERROR at line %d: %s\n",
        yylineno,
        message
    );
}

int main(void)
{
    int result;

    symbol_table_init(&symbol_table);

    printf("========== SYNTAX ANALYSIS ==========\n\n");

    result = yyparse();

    printf("\n=====================================\n");

    if (
        result == 0 &&
        syntax_error_count == 0 &&
        lexical_error_count == 0
    ) {
        printf("Syntax analysis completed successfully.\n");
        printf("No syntax errors found.\n");

        printf("\n========== ABSTRACT SYNTAX TREE ==========\n\n");

        ast_print(ast_root, 0);

        printf("\n");
        print_symbol_table(&symbol_table);

        if (symbol_error_count == 0) {
            printf("\nSymbol Table analysis completed successfully.\n");
        } else {
            printf("\nSymbol Table analysis completed with errors.\n");
            printf("Symbol errors: %d\n", symbol_error_count);
        }

        ast_free(ast_root);
        free_symbol_table(&symbol_table);

        return symbol_error_count == 0 ? 0 : 1;
    }

    printf("Syntax analysis completed with errors.\n");
    printf("Syntax errors: %d\n", syntax_error_count);
    printf("Lexical errors: %d\n", lexical_error_count);
    printf("Symbol errors: %d\n", symbol_error_count);

    ast_free(ast_root);
    free_symbol_table(&symbol_table);

    return 1;
}