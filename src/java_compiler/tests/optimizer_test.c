
#include <stdio.h>

#include "../tac/tac.h"
#include "../optimizer/optimizer.h"

static TACProgram program;

static void setInstruction(int index,
                           const char *result,
                           const char *arg1,
                           const char *op,
                           const char *arg2)
{
    snprintf(program.instructions[index].result,
             TAC_MAX_OPERAND, "%s", result);

    snprintf(program.instructions[index].arg1,
             TAC_MAX_OPERAND, "%s", arg1);

    snprintf(program.instructions[index].op,
             TAC_MAX_OPERAND, "%s", op);

    snprintf(program.instructions[index].arg2,
             TAC_MAX_OPERAND, "%s", arg2);
}

int main(void)
{
    printf("========== CONTROL FLOW OPTIMIZER TEST ==========\n");

    initTAC(&program);

    setInstruction(0, "number", "10", "", "");
    setInstruction(1, "t1", "number", ">", "5");
    setInstruction(2, "", "t1", "ifFalse", "L1");
    setInstruction(3, "t2", "number", "+", "1");
    setInstruction(4, "number", "t2", "", "");
    setInstruction(5, "L1", "", "", "");
    setInstruction(6, "", "number", "print", "");

    program.count = 7;

    printf("\n--- BEFORE OPTIMIZATION ---\n");
    printTAC(&program);

    if (!optimizeTAC(&program)) {
        printf("\nControl Flow Optimization Failed!\n");
        return 1;
    }

    printf("\n--- AFTER OPTIMIZATION ---\n");
    printOptimizedTAC(&program);

    printf("\n========== CONTROL FLOW TEST COMPLETED ==========\n");

    return 0;
}
