

#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_PRINT,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK,
    NODE_BINARY,
    NODE_UNARY,
    NODE_IDENTIFIER,
    NODE_INT,
    NODE_FLOAT,
    NODE_BOOL
} NodeType;

typedef struct ASTNode {
    NodeType type;

    char *name;
    char *declared_type;
    char *operator;

    int int_value;
    float float_value;
    int line;

    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *condition;
    struct ASTNode *body;
    struct ASTNode *else_body;
    struct ASTNode *next;

} ASTNode;

ASTNode *createNode(NodeType type);
ASTNode *createValueNode(NodeType type, int value);
ASTNode *createIdentifierNode(char *name);
ASTNode *createOperatorNode(char *operator,
                            ASTNode *left,
                            ASTNode *right);

void printAST(ASTNode *node);
void freeAST(ASTNode *node);

#endif
