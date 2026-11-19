
#include "semantic.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* =========================================
   BASIC HELPERS
   ========================================= */

static void copyText(char *destination, const char *source) {
    snprintf(destination, MAX_LEXEME, "%s", source);
}

static int isNumericType(const char *type) {
    return strcmp(type, "int") == 0 ||
           strcmp(type, "float") == 0 ||
           strcmp(type, "double") == 0 ||
           strcmp(type, "long") == 0 ||
           strcmp(type, "short") == 0 ||
           strcmp(type, "byte") == 0 ||
           strcmp(type, "char") == 0;
}

static int isIntegerType(const char *type) {
    return strcmp(type, "int") == 0 ||
           strcmp(type, "long") == 0 ||
           strcmp(type, "short") == 0 ||
           strcmp(type, "byte") == 0;
}

static int isBooleanType(const char *type) {
    return strcmp(type, "boolean") == 0;
}

/* =========================================
   SCOPE MANAGEMENT
   ========================================= */

static int enterScope(SemanticAnalyzer *analyzer) {
    if (analyzer->scopeCount >= MAX_SCOPES) {
        printf("Semantic Error: Maximum scope limit reached.\n");
        analyzer->errorCount++;
        return analyzer->currentScope;
    }

    int newId = analyzer->scopeCount;

    analyzer->scopes[newId].id = newId;
    analyzer->scopes[newId].parentId = analyzer->currentScope;

    analyzer->scopeCount++;
    analyzer->currentScope = newId;

    return newId;
}

static void exitScope(SemanticAnalyzer *analyzer) {
    if (analyzer->currentScope >= 0) {
        analyzer->currentScope =
            analyzer->scopes[analyzer->currentScope].parentId;
    }
}

/* =========================================
   SYMBOL TABLE
   ========================================= */

static int findSymbolInScope(
    SemanticAnalyzer *analyzer,
    const char *name,
    int scopeId
) {
    for (int i = analyzer->count - 1; i >= 0; i--) {
        if (analyzer->symbols[i].scopeId == scopeId &&
            strcmp(analyzer->symbols[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

/* Search current scope, then its parent scopes */

static int findSymbol(
    SemanticAnalyzer *analyzer,
    const char *name
) {
    int scopeId = analyzer->currentScope;

    while (scopeId >= 0) {
        int index = findSymbolInScope(
            analyzer,
            name,
            scopeId
        );

        if (index != -1) {
            return index;
        }

        scopeId = analyzer->scopes[scopeId].parentId;
    }

    return -1;
}

static int addSymbol(
    SemanticAnalyzer *analyzer,
    const char *name,
    const char *type,
    int initialized
) {
    if (findSymbolInScope(
            analyzer,
            name,
            analyzer->currentScope
        ) != -1) {

        printf(
            "Semantic Error: Variable '%s' already declared in this scope.\n",
            name
        );

        analyzer->errorCount++;
        return -1;
    }

    if (analyzer->count >= MAX_SYMBOLS) {
        printf("Semantic Error: Symbol table is full.\n");
        analyzer->errorCount++;
        return -1;
    }

    Symbol *symbol = &analyzer->symbols[analyzer->count];

    copyText(symbol->name, name);
    copyText(symbol->type, type);

    symbol->initialized = initialized;
    symbol->line = 0;
    symbol->scopeId = analyzer->currentScope;

    analyzer->count++;

    return analyzer->count - 1;
}

/* =========================================
   TYPE COMPATIBILITY
   ========================================= */

static int canAssign(
    const char *target,
    const char *source
) {
    if (strcmp(target, source) == 0) {
        return 1;
    }

    /* Java-style numeric widening */

    if (strcmp(target, "double") == 0 &&
        (strcmp(source, "float") == 0 ||
         strcmp(source, "long") == 0 ||
         strcmp(source, "int") == 0 ||
         strcmp(source, "short") == 0 ||
         strcmp(source, "byte") == 0 ||
         strcmp(source, "char") == 0)) {
        return 1;
    }

    if (strcmp(target, "float") == 0 &&
        (strcmp(source, "long") == 0 ||
         strcmp(source, "int") == 0 ||
         strcmp(source, "short") == 0 ||
         strcmp(source, "byte") == 0 ||
         strcmp(source, "char") == 0)) {
        return 1;
    }

    if (strcmp(target, "long") == 0 &&
        (strcmp(source, "int") == 0 ||
         strcmp(source, "short") == 0 ||
         strcmp(source, "byte") == 0 ||
         strcmp(source, "char") == 0)) {
        return 1;
    }

    if (strcmp(target, "int") == 0 &&
        (strcmp(source, "short") == 0 ||
         strcmp(source, "byte") == 0 ||
         strcmp(source, "char") == 0)) {
        return 1;
    }

    return 0;
}

static void promoteNumericType(
    const char *left,
    const char *right,
    char *result
) {
    if (strcmp(left, "double") == 0 ||
        strcmp(right, "double") == 0) {
        copyText(result, "double");
    }
    else if (strcmp(left, "float") == 0 ||
             strcmp(right, "float") == 0) {
        copyText(result, "float");
    }
    else if (strcmp(left, "long") == 0 ||
             strcmp(right, "long") == 0) {
        copyText(result, "long");
    }
    else {
        copyText(result, "int");
    }
}

/* =========================================
   LITERAL TYPE DETECTION
   ========================================= */


static void getLiteralType(
    const char *value,
    char *result
) {
    /* Boolean literals */
    if (strcmp(value, "true") == 0 ||
        strcmp(value, "false") == 0) {
        copyText(result, "boolean");
        return;
    }

    /* Null literal */
    if (strcmp(value, "null") == 0) {
        copyText(result, "null");
        return;
    }

    size_t length = strlen(value);

    /* String literals with quotation marks */
    if (length >= 2 &&
        value[0] == '"' &&
        value[length - 1] == '"') {
        copyText(result, "String");
        return;
    }

    /* Character literals with quotation marks */
    if (length >= 3 &&
        value[0] == '\'' &&
        value[length - 1] == '\'') {
        copyText(result, "char");
        return;
    }

    /* Numeric literals */
    char *end = NULL;
    strtod(value, &end);

    if (end != value && *end == '\0') {

        if (strchr(value, '.') != NULL ||
            strchr(value, 'e') != NULL ||
            strchr(value, 'E') != NULL) {
            copyText(result, "double");
        }
        else {
            copyText(result, "int");
        }

        return;
    }

    /*
       Our parser currently removes quotation marks
       from string literals.

       Since this function receives an AST LITERAL,
       an otherwise unrecognized literal is treated
       as a String.
    */
    copyText(result, "String");
}


/* =========================================
   EXPRESSION TYPE CHECKING
   ========================================= */

static void inferExpressionType(
    SemanticAnalyzer *analyzer,
    ASTNode *node,
    char *result
) {
    if (node == NULL) {
        copyText(result, "unknown");
        return;
    }

    if (strcmp(node->type, "LITERAL") == 0) {
        getLiteralType(node->value, result);
        return;
    }

    if (strcmp(node->type, "IDENTIFIER") == 0) {
        int index = findSymbol(analyzer, node->value);

        if (index == -1) {
            printf(
                "Semantic Error: Undeclared variable '%s'.\n",
                node->value
            );

            analyzer->errorCount++;
            copyText(result, "unknown");
            return;
        }

        if (!analyzer->symbols[index].initialized) {
            printf(
                "Semantic Error: Variable '%s' may be uninitialized.\n",
                node->value
            );

            analyzer->errorCount++;
        }

        copyText(result, analyzer->symbols[index].type);
        return;
    }

    if (strcmp(node->type, "UNARY_EXPRESSION") == 0) {
        if (node->childCount < 1) {
            copyText(result, "unknown");
            return;
        }

        char operandType[MAX_LEXEME];

        inferExpressionType(
            analyzer,
            node->children[0],
            operandType
        );

        if (strcmp(node->value, "!") == 0) {
            if (!isBooleanType(operandType)) {
                printf(
                    "Semantic Error: Logical NOT requires a boolean operand.\n"
                );

                analyzer->errorCount++;
            }

            copyText(result, "boolean");
        }
        else {
            if (!isNumericType(operandType)) {
                printf(
                    "Semantic Error: Unary arithmetic operator requires a numeric operand.\n"
                );

                analyzer->errorCount++;
                copyText(result, "unknown");
            }
            else {
                copyText(result, operandType);
            }
        }

        return;
    }

    if (strcmp(node->type, "BINARY_EXPRESSION") == 0) {
        if (node->childCount < 2) {
            copyText(result, "unknown");
            return;
        }

        char leftType[MAX_LEXEME];
        char rightType[MAX_LEXEME];

        inferExpressionType(
            analyzer,
            node->children[0],
            leftType
        );

        inferExpressionType(
            analyzer,
            node->children[1],
            rightType
        );

        const char *op = node->value;

        if (strcmp(op, "&&") == 0 ||
            strcmp(op, "||") == 0) {

            if (!isBooleanType(leftType) ||
                !isBooleanType(rightType)) {
                printf(
                    "Semantic Error: Logical operator '%s' requires boolean operands.\n",
                    op
                );

                analyzer->errorCount++;
            }

            copyText(result, "boolean");
            return;
        }

        if (strcmp(op, "==") == 0 ||
            strcmp(op, "!=") == 0) {

            if (!(canAssign(leftType, rightType) ||
                  canAssign(rightType, leftType))) {
                printf(
                    "Semantic Error: Incompatible types in comparison '%s'.\n",
                    op
                );

                analyzer->errorCount++;
            }

            copyText(result, "boolean");
            return;
        }

        if (strcmp(op, "<") == 0 ||
            strcmp(op, ">") == 0 ||
            strcmp(op, "<=") == 0 ||
            strcmp(op, ">=") == 0) {

            if (!isNumericType(leftType) ||
                !isNumericType(rightType)) {
                printf(
                    "Semantic Error: Relational operator '%s' requires numeric operands.\n",
                    op
                );

                analyzer->errorCount++;
            }

            copyText(result, "boolean");
            return;
        }

        if (strcmp(op, "+") == 0 &&
            (strcmp(leftType, "String") == 0 ||
             strcmp(rightType, "String") == 0)) {

            copyText(result, "String");
            return;
        }

        if (strcmp(op, "+") == 0 ||
            strcmp(op, "-") == 0 ||
            strcmp(op, "*") == 0 ||
            strcmp(op, "/") == 0 ||
            strcmp(op, "%") == 0) {

            if (!isNumericType(leftType) ||
                !isNumericType(rightType)) {

                printf(
                    "Semantic Error: Arithmetic operator '%s' requires numeric operands.\n",
                    op
                );

                analyzer->errorCount++;
                copyText(result, "unknown");
                return;
            }

            if (strcmp(op, "%") == 0 &&
                (!isIntegerType(leftType) ||
                 !isIntegerType(rightType))) {

                printf(
                    "Semantic Error: Modulo operator requires integer operands.\n"
                );

                analyzer->errorCount++;
            }

            promoteNumericType(leftType, rightType, result);
            return;
        }
    }

    copyText(result, "unknown");
}

/* =========================================
   AST SEMANTIC ANALYSIS
   ========================================= */

static void analyzeNode(
    SemanticAnalyzer *analyzer,
    ASTNode *node
) {
    if (node == NULL) {
        return;
    }

    /* PROGRAM */

    if (strcmp(node->type, "PROGRAM") == 0) {
        for (int i = 0; i < node->childCount; i++) {
            analyzeNode(analyzer, node->children[i]);
        }

        return;
    }

    /* BLOCK creates a new scope */

    if (strcmp(node->type, "BLOCK") == 0) {
        enterScope(analyzer);

        for (int i = 0; i < node->childCount; i++) {
            analyzeNode(analyzer, node->children[i]);
        }

        exitScope(analyzer);
        return;
    }

    /* VARIABLE DECLARATION */

if (strcmp(node->type, "VARIABLE_DECLARATION") == 0) {
    int index = addSymbol(
        analyzer,
        node->value,
        node->dataType,
        0
    );

    if (node->childCount > 0) {
        char expressionType[MAX_LEXEME];

        inferExpressionType(
            analyzer,
            node->children[0],
            expressionType
        );

        if (index != -1 &&
            strcmp(expressionType, "unknown") != 0) {

            if (!canAssign(
                    analyzer->symbols[index].type,
                    expressionType
                )) {

                printf(
                    "Semantic Error: Cannot assign '%s' to variable '%s' of type '%s'.\n",
                    expressionType,
                    node->value,
                    node->dataType
                );

                analyzer->errorCount++;
            }
            else {
                analyzer->symbols[index].initialized = 1;
            }
        }
    }

    return;
}
    /* ASSIGNMENT */

    if (strcmp(node->type, "ASSIGNMENT") == 0) {
        int index = findSymbol(analyzer, node->value);

        if (index == -1) {
            printf(
                "Semantic Error: Assignment to undeclared variable '%s'.\n",
                node->value
            );

            analyzer->errorCount++;
        }

        if (node->childCount < 1) {
            printf("Semantic Error: Assignment has no expression.\n");
            analyzer->errorCount++;
            return;
        }

        char expressionType[MAX_LEXEME];

        inferExpressionType(
            analyzer,
            node->children[0],
            expressionType
        );

        if (index != -1 &&
            !canAssign(
                analyzer->symbols[index].type,
                expressionType
            )) {

            printf(
                "Semantic Error: Cannot assign '%s' to variable '%s' of type '%s'.\n",
                expressionType,
                node->value,
                analyzer->symbols[index].type
            );

            analyzer->errorCount++;
        }
        else if (index != -1) {
            analyzer->symbols[index].initialized = 1;
        }

        return;
    }

    /* IF STATEMENT */

    if (strcmp(node->type, "IF") == 0) {
        if (node->childCount < 2) {
            printf("Semantic Error: Invalid IF statement.\n");
            analyzer->errorCount++;
            return;
        }

        char conditionType[MAX_LEXEME];

        inferExpressionType(
            analyzer,
            node->children[0],
            conditionType
        );

        if (!isBooleanType(conditionType)) {
            printf(
                "Semantic Error: IF condition must be boolean.\n"
            );

            analyzer->errorCount++;
        }

        analyzeNode(analyzer, node->children[1]);

        if (node->childCount >= 3) {
            analyzeNode(analyzer, node->children[2]);
        }

        return;
    }

    /* WHILE STATEMENT */

    if (strcmp(node->type, "WHILE") == 0) {
        if (node->childCount < 2) {
            printf("Semantic Error: Invalid WHILE statement.\n");
            analyzer->errorCount++;
            return;
        }

        char conditionType[MAX_LEXEME];

        inferExpressionType(
            analyzer,
            node->children[0],
            conditionType
        );

        if (!isBooleanType(conditionType)) {
            printf(
                "Semantic Error: WHILE condition must be boolean.\n"
            );

            analyzer->errorCount++;
        }

        analyzeNode(analyzer, node->children[1]);
        return;
    }

    /* OTHER STATEMENTS */

    if (strcmp(node->type, "PRINT") == 0 ||
        strcmp(node->type, "RETURN") == 0) {

        for (int i = 0; i < node->childCount; i++) {
            char expressionType[MAX_LEXEME];

            inferExpressionType(
                analyzer,
                node->children[i],
                expressionType
            );
        }

        return;
    }

    /* Expressions and remaining nodes */

    for (int i = 0; i < node->childCount; i++) {
        analyzeNode(analyzer, node->children[i]);
    }
}

/* =========================================
   INITIALIZATION
   ========================================= */

void initSemantic(SemanticAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->errorCount = 0;

    analyzer->scopeCount = 0;
    analyzer->currentScope = -1;
}

/* =========================================
   START SEMANTIC ANALYSIS
   ========================================= */

int analyzeSemantics(
    SemanticAnalyzer *analyzer,
    ASTNode *root
) {
    if (analyzer == NULL || root == NULL) {
        return 0;
    }

    analyzer->errorCount = 0;
    analyzer->count = 0;
    analyzer->scopeCount = 0;
    analyzer->currentScope = -1;

    analyzeNode(analyzer, root);

    return analyzer->errorCount == 0;
}

/* =========================================
   PRINT SYMBOL TABLE
   ========================================= */

void printSymbolTable(SemanticAnalyzer *analyzer) {
    printf("\n========== JAVA SYMBOL TABLE ==========\n");

    printf(
        "%-15s %-12s %-15s %-10s\n",
        "Name",
        "Type",
        "Initialized",
        "Scope"
    );

    printf("------------------------------------------------------\n");

    for (int i = 0; i < analyzer->count; i++) {
        Symbol *symbol = &analyzer->symbols[i];

        printf(
            "%-15s %-12s %-15s %-10d\n",
            symbol->name,
            symbol->type,
            symbol->initialized ? "Yes" : "No",
            symbol->scopeId
        );
    }

    printf("======================================================\n");
}
