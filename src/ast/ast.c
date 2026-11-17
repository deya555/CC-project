
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int line_num;

/* Copy a string */
char *copyString(const char *str)
{
    if (str == NULL)
        return NULL;

    char *copy = malloc(strlen(str) + 1);

    if (copy == NULL)
        return NULL;

    strcpy(copy, str);
    return copy;
}

/* Create a basic AST node */
ASTNode *createNode(NodeType type)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL)
        return NULL;

    node->type = type;

    node->name = NULL;
    node->operator = NULL;
    node->declared_type = NULL;

    node->int_value = 0;
    node->float_value = 0.0;
    
    /* 2. Automatically stamp the node with the current lexer line number */
    node->line = line_num; 

    node->left = NULL;
    node->right = NULL;
    node->condition = NULL;
    node->body = NULL;
    node->else_body = NULL;
    node->next = NULL;

    return node;
}

/* Create a value node */
ASTNode *createValueNode(NodeType type, int value)
{
    ASTNode *node = createNode(type);

    if (node == NULL)
        return NULL;

    node->int_value = value;

    return node;
}

/* Create an identifier node */
ASTNode *createIdentifierNode(char *name)
{
    ASTNode *node = createNode(NODE_IDENTIFIER);

    if (node == NULL)
        return NULL;

    node->name = copyString(name);

    return node;
}

/* Create an operator node */
ASTNode *createOperatorNode(
    char *operator,
    ASTNode *left,
    ASTNode *right)
{
    ASTNode *node = createNode(NODE_BINARY);

    if (node == NULL)
        return NULL;

    node->operator = copyString(operator);
    node->left = left;
    node->right = right;

    return node;
}

/* Display the name of each AST node */
void printNodeLabel(ASTNode *node)
{
    switch (node->type)
    {
        case NODE_PROGRAM:
            printf("PROGRAM");
            break;

        case NODE_DECLARATION:
            printf("DECLARATION: %s %s",
                   node->declared_type ? node->declared_type : "",
                   node->name ? node->name : "");
            break;

        case NODE_ASSIGNMENT:
            printf("ASSIGNMENT");
            break;

        case NODE_PRINT:
            printf("PRINT");
            break;

        case NODE_IF:
            printf("IF");
            break;

        case NODE_WHILE:
            printf("WHILE");
            break;

        case NODE_BLOCK:
            printf("BLOCK");
            break;

        case NODE_BINARY:
            printf("BINARY OPERATOR: %s",
                   node->operator ? node->operator : "");
            break;

        case NODE_UNARY:
            printf("UNARY OPERATOR: %s",
                   node->operator ? node->operator : "");
            break;

        case NODE_IDENTIFIER:
            printf("IDENTIFIER: %s",
                   node->name ? node->name : "");
            break;

        case NODE_INT:
            printf("INTEGER: %d", node->int_value);
            break;

        case NODE_FLOAT:
            printf("FLOAT: %f", node->float_value);
            break;

        case NODE_BOOL:
            printf("BOOLEAN: %s",
                   node->int_value ? "true" : "false");
            break;
    }

    if (node->line > 0)
        printf(" (line %d)", node->line);

    printf("\n");
}

/* Print one AST node and its children */
void printASTNode(ASTNode *node, const char *prefix, int isLast);

/* Print a list of sibling nodes */
void printASTList(ASTNode *node, const char *prefix, int branchLast)
{
    while (node != NULL)
    {
        int isLast = (node->next == NULL);

        printASTNode(node, prefix, branchLast && isLast);

        node = node->next;
    }
}

/* Print the hierarchical AST */
void printASTNode(ASTNode *node, const char *prefix, int isLast)
{
    if (node == NULL)
        return;

    printf("%s", prefix);

    if (node->type != NODE_PROGRAM)
        printf("%s", isLast ? "`-- " : "|-- ");

    printNodeLabel(node);

    /* Prepare indentation for children */
    char newPrefix[1024];

    snprintf(newPrefix, sizeof(newPrefix), "%s%s",
             prefix,
             isLast ? "    " : "|   ");

    /* Collect child nodes */
    ASTNode *children[5];

    children[0] = node->left;
    children[1] = node->right;
    children[2] = node->condition;
    children[3] = node->body;
    children[4] = node->else_body;

    int count = 0;

    for (int i = 0; i < 5; i++)
    {
        if (children[i] != NULL)
            count++;
    }

    int printed = 0;

    for (int i = 0; i < 5; i++)
    {
        if (children[i] != NULL)
        {
            printed++;

            printASTList(
                children[i],
                newPrefix,
                printed == count
            );
        }
    }
}

/* Public AST printing function */
void printAST(ASTNode *node)
{
    if (node == NULL)
        return;

    printASTList(node, "", 1);
}

/* Free AST memory */
void freeAST(ASTNode *node)
{
    if (node == NULL)
        return;

    freeAST(node->left);
    freeAST(node->right);
    freeAST(node->condition);
    freeAST(node->body);
    freeAST(node->else_body);
    freeAST(node->next);

    free(node->name);
    free(node->operator);
    free(node->declared_type);

    free(node);
}
