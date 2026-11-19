
#ifndef JAVA_SEMANTIC_H
#define JAVA_SEMANTIC_H

#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"

/* Analyze the complete AST using the symbol table.
   Returns 1 if successful, 0 if semantic errors are found. */
int analyzeSemantics(const ASTNode *root, SymbolTable *table);

#endif
