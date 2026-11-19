
#ifndef AST_H
#define AST_H

#include <stdio.h>

#define AST_MAX_CHILDREN 20
#define AST_MAX_VALUE 256
#define AST_MAX_TYPE 50

typedef enum {
    AST_PROGRAM,
    AST_BLOCK,

    AST_VARIABLE_DECLARATION,
    AST_ASSIGNMENT,
    AST_PRINT,

    AST_IF,
    AST_WHILE,

    AST_BINARY_EXPRESSION,
    AST_UNARY_EXPRESSION,

    AST_IDENTIFIER,
    AST_LITERAL,

    AST_EMPTY
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;

    char value[AST_MAX_VALUE];
    char dataType[AST_MAX_TYPE];

    struct ASTNode *children[AST_MAX_CHILDREN];
    int childCount;
} ASTNode;

/* Create an AST node */
ASTNode *createASTNode(ASTNodeType type,
                       const char *value,
                       const char *dataType);

/* Add a child node */
int addASTChild(ASTNode *parent, ASTNode *child);

/* Print the AST */
void printAST(const ASTNode *node, int level);

/* Get node type name */
const char *astNodeTypeName(ASTNodeType type);

/* Free AST memory */
void freeAST(ASTNode *node);

#endif
