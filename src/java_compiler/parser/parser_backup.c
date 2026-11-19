
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    fprintf(stderr, "Syntax Error at line %d: Expected '%s'\n",
            token ? token->line : 0, value);

    parser->hasError = 1;
    return 0;
}

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
    if (parent->childCount >= 20)
        return 0;

    parent->children[parent->childCount++] = child;
    return 1;
}

static ASTNode *parseExpression(Parser *parser);

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

        if (!expect(parser, ")")) {
            freeAST(node);
            return NULL;
        }

        return node;
    }

    fprintf(stderr, "Syntax Error at line %d: Invalid expression\n",
            token->line);

    parser->hasError = 1;
    return NULL;
}

static int precedence(const char *op) {
    if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0)
        return 1;

    if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 ||
        strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0)
        return 2;

    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 3;

    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 ||
        strcmp(op, "%") == 0)
        return 4;

    return 0;
}

static ASTNode *parseBinary(Parser *parser, int minPrec) {
    ASTNode *left = parsePrimary(parser);

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

        addChild(binary, left);
        addChild(binary, right);

        left = binary;
    }

    return left;
}

static ASTNode *parseExpression(Parser *parser) {
    return parseBinary(parser, 1);
}

static ASTNode *parseStatement(Parser *parser) {
    Token *token = current(parser);

    if (!token)
        return NULL;

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

        ASTNode *declaration = createNode(
    "VARIABLE_DECLARATION",
    token->lexeme
);

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

    if (match(parser, "return")) {
        ASTNode *node = createNode("RETURN", "");

        if (!check(parser, ";")) {
            ASTNode *expr = parseExpression(parser);

            if (!expr || !addChild(node, expr)) {
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
            !expect(parser, ")") || !expect(parser, ";")) {
            freeAST(node);
            return NULL;
        }

        return node;
    }

    if (token->type == TOKEN_IDENTIFIER) {
        char name[MAX_LEXEME];
        snprintf(name, sizeof(name), "%s", token->lexeme);
        parser->position++;

        if (!expect(parser, "="))
            return NULL;

        ASTNode *node = createNode("ASSIGNMENT", name);
        ASTNode *expr = parseExpression(parser);

        if (!expr || !addChild(node, expr) || !expect(parser, ";")) {
            freeAST(node);
            return NULL;
        }

        return node;
    }

    fprintf(stderr, "Syntax Error at line %d: Unsupported statement\n",
            token->line);

    parser->hasError = 1;
    return NULL;
}

void initParser(Parser *parser, Token *tokens, int count) {
    parser->position = 0;
    parser->tokenCount = count;
    parser->hasError = 0;

    for (int i = 0; i < count; i++)
        parser->tokens[i] = tokens[i];
}

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
            if (current(parser))
                parser->position++;

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

                if (match(parser, "void")) {
                    if (current(parser))
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

                    if (!expect(parser, "{"))
                        break;

                    ASTNode *block = createNode("BLOCK", "");

                    while (current(parser) &&
                           !check(parser, "}") &&
                           !parser->hasError) {

                        ASTNode *stmt = parseStatement(parser);

                        if (!stmt || !addChild(block, stmt)) {
                            freeAST(stmt);
                            break;
                        }
                    }

                    if (!expect(parser, "}")) {
                        freeAST(block);
                        break;
                    }

                    addChild(root, block);
                    continue;
                }

                parser->hasError = 1;
                break;
            }

            expect(parser, "}");
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

void printAST(ASTNode *node, int level) {
    if (!node)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    printf("%s", node->type);

    if (node->value[0])
        printf(" (%s)", node->value);

    printf("\n");

    for (int i = 0; i < node->childCount; i++)
        printAST(node->children[i], level + 1);
}

void freeAST(ASTNode *node) {
    if (!node)
        return;

    for (int i = 0; i < node->childCount; i++)
        freeAST(node->children[i]);

    free(node);
}
