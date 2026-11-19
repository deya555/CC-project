
#include "semantic.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int semanticErrors = 0;

/* ================= ERROR REPORTING ================= */

static void reportError(const char *message, const char *name)
{
    semanticErrors++;

    if (name != NULL) {
        fprintf(stderr, "Semantic Error: %s '%s'.\n", message, name);
    } else {
        fprintf(stderr, "Semantic Error: %s\n", message);
    }
}

/* ================= TYPE HELPERS ================= */

static int sameType(const char *a, const char *b)
{
    if (a == NULL || b == NULL)
        return 0;

    return strcmp(a, b) == 0 ||
           (strcmp(a, "String") == 0 && strcmp(b, "string") == 0) ||
           (strcmp(a, "string") == 0 && strcmp(b, "String") == 0);
}

static int isNumeric(const char *type)
{
    return type != NULL &&
           (strcmp(type, "int") == 0 ||
            strcmp(type, "float") == 0 ||
            strcmp(type, "double") == 0);
}

static int canAssign(const char *target, const char *source)
{
    if (target == NULL || source == NULL)
        return 0;

    if (sameType(target, source))
        return 1;

    /* Allow safe numeric widening. */
    if (strcmp(target, "double") == 0 && isNumeric(source))
        return 1;

    if (strcmp(target, "float") == 0 &&
        strcmp(source, "int") == 0)
        return 1;

    return 0;
}

/* ================= EXPRESSION ANALYSIS ================= */

static const char *analyzeExpression(
    const ASTNode *node,
    SymbolTable *table
);

static const char *analyzeLiteral(const char *value)
{
    if (value == NULL)
        return NULL;

    if (strcmp(value, "true") == 0 ||
        strcmp(value, "false") == 0)
        return "boolean";

    if (value[0] == '"')
        return "String";

    if (value[0] == '\'')
        return "char";

    if (strchr(value, '.') != NULL)
        return "double";

    if (isdigit((unsigned char)value[0]))
        return "int";

    return NULL;
}

static const char *analyzeIdentifier(
    const ASTNode *node,
    SymbolTable *table
)
{
    Symbol *symbol = lookupSymbol(table, node->value);

    if (symbol == NULL) {
        reportError("Undeclared variable", node->value);
        return NULL;
    }

    if (!symbol->isInitialized) {
        reportError("Variable used before initialization",
                    node->value);
        return NULL;
    }

    return symbol->dataType;
}

static const char *analyzeUnary(
    const ASTNode *node,
    SymbolTable *table
)
{
    if (node->childCount != 1) {
        reportError("Invalid unary expression", NULL);
        return NULL;
    }

    const char *operand =
        analyzeExpression(node->children[0], table);

    if (operand == NULL)
        return NULL;

    if (strcmp(node->value, "!") == 0) {
        if (!sameType(operand, "boolean")) {
            reportError("Logical NOT requires a boolean operand",
                        NULL);
            return NULL;
        }

        return "boolean";
    }

    if (strcmp(node->value, "-") == 0 ||
        strcmp(node->value, "+") == 0) {

        if (!isNumeric(operand)) {
            reportError("Unary arithmetic requires a numeric operand",
                        NULL);
            return NULL;
        }

        return operand;
    }

    reportError("Unknown unary operator", node->value);
    return NULL;
}

static const char *analyzeBinary(
    const ASTNode *node,
    SymbolTable *table
)
{
    if (node->childCount != 2) {
        reportError("Invalid binary expression", NULL);
        return NULL;
    }

    const char *left =
        analyzeExpression(node->children[0], table);

    const char *right =
        analyzeExpression(node->children[1], table);

    if (left == NULL || right == NULL)
        return NULL;

    const char *op = node->value;

    /* Arithmetic operators */
    if (strcmp(op, "+") == 0 ||
        strcmp(op, "-") == 0 ||
        strcmp(op, "*") == 0 ||
        strcmp(op, "/") == 0 ||
        strcmp(op, "%") == 0) {

        if (!isNumeric(left) || !isNumeric(right)) {
            reportError("Arithmetic operator requires numeric operands",
                        op);
            return NULL;
        }

        if (strcmp(left, "double") == 0 ||
            strcmp(right, "double") == 0)
            return "double";

        if (strcmp(left, "float") == 0 ||
            strcmp(right, "float") == 0)
            return "float";

        return "int";
    }

    /* Relational operators */
    if (strcmp(op, "<") == 0 ||
        strcmp(op, ">") == 0 ||
        strcmp(op, "<=") == 0 ||
        strcmp(op, ">=") == 0) {

        if (!isNumeric(left) || !isNumeric(right)) {
            reportError("Relational operator requires numeric operands",
                        op);
            return NULL;
        }

        return "boolean";
    }

    /* Equality operators */
    if (strcmp(op, "==") == 0 ||
        strcmp(op, "!=") == 0) {

        if (!(sameType(left, right) ||
              (isNumeric(left) && isNumeric(right)))) {
            reportError("Incompatible types in equality expression",
                        op);
            return NULL;
        }

        return "boolean";
    }

    /* Logical operators */
    if (strcmp(op, "&&") == 0 ||
        strcmp(op, "||") == 0) {

        if (!sameType(left, "boolean") ||
            !sameType(right, "boolean")) {
            reportError("Logical operator requires boolean operands",
                        op);
            return NULL;
        }

        return "boolean";
    }

    reportError("Unknown binary operator", op);
    return NULL;
}

static const char *analyzeExpression(
    const ASTNode *node,
    SymbolTable *table
)
{
    if (node == NULL)
        return NULL;

    switch (node->type) {
        case AST_LITERAL:
            return analyzeLiteral(node->value);

        case AST_IDENTIFIER:
            return analyzeIdentifier(node, table);

        case AST_BINARY_EXPRESSION:
            return analyzeBinary(node, table);

        case AST_UNARY_EXPRESSION:
            return analyzeUnary(node, table);

        default:
            reportError("Invalid expression node", NULL);
            return NULL;
    }
}

/* ================= STATEMENT ANALYSIS ================= */

static int analyzeNode(const ASTNode *node, SymbolTable *table);

static int analyzeDeclaration(
    const ASTNode *node,
    SymbolTable *table
)
{
    int initialized = node->childCount > 0;

    if (!insertSymbol(table,
                      node->value,
                      node->dataType,
                      0)) {
        reportError("Duplicate declaration or symbol table full",
                    node->value);
        return 0;
    }

    if (initialized) {
        const char *actualType =
            analyzeExpression(node->children[0], table);

        if (actualType == NULL)
            return 0;

        if (!canAssign(node->dataType, actualType)) {
            reportError("Incompatible initializer type for variable",
                        node->value);
            return 0;
        }

        updateSymbolInitialization(table, node->value);
    }

    return 1;
}

static int analyzeAssignment(
    const ASTNode *node,
    SymbolTable *table
)
{
    Symbol *symbol = lookupSymbol(table, node->value);

    if (symbol == NULL) {
        reportError("Assignment to undeclared variable",
                    node->value);
        return 0;
    }

    if (node->childCount != 1) {
        reportError("Invalid assignment expression", node->value);
        return 0;
    }

    const char *actualType =
        analyzeExpression(node->children[0], table);

    if (actualType == NULL)
        return 0;

    if (!canAssign(symbol->dataType, actualType)) {
        reportError("Incompatible assignment type for variable",
                    node->value);
        return 0;
    }

    updateSymbolInitialization(table, node->value);
    return 1;
}

static int analyzeCondition(
    const ASTNode *node,
    SymbolTable *table
)
{
    const char *type = analyzeExpression(node, table);

    if (type == NULL)
        return 0;

    if (!sameType(type, "boolean")) {
        reportError("Condition must have boolean type", NULL);
        return 0;
    }

    return 1;
}

/* ================= BLOCK ANALYSIS ================= */

static int analyzeBlock(
    const ASTNode *node,
    SymbolTable *table
)
{
    if (!enterScope(table)) {
        reportError("Could not enter block scope", NULL);
        return 0;
    }

    int success = 1;

    for (int i = 0; i < node->childCount; i++) {
        if (!analyzeNode(node->children[i], table))
            success = 0;
    }

    if (!exitScope(table)) {
        reportError("Could not exit block scope", NULL);
        success = 0;
    }

    return success;
}

/* ================= IF ANALYSIS ================= */

static int analyzeIf(
    const ASTNode *node,
    SymbolTable *table
)
{
    if (node->childCount < 2 || node->childCount > 3) {
        reportError("Invalid IF statement", NULL);
        return 0;
    }

    int success = analyzeCondition(node->children[0], table);

    if (!analyzeNode(node->children[1], table))
        success = 0;

    if (node->childCount == 3) {
        if (!analyzeNode(node->children[2], table))
            success = 0;
    }

    return success;
}

/* ================= WHILE ANALYSIS ================= */

static int analyzeWhile(
    const ASTNode *node,
    SymbolTable *table
)
{
    if (node->childCount != 2) {
        reportError("Invalid WHILE statement", NULL);
        return 0;
    }

    int success = analyzeCondition(node->children[0], table);

    if (!analyzeNode(node->children[1], table))
        success = 0;

    return success;
}

/* ================= GENERAL NODE ANALYSIS ================= */

static int analyzeNode(const ASTNode *node, SymbolTable *table)
{
    if (node == NULL)
        return 0;

    switch (node->type) {
        case AST_PROGRAM:
            {
                int success = 1;

                for (int i = 0; i < node->childCount; i++) {
                    if (!analyzeNode(node->children[i], table))
                        success = 0;
                }

                return success;
            }

        case AST_BLOCK:
            return analyzeBlock(node, table);

        case AST_VARIABLE_DECLARATION:
            return analyzeDeclaration(node, table);

        case AST_ASSIGNMENT:
            return analyzeAssignment(node, table);

        case AST_PRINT:
            if (node->childCount != 1) {
                reportError("Invalid print statement", NULL);
                return 0;
            }

            return analyzeExpression(node->children[0], table) != NULL;

        case AST_IF:
            return analyzeIf(node, table);

        case AST_WHILE:
            return analyzeWhile(node, table);

        case AST_EMPTY:
            return 1;

        default:
            reportError("Unexpected AST node in statement", NULL);
            return 0;
    }
}

/* ================= PUBLIC ANALYZER FUNCTION ================= */

int analyzeSemantics(const ASTNode *root, SymbolTable *table)
{
    if (root == NULL || table == NULL) {
        fprintf(stderr, "Semantic Error: Invalid analyzer input.\n");
        return 0;
    }

    semanticErrors = 0;

    printf("\n========== SEMANTIC ANALYSIS ==========\n");

    int success = analyzeNode(root, table);

    if (semanticErrors == 0 && success) {
        printf("Semantic Analysis Successful!\n");
        printf("No semantic errors found.\n");
        return 1;
    }

    printf("Semantic Analysis Failed!\n");
    printf("Total semantic errors: %d\n", semanticErrors);

    return 0;
}
