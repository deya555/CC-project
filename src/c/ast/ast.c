#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

static char *copy_string(const char *text)
{
    if (text == NULL)
        return NULL;

    char *copy = malloc(strlen(text) + 1);

    if (copy != NULL)
        strcpy(copy, text);

    return copy;
}

ASTNode *ast_create(ASTNodeType type, const char *value)
{
    ASTNode *node = malloc(sizeof(ASTNode));

    if (node == NULL)
    {
        fprintf(stderr, "Error: AST memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    node->type = type;
    node->value = copy_string(value);
    node->child = NULL;
    node->next = NULL;

    return node;
}

void ast_add_child(ASTNode *parent, ASTNode *child)
{
    if (parent == NULL || child == NULL)
        return;

    if (parent->child == NULL)
    {
        parent->child = child;
        return;
    }

    ASTNode *current = parent->child;

    while (current->next != NULL)
        current = current->next;

    current->next = child;
}

static const char *ast_node_name(ASTNodeType type)
{
    switch (type)
    {
        case AST_PROGRAM: return "PROGRAM";
        case AST_DECLARATION: return "DECLARATION";
        case AST_TYPE: return "TYPE";
        case AST_VARIABLE: return "VARIABLE";
        case AST_ASSIGNMENT: return "ASSIGNMENT";
        case AST_IF: return "IF";
        case AST_WHILE: return "WHILE";
        case AST_BLOCK: return "BLOCK";
        case AST_PRINT: return "PRINT";
        case AST_RETURN: return "RETURN";
        case AST_BINARY_EXPR: return "BINARY_EXPRESSION";
        case AST_UNARY_EXPR: return "UNARY_EXPRESSION";
        case AST_IDENTIFIER: return "IDENTIFIER";
        case AST_INTEGER: return "INTEGER";
        case AST_FLOAT: return "FLOAT";
        case AST_STRING: return "STRING";
        case AST_CHAR: return "CHAR";
        case AST_BOOLEAN: return "BOOLEAN";
        case AST_EXPRESSION_STATEMENT:
            return "EXPRESSION_STATEMENT";
        default: return "UNKNOWN";
    }
}

static void print_tree(ASTNode *node, int depth)
{
    if (node == NULL)
        return;

    for (int i = 0; i < depth; i++)
        printf("  ");

    printf("%s", ast_node_name(node->type));

    if (node->value != NULL)
        printf(" : %s", node->value);

    printf("\n");

    print_tree(node->child, depth + 1);
    print_tree(node->next, depth);
}

void ast_print(ASTNode *root)
{
    print_tree(root, 0);
}

void ast_free(ASTNode *root)
{
    if (root == NULL)
        return;

    ast_free(root->child);
    ast_free(root->next);

    free(root->value);
    free(root);
}