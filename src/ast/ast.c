
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* Copy a string into newly allocated memory */
char *copyString(const char *str)
{
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
    node->declared_type = NULL;
    node->operator = NULL;

    node->int_value = 0;
    node->float_value = 0.0;

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

    if (type == NODE_FLOAT)
        node->float_value = (float)value;

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

/* Print the AST */
void printAST(ASTNode *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
        case NODE_PROGRAM:
            printf("PROGRAM\n");
            break;

        case NODE_DECLARATION:
            printf("DECLARATION: %s %s\n",
                node->declared_type ? node->declared_type : "",
                node->name ? node->name : "");
            break;

        case NODE_ASSIGNMENT:
            printf("ASSIGNMENT\n");
            break;

        case NODE_PRINT:
            printf("PRINT\n");
            break;

        case NODE_IF:
            printf("IF\n");
            break;

        case NODE_WHILE:
            printf("WHILE\n");
            break;

        case NODE_BLOCK:
            printf("BLOCK\n");
            break;

        case NODE_BINARY:
            printf("OPERATOR: %s\n",
                   node->operator ? node->operator : "");
            break;

        case NODE_UNARY:
            printf("UNARY OPERATOR: %s\n",
                   node->operator ? node->operator : "");
            break;

        case NODE_IDENTIFIER:
            printf("IDENTIFIER: %s\n",
                   node->name ? node->name : "");
            break;

        case NODE_INT:
            printf("INTEGER: %d\n", node->int_value);
            break;

        case NODE_FLOAT:
            printf("FLOAT: %f\n", node->float_value);
            break;

        case NODE_BOOL:
            printf("BOOLEAN: %s\n",
                   node->int_value ? "true" : "false");
            break;
    }

    printAST(node->left);
    printAST(node->right);
    printAST(node->condition);
    printAST(node->body);
    printAST(node->else_body);
    printAST(node->next);
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
    free(node->declared_type);
    free(node->operator);
    free(node);
}
