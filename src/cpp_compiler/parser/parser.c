
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- AST MEMORY ---------- */

static CPPASTNode *create_node(
    CPPASTNodeType type,
    const char *value
)
{
    CPPASTNode *node = calloc(1, sizeof(CPPASTNode));

    if (node == NULL) {
        fprintf(stderr, "Parser error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    node->type = type;
    node->operator_type = CPP_TOKEN_UNKNOWN;

    if (value != NULL) {
        snprintf(node->value, sizeof(node->value), "%s", value);
    }

    return node;
}

static void append_node(CPPASTNode **head, CPPASTNode *node)
{
    if (node == NULL)
        return;

    if (*head == NULL) {
        *head = node;
        return;
    }

    CPPASTNode *current = *head;

    while (current->next != NULL)
        current = current->next;

    current->next = node;
}

/* ---------- PARSER TOKEN HANDLING ---------- */

static void advance_token(CPPParser *parser)
{
    parser->current_token =
        cpp_lexer_next_token(&parser->lexer);
}

static int check_token(
    CPPParser *parser,
    CPPTokenType type
)
{
    return parser->current_token.type == type;
}

static int match_token(
    CPPParser *parser,
    CPPTokenType type
)
{
    if (!check_token(parser, type))
        return 0;

    advance_token(parser);
    return 1;
}

static void parser_error(CPPParser *parser, const char *message)
{
    fprintf(
        stderr,
        "Syntax Error at line %d, column %d: %s (found '%s')\n",
        parser->current_token.line,
        parser->current_token.column,
        message,
        parser->current_token.lexeme
    );

    parser->had_error = 1;
    parser->error_count++;
}

static int expect_token(
    CPPParser *parser,
    CPPTokenType type,
    const char *message
)
{
    if (match_token(parser, type))
        return 1;

    parser_error(parser, message);
    return 0;
}

static int is_type_token(CPPTokenType type)
{
    return type == CPP_TOKEN_INT ||
           type == CPP_TOKEN_FLOAT ||
           type == CPP_TOKEN_DOUBLE ||
           type == CPP_TOKEN_CHAR ||
           type == CPP_TOKEN_BOOL ||
           type == CPP_TOKEN_VOID;
}

/* ---------- FORWARD DECLARATIONS ---------- */

static CPPASTNode *parse_statement(CPPParser *parser);
static CPPASTNode *parse_expression(CPPParser *parser);
static CPPASTNode *parse_block(CPPParser *parser);
static CPPASTNode *parse_declaration(CPPParser *parser);

/* ---------- EXPRESSIONS ---------- */

static CPPASTNode *parse_primary(CPPParser *parser)
{
    if (check_token(parser, CPP_TOKEN_INTEGER_LITERAL) ||
        check_token(parser, CPP_TOKEN_FLOAT_LITERAL) ||
        check_token(parser, CPP_TOKEN_STRING_LITERAL) ||
        check_token(parser, CPP_TOKEN_CHAR_LITERAL) ||
        check_token(parser, CPP_TOKEN_TRUE) ||
        check_token(parser, CPP_TOKEN_FALSE)) {

        CPPASTNode *node = create_node(
            AST_LITERAL,
            parser->current_token.lexeme
        );

        advance_token(parser);
        return node;
    }

    if (match_token(parser, CPP_TOKEN_LEFT_PAREN)) {
        CPPASTNode *node = parse_expression(parser);

        expect_token(
            parser,
            CPP_TOKEN_RIGHT_PAREN,
            "Expected ')' after expression"
        );

        return node;
    }

    if (check_token(parser, CPP_TOKEN_IDENTIFIER)) {

        char name[CPP_MAX_LEXEME];

        snprintf(
            name,
            sizeof(name),
            "%s",
            parser->current_token.lexeme
        );

        advance_token(parser);

        CPPASTNode *node = create_node(AST_IDENTIFIER, name);

        if (match_token(parser, CPP_TOKEN_LEFT_PAREN)) {

            node->type = AST_FUNCTION_CALL;

            CPPASTNode *arguments = NULL;

            if (!check_token(parser, CPP_TOKEN_RIGHT_PAREN)) {
                do {
                    CPPASTNode *argument =
                        parse_expression(parser);

                    if (argument == NULL)
                        break;

                    append_node(&arguments, argument);

                } while (match_token(parser, CPP_TOKEN_COMMA));
            }

            node->left = arguments;

            expect_token(
                parser,
                CPP_TOKEN_RIGHT_PAREN,
                "Expected ')' after function arguments"
            );
        }

        return node;
    }

    parser_error(parser, "Expected an expression");

    if (!check_token(parser, CPP_TOKEN_EOF))
        advance_token(parser);

    return NULL;
}

static CPPASTNode *parse_unary(CPPParser *parser)
{
    if (check_token(parser, CPP_TOKEN_MINUS) ||
        check_token(parser, CPP_TOKEN_PLUS) ||
        check_token(parser, CPP_TOKEN_LOGICAL_NOT) ||
        check_token(parser, CPP_TOKEN_BITWISE_NOT) ||
        check_token(parser, CPP_TOKEN_INCREMENT) ||
        check_token(parser, CPP_TOKEN_DECREMENT)) {

        CPPTokenType op = parser->current_token.type;

        char operator_name[CPP_MAX_LEXEME];

        snprintf(
            operator_name,
            sizeof(operator_name),
            "%s",
            parser->current_token.lexeme
        );

        advance_token(parser);

        CPPASTNode *node = create_node(
            AST_UNARY_EXPRESSION,
            operator_name
        );

        node->operator_type = op;
        node->right = parse_unary(parser);

        return node;
    }

    return parse_primary(parser);
}

static CPPASTNode *make_binary(
    CPPASTNode *left,
    CPPTokenType op,
    const char *operator_name,
    CPPASTNode *right
)
{
    CPPASTNode *node = create_node(
        AST_BINARY_EXPRESSION,
        operator_name
    );

    node->operator_type = op;
    node->left = left;
    node->right = right;

    return node;
}

static CPPASTNode *parse_multiplicative(CPPParser *parser)
{
    CPPASTNode *left = parse_unary(parser);

    while (check_token(parser, CPP_TOKEN_MULTIPLY) ||
           check_token(parser, CPP_TOKEN_DIVIDE) ||
           check_token(parser, CPP_TOKEN_MODULO)) {

        CPPTokenType op = parser->current_token.type;

        char name[CPP_MAX_LEXEME];

        snprintf(name, sizeof(name), "%s",
                 parser->current_token.lexeme);

        advance_token(parser);

        CPPASTNode *right = parse_unary(parser);

        left = make_binary(left, op, name, right);
    }

    return left;
}

static CPPASTNode *parse_additive(CPPParser *parser)
{
    CPPASTNode *left = parse_multiplicative(parser);

    while (check_token(parser, CPP_TOKEN_PLUS) ||
           check_token(parser, CPP_TOKEN_MINUS)) {

        CPPTokenType op = parser->current_token.type;

        char name[CPP_MAX_LEXEME];

        snprintf(name, sizeof(name), "%s",
                 parser->current_token.lexeme);

        advance_token(parser);

        CPPASTNode *right = parse_multiplicative(parser);

        left = make_binary(left, op, name, right);
    }

    return left;
}

static CPPASTNode *parse_relational(CPPParser *parser)
{
    CPPASTNode *left = parse_additive(parser);

    while (check_token(parser, CPP_TOKEN_LESS) ||
           check_token(parser, CPP_TOKEN_GREATER) ||
           check_token(parser, CPP_TOKEN_LESS_EQUAL) ||
           check_token(parser, CPP_TOKEN_GREATER_EQUAL)) {

        CPPTokenType op = parser->current_token.type;

        char name[CPP_MAX_LEXEME];

        snprintf(name, sizeof(name), "%s",
                 parser->current_token.lexeme);

        advance_token(parser);

        CPPASTNode *right = parse_additive(parser);

        left = make_binary(left, op, name, right);
    }

    return left;
}

static CPPASTNode *parse_equality(CPPParser *parser)
{
    CPPASTNode *left = parse_relational(parser);

    while (check_token(parser, CPP_TOKEN_EQUAL) ||
           check_token(parser, CPP_TOKEN_NOT_EQUAL)) {

        CPPTokenType op = parser->current_token.type;

        char name[CPP_MAX_LEXEME];

        snprintf(name, sizeof(name), "%s",
                 parser->current_token.lexeme);

        advance_token(parser);

        CPPASTNode *right = parse_relational(parser);

        left = make_binary(left, op, name, right);
    }

    return left;
}

static CPPASTNode *parse_logical_and(CPPParser *parser)
{
    CPPASTNode *left = parse_equality(parser);

    while (match_token(parser, CPP_TOKEN_LOGICAL_AND)) {

        CPPASTNode *right = parse_equality(parser);

        left = make_binary(
            left,
            CPP_TOKEN_LOGICAL_AND,
            "&&",
            right
        );
    }

    return left;
}

static CPPASTNode *parse_logical_or(CPPParser *parser)
{
    CPPASTNode *left = parse_logical_and(parser);

    while (match_token(parser, CPP_TOKEN_LOGICAL_OR)) {

        CPPASTNode *right = parse_logical_and(parser);

        left = make_binary(
            left,
            CPP_TOKEN_LOGICAL_OR,
            "||",
            right
        );
    }

    return left;
}

static CPPASTNode *parse_assignment(CPPParser *parser)
{
    CPPASTNode *left = parse_logical_or(parser);

    if (check_token(parser, CPP_TOKEN_ASSIGN) ||
        check_token(parser, CPP_TOKEN_PLUS_ASSIGN) ||
        check_token(parser, CPP_TOKEN_MINUS_ASSIGN) ||
        check_token(parser, CPP_TOKEN_MULTIPLY_ASSIGN) ||
        check_token(parser, CPP_TOKEN_DIVIDE_ASSIGN) ||
        check_token(parser, CPP_TOKEN_MODULO_ASSIGN)) {

        CPPTokenType op = parser->current_token.type;

        char name[CPP_MAX_LEXEME];

        snprintf(name, sizeof(name), "%s",
                 parser->current_token.lexeme);

        advance_token(parser);

        CPPASTNode *right = parse_assignment(parser);

        CPPASTNode *node = create_node(
            AST_ASSIGNMENT,
            name
        );

        node->operator_type = op;
        node->left = left;
        node->right = right;

        return node;
    }

    return left;
}

static CPPASTNode *parse_expression(CPPParser *parser)
{
    return parse_assignment(parser);
}

/* ---------- DECLARATIONS ---------- */

static CPPASTNode *parse_variable_declaration(CPPParser *parser)
{
    char type_name[CPP_MAX_LEXEME];

    snprintf(
        type_name,
        sizeof(type_name),
        "%s",
        parser->current_token.lexeme
    );

    advance_token(parser);

    if (!check_token(parser, CPP_TOKEN_IDENTIFIER)) {
        parser_error(parser, "Expected variable name");
        return NULL;
    }

    CPPASTNode *node = create_node(
        AST_VARIABLE_DECLARATION,
        type_name
    );

    node->left = create_node(
        AST_IDENTIFIER,
        parser->current_token.lexeme
    );

    advance_token(parser);

    if (match_token(parser, CPP_TOKEN_ASSIGN)) {
        node->right = parse_expression(parser);
    }

    if (!expect_token(
            parser,
            CPP_TOKEN_SEMICOLON,
            "Expected ';' after variable declaration")) {
        return node;
    }

    return node;
}

static CPPASTNode *parse_declaration(CPPParser *parser)
{
    char type_name[CPP_MAX_LEXEME];

    snprintf(
        type_name,
        sizeof(type_name),
        "%s",
        parser->current_token.lexeme
    );

    advance_token(parser);

    if (!check_token(parser, CPP_TOKEN_IDENTIFIER)) {
        parser_error(parser, "Expected identifier after type");
        return NULL;
    }

    char name[CPP_MAX_LEXEME];

    snprintf(
        name,
        sizeof(name),
        "%s",
        parser->current_token.lexeme
    );

    advance_token(parser);

    /* Function declaration */
    if (match_token(parser, CPP_TOKEN_LEFT_PAREN)) {

        CPPASTNode *function = create_node(
            AST_FUNCTION_DECLARATION,
            name
        );

        function->left = create_node(
            AST_IDENTIFIER,
            type_name
        );

        CPPASTNode *parameters = NULL;

        if (!check_token(parser, CPP_TOKEN_RIGHT_PAREN)) {

            do {
                if (!is_type_token(parser->current_token.type)) {
                    parser_error(parser, "Expected parameter type");
                    break;
                }

                char parameter_type[CPP_MAX_LEXEME];

                snprintf(
                    parameter_type,
                    sizeof(parameter_type),
                    "%s",
                    parser->current_token.lexeme
                );

                advance_token(parser);

                if (!check_token(parser, CPP_TOKEN_IDENTIFIER)) {
                    parser_error(parser, "Expected parameter name");
                    break;
                }

                CPPASTNode *parameter = create_node(
                    AST_PARAMETER,
                    parameter_type
                );

                parameter->left = create_node(
                    AST_IDENTIFIER,
                    parser->current_token.lexeme
                );

                advance_token(parser);

                append_node(&parameters, parameter);

            } while (match_token(parser, CPP_TOKEN_COMMA));
        }

        function->right = parameters;

        expect_token(
            parser,
            CPP_TOKEN_RIGHT_PAREN,
            "Expected ')' after function parameters"
        );

        if (check_token(parser, CPP_TOKEN_LEFT_BRACE)) {
            function->third = parse_block(parser);
        } else {
            expect_token(
                parser,
                CPP_TOKEN_SEMICOLON,
                "Expected function body or ';'"
            );
        }

        return function;
    }

    /* Variable declaration */
    CPPASTNode *node = create_node(
        AST_VARIABLE_DECLARATION,
        type_name
    );

    node->left = create_node(AST_IDENTIFIER, name);

    if (match_token(parser, CPP_TOKEN_ASSIGN)) {
        node->right = parse_expression(parser);
    }

    expect_token(
        parser,
        CPP_TOKEN_SEMICOLON,
        "Expected ';' after variable declaration"
    );

    return node;
}

/* ---------- STATEMENTS ---------- */

static CPPASTNode *parse_if(CPPParser *parser)
{
    advance_token(parser);

    CPPASTNode *node = create_node(AST_IF, "if");

    expect_token(
        parser,
        CPP_TOKEN_LEFT_PAREN,
        "Expected '(' after if"
    );

    node->left = parse_expression(parser);

    expect_token(
        parser,
        CPP_TOKEN_RIGHT_PAREN,
        "Expected ')' after if condition"
    );

    node->right = parse_statement(parser);

    if (match_token(parser, CPP_TOKEN_ELSE)) {
        node->third = parse_statement(parser);
    }

    return node;
}

static CPPASTNode *parse_while(CPPParser *parser)
{
    advance_token(parser);

    CPPASTNode *node = create_node(AST_WHILE, "while");

    expect_token(
        parser,
        CPP_TOKEN_LEFT_PAREN,
        "Expected '(' after while"
    );

    node->left = parse_expression(parser);

    expect_token(
        parser,
        CPP_TOKEN_RIGHT_PAREN,
        "Expected ')' after while condition"
    );

    node->right = parse_statement(parser);

    return node;
}

static CPPASTNode *parse_for(CPPParser *parser)
{
    advance_token(parser);

    CPPASTNode *node = create_node(AST_FOR, "for");

    expect_token(
        parser,
        CPP_TOKEN_LEFT_PAREN,
        "Expected '(' after for"
    );

    if (!check_token(parser, CPP_TOKEN_SEMICOLON)) {
        if (is_type_token(parser->current_token.type))
            node->left = parse_variable_declaration(parser);
        else
            node->left = parse_expression(parser);
    } else {
        advance_token(parser);
    }

    if (!check_token(parser, CPP_TOKEN_SEMICOLON))
        node->right = parse_expression(parser);

    expect_token(
        parser,
        CPP_TOKEN_SEMICOLON,
        "Expected ';' after for condition"
    );

    if (!check_token(parser, CPP_TOKEN_RIGHT_PAREN))
        node->third = parse_expression(parser);

    expect_token(
        parser,
        CPP_TOKEN_RIGHT_PAREN,
        "Expected ')' after for clauses"
    );

    /* The loop body is stored after the third child. */
    CPPASTNode *body = parse_statement(parser);

    if (node->third == NULL) {
        node->third = body;
    } else {
        node->third->next = body;
    }

    return node;
}

static CPPASTNode *parse_return(CPPParser *parser)
{
    advance_token(parser);

    CPPASTNode *node = create_node(AST_RETURN, "return");

    if (!check_token(parser, CPP_TOKEN_SEMICOLON))
        node->left = parse_expression(parser);

    expect_token(
        parser,
        CPP_TOKEN_SEMICOLON,
        "Expected ';' after return"
    );

    return node;
}

static CPPASTNode *parse_block(CPPParser *parser)
{
    expect_token(
        parser,
        CPP_TOKEN_LEFT_BRACE,
        "Expected '{'"
    );

    CPPASTNode *block = create_node(AST_BLOCK, "block");

    while (!check_token(parser, CPP_TOKEN_RIGHT_BRACE) &&
           !check_token(parser, CPP_TOKEN_EOF)) {

        int previous_error_count = parser->error_count;

        CPPASTNode *statement = parse_statement(parser);

        append_node(&block->left, statement);

        /* Prevent an infinite loop after malformed input. */
        if (parser->error_count > previous_error_count &&
            !check_token(parser, CPP_TOKEN_EOF)) {
            advance_token(parser);
        }
    }

    expect_token(
        parser,
        CPP_TOKEN_RIGHT_BRACE,
        "Expected '}' after block"
    );

    return block;
}

static CPPASTNode *parse_statement(CPPParser *parser)
{
    if (check_token(parser, CPP_TOKEN_LEFT_BRACE))
        return parse_block(parser);

    if (check_token(parser, CPP_TOKEN_IF))
        return parse_if(parser);

    if (check_token(parser, CPP_TOKEN_WHILE))
        return parse_while(parser);

    if (check_token(parser, CPP_TOKEN_FOR))
        return parse_for(parser);

    if (check_token(parser, CPP_TOKEN_RETURN))
        return parse_return(parser);

    if (check_token(parser, CPP_TOKEN_BREAK)) {
        advance_token(parser);

        CPPASTNode *node = create_node(AST_BREAK, "break");

        expect_token(
            parser,
            CPP_TOKEN_SEMICOLON,
            "Expected ';' after break"
        );

        return node;
    }

    if (check_token(parser, CPP_TOKEN_CONTINUE)) {
        advance_token(parser);

        CPPASTNode *node = create_node(AST_CONTINUE, "continue");

        expect_token(
            parser,
            CPP_TOKEN_SEMICOLON,
            "Expected ';' after continue"
        );

        return node;
    }

    if (is_type_token(parser->current_token.type))
        return parse_declaration(parser);

    if (match_token(parser, CPP_TOKEN_SEMICOLON))
        return create_node(AST_EMPTY, "empty");

    CPPASTNode *node = create_node(
        AST_EXPRESSION_STATEMENT,
        "expression"
    );

    node->left = parse_expression(parser);

    expect_token(
        parser,
        CPP_TOKEN_SEMICOLON,
        "Expected ';' after expression"
    );

    return node;
}

/* ---------- PROGRAM ---------- */

void cpp_parser_init(CPPParser *parser, const char *source)
{
    if (parser == NULL)
        return;

    memset(parser, 0, sizeof(*parser));

    cpp_lexer_init(&parser->lexer, source);
    advance_token(parser);
}

CPPASTNode *cpp_parse_program(CPPParser *parser)
{
    if (parser == NULL)
        return NULL;

    CPPASTNode *program = create_node(AST_PROGRAM, "program");

    while (!check_token(parser, CPP_TOKEN_EOF)) {

        int previous_position = parser->lexer.position;

        int previous_errors = parser->error_count;

        CPPASTNode *statement = parse_statement(parser);

        append_node(&program->left, statement);

        if (parser->lexer.position == previous_position &&
            parser->error_count == previous_errors) {
            parser_error(parser, "Parser could not advance");
            advance_token(parser);
        }
    }

    return program;
}

int cpp_parser_had_error(CPPParser *parser)
{
    return parser == NULL ? 1 : parser->had_error;
}

/* ---------- AST PRINTING ---------- */

const char *cpp_ast_node_name(CPPASTNodeType type)
{
    switch (type) {
        case AST_PROGRAM: return "PROGRAM";
        case AST_BLOCK: return "BLOCK";
        case AST_VARIABLE_DECLARATION: return "VARIABLE_DECLARATION";
        case AST_FUNCTION_DECLARATION: return "FUNCTION_DECLARATION";
        case AST_PARAMETER: return "PARAMETER";
        case AST_EXPRESSION_STATEMENT: return "EXPRESSION_STATEMENT";
        case AST_ASSIGNMENT: return "ASSIGNMENT";
        case AST_IF: return "IF";
        case AST_WHILE: return "WHILE";
        case AST_FOR: return "FOR";
        case AST_RETURN: return "RETURN";
        case AST_BREAK: return "BREAK";
        case AST_CONTINUE: return "CONTINUE";
        case AST_BINARY_EXPRESSION: return "BINARY_EXPRESSION";
        case AST_UNARY_EXPRESSION: return "UNARY_EXPRESSION";
        case AST_LITERAL: return "LITERAL";
        case AST_IDENTIFIER: return "IDENTIFIER";
        case AST_FUNCTION_CALL: return "FUNCTION_CALL";
        case AST_EMPTY: return "EMPTY";
        default: return "UNKNOWN_AST_NODE";
    }
}

static void print_tree(
    CPPASTNode *node,
    int indent,
    const char *label
)
{
    if (node == NULL)
        return;

    for (int i = 0; i < indent; i++)
        printf("  ");

    printf("%s%s", label, cpp_ast_node_name(node->type));

    if (node->value[0] != '\0')
        printf(" (%s)", node->value);

    printf("\n");

    if (node->left != NULL)
        print_tree(node->left, indent + 1, "LEFT: ");

    if (node->right != NULL)
        print_tree(node->right, indent + 1, "RIGHT: ");

    if (node->third != NULL)
        print_tree(node->third, indent + 1, "THIRD: ");

    if (node->next != NULL)
        print_tree(node->next, indent, "NEXT: ");
}

void cpp_ast_print(CPPASTNode *node, int indent)
{
    if (node == NULL)
        return;

    print_tree(node, indent, "");
}

/* ---------- AST MEMORY FREE ---------- */

void cpp_ast_free(CPPASTNode *node)
{
    if (node == NULL)
        return;

    cpp_ast_free(node->left);
    cpp_ast_free(node->right);
    cpp_ast_free(node->third);
    cpp_ast_free(node->next);

    free(node);
}
