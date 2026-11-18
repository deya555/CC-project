#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../ast/ast.h"
#include "../symbol_table/symbol_table.h"

int analyze_semantics(ASTNode *root, SymbolTable *table);

#endif