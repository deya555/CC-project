
#include "semantic.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initSemantic(SemanticAnalyzer *analyzer) {
    analyzer->count = 0;
    analyzer->errorCount = 0;
}

static int findSymbol(SemanticAnalyzer *analyzer, const char *name) {
    for (int i = 0; i < analyzer->count; i++) {
        if (strcmp(analyzer->symbols[i].name, name) == 0)
            return i;
    }

    return -1;
}

static void addSymbol(
    SemanticAnalyzer *analyzer,
    const char *name,
    const char *type,
    int initialized
) {
    if (findSymbol(analyzer, name) != -1) {
        printf("Semantic Error: Variable '%s' already declared.\n", name);
        analyzer->errorCount++;
        return;
    }

    if (analyzer->count >= MAX_SYMBOLS) {
        printf("Semantic Error: Symbol table is full.\n");
        analyzer->errorCount++;
        return;
    }

    Symbol *symbol = &analyzer->symbols[analyzer->count++];

    snprintf(symbol->name, sizeof(symbol->name), "%s", name);
    snprintf(symbol->type, sizeof(symbol->type), "%s", type);

    symbol->initialized = initialized;
    symbol->line = 0;
}

static void checkExpression(
    SemanticAnalyzer *analyzer,
    ASTNode *node
) {
    if (!node)
        return;

    if (strcmp(node->type, "IDENTIFIER") == 0) {
        int index = findSymbol(analyzer, node->value);

        if (index == -1) {
            printf("Semantic Error: Undeclared variable '%s'.\n",
                   node->value);
            analyzer->errorCount++;
        }
        else if (!analyzer->symbols[index].initialized) {
            printf("Semantic Error: Variable '%s' may be uninitialized.\n",
                   node->value);
            analyzer->errorCount++;
        }

        return;
    }

    for (int i = 0; i < node->childCount; i++) {
        checkExpression(analyzer, node->children[i]);
    }
}

static void analyzeNode(
    SemanticAnalyzer *analyzer,
    ASTNode *node
) {
    if (!node)
        return;

    if (strcmp(node->type, "VARIABLE_DECLARATION") == 0) {
        int initialized = node->childCount > 0;

        addSymbol(
            analyzer,
            node->value,
            node->type,
            initialized
        );

        if (initialized)
            checkExpression(analyzer, node->children[0]);

        return;
    }

    if (strcmp(node->type, "ASSIGNMENT") == 0) {
        int index = findSymbol(analyzer, node->value);

        if (index == -1) {
            printf("Semantic Error: Assignment to undeclared variable '%s'.\n",
                   node->value);
            analyzer->errorCount++;
        }

        if (node->childCount > 0)
            checkExpression(analyzer, node->children[0]);

        if (index != -1)
            analyzer->symbols[index].initialized = 1;

        return;
    }

    if (strcmp(node->type, "IDENTIFIER") == 0) {
        checkExpression(analyzer, node);
        return;
    }

    for (int i = 0; i < node->childCount; i++) {
        analyzeNode(analyzer, node->children[i]);
    }
}

int analyzeSemantics(
    SemanticAnalyzer *analyzer,
    ASTNode *root
) {
    analyzer->errorCount = 0;

    analyzeNode(analyzer, root);

    return analyzer->errorCount == 0;
}

void printSymbolTable(SemanticAnalyzer *analyzer) {
    printf("\n========== JAVA SYMBOL TABLE ==========\n");

    printf("%-15s %-15s %-15s\n",
           "Name", "Type", "Initialized");

    printf("-----------------------------------------------\n");

    for (int i = 0; i < analyzer->count; i++) {
        Symbol *symbol = &analyzer->symbols[i];

        printf("%-15s %-15s %-15s\n",
               symbol->name,
               symbol->type,
               symbol->initialized ? "Yes" : "No");
    }

    printf("===============================================\n");
}
