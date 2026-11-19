
#include "tac.h"

#include <stdio.h>
#include <string.h>

static int addInstruction(
    TACProgram *program,
    const char *result,
    const char *arg1,
    const char *op,
    const char *arg2
)
{
    if (program == NULL ||
        program->count >= TAC_MAX_INSTRUCTIONS) {
        if (program != NULL)
            program->hasError = 1;

        return 0;
    }

    TACInstruction *instruction =
        &program->instructions[program->count];

    snprintf(instruction->result, TAC_MAX_OPERAND, "%s",
             result ? result : "");

    snprintf(instruction->arg1, TAC_MAX_OPERAND, "%s",
             arg1 ? arg1 : "");

    snprintf(instruction->op, TAC_MAX_OPERAND, "%s",
             op ? op : "");

    snprintf(instruction->arg2, TAC_MAX_OPERAND, "%s",
             arg2 ? arg2 : "");

    program->count++;

    return 1;
}

static int newTemp(TACProgram *program, char *buffer)
{
    program->tempCount++;

    snprintf(buffer, TAC_MAX_OPERAND, "t%d",
             program->tempCount);

    return 1;
}

static int newLabel(TACProgram *program, char *buffer)
{
    program->labelCount++;

    snprintf(buffer, TAC_MAX_OPERAND, "L%d",
             program->labelCount);

    return 1;
}

static int generateExpression(
    const ASTNode *node,
    TACProgram *program,
    char *result
);

static int generateStatement(
    const ASTNode *node,
    TACProgram *program
);

/* ================= EXPRESSION GENERATION ================= */

static int generateExpression(
    const ASTNode *node,
    TACProgram *program,
    char *result
)
{
    if (node == NULL || program == NULL || result == NULL)
        return 0;

    if (node->type == AST_LITERAL ||
        node->type == AST_IDENTIFIER) {

        snprintf(result, TAC_MAX_OPERAND, "%s", node->value);
        return 1;
    }

    if (node->type == AST_UNARY_EXPRESSION) {
        if (node->childCount != 1)
            return 0;

        char operand[TAC_MAX_OPERAND];

        if (!generateExpression(node->children[0],
                                program, operand))
            return 0;

        newTemp(program, result);

        return addInstruction(program, result,
                              node->value, operand, NULL);
    }

    if (node->type == AST_BINARY_EXPRESSION) {
        if (node->childCount != 2)
            return 0;

        char left[TAC_MAX_OPERAND];
        char right[TAC_MAX_OPERAND];

        if (!generateExpression(node->children[0],
                                program, left))
            return 0;

        if (!generateExpression(node->children[1],
                                program, right))
            return 0;

        newTemp(program, result);

        return addInstruction(program, result,
                              left, node->value, right);
    }

    program->hasError = 1;
    return 0;
}

/* ================= VARIABLE DECLARATION ================= */

static int generateDeclaration(
    const ASTNode *node,
    TACProgram *program
)
{
    if (node->childCount == 0) {
        return addInstruction(program, node->value,
                              NULL, NULL, NULL);
    }

    char expression[TAC_MAX_OPERAND];

    if (!generateExpression(node->children[0],
                            program, expression))
        return 0;

    return addInstruction(program, node->value,
                          expression, NULL, NULL);
}

/* ================= ASSIGNMENT ================= */

static int generateAssignment(
    const ASTNode *node,
    TACProgram *program
)
{
    if (node->childCount != 1)
        return 0;

    char expression[TAC_MAX_OPERAND];

    if (!generateExpression(node->children[0],
                            program, expression))
        return 0;

    return addInstruction(program, node->value,
                          expression, NULL, NULL);
}

/* ================= IF GENERATION ================= */

static int generateIf(
    const ASTNode *node,
    TACProgram *program
)
{
    if (node->childCount < 2 || node->childCount > 3)
        return 0;

    char condition[TAC_MAX_OPERAND];
    char falseLabel[TAC_MAX_OPERAND];
    char endLabel[TAC_MAX_OPERAND];

    if (!generateExpression(node->children[0],
                            program, condition))
        return 0;

    newLabel(program, falseLabel);

    if (!addInstruction(program, NULL, condition,
                        "ifFalse", falseLabel))
        return 0;

    if (!generateStatement(node->children[1], program))
        return 0;

    if (node->childCount == 3) {
        newLabel(program, endLabel);

        if (!addInstruction(program, NULL, NULL,
                            "goto", endLabel))
            return 0;

        if (!addInstruction(program, NULL, NULL,
                            "label", falseLabel))
            return 0;

        if (!generateStatement(node->children[2], program))
            return 0;

        return addInstruction(program, NULL, NULL,
                              "label", endLabel);
    }

    return addInstruction(program, NULL, NULL,
                          "label", falseLabel);
}

/* ================= WHILE GENERATION ================= */

static int generateWhile(
    const ASTNode *node,
    TACProgram *program
)
{
    if (node->childCount != 2)
        return 0;

    char startLabel[TAC_MAX_OPERAND];
    char endLabel[TAC_MAX_OPERAND];
    char condition[TAC_MAX_OPERAND];

    newLabel(program, startLabel);
    newLabel(program, endLabel);

    if (!addInstruction(program, NULL, NULL,
                        "label", startLabel))
        return 0;

    if (!generateExpression(node->children[0],
                            program, condition))
        return 0;

    if (!addInstruction(program, NULL, condition,
                        "ifFalse", endLabel))
        return 0;

    if (!generateStatement(node->children[1], program))
        return 0;

    if (!addInstruction(program, NULL, NULL,
                        "goto", startLabel))
        return 0;

    return addInstruction(program, NULL, NULL,
                          "label", endLabel);
}

/* ================= PRINT GENERATION ================= */

static int generatePrint(
    const ASTNode *node,
    TACProgram *program
)
{
    if (node->childCount != 1)
        return 0;

    char expression[TAC_MAX_OPERAND];

    if (!generateExpression(node->children[0],
                            program, expression))
        return 0;

    return addInstruction(program, NULL, expression,
                          "print", NULL);
}

/* ================= STATEMENT GENERATION ================= */

static int generateStatement(
    const ASTNode *node,
    TACProgram *program
)
{
    if (node == NULL || program == NULL)
        return 0;

    switch (node->type) {
        case AST_PROGRAM:
        case AST_BLOCK:
            for (int i = 0; i < node->childCount; i++) {
                if (!generateStatement(node->children[i],
                                       program))
                    return 0;
            }
            return 1;

        case AST_VARIABLE_DECLARATION:
            return generateDeclaration(node, program);

        case AST_ASSIGNMENT:
            return generateAssignment(node, program);

        case AST_PRINT:
            return generatePrint(node, program);

        case AST_IF:
            return generateIf(node, program);

        case AST_WHILE:
            return generateWhile(node, program);

        case AST_EMPTY:
            return 1;

        default:
            program->hasError = 1;
            return 0;
    }
}

/* ================= INITIALIZE TAC ================= */

void initTAC(TACProgram *program)
{
    if (program == NULL)
        return;

    memset(program, 0, sizeof(*program));
}

/* ================= GENERATE TAC ================= */

int generateTAC(const ASTNode *root, TACProgram *program)
{
    if (root == NULL || program == NULL)
        return 0;

    initTAC(program);

    printf("\n========== TAC GENERATION ==========\n");

    if (!generateStatement(root, program) ||
        program->hasError) {
        printf("TAC Generation Failed!\n");
        program->hasError = 1;
        return 0;
    }

    printf("TAC Generation Successful!\n");
    return 1;
}


/* ================= PRINT TAC ================= */

void printTAC(const TACProgram *program)
{
    if (program == NULL)
        return;

    printf("\n========== THREE ADDRESS CODE ==========\n");

    for (int i = 0; i < program->count; i++) {
        const TACInstruction *instruction =
            &program->instructions[i];

        if (strcmp(instruction->op, "label") == 0) {
            printf("%s:\n", instruction->arg2);
        }
        else if (strcmp(instruction->op, "goto") == 0) {
            printf("goto %s\n", instruction->arg2);
        }
        else if (strcmp(instruction->op, "ifFalse") == 0) {
            printf("ifFalse %s goto %s\n",
                   instruction->arg1,
                   instruction->arg2);
        }
        else if (strcmp(instruction->op, "print") == 0) {
            printf("print %s\n", instruction->arg1);
        }
        else if (instruction->op[0] == '\0') {
            if (instruction->arg1[0] != '\0') {
                printf("%s = %s\n",
                       instruction->result,
                       instruction->arg1);
            } else {
                printf("%s\n", instruction->result);
            }
        }
        else if (instruction->arg2[0] == '\0' &&
                 (strcmp(instruction->op, "!") == 0 ||
                  strcmp(instruction->op, "-") == 0 ||
                  strcmp(instruction->op, "+") == 0)) {

            printf("%s = %s%s\n",
                   instruction->result,
                   instruction->op,
                   instruction->arg1);
        }
        else {
            printf("%s = %s %s %s\n",
                   instruction->result,
                   instruction->arg1,
                   instruction->op,
                   instruction->arg2);
        }
    }

    printf("========================================\n");
}
