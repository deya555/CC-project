
#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"

/* Analyze the AST for semantic errors */
int analyzeSemantics(ASTNode *root, SymbolTable *table);

#endif
