
#ifndef JAVA_LEXER_H
#define JAVA_LEXER_H

#define MAX_LEXEME 256
#define MAX_TOKENS 1000

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_SEPARATOR,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME];
    int line;
} Token;

typedef struct {
    const char *source;
    int position;
    int line;
} Lexer;

void initLexer(Lexer *lexer, const char *source);

Token getNextToken(Lexer *lexer);

const char *tokenTypeName(TokenType type);

#endif
