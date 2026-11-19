
#include <stdio.h>
#include <string.h>

#include "symbol_table.h"

void initSymbolTable(SymbolTable *table)
{
    if (table == NULL)
        return;

    table->count = 0;
    table->currentScope = 0;
}

int enterScope(SymbolTable *table)
{
    if (table == NULL)
        return 0;

    table->currentScope++;
    return 1;
}

int exitScope(SymbolTable *table)
{
    if (table == NULL || table->currentScope <= 0)
        return 0;

    int writeIndex = 0;

    for (int i = 0; i < table->count; i++) {
        if (table->symbols[i].scopeLevel < table->currentScope) {
            if (writeIndex != i)
                table->symbols[writeIndex] = table->symbols[i];

            writeIndex++;
        }
    }

    table->count = writeIndex;
    table->currentScope--;

    return 1;
}

int insertSymbol(
    SymbolTable *table,
    const char *name,
    const char *dataType,
    int isInitialized
)
{
    if (table == NULL || name == NULL || dataType == NULL)
        return 0;

    if (table->count >= MAX_SYMBOLS)
        return 0;

    if (strlen(name) >= MAX_NAME_LENGTH ||
        strlen(dataType) >= MAX_TYPE_LENGTH)
        return 0;

    /* Prevent duplicate declarations in the same scope. */
    for (int i = table->count - 1; i >= 0; i--) {
        if (table->symbols[i].scopeLevel < table->currentScope)
            break;

        if (table->symbols[i].scopeLevel == table->currentScope &&
            strcmp(table->symbols[i].name, name) == 0) {
            return 0;
        }
    }

    Symbol *symbol = &table->symbols[table->count];

    strcpy(symbol->name, name);
    strcpy(symbol->dataType, dataType);

    symbol->scopeLevel = table->currentScope;
    symbol->isInitialized = isInitialized;

    table->count++;

    return 1;
}

Symbol *lookupSymbol(SymbolTable *table, const char *name)
{
    if (table == NULL || name == NULL)
        return NULL;

    /* Search from newest declaration to oldest. */
    for (int i = table->count - 1; i >= 0; i--) {
        if (strcmp(table->symbols[i].name, name) == 0)
            return &table->symbols[i];
    }

    return NULL;
}

int updateSymbolInitialization(SymbolTable *table, const char *name)
{
    Symbol *symbol = lookupSymbol(table, name);

    if (symbol == NULL)
        return 0;

    symbol->isInitialized = 1;
    return 1;
}

void printSymbolTable(const SymbolTable *table)
{
    if (table == NULL)
        return;

    printf("\n========== SYMBOL TABLE ==========\n");
    printf("%-20s %-15s %-10s %-15s\n",
           "NAME", "DATA TYPE", "SCOPE", "INITIALIZED");

    for (int i = 0; i < table->count; i++) {
        printf("%-20s %-15s %-10d %-15s\n",
               table->symbols[i].name,
               table->symbols[i].dataType,
               table->symbols[i].scopeLevel,
               table->symbols[i].isInitialized ? "Yes" : "No");
    }

    printf("==================================\n");
}
