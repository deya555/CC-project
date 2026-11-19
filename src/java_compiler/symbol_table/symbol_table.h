
#ifndef JAVA_SYMBOL_TABLE_H
#define JAVA_SYMBOL_TABLE_H

#define MAX_SYMBOLS 500
#define MAX_NAME_LENGTH 100
#define MAX_TYPE_LENGTH 50

typedef struct {
    char name[MAX_NAME_LENGTH];
    char dataType[MAX_TYPE_LENGTH];
    int scopeLevel;
    int isInitialized;
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
    int currentScope;
} SymbolTable;

void initSymbolTable(SymbolTable *table);

int enterScope(SymbolTable *table);
int exitScope(SymbolTable *table);

int insertSymbol(
    SymbolTable *table,
    const char *name,
    const char *dataType,
    int isInitialized
);

Symbol *lookupSymbol(SymbolTable *table, const char *name);

int updateSymbolInitialization(SymbolTable *table, const char *name);

void printSymbolTable(const SymbolTable *table);

#endif
