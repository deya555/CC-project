
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL
} DataType;

typedef struct Symbol {
    char *name;
    DataType type;
    int scope_level;
    int line_number;
    struct Symbol *next;
} Symbol;

typedef struct {
    Symbol *head;
    int current_scope;
} SymbolTable;

/* Symbol table functions */
void initSymbolTable(SymbolTable *table);

void enterScope(SymbolTable *table);

void exitScope(SymbolTable *table);

int insertSymbol(SymbolTable *table,
                 const char *name,
                 DataType type,
                 int line_number);

Symbol *lookupSymbol(SymbolTable *table, const char *name);

void printSymbolTable(SymbolTable *table);

void freeSymbolTable(SymbolTable *table);

#endif
