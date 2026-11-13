
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

/* Initialize the symbol table */
void initSymbolTable(SymbolTable *table)
{
    table->head = NULL;
    table->current_scope = 0;
}

/* Enter a new scope */
void enterScope(SymbolTable *table)
{
    table->current_scope++;
}

/* Exit the current scope */
void exitScope(SymbolTable *table)
{
    Symbol *current = table->head;
    Symbol *previous = NULL;

    while (current != NULL)
    {
        if (current->scope_level == table->current_scope)
        {
            Symbol *temp = current;

            if (previous == NULL)
                table->head = current->next;
            else
                previous->next = current->next;

            current = current->next;

            free(temp->name);
            free(temp);
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }

    if (table->current_scope > 0)
        table->current_scope--;
}

/* Insert a variable into the symbol table */
int insertSymbol(SymbolTable *table,
                 const char *name,
                 DataType type,
                 int line_number)
{
    Symbol *current = table->head;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0 &&
            current->scope_level == table->current_scope)
        {
            return 0; /* Already declared in this scope */
        }

        current = current->next;
    }

    Symbol *newSymbol = malloc(sizeof(Symbol));

    if (newSymbol == NULL)
        return 0;

    newSymbol->name = malloc(strlen(name) + 1);

    if (newSymbol->name == NULL)
    {
        free(newSymbol);
        return 0;
    }

    strcpy(newSymbol->name, name);

    newSymbol->type = type;
    newSymbol->scope_level = table->current_scope;
    newSymbol->line_number = line_number;

    newSymbol->next = table->head;
    table->head = newSymbol;

    return 1;
}

/* Find a variable */
Symbol *lookupSymbol(SymbolTable *table, const char *name)
{
    Symbol *current = table->head;

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
            return current;

        current = current->next;
    }

    return NULL;
}

/* Display all variables */
void printSymbolTable(SymbolTable *table)
{
    Symbol *current = table->head;

    printf("\nSYMBOL TABLE\n");
    printf("----------------------------------------\n");
    printf("Name\tType\tScope\tLine\n");
    printf("----------------------------------------\n");

    while (current != NULL)
    {
        char *typeName;

        if (current->type == TYPE_INT)
            typeName = "int";
        else if (current->type == TYPE_FLOAT)
            typeName = "float";
        else
            typeName = "bool";

        printf("%s\t%s\t%d\t%d\n",
               current->name,
               typeName,
               current->scope_level,
               current->line_number);

        current = current->next;
    }
}

/* Free symbol table memory */
void freeSymbolTable(SymbolTable *table)
{
    Symbol *current = table->head;

    while (current != NULL)
    {
        Symbol *temp = current;
        current = current->next;

        free(temp->name);
        free(temp);
    }

    table->head = NULL;
}
