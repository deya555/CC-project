
#ifndef CPP_SEMANTIC_H
#define CPP_SEMANTIC_H

#include "../parser/parser.h"

#define CPP_MAX_SYMBOLS 500
#define CPP_MAX_ERRORS 100

typedef enum {
    CPP_TYPE_INT,
    CPP_TYPE_FLOAT,
    CPP_TYPE_DOUBLE,
    CPP_TYPE_CHAR,
    CPP_TYPE_BOOL,
    CPP_TYPE_VOID,
    CPP_TYPE_STRING,
    CPP_TYPE_UNKNOWN
} CPPDataType;

typedef struct {
    char name[CPP_MAX_LEXEME];
    CPPDataType type;
    int initialized;
} CPPSymbol;

typedef struct {
    CPPSymbol symbols[CPP_MAX_SYMBOLS];
    int symbol_count;

    char errors[CPP_MAX_ERRORS][256];
    int error_count;

    int had_error;
} CPPSemanticAnalyzer;

/* Initialize semantic analyzer */
void cpp_semantic_init(CPPSemanticAnalyzer *analyzer);

/* Analyze an AST */
int cpp_semantic_analyze(
    CPPSemanticAnalyzer *analyzer,
    CPPASTNode *root
);

/* Print semantic errors */
void cpp_semantic_print_errors(CPPSemanticAnalyzer *analyzer);

/* Get readable data type name */
const char *cpp_data_type_name(CPPDataType type);

#endif
