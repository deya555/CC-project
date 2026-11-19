
#include "parser.h"

#include <stdio.h>
#include <string.h>

static Token *currentToken(Parser *parser)
{
    if (parser->position >= parser->tokenCount) {
        return &parser->tokens[parser->tokenCount - 1];
    }

    return &parser->tokens[parser->position];
}

static int check(Parser *parser, TokenType type)
{
    return currentToken(parser)->type == type;
}

static Token *advance(Parser *parser)
{
    Token *token = currentToken(parser);

    if (token->type != TOKEN_EOF &&
        parser->position < parser->tokenCount - 1) {
        parser->position++;
    }

    return token;
}

static int match(Parser *parser, TokenType type)
{
    if (!check(parser, type)) {
        return 0;
    }

    advance(parser);
    return 1;
}

static int expect(Parser *parser, TokenType type, const char *message)
{
    if (match(parser, type)) {
        return 1;
    }

    fprintf(stderr,
            "Syntax Error at line %d, column %d: %s. Found '%s'.\n",
            currentToken(parser)->line,
            currentToken(parser)->column,
            message,
            currentToken(parser)->lexeme);

    parser->hasError = 1;
    return 0;
}

static ASTNode *newNode(ASTNodeType type,
                        const char *value,
                        const char *dataType)
{
    return createASTNode(type, value, dataType);
}

static int attach(ASTNode *parent, ASTNode *child)
{
    if (parent == NULL || child == NULL) {
        return 0;
    }

    if (!addASTChild(parent, child)) {
        freeAST(child);
        return 0;
    }

    return 1;
}

static ASTNode *parseExpression(Parser *parser);
static ASTNode *parseStatement(Parser *parser);
static ASTNode *parseBlock(Parser *parser);

/* ================= PRIMARY EXPRESSION ================= */

static ASTNode *parsePrimary(Parser *parser)
{
    Token *token = currentToken(parser);

    if (match(parser, TOKEN_INTEGER_LITERAL) ||
        match(parser, TOKEN_FLOAT_LITERAL) ||
        match(parser, TOKEN_STRING_LITERAL) ||
        match(parser, TOKEN_CHAR_LITERAL) ||
        match(parser, TOKEN_TRUE) ||
        match(parser, TOKEN_FALSE)) {

        return newNode(AST_LITERAL, token->lexeme, NULL);
    }

    if (match(parser, TOKEN_IDENTIFIER)) {
        return newNode(AST_IDENTIFIER, token->lexeme, NULL);
    }

    if (match(parser, TOKEN_LEFT_PAREN)) {
        ASTNode *expression = parseExpression(parser);

        if (expression == NULL) {
            return NULL;
        }

        if (!expect(parser, TOKEN_RIGHT_PAREN,
                    "Expected ')' after expression")) {
            freeAST(expression);
            return NULL;
        }

        return expression;
    }

    fprintf(stderr,
            "Syntax Error at line %d, column %d: Expected expression.\n",
            token->line, token->column);

    parser->hasError = 1;
    return NULL;
}

/* ================= UNARY EXPRESSION ================= */

static ASTNode *parseUnary(Parser *parser)
{
    if (check(parser, TOKEN_NOT) ||
        check(parser, TOKEN_MINUS) ||
        check(parser, TOKEN_PLUS)) {

        Token *operatorToken = advance(parser);

        ASTNode *node = newNode(AST_UNARY_EXPRESSION,
                                operatorToken->lexeme, NULL);

        if (node == NULL) {
            return NULL;
        }

        ASTNode *operand = parseUnary(parser);

        if (!attach(node, operand)) {
            freeAST(node);
            return NULL;
        }

        return node;
    }

    return parsePrimary(parser);
}

/* ================= MULTIPLICATION ================= */

static ASTNode *parseMultiplication(Parser *parser)
{
    ASTNode *left = parseUnary(parser);

    while (left != NULL &&
           (check(parser, TOKEN_MULTIPLY) ||
            check(parser, TOKEN_DIVIDE) ||
            check(parser, TOKEN_MODULO))) {

        Token *operatorToken = advance(parser);

        ASTNode *right = parseUnary(parser);

        ASTNode *node = newNode(AST_BINARY_EXPRESSION,
                                operatorToken->lexeme, NULL);

        if (node == NULL) {
            freeAST(left);
            freeAST(right);
            return NULL;
        }

        if (!attach(node, left) || !attach(node, right)) {
            freeAST(node);
            return NULL;
        }

        left = node;
    }

    return left;
}

/* ================= ADDITION ================= */

static ASTNode *parseAddition(Parser *parser)
{
    ASTNode *left = parseMultiplication(parser);

    while (left != NULL &&
           (check(parser, TOKEN_PLUS) ||
            check(parser, TOKEN_MINUS))) {

        Token *operatorToken = advance(parser);

        ASTNode *right = parseMultiplication(parser);

        ASTNode *node = newNode(AST_BINARY_EXPRESSION,
                                operatorToken->lexeme, NULL);

        if (node == NULL) {
            freeAST(left);
            freeAST(right);
            return NULL;
        }

        if (!attach(node, left) || !attach(node, right)) {
            freeAST(node);
            return NULL;
        }

        left = node;
    }

    return left;
}

/* ================= RELATIONAL ================= */

static ASTNode *parseRelational(Parser *parser)
{
    ASTNode *left = parseAddition(parser);

    while (left != NULL &&
           (check(parser, TOKEN_LESS) ||
            check(parser, TOKEN_GREATER) ||
            check(parser, TOKEN_LESS_EQUAL) ||
            check(parser, TOKEN_GREATER_EQUAL))) {

        Token *operatorToken = advance(parser);

        ASTNode *right = parseAddition(parser);

        ASTNode *node = newNode(AST_BINARY_EXPRESSION,
                                operatorToken->lexeme, NULL);

        if (node == NULL) {
            freeAST(left);
            freeAST(right);
            return NULL;
        }

        if (!attach(node, left) || !attach(node, right)) {
            freeAST(node);
            return NULL;
        }

        left = node;
    }

    return left;
}

/* ================= EQUALITY ================= */

static ASTNode *parseEquality(Parser *parser)
{
    ASTNode *left = parseRelational(parser);

    while (left != NULL &&
           (check(parser, TOKEN_EQUAL) ||
            check(parser, TOKEN_NOT_EQUAL))) {

        Token *operatorToken = advance(parser);

        ASTNode *right = parseRelational(parser);

        ASTNode *node = newNode(AST_BINARY_EXPRESSION,
                                operatorToken->lexeme, NULL);

        if (node == NULL) {
            freeAST(left);
            freeAST(right);
            return NULL;
        }

        if (!attach(node, left) || !attach(node, right)) {
            freeAST(node);
            return NULL;
        }

        left = node;
    }

    return left;
}

/* ================= LOGICAL AND ================= */

static ASTNode *parseLogicalAnd(Parser *parser)
{
    ASTNode *left = parseEquality(parser);

    while (left != NULL && check(parser, TOKEN_AND)) {
        Token *operatorToken = advance(parser);

        ASTNode *right = parseEquality(parser);

        ASTNode *node = newNode(AST_BINARY_EXPRESSION,
                                operatorToken->lexeme, NULL);

        if (node == NULL) {
            freeAST(left);
            freeAST(right);
            return NULL;
        }

        if (!attach(node, left) || !attach(node, right)) {
            freeAST(node);
            return NULL;
        }

        left = node;
    }

    return left;
}

/* ================= LOGICAL OR ================= */

static ASTNode *parseLogicalOr(Parser *parser)
{
    ASTNode *left = parseLogicalAnd(parser);

    while (left != NULL && check(parser, TOKEN_OR)) {
        Token *operatorToken = advance(parser);

        ASTNode *right = parseLogicalAnd(parser);

        ASTNode *node = newNode(AST_BINARY_EXPRESSION,
                                operatorToken->lexeme, NULL);

        if (node == NULL) {
            freeAST(left);
            freeAST(right);
            return NULL;
        }

        if (!attach(node, left) || !attach(node, right)) {
            freeAST(node);
            return NULL;
        }

        left = node;
    }

    return left;
}

static ASTNode *parseExpression(Parser *parser)
{
    return parseLogicalOr(parser);
}

/* ================= VARIABLE DECLARATION ================= */

static int isDataType(TokenType type)
{
    return type == TOKEN_INT ||
           type == TOKEN_FLOAT ||
           type == TOKEN_DOUBLE ||
           type == TOKEN_BOOLEAN ||
           type == TOKEN_STRING ||
           type == TOKEN_CHAR;
}

static ASTNode *parseVariableDeclaration(Parser *parser)
{
    Token *typeToken = advance(parser);

    if (!check(parser, TOKEN_IDENTIFIER)) {
        expect(parser, TOKEN_IDENTIFIER, "Expected variable name");
        return NULL;
    }

    Token *nameToken = advance(parser);

    ASTNode *node = newNode(AST_VARIABLE_DECLARATION,
                            nameToken->lexeme,
                            typeToken->lexeme);

    if (node == NULL) {
        return NULL;
    }

    if (match(parser, TOKEN_ASSIGN)) {
        ASTNode *initializer = parseExpression(parser);

        if (!attach(node, initializer)) {
            freeAST(node);
            return NULL;
        }
    }

    if (!expect(parser, TOKEN_SEMICOLON,
                "Expected ';' after declaration")) {
        freeAST(node);
        return NULL;
    }

    return node;
}

/* ================= ASSIGNMENT ================= */

static ASTNode *parseAssignment(Parser *parser)
{
    Token *nameToken = advance(parser);

    ASTNode *node = newNode(AST_ASSIGNMENT,
                            nameToken->lexeme, NULL);

    if (node == NULL) {
        return NULL;
    }

    if (!expect(parser, TOKEN_ASSIGN, "Expected '='")) {
        freeAST(node);
        return NULL;
    }

    ASTNode *expression = parseExpression(parser);

    if (!attach(node, expression)) {
        freeAST(node);
        return NULL;
    }

    if (!expect(parser, TOKEN_SEMICOLON,
                "Expected ';' after assignment")) {
        freeAST(node);
        return NULL;
    }

    return node;
}

/* ================= PRINT ================= */

static ASTNode *parsePrint(Parser *parser)
{
    advance(parser);

    ASTNode *node = newNode(AST_PRINT, "print", NULL);

    if (node == NULL) {
        return NULL;
    }

    if (!expect(parser, TOKEN_LEFT_PAREN,
                "Expected '(' after print")) {
        freeAST(node);
        return NULL;
    }

    ASTNode *expression = parseExpression(parser);

    if (!attach(node, expression)) {
        freeAST(node);
        return NULL;
    }

    if (!expect(parser, TOKEN_RIGHT_PAREN,
                "Expected ')' after print expression") ||
        !expect(parser, TOKEN_SEMICOLON,
                "Expected ';' after print statement")) {

        freeAST(node);
        return NULL;
    }

    return node;
}

/* ================= IF / ELSE ================= */

static ASTNode *parseIf(Parser *parser)
{
    advance(parser);

    ASTNode *node = newNode(AST_IF, "if", NULL);

    if (node == NULL) {
        return NULL;
    }

    if (!expect(parser, TOKEN_LEFT_PAREN,
                "Expected '(' after if")) {
        freeAST(node);
        return NULL;
    }

    ASTNode *condition = parseExpression(parser);

    if (!attach(node, condition)) {
        freeAST(node);
        return NULL;
    }

    if (!expect(parser, TOKEN_RIGHT_PAREN,
                "Expected ')' after if condition")) {
        freeAST(node);
        return NULL;
    }

    ASTNode *thenBranch = parseStatement(parser);

    if (!attach(node, thenBranch)) {
        freeAST(node);
        return NULL;
    }

    if (match(parser, TOKEN_ELSE)) {
        ASTNode *elseBranch = parseStatement(parser);

        if (!attach(node, elseBranch)) {
            freeAST(node);
            return NULL;
        }
    }

    return node;
}

/* ================= WHILE ================= */

static ASTNode *parseWhile(Parser *parser)
{
    advance(parser);

    ASTNode *node = newNode(AST_WHILE, "while", NULL);

    if (node == NULL) {
        return NULL;
    }

    if (!expect(parser, TOKEN_LEFT_PAREN,
                "Expected '(' after while")) {
        freeAST(node);
        return NULL;
    }

    ASTNode *condition = parseExpression(parser);

    if (!attach(node, condition)) {
        freeAST(node);
        return NULL;
    }

    if (!expect(parser, TOKEN_RIGHT_PAREN,
                "Expected ')' after while condition")) {
        freeAST(node);
        return NULL;
    }

    ASTNode *body = parseStatement(parser);

    if (!attach(node, body)) {
        freeAST(node);
        return NULL;
    }

    return node;
}

/* ================= BLOCK ================= */

static ASTNode *parseBlock(Parser *parser)
{
    if (!expect(parser, TOKEN_LEFT_BRACE, "Expected '{'")) {
        return NULL;
    }

    ASTNode *block = newNode(AST_BLOCK, NULL, NULL);

    if (block == NULL) {
        return NULL;
    }

    while (!check(parser, TOKEN_RIGHT_BRACE) &&
           !check(parser, TOKEN_EOF)) {

        int oldPosition = parser->position;

        ASTNode *statement = parseStatement(parser);

        if (!attach(block, statement)) {
            freeAST(block);
            return NULL;
        }

        if (parser->hasError ||
            parser->position == oldPosition) {

            if (!parser->hasError) {
                fprintf(stderr,
                        "Syntax Error: Parser made no progress.\n");
                parser->hasError = 1;
            }

            freeAST(block);
            return NULL;
        }
    }

    if (!expect(parser, TOKEN_RIGHT_BRACE,
                "Expected '}' after block")) {
        freeAST(block);
        return NULL;
    }

    return block;
}

/* ================= STATEMENT ================= */

static ASTNode *parseStatement(Parser *parser)
{
    if (isDataType(currentToken(parser)->type)) {
        return parseVariableDeclaration(parser);
    }

    if (check(parser, TOKEN_IDENTIFIER)) {
        return parseAssignment(parser);
    }

    if (check(parser, TOKEN_PRINT)) {
        return parsePrint(parser);
    }

    if (check(parser, TOKEN_IF)) {
        return parseIf(parser);
    }

    if (check(parser, TOKEN_WHILE)) {
        return parseWhile(parser);
    }

    if (check(parser, TOKEN_LEFT_BRACE)) {
        return parseBlock(parser);
    }

    fprintf(stderr,
            "Syntax Error at line %d, column %d: Unexpected token '%s'.\n",
            currentToken(parser)->line,
            currentToken(parser)->column,
            currentToken(parser)->lexeme);

    parser->hasError = 1;
    return NULL;
}

/* ================= INITIALIZE PARSER ================= */

void initParser(Parser *parser, const Token *tokens, int count)
{
    if (parser == NULL) {
        return;
    }

    memset(parser, 0, sizeof(*parser));

    if (tokens == NULL ||
        count <= 0 ||
        count > MAX_TOKENS ||
        tokens[count - 1].type != TOKEN_EOF) {

        parser->hasError = 1;
        return;
    }

    memcpy(parser->tokens,
           tokens,
           (size_t)count * sizeof(Token));

    parser->tokenCount = count;
    parser->position = 0;
}

/* ================= PARSE PROGRAM ================= */

ASTNode *parseProgram(Parser *parser)
{
    if (parser == NULL ||
        parser->hasError ||
        parser->tokenCount <= 0) {
        return NULL;
    }

    ASTNode *program = newNode(AST_PROGRAM, "PROGRAM", NULL);

    if (program == NULL) {
        return NULL;
    }

    while (!check(parser, TOKEN_EOF)) {
        int oldPosition = parser->position;

        ASTNode *statement = parseStatement(parser);

        if (!attach(program, statement)) {
            freeAST(program);
            return NULL;
        }

        if (parser->hasError ||
            parser->position == oldPosition) {

            freeAST(program);
            return NULL;
        }
    }

    return program;
}
