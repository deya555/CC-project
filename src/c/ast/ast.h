#ifndef C_AST_H
#define C_AST_H

typedef enum {
    AST_PROGRAM,
    AST_DECLARATION,
    AST_TYPE,
    AST_VARIABLE,
    AST_ASSIGNMENT,
    AST_IF,
    AST_WHILE,
    AST_BLOCK,
    AST_PRINT,
    AST_RETURN,
    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_IDENTIFIER,
    AST_INTEGER,
    AST_FLOAT,
    AST_STRING,
    AST_CHAR,
    AST_BOOLEAN,
    AST_EXPRESSION_STATEMENT
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char *value;

    struct ASTNode *child;
    struct ASTNode *next;

} ASTNode;

/* Create a new AST node */
ASTNode *ast_create(ASTNodeType type, const char *value);

/* Add a child node */
void ast_add_child(ASTNode *parent, ASTNode *child);

/* Print the AST */
void ast_print(ASTNode *root);

/* Free AST memory */
void ast_free(ASTNode *root);

#endif