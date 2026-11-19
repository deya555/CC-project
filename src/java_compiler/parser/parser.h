
#ifndef JAVA_PARSER_H
#define JAVA_PARSER_H

#include "../lexer/lexer.h"

typedef struct ASTNode {
    char type[50];
    char value[MAX_LEXEME];
    struct ASTNode *children[20];
    int childCount;
} ASTNode;

typedef struct {
    Token tokens[MAX_TOKENS];
    int position;
    int tokenCount;
    int hasError;
} Parser;

void initParser(Parser *parser, Token *tokens, int count);

ASTNode *parseProgram(Parser *parser);

void printAST(ASTNode *node, int level);

void freeAST(ASTNode *node);

#endif
