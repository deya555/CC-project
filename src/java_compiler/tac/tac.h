
#ifndef JAVA_TAC_H
#define JAVA_TAC_H

#include "../ast/ast.h"

#define TAC_MAX_INSTRUCTIONS 1000
#define TAC_MAX_OPERAND 256

typedef struct {
    char result[TAC_MAX_OPERAND];
    char arg1[TAC_MAX_OPERAND];
    char op[TAC_MAX_OPERAND];
    char arg2[TAC_MAX_OPERAND];
} TACInstruction;

typedef struct {
    TACInstruction instructions[TAC_MAX_INSTRUCTIONS];
    int count;
    int tempCount;
    int labelCount;
    int hasError;
} TACProgram;

void initTAC(TACProgram *program);

int generateTAC(const ASTNode *root, TACProgram *program);

void printTAC(const TACProgram *program);

#endif
