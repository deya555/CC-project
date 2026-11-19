
#include <stdio.h>
#include <string.h>

#include "tac.h"

static void addInstruction(TACGenerator *generator, const char *instruction) {
    if (generator->count >= MAX_TAC_INSTRUCTIONS) {
        generator->hasError = 1;
        return;
    }

    snprintf(
        generator->instructions[generator->count],
        MAX_TAC_LINE,
        "%s",
        instruction
    );

    generator->count++;
}

static void newTemporary(TACGenerator *generator, char *result) {
    generator->tempCount++;

    snprintf(
        result,
        MAX_TAC_LINE,
        "t%d",
        generator->tempCount
    );
}

static void generateExpression(
    TACGenerator *generator,
    ASTNode *node,
    char *result
) {
    if (node == NULL || generator->hasError) {
        generator->hasError = 1;
        return;
    }

    if (strcmp(node->type, "LITERAL") == 0 ||
        strcmp(node->type, "IDENTIFIER") == 0) {

        snprintf(result, MAX_TAC_LINE, "%s", node->value);
        return;
    }

    if (strcmp(node->type, "BINARY_EXPRESSION") == 0) {
        char left[MAX_TAC_LINE];
        char right[MAX_TAC_LINE];
        char instruction[MAX_TAC_LINE];

        if (node->childCount < 2) {
            generator->hasError = 1;
            return;
        }

        generateExpression(generator, node->children[0], left);
        generateExpression(generator, node->children[1], right);

        if (generator->hasError) {
            return;
        }

        newTemporary(generator, result);

        snprintf(
            instruction,
            sizeof(instruction),
            "%s = %s %s %s",
            result,
            left,
            node->value,
            right
        );

        addInstruction(generator, instruction);
        return;
    }

    generator->hasError = 1;
}

static void generateNode(TACGenerator *generator, ASTNode *node) {
    if (node == NULL || generator->hasError) {
        return;
    }

    if (strcmp(node->type, "PROGRAM") == 0 ||
        strcmp(node->type, "BLOCK") == 0) {

        for (int i = 0; i < node->childCount; i++) {
            generateNode(generator, node->children[i]);
        }

        return;
    }

    if (strcmp(node->type, "VARIABLE_DECLARATION") == 0) {
        if (node->childCount > 0) {
            ASTNode *initializer = node->children[0];
            char result[MAX_TAC_LINE];
            char instruction[MAX_TAC_LINE];

            generateExpression(generator, initializer, result);

            if (generator->hasError) {
                return;
            }

            snprintf(
                instruction,
                sizeof(instruction),
                "%s = %s",
                node->value,
                result
            );

            addInstruction(generator, instruction);
        }

        return;
    }

    if (strcmp(node->type, "ASSIGNMENT") == 0) {
        if (node->childCount < 1) {
            generator->hasError = 1;
            return;
        }

        char result[MAX_TAC_LINE];
        char instruction[MAX_TAC_LINE];

        generateExpression(generator, node->children[0], result);

        if (generator->hasError) {
            return;
        }

        snprintf(
            instruction,
            sizeof(instruction),
            "%s = %s",
            node->value,
            result
        );

        addInstruction(generator, instruction);
        return;
    }

    if (strcmp(node->type, "PRINT") == 0) {
        if (node->childCount < 1) {
            generator->hasError = 1;
            return;
        }

        char result[MAX_TAC_LINE];
        char instruction[MAX_TAC_LINE];

        generateExpression(generator, node->children[0], result);

        if (generator->hasError) {
            return;
        }

        snprintf(
            instruction,
            sizeof(instruction),
            "PRINT %s",
            result
        );

        addInstruction(generator, instruction);
        return;
    }

    if (strcmp(node->type, "RETURN") == 0) {
        addInstruction(generator, "RETURN");
        return;
    }

    generator->hasError = 1;
}

void initTAC(TACGenerator *generator) {
    generator->count = 0;
    generator->tempCount = 0;
    generator->hasError = 0;
}

int generateTAC(TACGenerator *generator, ASTNode *root) {
    if (generator == NULL || root == NULL) {
        return 0;
    }

    generateNode(generator, root);

    return !generator->hasError;
}

void printTAC(TACGenerator *generator) {
    printf("\n========== JAVA THREE ADDRESS CODE ==========\n");

    for (int i = 0; i < generator->count; i++) {
        printf("%s\n", generator->instructions[i]);
    }

    printf("=============================================\n");
}
