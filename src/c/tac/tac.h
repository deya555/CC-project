
#ifndef C_TAC_H
#define C_TAC_H

#include "../ast/ast.h"

void tac_generate(ASTNode *root);
void tac_print(void);
void tac_free(void);

#endif
