#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct Symbol {
    char *name;
    char *type;
    int declaration_line;
    struct Symbol *next;
} Symbol;

typedef struct Scope {
    int scope_id;
    int level;
    struct Scope *parent;
    Symbol *symbols;
    struct Scope *next;
} Scope;

typedef struct SymbolTable {
    Scope *current_scope;
    Scope *all_scopes;
    int next_scope_id;
} SymbolTable;

void symbol_table_init(SymbolTable *table);

void enter_scope(SymbolTable *table);

void exit_scope(SymbolTable *table);

int insert_symbol(
    SymbolTable *table,
    const char *name,
    const char *type,
    int line
);

Symbol *lookup_symbol(
    SymbolTable *table,
    const char *name
);

Symbol *lookup_symbol_in_scope(
    SymbolTable *table,
    const char *name,
    int scope_id
);

void print_symbol_table(SymbolTable *table);

void free_symbol_table(SymbolTable *table);

#endif