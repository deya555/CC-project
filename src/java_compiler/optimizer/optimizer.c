#include "optimizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_CONSTANTS 500

typedef struct {
    char name[TAC_MAX_OPERAND];
    char value[TAC_MAX_OPERAND];
    int valid;
} Constant;

static Constant constants[MAX_CONSTANTS];
static int constantCount = 0;

/* ================= CONSTANT TABLE ================= */

static void clearConstants(void)
{
    constantCount = 0;
    memset(constants, 0, sizeof(constants));
}

static int isNumber(const char *value)
{
    char *end;

    if (value == NULL || value[0] == '\0')
        return 0;

    strtod(value, &end);

    return end != value && *end == '\0';
}

static int findConstant(const char *name)
{
    int i;

    for (i = 0; i < constantCount; i++) {
        if (constants[i].valid &&
            strcmp(constants[i].name, name) == 0)
            return i;
    }

    return -1;
}

static void removeConstant(const char *name)
{
    int index = findConstant(name);

    if (index >= 0)
        constants[index].valid = 0;
}

static void setConstant(const char *name, const char *value)
{
    int index;

    if (name == NULL || value == NULL || !isNumber(value))
        return;

    index = findConstant(name);

    if (index < 0) {
        if (constantCount >= MAX_CONSTANTS)
            return;

        index = constantCount++;
    }

    snprintf(constants[index].name,
             TAC_MAX_OPERAND, "%s", name);

    snprintf(constants[index].value,
             TAC_MAX_OPERAND, "%s", value);

    constants[index].valid = 1;
}

/* ================= CONSTANT PROPAGATION ================= */

static void propagateConstant(char *operand)
{
    int index;

    if (operand == NULL || operand[0] == '\0')
        return;

    index = findConstant(operand);

    if (index >= 0) {
        snprintf(operand, TAC_MAX_OPERAND, "%s",
                 constants[index].value);
    }
}

/* ================= CONSTANT FOLDING ================= */

static int calculateConstant(
    const char *left,
    const char *op,
    const char *right,
    char *result)
{
    double a;
    double b;
    double value;

    if (!isNumber(left) || !isNumber(right))
        return 0;

    a = strtod(left, NULL);
    b = strtod(right, NULL);

    if (strcmp(op, "+") == 0)
        value = a + b;
    else if (strcmp(op, "-") == 0)
        value = a - b;
    else if (strcmp(op, "*") == 0)
        value = a * b;
    else if (strcmp(op, "/") == 0) {
        if (b == 0)
            return 0;

        value = a / b;
    }
    else if (strcmp(op, "%") == 0) {
        long long x = (long long)a;
        long long y = (long long)b;

        if (y == 0)
            return 0;

        value = (double)(x % y);
    }
    else if (strcmp(op, ">") == 0)
        value = a > b;
    else if (strcmp(op, "<") == 0)
        value = a < b;
    else if (strcmp(op, ">=") == 0)
        value = a >= b;
    else if (strcmp(op, "<=") == 0)
        value = a <= b;
    else if (strcmp(op, "==") == 0)
        value = a == b;
    else if (strcmp(op, "!=") == 0)
        value = a != b;
    else
        return 0;

    snprintf(result, TAC_MAX_OPERAND, "%.15g", value);

    return 1;
}

/* ================= CONTROL FLOW DETECTION ================= */

/*
 * A label in our TAC representation looks like:
 *
 * result = L1
 * arg1   = ""
 * op     = ""
 * arg2   = ""
 *
 * Do not confuse it with an ordinary assignment.
 */
static int isLabelInstruction(const TACInstruction *instruction)
{
    const char *name = instruction->result;
    int i;

    if (name[0] != 'L' ||
        instruction->arg1[0] != '\0' ||
        instruction->op[0] != '\0' ||
        instruction->arg2[0] != '\0')
        return 0;

    if (name[1] == '\0')
        return 0;

    for (i = 1; name[i] != '\0'; i++) {
        if (name[i] < '0' || name[i] > '9')
            return 0;
    }

    return 1;
}

static int hasControlFlow(const TACProgram *program)
{
    int i;

    for (i = 0; i < program->count; i++) {
        const TACInstruction *instruction =
            &program->instructions[i];

        if (strcmp(instruction->op, "ifFalse") == 0 ||
            strcmp(instruction->op, "goto") == 0 ||
            strcmp(instruction->op, "label") == 0 ||
            isLabelInstruction(instruction)) {
            return 1;
        }
    }

    return 0;
}

/* ================= DEAD CODE ELIMINATION ================= */

static int operandUsed(
    const TACProgram *program,
    int start,
    const char *name)
{
    int i;

    for (i = start; i < program->count; i++) {
        const TACInstruction *instruction =
            &program->instructions[i];

        if (strcmp(instruction->arg1, name) == 0 ||
            strcmp(instruction->arg2, name) == 0)
            return 1;
    }

    return 0;
}

static int isRemovableAssignment(
    const TACInstruction *instruction)
{
    if (instruction->result[0] == '\0')
        return 0;

    /* Never remove labels. */
    if (isLabelInstruction(instruction))
        return 0;

    /* Never remove control-flow instructions. */
    if (strcmp(instruction->op, "ifFalse") == 0 ||
        strcmp(instruction->op, "goto") == 0 ||
        strcmp(instruction->op, "label") == 0 ||
        strcmp(instruction->op, "print") == 0)
        return 0;

    return 1;
}

static void eliminateDeadCode(TACProgram *program)
{
    int i;
    int changed;

    /*
     * IMPORTANT:
     * This simple DCE pass is only safe for linear TAC.
     * Preserve every instruction when control flow exists.
     */
    if (hasControlFlow(program)) {
        printf("Control flow detected: dead-code elimination skipped.\n");
        return;
    }

    do {
        changed = 0;

        for (i = 0; i < program->count; i++) {
            TACInstruction *instruction =
                &program->instructions[i];

            if (!isRemovableAssignment(instruction))
                continue;

            if (!operandUsed(program, i + 1,
                             instruction->result)) {

                printf("Removed dead instruction: %s\n",
                       instruction->result);

                for (int j = i; j < program->count - 1; j++) {
                    program->instructions[j] =
                        program->instructions[j + 1];
                }

                program->count--;
                changed = 1;
                break;
            }
        }

    } while (changed);
}

/* ================= OPTIMIZATION ================= */

int optimizeTAC(TACProgram *program)
{
    int i;

    if (program == NULL || program->hasError)
        return 0;

    clearConstants();

    printf("\n========== TAC OPTIMIZATION ==========\n");

    for (i = 0; i < program->count; i++) {
        TACInstruction *instruction =
            &program->instructions[i];

        /* Preserve labels and clear assumptions. */
        if (isLabelInstruction(instruction) ||
            strcmp(instruction->op, "label") == 0 ||
            strcmp(instruction->op, "goto") == 0 ||
            strcmp(instruction->op, "ifFalse") == 0) {

            clearConstants();
            continue;
        }

        /* Print instruction. */
        if (strcmp(instruction->op, "print") == 0) {
            propagateConstant(instruction->arg1);
            continue;
        }

        /* Unary operation. */
        if (strcmp(instruction->op, "!") == 0 ||
            (strcmp(instruction->op, "-") == 0 &&
             instruction->arg2[0] == '\0')) {

            propagateConstant(instruction->arg1);

            if (isNumber(instruction->arg1)) {
                double value =
                    strtod(instruction->arg1, NULL);

                if (strcmp(instruction->op, "-") == 0)
                    value = -value;
                else
                    value = !value;

                snprintf(instruction->arg1,
                         TAC_MAX_OPERAND, "%.15g", value);

                instruction->op[0] = '\0';
                instruction->arg2[0] = '\0';

                setConstant(instruction->result,
                            instruction->arg1);
            }
            else {
                removeConstant(instruction->result);
            }

            continue;
        }

        /* Binary operation. */
        if (instruction->op[0] != '\0') {
            propagateConstant(instruction->arg1);
            propagateConstant(instruction->arg2);

            removeConstant(instruction->result);

            {
                char folded[TAC_MAX_OPERAND];

                if (calculateConstant(
                        instruction->arg1,
                        instruction->op,
                        instruction->arg2,
                        folded)) {

                    snprintf(instruction->arg1,
                             TAC_MAX_OPERAND, "%s", folded);

                    instruction->op[0] = '\0';
                    instruction->arg2[0] = '\0';

                    setConstant(instruction->result, folded);
                }
            }

            continue;
        }

        /* Simple assignment. */
        if (instruction->result[0] != '\0' &&
            instruction->arg1[0] != '\0') {

            propagateConstant(instruction->arg1);

            if (isNumber(instruction->arg1))
                setConstant(instruction->result,
                            instruction->arg1);
            else
                removeConstant(instruction->result);
        }
    }

    eliminateDeadCode(program);

    printf("TAC Optimization Completed!\n");

    return 1;
}

/* ================= PRINT OPTIMIZED TAC ================= */

void printOptimizedTAC(const TACProgram *program)
{
    if (program == NULL)
        return;

    printf("\n========== OPTIMIZED THREE ADDRESS CODE ==========\n");

    printTAC(program);
}
