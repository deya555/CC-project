
#ifndef CPP_TAC_H
#define CPP_TAC_H

#include "../parser/parser.h"

#define CPP_TAC_MAX_INSTRUCTIONS 1000
#define CPP_TAC_MAX_LENGTH 256

typedef struct {
    char instructions[CPP_TAC_MAX_INSTRUCTIONS][CPP_TAC_MAX_LENGTH];
    int instruction_count;
    int temporary_count;
    int label_count;
    int had_error;
} CPPTAC;

/* Initialize TAC generator */
void cpp_tac_init(CPPTAC *tac);

/* Generate TAC from AST */
int cpp_tac_generate(CPPTAC *tac, CPPASTNode *root);

/* Print generated TAC */
void cpp_tac_print(CPPTAC *tac);

#endif
