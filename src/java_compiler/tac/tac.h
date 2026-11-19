
#ifndef JAVA_TAC_H
#define JAVA_TAC_H

#include "../parser/parser.h"

#define MAX_TAC_INSTRUCTIONS 1000
#define MAX_TAC_LINE 256

typedef struct {
    char instructions[MAX_TAC_INSTRUCTIONS][MAX_TAC_LINE];
    int count;
    int tempCount;
    int labelCount;
    int hasError;
} TACGenerator;

void initTAC(TACGenerator *generator);
int generateTAC(TACGenerator *generator, ASTNode *root);
void printTAC(TACGenerator *generator);

#endif
