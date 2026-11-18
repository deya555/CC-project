
#include <stdio.h>
#include <string.h>
#include "semantic.h"

static void analyze_node(ASTNode *node, SymbolTable *table);

static void analyze_declaration(ASTNode *node, SymbolTable *table)
{
    ASTNode *type_node = node->child;

    if (type_node == NULL || type_node->next == NULL)
        return;

    ASTNode *variables_block = type_node->next;
    ASTNode *variable = variables_block->child;

    while (variable != NULL)
    {
        if (variable->type == AST_VARIABLE)
        {
            int initialized = (variable->child != NULL);

            if (!symbol_table_add(
                    table,
                    variable->value,
                    type_node->value,
                    initialized))
            {
                printf("Semantic Error: Invalid declaration of '%s'.\n",
                       variable->value);
            }

            if (variable->child != NULL)
            {
                analyze_node(variable->child, table);
            }
        }

        variable = variable->next;
    }
}

static void analyze_node(ASTNode *node, SymbolTable *table)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
        case AST_PROGRAM:
        {
            ASTNode *child = node->child;

            while (child != NULL)
            {
                analyze_node(child, table);
                child = child->next;
            }

            return;
        }

        case AST_BLOCK:
        {
            // The top-level statement block uses global scope.
            // Nested blocks create their own scopes.
            if (node->value != NULL &&
                strcmp(node->value, "Block") == 0)
            {
                symbol_table_enter_scope(table);
            }

            ASTNode *child = node->child;

            while (child != NULL)
            {
                analyze_node(child, table);
                child = child->next;
            }

            if (node->value != NULL &&
                strcmp(node->value, "Block") == 0)
            {
                symbol_table_exit_scope(table);
            }

            return;
        }

        case AST_DECLARATION:
        {
            analyze_declaration(node, table);
            return;
        }

        case AST_IDENTIFIER:
        {
            if (symbol_table_lookup(table, node->value) == NULL)
            {
                printf("Semantic Error: Undeclared variable '%s'.\n",
                       node->value);
            }

            return;
        }

        default:
            break;
    }

    ASTNode *child = node->child;

    while (child != NULL)
    {
        analyze_node(child, table);
        child = child->next;
    }
}

void semantic_analyze(ASTNode *root, SymbolTable *table)
{
    if (root == NULL || table == NULL)
        return;

    symbol_table_init(table);
    analyze_node(root, table);
}
