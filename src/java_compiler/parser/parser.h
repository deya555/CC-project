
#ifndef JAVA_PARSER_H
#define JAVA_PARSER_H

#include "../lexer/lexer.h"
#include "../ast/ast.h"

typedef struct {
    Token tokens[MAX_TOKENS];
    int position;
    int tokenCount;
    int hasError;
} Parser;

void initParser(Parser *parser, const Token *tokens, int count);
ASTNode *parseProgram(Parser *parser);

#endif
