
#ifndef C_SYMBOL_TABLE_H
#define C_SYMBOL_TABLE_H

#define MAX_SYMBOLS 100
#define MAX_NAME_LENGTH 100
#define MAX_TYPE_LENGTH 20

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
} SymbolTable;

// Initialize the symbol table
void symbol_table_init(SymbolTable *table);

// Enter a new scope
void symbol_table_enter_scope(SymbolTable *table);

// Exit the current scope
void symbol_table_exit_scope(SymbolTable *table);

// Add a variable to the symbol table
int symbol_table_add(
    SymbolTable *table,
    const char *name,
    const char *type,
    int is_initialized
);

// Search for a variable
Symbol *symbol_table_lookup(
    SymbolTable *table,
    const char *name
);

// Display all symbols
void symbol_table_print(SymbolTable *table);

#endif
