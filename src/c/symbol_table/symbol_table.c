
#include <stdio.h>
#include <string.h>
#include "symbol_table.h"

// Initialize the symbol table
void symbol_table_init(SymbolTable *table)
{
    table->count = 0;
    table->current_scope = 0;
}

// Enter a new scope
void symbol_table_enter_scope(SymbolTable *table)
{
    table->current_scope++;
}

// Exit the current scope
void symbol_table_exit_scope(SymbolTable *table)
{
    if (table->current_scope > 0)
    {
        // Remove symbols declared inside the exiting scope
        int new_count = 0;

        for (int i = 0; i < table->count; i++)
        {
            if (table->symbols[i].scope_level < table->current_scope)
            {
                table->symbols[new_count++] = table->symbols[i];
            }
        }

        table->count = new_count;
        table->current_scope--;
    }
}

// Add a variable to the symbol table
int symbol_table_add(
    SymbolTable *table,
    const char *name,
    const char *type,
    int is_initialized)
{
    if (table->count >= MAX_SYMBOLS)
    {
        printf("Error: Symbol table is full!\n");
        return 0;
    }

    // Check for redeclaration in the same scope
    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->symbols[i].name, name) == 0 &&
            table->symbols[i].scope_level == table->current_scope)
        {
            printf("Semantic Error: Variable '%s' already declared in this scope.\n", name);
            return 0;
        }
    }

    Symbol *symbol = &table->symbols[table->count];

    strncpy(symbol->name, name, MAX_NAME_LENGTH - 1);
    symbol->name[MAX_NAME_LENGTH - 1] = '\0';

    strncpy(symbol->type, type, MAX_TYPE_LENGTH - 1);
    symbol->type[MAX_TYPE_LENGTH - 1] = '\0';

    symbol->scope_level = table->current_scope;
    symbol->is_initialized = is_initialized;

    table->count++;

    return 1;
}

// Search for a variable
Symbol *symbol_table_lookup(SymbolTable *table, const char *name)
{
    // Search from the most recently declared symbol
    for (int i = table->count - 1; i >= 0; i--)
    {
        if (strcmp(table->symbols[i].name, name) == 0)
        {
            return &table->symbols[i];
        }
    }

    return NULL;
}

// Display all symbols
void symbol_table_print(SymbolTable *table)
{
    printf("\n========== SYMBOL TABLE ==========\n");
    printf("%-20s %-15s %-15s %-15s\n",
           "Name", "Type", "Scope", "Initialized");

    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < table->count; i++)
    {
        printf("%-20s %-15s %-15d %-15s\n",
               table->symbols[i].name,
               table->symbols[i].type,
               table->symbols[i].scope_level,
               table->symbols[i].is_initialized ? "Yes" : "No");
    }

    printf("==================================\n");
}
