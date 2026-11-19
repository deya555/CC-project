
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "semantic.h"

/* ---------- Symbol Table ---------- */
static CPPDataType analyze_expression(
    CPPSemanticAnalyzer *analyzer,
    CPPASTNode *node
);
static int find_symbol(
    CPPSemanticAnalyzer *analyzer,
    const char *name
) {
    for (int i = analyzer->symbol_count - 1; i >= 0; i--) {
        if (strcmp(analyzer->symbols[i].name, name) == 0)
            return i;
    }

    return -1;
}

static void add_error(
    CPPSemanticAnalyzer *analyzer,
    const char *message
) {
    if (analyzer->error_count >= CPP_MAX_ERRORS)
        return;

    snprintf(
        analyzer->errors[analyzer->error_count],
        sizeof(analyzer->errors[0]),
        "%s",
        message
    );

    analyzer->error_count++;
    analyzer->had_error = 1;
}

/* ---------- Data Types ---------- */

const char *cpp_data_type_name(CPPDataType type) {
    switch (type) {
        case CPP_TYPE_INT: return "int";
        case CPP_TYPE_FLOAT: return "float";
        case CPP_TYPE_DOUBLE: return "double";
        case CPP_TYPE_CHAR: return "char";
        case CPP_TYPE_BOOL: return "bool";
        case CPP_TYPE_VOID: return "void";
        case CPP_TYPE_STRING: return "string";
        default: return "unknown";
    }
}

static CPPDataType get_type_from_name(const char *name) {
    if (strcmp(name, "int") == 0)
        return CPP_TYPE_INT;

    if (strcmp(name, "float") == 0)
        return CPP_TYPE_FLOAT;

    if (strcmp(name, "double") == 0)
        return CPP_TYPE_DOUBLE;

    if (strcmp(name, "char") == 0)
        return CPP_TYPE_CHAR;

    if (strcmp(name, "bool") == 0)
        return CPP_TYPE_BOOL;

    if (strcmp(name, "void") == 0)
        return CPP_TYPE_VOID;

    if (strcmp(name, "string") == 0)
        return CPP_TYPE_STRING;

    return CPP_TYPE_UNKNOWN;
}

static int is_numeric(CPPDataType type) {
    return type == CPP_TYPE_INT ||
           type == CPP_TYPE_FLOAT ||
           type == CPP_TYPE_DOUBLE ||
           type == CPP_TYPE_CHAR;
}

static int is_compatible(CPPDataType target, CPPDataType source) {
    if (target == source)
        return 1;

    if (is_numeric(target) && is_numeric(source))
        return 1;

    return 0;
}

/* ---------- Symbol Declaration ---------- */

static void declare_variable(
    CPPSemanticAnalyzer *analyzer,
    CPPASTNode *node
) {
    if (!node)
        return;

    CPPDataType type = get_type_from_name(node->value);

    CPPASTNode *identifier = node->left;

    if (!identifier)
        return;

    if (type == CPP_TYPE_UNKNOWN) {
        char message[256];

        snprintf(
            message,
            sizeof(message),
            "Unknown data type '%s'.",
            node->value
        );

        add_error(analyzer, message);
        return;
    }

    if (type == CPP_TYPE_VOID) {
        add_error(analyzer, "Variables cannot have void type.");
        return;
    }

    if (find_symbol(analyzer, identifier->value) >= 0) {
        char message[256];

        snprintf(
            message,
            sizeof(message),
            "Variable '%s' is already declared.",
            identifier->value
        );

        add_error(analyzer, message);
        return;
    }

    if (analyzer->symbol_count >= CPP_MAX_SYMBOLS) {
        add_error(analyzer, "Symbol table is full.");
        return;
    }

    CPPSymbol *symbol =
        &analyzer->symbols[analyzer->symbol_count++];

    snprintf(
        symbol->name,
        sizeof(symbol->name),
        "%s",
        identifier->value
    );

    symbol->type = type;
    symbol->initialized = 0;

    /* Check initializer, if present */
    if (node->right) {
        CPPDataType value_type = analyze_expression(
            analyzer,
            node->right
        );

        if (!is_compatible(type, value_type)) {
            char message[256];

            snprintf(
                message,
                sizeof(message),
                "Cannot initialize '%s' of type %s with type %s.",
                identifier->value,
                cpp_data_type_name(type),
                cpp_data_type_name(value_type)
            );

            add_error(analyzer, message);
        } else {
            symbol->initialized = 1;
        }
    }
}

/* ---------- Expression Analysis ---------- */

static CPPDataType analyze_expression(
    CPPSemanticAnalyzer *analyzer,
    CPPASTNode *node
) {
    if (!node)
        return CPP_TYPE_UNKNOWN;

    switch (node->type) {

        case AST_LITERAL: {
            if (node->value[0] == '"')
                return CPP_TYPE_STRING;

            if (node->value[0] == '\'')
                return CPP_TYPE_CHAR;

            if (strcmp(node->value, "true") == 0 ||
                strcmp(node->value, "false") == 0)
                return CPP_TYPE_BOOL;

            if (strchr(node->value, '.') != NULL)
                return CPP_TYPE_DOUBLE;

            return CPP_TYPE_INT;
        }

        case AST_IDENTIFIER: {
            int index = find_symbol(analyzer, node->value);

            if (index < 0) {
                char message[256];

                snprintf(
                    message,
                    sizeof(message),
                    "Undeclared variable '%s'.",
                    node->value
                );

                add_error(analyzer, message);
                return CPP_TYPE_UNKNOWN;
            }

            return analyzer->symbols[index].type;
        }

        case AST_UNARY_EXPRESSION: {
            CPPDataType operand = analyze_expression(
                analyzer,
                node->left
            );

            if (strcmp(node->value, "!") == 0) {
                if (operand != CPP_TYPE_BOOL &&
                    operand != CPP_TYPE_UNKNOWN) {
                    add_error(
                        analyzer,
                        "Logical NOT operator requires a boolean operand."
                    );
                }

                return CPP_TYPE_BOOL;
            }

            if (!is_numeric(operand) &&
                operand != CPP_TYPE_UNKNOWN) {
                add_error(
                    analyzer,
                    "Unary arithmetic operator requires a numeric operand."
                );

                return CPP_TYPE_UNKNOWN;
            }

            return operand;
        }

        case AST_BINARY_EXPRESSION: {
            CPPDataType left = analyze_expression(
                analyzer,
                node->left
            );

            CPPDataType right = analyze_expression(
                analyzer,
                node->right
            );

            if (left == CPP_TYPE_UNKNOWN ||
                right == CPP_TYPE_UNKNOWN)
                return CPP_TYPE_UNKNOWN;

            switch (node->operator_type) {

                case CPP_TOKEN_PLUS:
                case CPP_TOKEN_MINUS:
                case CPP_TOKEN_MULTIPLY:
                case CPP_TOKEN_DIVIDE:
                case CPP_TOKEN_MODULO:

                    if (!is_numeric(left) || !is_numeric(right)) {
                        add_error(
                            analyzer,
                            "Arithmetic operators require numeric operands."
                        );

                        return CPP_TYPE_UNKNOWN;
                    }

                    if (left == CPP_TYPE_DOUBLE ||
                        right == CPP_TYPE_DOUBLE)
                        return CPP_TYPE_DOUBLE;

                    if (left == CPP_TYPE_FLOAT ||
                        right == CPP_TYPE_FLOAT)
                        return CPP_TYPE_FLOAT;

                    return CPP_TYPE_INT;

                case CPP_TOKEN_GREATER:
                case CPP_TOKEN_GREATER_EQUAL:
                case CPP_TOKEN_LESS:
                case CPP_TOKEN_LESS_EQUAL:
                case CPP_TOKEN_EQUAL:
                case CPP_TOKEN_NOT_EQUAL:

                    if (!is_compatible(left, right)) {
                        add_error(
                            analyzer,
                            "Comparison operands have incompatible types."
                        );
                    }

                    return CPP_TYPE_BOOL;

                case CPP_TOKEN_LOGICAL_AND:
                case CPP_TOKEN_LOGICAL_OR:

                    if ((left != CPP_TYPE_BOOL &&
                         !is_numeric(left)) ||
                        (right != CPP_TYPE_BOOL &&
                         !is_numeric(right))) {
                        add_error(
                            analyzer,
                            "Logical operators require boolean or numeric operands."
                        );
                    }

                    return CPP_TYPE_BOOL;

                default:
                    return CPP_TYPE_UNKNOWN;
            }
        }

        case AST_ASSIGNMENT: {
            CPPDataType right = analyze_expression(
                analyzer,
                node->right
            );

            if (!node->left ||
                node->left->type != AST_IDENTIFIER)
                return CPP_TYPE_UNKNOWN;

            int index = find_symbol(
                analyzer,
                node->left->value
            );

            if (index < 0) {
                char message[256];

                snprintf(
                    message,
                    sizeof(message),
                    "Undeclared variable '%s'.",
                    node->left->value
                );

                add_error(analyzer, message);
                return CPP_TYPE_UNKNOWN;
            }

            CPPDataType target = analyzer->symbols[index].type;

            if (!is_compatible(target, right)) {
                char message[256];

                snprintf(
                    message,
                    sizeof(message),
                    "Cannot assign %s to variable '%s' of type %s.",
                    cpp_data_type_name(right),
                    node->left->value,
                    cpp_data_type_name(target)
                );

                add_error(analyzer, message);
            }

            analyzer->symbols[index].initialized = 1;

            return target;
        }

        default:
            return CPP_TYPE_UNKNOWN;
    }
}

/* ---------- Statement Analysis ---------- */

static void analyze_statement(
    CPPSemanticAnalyzer *analyzer,
    CPPASTNode *node
) {
    if (!node)
        return;

    switch (node->type) {

        case AST_PROGRAM:
        case AST_BLOCK:
            for (CPPASTNode *child = node->left;
                 child;
                 child = child->next) {
                analyze_statement(analyzer, child);
            }
            break;

        case AST_VARIABLE_DECLARATION:
            declare_variable(analyzer, node);
            break;

        case AST_ASSIGNMENT:
        case AST_BINARY_EXPRESSION:
        case AST_UNARY_EXPRESSION:
        case AST_LITERAL:
        case AST_IDENTIFIER:
            analyze_expression(analyzer, node);
            break;

        case AST_EXPRESSION_STATEMENT:
            analyze_expression(analyzer, node->left);
            break;

        case AST_IF: {
            CPPDataType condition = analyze_expression(
                analyzer,
                node->left
            );

            if (condition != CPP_TYPE_BOOL &&
                condition != CPP_TYPE_INT &&
                condition != CPP_TYPE_UNKNOWN) {
                add_error(
                    analyzer,
                    "If condition must be boolean or numeric."
                );
            }

            analyze_statement(analyzer, node->right);

            if (node->third)
                analyze_statement(analyzer, node->third);

            break;
        }

        case AST_WHILE: {
            CPPDataType condition = analyze_expression(
                analyzer,
                node->left
            );

            if (condition != CPP_TYPE_BOOL &&
                condition != CPP_TYPE_INT &&
                condition != CPP_TYPE_UNKNOWN) {
                add_error(
                    analyzer,
                    "While condition must be boolean or numeric."
                );
            }

            analyze_statement(analyzer, node->right);
            break;
        }

        case AST_FOR:
            analyze_statement(analyzer, node->left);
            analyze_statement(analyzer, node->right);

            if (node->third)
                analyze_statement(analyzer, node->third);

            break;

        case AST_RETURN:
            if (node->left)
                analyze_expression(analyzer, node->left);
            break;

        case AST_FUNCTION_CALL:
            add_error(
                analyzer,
                "Function call checking is not implemented yet."
            );
            break;

        default:
            break;
    }
}

/* ---------- Public API ---------- */

void cpp_semantic_init(CPPSemanticAnalyzer *analyzer) {
    if (!analyzer)
        return;

    memset(analyzer, 0, sizeof(*analyzer));
}

int cpp_semantic_analyze(
    CPPSemanticAnalyzer *analyzer,
    CPPASTNode *root
) {
    if (!analyzer || !root)
        return 0;

    analyze_statement(analyzer, root);

    return analyzer->had_error ? 0 : 1;
}

void cpp_semantic_print_errors(CPPSemanticAnalyzer *analyzer) {
    if (!analyzer)
        return;

    if (analyzer->error_count == 0) {
        printf("Semantic Analysis Successful!\n");
        return;
    }

    printf("\nSemantic Errors:\n");

    for (int i = 0; i < analyzer->error_count; i++) {
        printf("%d. %s\n", i + 1, analyzer->errors[i]);
    }
}
