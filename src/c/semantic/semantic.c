#include <stdio.h>
#include <string.h>
#include "semantic.h"

int semantic_error_count = 0;

static void analyze_node(ASTNode *node, SymbolTable *table);

static const char *get_expression_type(ASTNode *node, SymbolTable *table)
{
    if (node == NULL)
        return "unknown";

    switch (node->type)
    {
        case AST_INTEGER:
            return "int";

        case AST_FLOAT:
            return "float";

        case AST_STRING:
            return "string";

        case AST_CHAR:
            return "char";

        case AST_BOOLEAN:
            return "bool";

        case AST_IDENTIFIER:
        {
            Symbol *symbol = symbol_table_lookup(table, node->value);
            return symbol ? symbol->type : "unknown";
        }

        case AST_UNARY_EXPR:
        {
            ASTNode *operand = node->child;

            if (strcmp(node->value, "!") == 0)
                return "bool";

            if (operand == NULL)
                return "unknown";

            return get_expression_type(operand, table);
        }

        case AST_BINARY_EXPR:
        {
            const char *left = get_expression_type(node->child, table);

            ASTNode *right_node =
                node->child ? node->child->next : NULL;

            const char *right =
                get_expression_type(right_node, table);

            if (strcmp(left, "unknown") == 0 ||
                strcmp(right, "unknown") == 0)
                return "unknown";

            if (strcmp(node->value, "==") == 0 ||
                strcmp(node->value, "!=") == 0 ||
                strcmp(node->value, "<") == 0 ||
                strcmp(node->value, ">") == 0 ||
                strcmp(node->value, "<=") == 0 ||
                strcmp(node->value, ">=") == 0 ||
                strcmp(node->value, "&&") == 0 ||
                strcmp(node->value, "||") == 0)
                return "bool";

            if (strcmp(left, "float") == 0 ||
                strcmp(right, "float") == 0)
                return "float";

            return "int";
        }

        default:
            return "unknown";
    }
}

static void analyze_declaration(ASTNode *node, SymbolTable *table)
{
    ASTNode *type_node = node->child;

    if (type_node == NULL || type_node->next == NULL)
        return;

    ASTNode *variable = type_node->next->child;

    while (variable != NULL)
    {
        if (variable->type == AST_VARIABLE)
        {
            ASTNode *initializer = variable->child;
            int initialized = initializer != NULL;

            if (initialized)
                analyze_node(initializer, table);

            if (!symbol_table_add(
                    table,
                    variable->value,
                    type_node->value,
                    initialized))
            {
                /* Redeclaration error is reported by symbol_table_add. */
                semantic_error_count++;
            }
            else if (initialized)
            {
                const char *actual_type =
                    get_expression_type(initializer, table);

                if (strcmp(actual_type, "unknown") != 0 &&
                    strcmp(type_node->value, actual_type) != 0 &&
                    !(strcmp(type_node->value, "float") == 0 &&
                      strcmp(actual_type, "int") == 0))
                {
                    semantic_error_count++;

                    printf(
                        "Type Error: Cannot initialize '%s' of type '%s' with '%s'.\n",
                        variable->value,
                        type_node->value,
                        actual_type
                    );
                }
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
        case AST_BLOCK:
        {
            int create_scope =
                node->type == AST_BLOCK &&
                node->value != NULL &&
                strcmp(node->value, "Block") == 0;

            if (create_scope)
                symbol_table_enter_scope(table);

            ASTNode *child = node->child;

            while (child != NULL)
            {
                analyze_node(child, table);
                child = child->next;
            }

            if (create_scope)
                symbol_table_exit_scope(table);

            return;
        }

        case AST_DECLARATION:
            analyze_declaration(node, table);
            return;

        case AST_ASSIGNMENT:
        {
            ASTNode *left = node->child;
            ASTNode *right = left ? left->next : NULL;

            if (left == NULL || right == NULL)
                return;

            Symbol *symbol =
                symbol_table_lookup(table, left->value);

            if (symbol == NULL)
            {
                /* Count undeclared assignment targets as semantic errors. */
                semantic_error_count++;

                printf(
                    "Semantic Error: Undeclared variable '%s'.\n",
                    left->value
                );

                analyze_node(right, table);
                return;
            }

            analyze_node(right, table);

            const char *right_type =
                get_expression_type(right, table);

            if (strcmp(right_type, "unknown") != 0 &&
                strcmp(symbol->type, right_type) != 0 &&
                !(strcmp(symbol->type, "float") == 0 &&
                  strcmp(right_type, "int") == 0))
            {
                semantic_error_count++;

                printf(
                    "Type Error: Cannot assign '%s' to variable '%s' of type '%s'.\n",
                    right_type,
                    left->value,
                    symbol->type
                );

                return;
            }

            symbol->is_initialized = 1;
            return;
        }

        case AST_IDENTIFIER:
        {
            Symbol *symbol =
                symbol_table_lookup(table, node->value);

            if (symbol == NULL)
            {
                semantic_error_count++;

                printf(
                    "Semantic Error: Undeclared variable '%s'.\n",
                    node->value
                );
            }
            else if (!symbol->is_initialized)
            {
                semantic_error_count++;

                printf(
                    "Semantic Error: Variable '%s' used before initialization.\n",
                    node->value
                );
            }

            return;
        }

        case AST_BOOLEAN:
        case AST_INTEGER:
        case AST_FLOAT:
        case AST_STRING:
        case AST_CHAR:
            return;

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

    semantic_error_count = 0;

    symbol_table_init(table);
    analyze_node(root, table);
}