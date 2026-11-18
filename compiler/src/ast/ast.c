
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

/* Make a copy of a string */
char *ast_copy(const char *text)
{
    char *copy;

    if (text == NULL) {
        return NULL;
    }

    copy = (char *)malloc(strlen(text) + 1);

    if (copy == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(copy, text);

    return copy;
}

/* Create a new AST node */
ASTNode *ast_create(const char *kind, const char *value)
{
    ASTNode *node;

    node = (ASTNode *)malloc(sizeof(ASTNode));

    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    node->kind = ast_copy(kind);
    node->value = ast_copy(value);

    /* Default metadata */
    node->line = 0;
    node->scope_id = -1;

    node->first_child = NULL;
    node->next_sibling = NULL;

    return node;
}

/* Set source line and scope information */
void ast_set_metadata(ASTNode *node, int line, int scope_id)
{
    if (node == NULL) {
        return;
    }

    node->line = line;
    node->scope_id = scope_id;
}

/* Add a child to a parent node */
void ast_add_child(ASTNode *parent, ASTNode *child)
{
    ASTNode *current;

    if (parent == NULL || child == NULL) {
        return;
    }

    if (parent->first_child == NULL) {
        parent->first_child = child;
        return;
    }

    current = parent->first_child;

    while (current->next_sibling != NULL) {
        current = current->next_sibling;
    }

    current->next_sibling = child;
}

/* Display the AST */
static void print_nodes(ASTNode *root, int depth)
{
    ASTNode *current;
    int i;

    current = root;

    while (current != NULL) {

        for (i = 0; i < depth; i++) {
            printf("    ");
        }

        printf("%s", current->kind);

        if (current->value != NULL) {
            printf(" (%s)", current->value);
        }

        printf("\n");

        print_nodes(current->first_child, depth + 1);

        current = current->next_sibling;
    }
}

void ast_print(ASTNode *root, int depth)
{
    print_nodes(root, depth);
}

/* Free AST memory */
void ast_free(ASTNode *root)
{
    ASTNode *current;
    ASTNode *next;

    current = root;

    while (current != NULL) {

        next = current->next_sibling;

        ast_free(current->first_child);

        free(current->kind);
        free(current->value);
        free(current);

        current = next;
    }
}
