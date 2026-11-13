
#ifndef AST_H
#define AST_H

typedef struct ASTNode {
    char *kind;
    char *value;

    struct ASTNode *first_child;
    struct ASTNode *next_sibling;
} ASTNode;

char *ast_copy(const char *text);

ASTNode *ast_create(const char *kind, const char *value);

void ast_add_child(ASTNode *parent, ASTNode *child);

void ast_print(ASTNode *root, int depth);

void ast_free(ASTNode *root);

#endif
