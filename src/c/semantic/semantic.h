
#ifndef C_SEMANTIC_H
#define C_SEMANTIC_H

#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"

// Perform semantic analysis on the AST
void semantic_analyze(ASTNode *root, SymbolTable *table);

#endif
