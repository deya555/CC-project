
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"

static char *copy_text(const char *text)
{
    char *copy = malloc(strlen(text) + 1);

    if (copy == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(copy, text);
    return copy;
}

static Scope *create_scope(int id, int level, Scope *parent)
{
    Scope *scope = malloc(sizeof(Scope));

    if (scope == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    scope->scope_id = id;
    scope->level = level;
    scope->parent = parent;
    scope->symbols = NULL;
    scope->next = NULL;

    return scope;
}

void symbol_table_init(SymbolTable *table)
{
    Scope *global_scope;

    table->all_scopes = NULL;
    table->next_scope_id = 0;

    global_scope = create_scope(
        table->next_scope_id++,
        0,
        NULL
    );

    table->current_scope = global_scope;
    table->all_scopes = global_scope;
}

void enter_scope(SymbolTable *table)
{
    Scope *new_scope;
    Scope *last;

    new_scope = create_scope(
        table->next_scope_id++,
        table->current_scope->level + 1,
        table->current_scope
    );

    last = table->all_scopes;

    while (last->next != NULL) {
        last = last->next;
    }

    last->next = new_scope;
    table->current_scope = new_scope;
}

void exit_scope(SymbolTable *table)
{
    if (table->current_scope != NULL &&
        table->current_scope->parent != NULL) {

        table->current_scope =
            table->current_scope->parent;
    }
}

int insert_symbol(
    SymbolTable *table,
    const char *name,
    const char *type,
    int line
)
{
    Symbol *current;
    Symbol *new_symbol;

    current = table->current_scope->symbols;

    while (current != NULL) {

        if (strcmp(current->name, name) == 0) {
            return 0;
        }

        current = current->next;
    }

    new_symbol = malloc(sizeof(Symbol));

    if (new_symbol == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    new_symbol->name = copy_text(name);
    new_symbol->type = copy_text(type);
    new_symbol->declaration_line = line;

    new_symbol->next =
        table->current_scope->symbols;

    table->current_scope->symbols = new_symbol;

    return 1;
}

Symbol *lookup_symbol(
    SymbolTable *table,
    const char *name
)
{
    Scope *scope;
    Symbol *current;

    scope = table->current_scope;

    while (scope != NULL) {

        current = scope->symbols;

        while (current != NULL) {

            if (strcmp(current->name, name) == 0) {
                return current;
            }

            current = current->next;
        }

        scope = scope->parent;
    }

    return NULL;
}

void print_symbol_table(SymbolTable *table)
{
    Scope *scope;
    Symbol *current;

    printf("\n========== SYMBOL TABLE ==========\n");

    scope = table->all_scopes;

    while (scope != NULL) {

        printf(
            "\nScope ID: %d | Level: %d\n",
            scope->scope_id,
            scope->level
        );

        printf("%-15s %-10s %-10s\n",
               "Name", "Type", "Line");

        printf("-----------------------------------\n");

        current = scope->symbols;

        while (current != NULL) {

            printf(
                "%-15s %-10s %-10d\n",
                current->name,
                current->type,
                current->declaration_line
            );

            current = current->next;
        }

        scope = scope->next;
    }

    printf("\n===================================\n");
}

void free_symbol_table(SymbolTable *table)
{
    Scope *scope;
    Scope *next_scope;

    scope = table->all_scopes;

    while (scope != NULL) {

        Symbol *current = scope->symbols;

        while (current != NULL) {

            Symbol *next_symbol = current->next;

            free(current->name);
            free(current->type);
            free(current);

            current = next_symbol;
        }

        next_scope = scope->next;

        free(scope);

        scope = next_scope;
    }

    table->current_scope = NULL;
    table->all_scopes = NULL;
}