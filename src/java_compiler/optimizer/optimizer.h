
#ifndef JAVA_OPTIMIZER_H
#define JAVA_OPTIMIZER_H

#include "../tac/tac.h"

/* Optimize the generated Three Address Code */
int optimizeTAC(TACProgram *program);

/* Display optimized Three Address Code */
void printOptimizedTAC(const TACProgram *program);

#endif
