
#ifndef CPP_PARSER_H
#define CPP_PARSER_H

#include "../lexer/lexer.h"

/* AST node types */
typedef enum {
    AST_PROGRAM,
    AST_BLOCK,

    /* Declarations */
    AST_VARIABLE_DECLARATION,
    AST_FUNCTION_DECLARATION,
    AST_PARAMETER,

    /* Statements */
    AST_EXPRESSION_STATEMENT,
    AST_ASSIGNMENT,
    AST_IF,
    AST_WHILE,
    AST_FOR,
    AST_RETURN,
    AST_BREAK,
    AST_CONTINUE,

    /* Expressions */
    AST_BINARY_EXPRESSION,
    AST_UNARY_EXPRESSION,
    AST_LITERAL,
    AST_IDENTIFIER,
    AST_FUNCTION_CALL,

    AST_EMPTY
} CPPASTNodeType;

/* Abstract Syntax Tree node */
typedef struct CPPASTNode {
    CPPASTNodeType type;

    char value[CPP_MAX_LEXEME];

    CPPTokenType operator_type;

    struct CPPASTNode *left;
    struct CPPASTNode *right;
    struct CPPASTNode *third;

    struct CPPASTNode *next;
} CPPASTNode;

/* Parser structure */
typedef struct {
    CPPLexer lexer;

    CPPToken current_token;

    int had_error;
    int error_count;
} CPPParser;

/* Initialize parser */
void cpp_parser_init(CPPParser *parser, const char *source);

/* Parse complete source code */
CPPASTNode *cpp_parse_program(CPPParser *parser);

/* Check parser errors */
int cpp_parser_had_error(CPPParser *parser);

/* Print AST */
void cpp_ast_print(CPPASTNode *node, int indent);

/* Free AST memory */
void cpp_ast_free(CPPASTNode *node);

/* Get AST node name */
const char *cpp_ast_node_name(CPPASTNodeType type);

#endif
