
#include <stdio.h>
#include <string.h>

#include "semantic.h"

/* Count semantic errors. */
static int semantic_error_count = 0;

/* Display a semantic error with its source line. */
static void report_error(ASTNode *node, const char *message)
{
    int line = node != NULL ? node->line : 0;

    fprintf(
        stderr,
        "SEMANTIC ERROR at line %d: %s\n",
        line,
        message
    );

    semantic_error_count++;
}

/* Check whether a type is numeric. */
static int is_numeric(const char *type)
{
    return type != NULL &&
        (
            strcmp(type, "int") == 0 ||
            strcmp(type, "float") == 0
        );
}

/* Check assignment compatibility. */
static int is_compatible(
    const char *expected,
    const char *actual
)
{
    if (expected == NULL || actual == NULL) {
        return 0;
    }

    if (strcmp(expected, actual) == 0) {
        return 1;
    }

    /* An int value can be assigned to a float variable. */
    if (
        strcmp(expected, "float") == 0 &&
        strcmp(actual, "int") == 0
    ) {
        return 1;
    }

    return 0;
}

/* Find a variable using the AST node's scope ID. */
static Symbol *find_variable(
    SymbolTable *table,
    ASTNode *node
)
{
    if (node == NULL || node->value == NULL) {
        return NULL;
    }

    return lookup_symbol_in_scope(
        table,
        node->value,
        node->scope_id
    );
}

/* Analyze an expression and return its resulting type. */
static const char *analyze_expression(
    ASTNode *node,
    SymbolTable *table
)
{
    ASTNode *left;
    ASTNode *right;
    const char *left_type;
    const char *right_type;
    Symbol *symbol;

    if (node == NULL) {
        return "error";
    }

    left = node->first_child;
    right = left != NULL ? left->next_sibling : NULL;

    /* Integer literal. */
    if (strcmp(node->kind, "IntegerLiteral") == 0) {
        return "int";
    }

    /* Floating-point literal. */
    if (strcmp(node->kind, "FloatLiteral") == 0) {
        return "float";
    }

    /* Boolean literal. */
    if (strcmp(node->kind, "BooleanLiteral") == 0) {
        return "bool";
    }

    /* Identifier. */
    if (strcmp(node->kind, "Identifier") == 0) {

        symbol = find_variable(table, node);

        if (symbol == NULL) {
            char message[256];

            snprintf(
                message,
                sizeof(message),
                "Variable '%s' is not declared or is not visible in this scope.",
                node->value != NULL ? node->value : ""
            );

            report_error(node, message);
            return "error";
        }

        return symbol->type;
    }

    /* Unary operation. */
    if (strcmp(node->kind, "UnaryOperation") == 0) {

        left_type = analyze_expression(left, table);

        if (strcmp(left_type, "error") == 0) {
            return "error";
        }

        if (strcmp(node->value, "!") == 0) {

            if (strcmp(left_type, "bool") != 0) {
                report_error(
                    node,
                    "Logical NOT requires a boolean expression."
                );

                return "error";
            }

            return "bool";
        }

        if (strcmp(node->value, "-") == 0) {

            if (!is_numeric(left_type)) {
                report_error(
                    node,
                    "Unary minus requires a numeric expression."
                );

                return "error";
            }

            return left_type;
        }

        return "error";
    }

    /* Arithmetic operations. */
    if (strcmp(node->kind, "BinaryOperation") == 0) {

        left_type = analyze_expression(left, table);
        right_type = analyze_expression(right, table);

        if (
            strcmp(left_type, "error") == 0 ||
            strcmp(right_type, "error") == 0
        ) {
            return "error";
        }

        if (
            !is_numeric(left_type) ||
            !is_numeric(right_type)
        ) {
            report_error(
                node,
                "Arithmetic operations require numeric operands."
            );

            return "error";
        }

        if (
            strcmp(node->value, "%") == 0 &&
            (
                strcmp(left_type, "int") != 0 ||
                strcmp(right_type, "int") != 0
            )
        ) {
            report_error(
                node,
                "Modulo operation requires integer operands."
            );

            return "error";
        }

        if (
            strcmp(left_type, "float") == 0 ||
            strcmp(right_type, "float") == 0
        ) {
            return "float";
        }

        return "int";
    }

    /* Relational operations. */
    if (strcmp(node->kind, "RelationalOperation") == 0) {

        left_type = analyze_expression(left, table);
        right_type = analyze_expression(right, table);

        if (
            strcmp(left_type, "error") == 0 ||
            strcmp(right_type, "error") == 0
        ) {
            return "error";
        }

        if (
            strcmp(node->value, "==") == 0 ||
            strcmp(node->value, "!=") == 0
        ) {
            if (
                !is_compatible(left_type, right_type) ||
                !is_compatible(right_type, left_type)
            ) {
                report_error(
                    node,
                    "Equality operands have incompatible types."
                );

                return "error";
            }

            return "bool";
        }

        if (
            !is_numeric(left_type) ||
            !is_numeric(right_type)
        ) {
            report_error(
                node,
                "Relational comparisons require numeric operands."
            );

            return "error";
        }

        return "bool";
    }

    /* Logical operations. */
    if (strcmp(node->kind, "LogicalOperation") == 0) {

        left_type = analyze_expression(left, table);
        right_type = analyze_expression(right, table);

        if (
            strcmp(left_type, "error") == 0 ||
            strcmp(right_type, "error") == 0
        ) {
            return "error";
        }

        if (
            strcmp(left_type, "bool") != 0 ||
            strcmp(right_type, "bool") != 0
        ) {
            report_error(
                node,
                "Logical operations require boolean operands."
            );

            return "error";
        }

        return "bool";
    }

    return "error";
}

/* Analyze statements and statement lists. */
static void analyze_statement(
    ASTNode *node,
    SymbolTable *table
)
{
    ASTNode *child;
    ASTNode *target;
    ASTNode *expression;
    const char *expression_type;
    Symbol *symbol;

    if (node == NULL) {
        return;
    }

    /* Analyze every statement in a list. */
    if (strcmp(node->kind, "Program") == 0 || strcmp(node->kind, "StatementList") == 0) {

        child = node->first_child;

        while (child != NULL) {
            analyze_statement(child, table);
            child = child->next_sibling;
        }

        return;
    }

    /* Variable declaration. */
    if (strcmp(node->kind, "VariableDeclaration") == 0) {

        target = node->first_child;
        expression = target != NULL
            ? target->next_sibling
            : NULL;

        if (expression != NULL) {

            expression_type =
                analyze_expression(expression, table);

            if (
                strcmp(expression_type, "error") != 0 &&
                !is_compatible(node->value, expression_type)
            ) {
                report_error(
                    node,
                    "Variable initializer type does not match the declared type."
                );
            }
        }

        return;
    }

    /* Assignment. */
    if (strcmp(node->kind, "Assignment") == 0) {

        target = node->first_child;
        expression = target != NULL
            ? target->next_sibling
            : NULL;

        symbol = find_variable(table, target);

        if (symbol == NULL) {

            char message[256];

            snprintf(
                message,
                sizeof(message),
                "Variable '%s' is not declared or is not visible in this scope.",
                target != NULL && target->value != NULL
                    ? target->value
                    : ""
            );

            report_error(target, message);

            analyze_expression(expression, table);
            return;
        }

        expression_type =
            analyze_expression(expression, table);

        if (
            strcmp(expression_type, "error") != 0 &&
            !is_compatible(symbol->type, expression_type)
        ) {
            report_error(
                node,
                "Assignment type mismatch."
            );
        }

        return;
    }

    /* Print statement. */
    if (strcmp(node->kind, "PrintStatement") == 0) {
        analyze_expression(node->first_child, table);
        return;
    }

    /* If statement. */
    if (
        strcmp(node->kind, "IfStatement") == 0 ||
        strcmp(node->kind, "IfElseStatement") == 0
    ) {
        child = node->first_child;

        expression_type =
            analyze_expression(child, table);

        if (
            strcmp(expression_type, "error") != 0 &&
            strcmp(expression_type, "bool") != 0
        ) {
            report_error(
                node,
                "If condition must have boolean type."
            );
        }

        if (child != NULL) {
            child = child->next_sibling;
        }

        if (child != NULL) {
            analyze_statement(child, table);
            child = child->next_sibling;
        }

        if (
            strcmp(node->kind, "IfElseStatement") == 0 &&
            child != NULL
        ) {
            analyze_statement(child, table);
        }

        return;
    }

    /* While statement. */
    if (strcmp(node->kind, "WhileStatement") == 0) {

        child = node->first_child;

        expression_type =
            analyze_expression(child, table);

        if (
            strcmp(expression_type, "error") != 0 &&
            strcmp(expression_type, "bool") != 0
        ) {
            report_error(
                node,
                "While condition must have boolean type."
            );
        }

        if (child != NULL) {
            analyze_statement(child->next_sibling, table);
        }

        return;
    }

    /* Nested block. */
    if (strcmp(node->kind, "Block") == 0) {
        analyze_statement(node->first_child, table);
        return;
    }

    /* Recoverable syntax-error node. */
    if (strcmp(node->kind, "Error") == 0) {
        return;
    }
}

/* Public semantic analysis function. */
int analyze_semantics(ASTNode *root, SymbolTable *table)
{
    semantic_error_count = 0;

    printf("\n========== SEMANTIC ANALYSIS ==========\n\n");

    if (root == NULL || table == NULL) {
        printf("Semantic analysis could not start.\n");
        return 1;
    }

    analyze_statement(root, table);

    printf("\n=======================================\n");

    if (semantic_error_count == 0) {
        printf("Semantic analysis completed successfully.\n");
        printf("No semantic errors found.\n");
    } else {
        printf("Semantic analysis completed with errors.\n");
        printf("Semantic errors: %d\n", semantic_error_count);
    }

    return semantic_error_count;
}
