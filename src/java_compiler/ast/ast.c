
#include "ast.h"

#include <stdlib.h>
#include <string.h>

ASTNode *createASTNode(ASTNodeType type,
                       const char *value,
                       const char *dataType)
{
    ASTNode *node = (ASTNode *)calloc(1, sizeof(ASTNode));

    if (node == NULL) {
        fprintf(stderr, "Error: AST memory allocation failed.\n");
        return NULL;
    }

    node->type = type;

    if (value != NULL) {
        snprintf(node->value, sizeof(node->value), "%s", value);
    }

    if (dataType != NULL) {
        snprintf(node->dataType, sizeof(node->dataType), "%s", dataType);
    }

    return node;
}

int addASTChild(ASTNode *parent, ASTNode *child)
{
    if (parent == NULL || child == NULL) {
        return 0;
    }

    if (parent->childCount >= AST_MAX_CHILDREN) {
        fprintf(stderr, "Error: Maximum AST children exceeded.\n");
        return 0;
    }

    parent->children[parent->childCount++] = child;
    return 1;
}

const char *astNodeTypeName(ASTNodeType type)
{
    switch (type) {
        case AST_PROGRAM:               return "PROGRAM";
        case AST_BLOCK:                 return "BLOCK";
        case AST_VARIABLE_DECLARATION:  return "VARIABLE_DECLARATION";
        case AST_ASSIGNMENT:            return "ASSIGNMENT";
        case AST_PRINT:                 return "PRINT";
        case AST_IF:                    return "IF";
        case AST_WHILE:                 return "WHILE";
        case AST_BINARY_EXPRESSION:     return "BINARY_EXPRESSION";
        case AST_UNARY_EXPRESSION:      return "UNARY_EXPRESSION";
        case AST_IDENTIFIER:            return "IDENTIFIER";
        case AST_LITERAL:               return "LITERAL";
        case AST_EMPTY:                 return "EMPTY";
        default:                        return "UNKNOWN";
    }
}

void printAST(const ASTNode *node, int level)
{
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    printf("%s", astNodeTypeName(node->type));

    if (node->value[0] != '\0') {
        printf(" (%s)", node->value);
    }

    if (node->dataType[0] != '\0') {
        printf(" : %s", node->dataType);
    }

    printf("\n");

    for (int i = 0; i < node->childCount; i++) {
        printAST(node->children[i], level + 1);
    }
}

void freeAST(ASTNode *node)
{
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < node->childCount; i++) {
        freeAST(node->children[i]);
    }

    free(node);
}
