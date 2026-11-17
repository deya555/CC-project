
%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"
#include "../semantic/semantic.h"
#include "../tac/codegen.h"

int yylex(void);
void yyerror(const char *s);

extern int line_num;

ASTNode *root = NULL;
SymbolTable symbolTable;

char *copyText(const char *text)
{
    char *copy = malloc(strlen(text) + 1);

    if (copy != NULL)
        strcpy(copy, text);

    return copy;
}

/* Add statements to a linked list */
ASTNode *appendNode(ASTNode *list, ASTNode *node)
{
    if (list == NULL)
        return node;

    ASTNode *temp = list;

    while (temp->next != NULL)
        temp = temp->next;

    temp->next = node;
    return list;
}
%}

%union {
    int int_value;
    float float_value;
    char *string;
    struct ASTNode *node;
}

%token T_INT T_FLOAT T_BOOL
%token T_IF T_ELSE T_WHILE T_PRINT
%token T_TRUE T_FALSE

%token <string> ID
%token <int_value> INT_CONST
%token <float_value> FLOAT_CONST

%token LE GE EQ NE AND OR

%type <node> program statement_list statement
%type <node> declaration assignment block expression

%left OR
%left AND
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/' '%'
%right '!' UMINUS

%nonassoc LOWER_THAN_ELSE
%nonassoc T_ELSE

%%

program:
    statement_list
    {
        root = createNode(NODE_PROGRAM);
        root->body = $1;
    }
    ;

statement_list:
    /* empty */
    {
        $$ = NULL;
    }
    | statement_list statement
    {
        $$ = appendNode($1, $2);
    }
    ;

statement:
    declaration
    {
        $$ = $1;
    }
    | assignment ';'
    {
        $$ = $1;
    }
    | T_PRINT expression ';'
    {
        $$ = createNode(NODE_PRINT);
        $$->left = $2;
    }
    | T_IF '(' expression ')' block %prec LOWER_THAN_ELSE
    {
        $$ = createNode(NODE_IF);
        $$->condition = $3;
        $$->body = $5;
    }
    | T_IF '(' expression ')' block T_ELSE block
    {
        $$ = createNode(NODE_IF);
        $$->condition = $3;
        $$->body = $5;
        $$->else_body = $7;
    }
    | T_WHILE '(' expression ')' block
    {
        $$ = createNode(NODE_WHILE);
        $$->condition = $3;
        $$->body = $5;
    }
    | block
    {
        $$ = $1;
    }
    ;



declaration:
    T_INT ID ';'
    {
        $$ = createNode(NODE_DECLARATION);
        $$->name = $2;
        $$->declared_type = copyText("int");
    }
    | T_FLOAT ID ';'
    {
        $$ = createNode(NODE_DECLARATION);
        $$->name = $2;
        $$->declared_type = copyText("float");
    }
    | T_BOOL ID ';'
    {
        $$ = createNode(NODE_DECLARATION);
        $$->name = $2;
        $$->declared_type = copyText("bool");
    }
    ;


assignment:
    ID '=' expression
    {

        $$ = createNode(NODE_ASSIGNMENT);
        $$->left = createIdentifierNode($1);
        $$->right = $3;

        free($1);
    }
    ;

block:
    '{'
    {
        enterScope(&symbolTable);
    }
    statement_list '}'
    {
        $$ = createNode(NODE_BLOCK);
        $$->body = $3;

        exitScope(&symbolTable);
    }
    ;

expression:
    expression OR expression
    {
        $$ = createOperatorNode("||", $1, $3);
    }
    | expression AND expression
    {
        $$ = createOperatorNode("&&", $1, $3);
    }
    | expression EQ expression
    {
        $$ = createOperatorNode("==", $1, $3);
    }
    | expression NE expression
    {
        $$ = createOperatorNode("!=", $1, $3);
    }
    | expression '<' expression
    {
        $$ = createOperatorNode("<", $1, $3);
    }
    | expression '>' expression
    {
        $$ = createOperatorNode(">", $1, $3);
    }
    | expression LE expression
    {
        $$ = createOperatorNode("<=", $1, $3);
    }
    | expression GE expression
    {
        $$ = createOperatorNode(">=", $1, $3);
    }
    | expression '+' expression
    {
        $$ = createOperatorNode("+", $1, $3);
    }
    | expression '-' expression
    {
        $$ = createOperatorNode("-", $1, $3);
    }
    | expression '*' expression
    {
        $$ = createOperatorNode("*", $1, $3);
    }
    | expression '/' expression
    {
        $$ = createOperatorNode("/", $1, $3);
    }
    | expression '%' expression
    {
        $$ = createOperatorNode("%", $1, $3);
    }
    | '!' expression
    {
        $$ = createOperatorNode("!", $2, NULL);
        $$->type = NODE_UNARY;
    }
    | '-' expression %prec UMINUS
    {
        $$ = createOperatorNode("-", $2, NULL);
        $$->type = NODE_UNARY;
    }
    | ID
    {
    $$ = createIdentifierNode($1);
    free($1);
    }
    | INT_CONST
    {
        $$ = createValueNode(NODE_INT, $1);
    }
    | FLOAT_CONST
    {
        $$ = createNode(NODE_FLOAT);
        $$->float_value = $1;
    }
    | T_TRUE
    {
        $$ = createValueNode(NODE_BOOL, 1);
    }
    | T_FALSE
    {
        $$ = createValueNode(NODE_BOOL, 0);
    }
    | '(' expression ')'
    {
        $$ = $2;
    }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Syntax error at line %d: %s\n",
            line_num, s);
}


int main(void)
{
    initSymbolTable(&symbolTable);

    printf("Enter your program (Ctrl+Z then Enter to finish):\n");

    if (yyparse() == 0)
    {
        printf("\nSyntax is valid!\n");

        printf("\nAbstract Syntax Tree:\n");
        printAST(root);

        int semanticErrors = analyzeSemantics(root, &symbolTable);

        if (semanticErrors == 0)
        {
            printf("\nSemantic analysis successful!\n");
        }
        else
        {
            printf("\nSemantic analysis failed!\n");
        }

        printSymbolTable(&symbolTable);

        /* Generate Three Address Code */
    if (semanticErrors == 0)
    {
        TACInstr *tac = generateTAC(root);
        printTAC(tac);
        freeTAC(tac);
    }

freeAST(root);
    }

    freeSymbolTable(&symbolTable);

    return 0;
}

