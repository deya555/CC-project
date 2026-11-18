
#include <stdio.h>
#include <string.h>

#include "semantic.h"

static void analyze_node(ASTNode *node, SymbolTable *table)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
        case AST_DECLARATION:
        {
            ASTNode *type_node = node->child;
            ASTNode *variables = type_node ? type_node->next : NULL;

            while (variables != NULL)
            {
                if (variables->type == AST_VARIABLE)
                {
                    ASTNode *initializer = variables->child;

                    int initialized = (initializer != NULL);

                    if (!symbol_table_add(
                            table,
                            variables->value,
                            type_node->value,
                            initialized))
                    {
                        printf("Semantic analysis: declaration error.\n");
                    }
                }

                variables = variables->next;
            }
            break;
        }

        case AST_BLOCK:
        {
            symbol_table_enter_scope(table);

            ASTNode *child = node->child;

            while (child != NULL)
            {
                analyze_node(child, table);
                child = child->next;
            }

            symbol_table_exit_scope(table);
            return;
        }

        case AST_IDENTIFIER:
        {
            if (symbol_table_lookup(table, node->value) == NULL)
            {
                printf("Semantic Error: Undeclared variable '%s'.\n",
                       node->value);
            }
            break;
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
