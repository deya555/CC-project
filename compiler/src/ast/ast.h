
#ifndef AST_H
#define AST_H

typedef struct ASTNode {
    char *kind;
    char *value;

    int line;
    int scope_id;

    struct ASTNode *first_child;
    struct ASTNode *next_sibling;
} ASTNode;

char *ast_copy(const char *text);

ASTNode *ast_create(const char *kind, const char *value);

void ast_set_metadata(ASTNode *node, int line, int scope_id);

void ast_add_child(ASTNode *parent, ASTNode *child);

void ast_print(ASTNode *root, int depth);

void ast_free(ASTNode *root);

#endif
