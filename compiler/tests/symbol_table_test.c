
#include <stdio.h>
#include "../src/symbol_table/symbol_table.h"

int main(void) {
    SymbolTable table;
    Symbol *found;

    symbol_table_init(&table);

    printf("Testing global scope...\n");

    insert_symbol(&table, "count", "int", 1);
    insert_symbol(&table, "rate", "float", 2);
    insert_symbol(&table, "active", "bool", 3);

    // Declare value in the global scope.
    insert_symbol(&table, "value", "float", 4);

    // Same-scope duplicate declaration must be rejected.
    if (!insert_symbol(&table, "count", "int", 5)) {
        printf("PASS: Duplicate declaration rejected.\n");
    } else {
        printf("FAIL: Duplicate declaration accepted.\n");
    }

    printf("\nTesting nested scope...\n");

    enter_scope(&table);

    // Same variable name is allowed in a different scope.
    if (insert_symbol(&table, "value", "int", 6)) {
        printf("PASS: Same name allowed in nested scope.\n");
    } else {
        printf("FAIL: Same name rejected in nested scope.\n");
    }

    // Lookup should find the nested variable first.
    found = lookup_symbol(&table, "value");

    if (found != NULL && found->type[0] == 'i') {
        printf("PASS: Nested scope variable found.\n");
    } else {
        printf("FAIL: Nested scope variable not found.\n");
    }

    exit_scope(&table);

    // After leaving nested scope, lookup should find global value.
    found = lookup_symbol(&table, "value");

    if (found != NULL && found->type[0] == 'f') {
        printf("PASS: Global scope variable found after exit.\n");
    } else {
        printf("FAIL: Global scope variable not found after exit.\n");
    }

    print_symbol_table(&table);

    free_symbol_table(&table);

    printf("\nSymbol table test completed.\n");

    return 0;
}