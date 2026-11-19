
#include <stdio.h>
#include <string.h>

#include "tac.h"

static void generateNode(
    TACGenerator *generator,
    ASTNode *node
);

/* Add a TAC instruction */

static void addInstruction(
    TACGenerator *generator,
    const char *instruction
) {
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

/* Generate temporary variables: t1, t2, t3... */

static void newTemporary(
    TACGenerator *generator,
    char *result
) {
    generator->tempCount++;

    snprintf(
        result,
        MAX_TAC_LINE,
        "t%d",
        generator->tempCount
    );
}

/* Generate labels: L1, L2, L3... */

static void newLabel(
    TACGenerator *generator,
    char *result
) {
    generator->labelCount++;

    snprintf(
        result,
        MAX_TAC_LINE,
        "L%d",
        generator->labelCount
    );
}

/* Generate TAC for expressions */

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

        generateExpression(
            generator,
            node->children[0],
            left
        );

        generateExpression(
            generator,
            node->children[1],
            right
        );

        if (generator->hasError) {
            return;
        }

        newTemporary(generator, result);

        snprintf(
            instruction,
            MAX_TAC_LINE,
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

/* Generate TAC for IF statements */

static void generateIf(
    TACGenerator *generator,
    ASTNode *node
) {
    if (node->childCount < 2) {
        generator->hasError = 1;
        return;
    }

    char condition[MAX_TAC_LINE];
    char elseLabel[MAX_TAC_LINE];
    char endLabel[MAX_TAC_LINE];
    char instruction[MAX_TAC_LINE];

    newLabel(generator, elseLabel);

    generateExpression(
        generator,
        node->children[0],
        condition
    );

    if (generator->hasError) {
        return;
    }

    snprintf(
        instruction,
        MAX_TAC_LINE,
        "IF_FALSE %s GOTO %s",
        condition,
        elseLabel
    );

    addInstruction(generator, instruction);

    if (generator->hasError) {
        return;
    }

    /* Generate IF body */

    generateNode(generator, node->children[1]);

    if (generator->hasError) {
        return;
    }

    /* Check whether ELSE exists */

    if (node->childCount >= 3) {

        newLabel(generator, endLabel);

        snprintf(
            instruction,
            MAX_TAC_LINE,
            "GOTO %s",
            endLabel
        );

        addInstruction(generator, instruction);

        snprintf(
            instruction,
            MAX_TAC_LINE,
            "LABEL %s",
            elseLabel
        );

        addInstruction(generator, instruction);

        /* Generate ELSE body */

        generateNode(generator, node->children[2]);

        snprintf(
            instruction,
            MAX_TAC_LINE,
            "LABEL %s",
            endLabel
        );

        addInstruction(generator, instruction);

    } else {

        snprintf(
            instruction,
            MAX_TAC_LINE,
            "LABEL %s",
            elseLabel
        );

        addInstruction(generator, instruction);
    }
}

/* Generate TAC for WHILE loops */

static void generateWhile(
    TACGenerator *generator,
    ASTNode *node
) {
    if (node->childCount < 2) {
        generator->hasError = 1;
        return;
    }

    char startLabel[MAX_TAC_LINE];
    char endLabel[MAX_TAC_LINE];
    char condition[MAX_TAC_LINE];
    char instruction[MAX_TAC_LINE];

    newLabel(generator, startLabel);
    newLabel(generator, endLabel);

    /* Loop start */

    snprintf(
        instruction,
        MAX_TAC_LINE,
        "LABEL %s",
        startLabel
    );

    addInstruction(generator, instruction);

    /* Evaluate loop condition */

    generateExpression(
        generator,
        node->children[0],
        condition
    );

    if (generator->hasError) {
        return;
    }

    snprintf(
        instruction,
        MAX_TAC_LINE,
        "IF_FALSE %s GOTO %s",
        condition,
        endLabel
    );

    addInstruction(generator, instruction);

    /* Generate loop body */

    generateNode(generator, node->children[1]);

    if (generator->hasError) {
        return;
    }

    /* Jump back to loop start */

    snprintf(
        instruction,
        MAX_TAC_LINE,
        "GOTO %s",
        startLabel
    );

    addInstruction(generator, instruction);

    /* Loop exit */

    snprintf(
        instruction,
        MAX_TAC_LINE,
        "LABEL %s",
        endLabel
    );

    addInstruction(generator, instruction);
}

/* Generate TAC for AST nodes */

static void generateNode(
    TACGenerator *generator,
    ASTNode *node
) {
    if (node == NULL || generator->hasError) {
        return;
    }

    /* PROGRAM and BLOCK */

    if (strcmp(node->type, "PROGRAM") == 0 ||
        strcmp(node->type, "BLOCK") == 0) {

        for (int i = 0; i < node->childCount; i++) {
            generateNode(generator, node->children[i]);
        }

        return;
    }

    /* VARIABLE DECLARATION */

    if (strcmp(node->type, "VARIABLE_DECLARATION") == 0) {

        if (node->childCount > 0) {

            char result[MAX_TAC_LINE];
            char instruction[MAX_TAC_LINE];

            generateExpression(
                generator,
                node->children[0],
                result
            );

            if (generator->hasError) {
                return;
            }

            snprintf(
                instruction,
                MAX_TAC_LINE,
                "%s = %s",
                node->value,
                result
            );

            addInstruction(generator, instruction);
        }

        return;
    }

    /* ASSIGNMENT */

    if (strcmp(node->type, "ASSIGNMENT") == 0) {

        if (node->childCount < 1) {
            generator->hasError = 1;
            return;
        }

        char result[MAX_TAC_LINE];
        char instruction[MAX_TAC_LINE];

        generateExpression(
            generator,
            node->children[0],
            result
        );

        if (generator->hasError) {
            return;
        }

        snprintf(
            instruction,
            MAX_TAC_LINE,
            "%s = %s",
            node->value,
            result
        );

        addInstruction(generator, instruction);
        return;
    }

    /* PRINT */

    if (strcmp(node->type, "PRINT") == 0) {

        if (node->childCount < 1) {
            generator->hasError = 1;
            return;
        }

        char result[MAX_TAC_LINE];
        char instruction[MAX_TAC_LINE];

        generateExpression(
            generator,
            node->children[0],
            result
        );

        if (generator->hasError) {
            return;
        }

        snprintf(
            instruction,
            MAX_TAC_LINE,
            "PRINT %s",
            result
        );

        addInstruction(generator, instruction);
        return;
    }

    /* RETURN */

    if (strcmp(node->type, "RETURN") == 0) {
        addInstruction(generator, "RETURN");
        return;
    }

    /* IF / ELSE */

    if (strcmp(node->type, "IF") == 0) {
        generateIf(generator, node);
        return;
    }

    /* WHILE */

    if (strcmp(node->type, "WHILE") == 0) {
        generateWhile(generator, node);
        return;
    }

    /* Unsupported AST node */

    generator->hasError = 1;
}

/* Initialize TAC generator */

void initTAC(TACGenerator *generator) {

    generator->count = 0;
    generator->tempCount = 0;
    generator->labelCount = 0;
    generator->hasError = 0;
}

/* Start TAC generation */

int generateTAC(
    TACGenerator *generator,
    ASTNode *root
) {
    if (generator == NULL || root == NULL) {
        return 0;
    }

    generateNode(generator, root);

    return !generator->hasError;
}

/* Print generated TAC */

void printTAC(TACGenerator *generator) {

    printf("\n========== JAVA THREE ADDRESS CODE ==========\n");

    for (int i = 0; i < generator->count; i++) {
        printf("%s\n", generator->instructions[i]);
    }

    printf("=============================================\n");
}
