
#include <stdio.h>

#include "../symbol_table/symbol_table.h"

int main(void)
{
    static SymbolTable table;

    printf("========== SYMBOL TABLE TEST ==========\n\n");

    /* Initialize symbol table */
    initSymbolTable(&table);

    /* Insert global variables */
    printf("Inserting global variables...\n");

    printf("number: %s\n",
           insertSymbol(&table, "number", "int", 1)
               ? "Success" : "Failed");

    printf("result: %s\n",
           insertSymbol(&table, "result", "float", 0)
               ? "Success" : "Failed");

    /* Check duplicate declaration */
    printf("\nTesting duplicate declaration...\n");

    printf("Duplicate number: %s\n",
           insertSymbol(&table, "number", "int", 1)
               ? "Unexpected Success" : "Correctly Rejected");

    /* Enter a new scope */
    printf("\nEntering inner scope...\n");

    enterScope(&table);

    insertSymbol(&table, "number", "double", 1);
    insertSymbol(&table, "flag", "boolean", 1);

    /* Lookup variable */
    printf("\nTesting variable lookup...\n");

    Symbol *symbol = lookupSymbol(&table, "number");

    if (symbol != NULL) {
        printf("Found number: type = %s, scope = %d\n",
               symbol->dataType,
               symbol->scopeLevel);
    } else {
        printf("Variable not found!\n");
    }

    /* Exit inner scope */
    printf("\nExiting inner scope...\n");

    exitScope(&table);

    /* Check inner-scope variable removal */
    printf("Looking for flag after scope exit: ");

    if (lookupSymbol(&table, "flag") == NULL)
        printf("Correctly Removed\n");
    else
        printf("Error: Still Exists\n");

    /* Update initialization */
    printf("\nUpdating result initialization...\n");

    updateSymbolInitialization(&table, "result");

    /* Print final table */
    printSymbolTable(&table);

    printf("\n========== TEST COMPLETED ==========\n");

    return 0;
}
