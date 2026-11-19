
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =========================================
   BASIC PARSER FUNCTIONS
========================================= */

static Token *current(Parser *parser) {
    if (parser->position >= parser->tokenCount)
        return NULL;

    return &parser->tokens[parser->position];
}

static int check(Parser *parser, const char *value) {
    Token *token = current(parser);

    return token && strcmp(token->lexeme, value) == 0;
}

static int match(Parser *parser, const char *value) {
    if (check(parser, value)) {
        parser->position++;
        return 1;
    }

    return 0;
}

static int expect(Parser *parser, const char *value) {
    if (match(parser, value))
        return 1;

    Token *token = current(parser);

    fprintf(stderr,
        "Syntax Error at line %d: Expected '%s'\n",
        token ? token->line : 0,
        value
    );

    parser->hasError = 1;
    return 0;
}

/* =========================================
   AST NODE CREATION
========================================= */

static ASTNode *createNode(const char *type, const char *value) {
    ASTNode *node = calloc(1, sizeof(ASTNode));

    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    snprintf(node->type, sizeof(node->type), "%s", type);
    snprintf(node->value, sizeof(node->value), "%s", value);

    return node;
}

static int addChild(ASTNode *parent, ASTNode *child) {
    if (!parent || !child || parent->childCount >= 20)
        return 0;

    parent->children[parent->childCount++] = child;
    return 1;
}

/* =========================================
   EXPRESSION PARSING
========================================= */

static ASTNode *parseExpression(Parser *parser);
static ASTNode *parseStatement(Parser *parser);
static ASTNode *parseBlock(Parser *parser);

static int precedence(const char *op) {

    if (strcmp(op, "||") == 0)
        return 1;

    if (strcmp(op, "&&") == 0)
        return 2;

    if (strcmp(op, "==") == 0 ||
        strcmp(op, "!=") == 0)
        return 3;

    if (strcmp(op, "<") == 0 ||
        strcmp(op, ">") == 0 ||
        strcmp(op, "<=") == 0 ||
        strcmp(op, ">=") == 0)
        return 4;

    if (strcmp(op, "+") == 0 ||
        strcmp(op, "-") == 0)
        return 5;

    if (strcmp(op, "*") == 0 ||
        strcmp(op, "/") == 0 ||
        strcmp(op, "%") == 0)
        return 6;

    return 0;
}

/* =========================================
   PRIMARY EXPRESSIONS
========================================= */

static ASTNode *parsePrimary(Parser *parser) {

    Token *token = current(parser);

    if (!token)
        return NULL;

    if (token->type == TOKEN_INTEGER ||
        token->type == TOKEN_FLOAT ||
        token->type == TOKEN_STRING ||
        strcmp(token->lexeme, "true") == 0 ||
        strcmp(token->lexeme, "false") == 0) {

        ASTNode *node = createNode("LITERAL", token->lexeme);

        parser->position++;

        return node;
    }

    if (token->type == TOKEN_IDENTIFIER) {

        ASTNode *node = createNode("IDENTIFIER", token->lexeme);

        parser->position++;

        return node;
    }

    if (match(parser, "(")) {

        ASTNode *node = parseExpression(parser);

        if (!node || !expect(parser, ")")) {
            freeAST(node);
            return NULL;
        }

        return node;
    }

    fprintf(stderr,
        "Syntax Error at line %d: Invalid expression\n",
        token->line
    );

    parser->hasError = 1;

    return NULL;
}

/* =========================================
   UNARY EXPRESSIONS
   Supports: !x, -x, +x
========================================= */

static ASTNode *parseUnary(Parser *parser) {

    if (check(parser, "!") ||
        check(parser, "-") ||
        check(parser, "+")) {

        char op[MAX_LEXEME];

        snprintf(op, sizeof(op), "%s", current(parser)->lexeme);

        parser->position++;

        ASTNode *operand = parseUnary(parser);

        if (!operand)
            return NULL;

        ASTNode *node = createNode("UNARY_EXPRESSION", op);

        if (!addChild(node, operand)) {
            freeAST(node);
            return NULL;
        }

        return node;
    }

    return parsePrimary(parser);
}

/* =========================================
   BINARY EXPRESSIONS
========================================= */

static ASTNode *parseBinary(Parser *parser, int minPrec) {

    ASTNode *left = parseUnary(parser);

    if (!left)
        return NULL;

    while (current(parser)) {

        Token *opToken = current(parser);

        int prec = precedence(opToken->lexeme);

        if (prec < minPrec || prec == 0)
            break;

        char op[MAX_LEXEME];

        snprintf(op, sizeof(op), "%s", opToken->lexeme);

        parser->position++;

        ASTNode *right = parseBinary(parser, prec + 1);

        if (!right) {
            freeAST(left);
            return NULL;
        }

        ASTNode *binary = createNode("BINARY_EXPRESSION", op);

        if (!addChild(binary, left) ||
            !addChild(binary, right)) {

            freeAST(binary);
            freeAST(left);
            freeAST(right);

            return NULL;
        }

        left = binary;
    }

    return left;
}

static ASTNode *parseExpression(Parser *parser) {
    return parseBinary(parser, 1);
}

/* =========================================
   BLOCK PARSING
   Supports nested { }
========================================= */

static ASTNode *parseBlock(Parser *parser) {

    if (!expect(parser, "{"))
        return NULL;

    ASTNode *block = createNode("BLOCK", "");

    while (current(parser) &&
           !check(parser, "}") &&
           !parser->hasError) {

        ASTNode *statement = parseStatement(parser);

        if (!statement || !addChild(block, statement)) {

            freeAST(statement);
            freeAST(block);

            parser->hasError = 1;

            return NULL;
        }
    }

    if (!expect(parser, "}")) {

        freeAST(block);

        return NULL;
    }

    return block;
}

/* =========================================
   IF / ELSE
========================================= */

static ASTNode *parseIf(Parser *parser) {

    if (!expect(parser, "if"))
        return NULL;

    if (!expect(parser, "("))
        return NULL;

    ASTNode *condition = parseExpression(parser);

    if (!condition || !expect(parser, ")")) {
        freeAST(condition);
        return NULL;
    }

    ASTNode *ifNode = createNode("IF", "");

    if (!addChild(ifNode, condition)) {
        freeAST(ifNode);
        freeAST(condition);
        return NULL;
    }

    ASTNode *thenBranch = parseStatement(parser);

    if (!thenBranch || !addChild(ifNode, thenBranch)) {
        freeAST(thenBranch);
        freeAST(ifNode);
        return NULL;
    }

    if (match(parser, "else")) {

        ASTNode *elseBranch = parseStatement(parser);

        if (!elseBranch || !addChild(ifNode, elseBranch)) {
            freeAST(elseBranch);
            freeAST(ifNode);
            return NULL;
        }
    }

    return ifNode;
}

/* =========================================
   WHILE LOOP
========================================= */

static ASTNode *parseWhile(Parser *parser) {

    if (!expect(parser, "while"))
        return NULL;

    if (!expect(parser, "("))
        return NULL;

    ASTNode *condition = parseExpression(parser);

    if (!condition || !expect(parser, ")")) {
        freeAST(condition);
        return NULL;
    }

    ASTNode *whileNode = createNode("WHILE", "");

    if (!addChild(whileNode, condition)) {
        freeAST(whileNode);
        freeAST(condition);
        return NULL;
    }

    ASTNode *body = parseStatement(parser);

    if (!body || !addChild(whileNode, body)) {
        freeAST(body);
        freeAST(whileNode);
        return NULL;
    }

    return whileNode;
}

/* =========================================
   STATEMENT PARSING
========================================= */

static ASTNode *parseStatement(Parser *parser) {

    Token *token = current(parser);

    if (!token)
        return NULL;

    /* Nested block */

    if (check(parser, "{"))
        return parseBlock(parser);

    /* IF */

    if (check(parser, "if"))
        return parseIf(parser);

    /* WHILE */

    if (check(parser, "while"))
        return parseWhile(parser);

    /* Variable declaration */

    if (strcmp(token->lexeme, "int") == 0 ||
        strcmp(token->lexeme, "double") == 0 ||
        strcmp(token->lexeme, "float") == 0 ||
        strcmp(token->lexeme, "boolean") == 0 ||
        strcmp(token->lexeme, "String") == 0 ||
        strcmp(token->lexeme, "char") == 0) {

        char type[MAX_LEXEME];

        snprintf(type, sizeof(type), "%s", token->lexeme);

        parser->position++;

        token = current(parser);

        if (!token || token->type != TOKEN_IDENTIFIER) {

            parser->hasError = 1;

            return NULL;
        }

        ASTNode *declaration =
            createNode("VARIABLE_DECLARATION", token->lexeme);

        snprintf(
            declaration->dataType,
            sizeof(declaration->dataType),
            "%s",
            type
        );

        parser->position++;

        if (match(parser, "=")) {

            ASTNode *expr = parseExpression(parser);

            if (!expr || !addChild(declaration, expr)) {
                freeAST(expr);
                freeAST(declaration);
                return NULL;
            }
        }

        if (!expect(parser, ";")) {
            freeAST(declaration);
            return NULL;
        }

        return declaration;
    }

    /* Return */

    if (match(parser, "return")) {

        ASTNode *node = createNode("RETURN", "");

        if (!check(parser, ";")) {

            ASTNode *expr = parseExpression(parser);

            if (!expr || !addChild(node, expr)) {
                freeAST(expr);
                freeAST(node);
                return NULL;
            }
        }

        if (!expect(parser, ";")) {
            freeAST(node);
            return NULL;
        }

        return node;
    }

    /* Java print */

    if (match(parser, "System")) {

        if (!expect(parser, ".") ||
            !expect(parser, "out") ||
            !expect(parser, ".") ||
            !(match(parser, "println") || match(parser, "print")) ||
            !expect(parser, "(")) {

            return NULL;
        }

        ASTNode *node = createNode("PRINT", "");

        ASTNode *expr = parseExpression(parser);

        if (!expr || !addChild(node, expr) ||
            !expect(parser, ")") ||
            !expect(parser, ";")) {

            freeAST(node);

            return NULL;
        }

        return node;
    }

    /* Assignment */

    if (token->type == TOKEN_IDENTIFIER) {

        char name[MAX_LEXEME];

        snprintf(name, sizeof(name), "%s", token->lexeme);

        parser->position++;

        if (!expect(parser, "="))
            return NULL;

        ASTNode *node = createNode("ASSIGNMENT", name);

        ASTNode *expr = parseExpression(parser);

        if (!expr || !addChild(node, expr) ||
            !expect(parser, ";")) {

            freeAST(node);

            return NULL;
        }

        return node;
    }

    fprintf(stderr,
        "Syntax Error at line %d: Unsupported statement\n",
        token->line
    );

    parser->hasError = 1;

    return NULL;
}

/* =========================================
   PARSER INITIALIZATION
========================================= */

void initParser(Parser *parser, Token *tokens, int count) {

    parser->position = 0;
    parser->tokenCount = count;
    parser->hasError = 0;

    if (count > MAX_TOKENS)
        count = MAX_TOKENS;

    for (int i = 0; i < count; i++)
        parser->tokens[i] = tokens[i];

    parser->tokenCount = count;
}

/* =========================================
   PROGRAM PARSING
========================================= */

ASTNode *parseProgram(Parser *parser) {

    ASTNode *root = createNode("PROGRAM", "");

    while (current(parser) &&
           current(parser)->type != TOKEN_EOF &&
           !parser->hasError) {

        if (match(parser, "public") ||
            match(parser, "private") ||
            match(parser, "protected") ||
            match(parser, "static")) {

            continue;
        }

        if (match(parser, "class")) {

            /* Class name */

            
            if (current(parser) &&
                current(parser)->type == TOKEN_IDENTIFIER) {

                parser->position++;

            } else {

                parser->hasError = 1;
                break;
            }

            if (!expect(parser, "{"))
                break;

            while (current(parser) &&
                   !check(parser, "}") &&
                   !parser->hasError) {

                if (match(parser, "public") ||
                    match(parser, "private") ||
                    match(parser, "protected") ||
                    match(parser, "static")) {

                    continue;
                }

                /* Method declaration */

                if (match(parser, "void")) {

                    if (!current(parser) ||
                        current(parser)->type != TOKEN_IDENTIFIER) {

                        parser->hasError = 1;
                        break;
                    }

                    parser->position++;

                    if (!expect(parser, "("))
                        break;

                    int depth = 1;

                    while (current(parser) && depth > 0) {

                        if (match(parser, "("))
                            depth++;

                        else if (match(parser, ")"))
                            depth--;

                        else
                            parser->position++;
                    }

                    if (depth != 0) {
                        parser->hasError = 1;
                        break;
                    }

                    ASTNode *block = parseBlock(parser);

                    if (!block || !addChild(root, block)) {
                        freeAST(block);
                        parser->hasError = 1;
                        break;
                    }

                    continue;
                }

                parser->hasError = 1;
                break;
            }

            if (!expect(parser, "}"))
                break;

            continue;
        }

        parser->hasError = 1;
    }

    if (parser->hasError) {
        freeAST(root);
        return NULL;
    }

    return root;
}

/* =========================================
   AST DISPLAY
========================================= */

void printAST(ASTNode *node, int level) {

    if (!node)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    printf("%s", node->type);

    if (node->value[0])
        printf(" (%s)", node->value);

    if (node->dataType[0])
        printf(" : %s", node->dataType);

    printf("\n");

    for (int i = 0; i < node->childCount; i++)
        printAST(node->children[i], level + 1);
}

/* =========================================
   FREE AST MEMORY
========================================= */

void freeAST(ASTNode *node) {

    if (!node)
        return;

    for (int i = 0; i < node->childCount; i++)
        freeAST(node->children[i]);

    free(node);
}
