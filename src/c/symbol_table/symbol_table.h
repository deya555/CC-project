
#ifndef C_SYMBOL_TABLE_H
#define C_SYMBOL_TABLE_H

#define MAX_SYMBOLS 100
#define MAX_NAME_LENGTH 100
#define MAX_TYPE_LENGTH 20
#define MAX_SCOPES 50

typedef struct
{
    char name[MAX_NAME_LENGTH];
    char type[MAX_TYPE_LENGTH];
    int scope_level;
    int is_initialized;
} Symbol;

typedef struct
{
    Symbol symbols[MAX_SYMBOLS];
    int count;
    int current_scope;
    int scope_stack[MAX_SCOPES];
} SymbolTable;

void symbol_table_init(SymbolTable *table);

void symbol_table_enter_scope(SymbolTable *table);

void symbol_table_exit_scope(SymbolTable *table);

int symbol_table_add(
    SymbolTable *table,
    const char *name,
    const char *type,
    int is_initialized
);

Symbol *symbol_table_lookup(SymbolTable *table, const char *name);

void symbol_table_print(SymbolTable *table);

#endif
