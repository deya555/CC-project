#ifndef CODEGEN_H
#define CODEGEN_H

#include "../ast/ast.h"

typedef enum {
    TAC_ASSIGN,
    TAC_ADD,
    TAC_SUB,
    TAC_MUL,
    TAC_DIV,
    TAC_MOD,
    TAC_REL,
    TAC_LOGIC,
    TAC_NEG,
    TAC_NOT,
    TAC_PRINT,
    TAC_LABEL,
    TAC_GOTO,
    TAC_IF_FALSE
} TACInstructionType;

typedef struct TACInstr {
    TACInstructionType type;
    char *result;
    char *arg1;
    char *arg2;
    char *operator;
    struct TACInstr *next;
} TACInstr;

TACInstr *generateTAC(ASTNode *root);
void printTAC(TACInstr *head);
void freeTAC(TACInstr *head);

#endif