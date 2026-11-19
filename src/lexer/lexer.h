
#ifndef JAVA_LEXER_H
#define JAVA_LEXER_H

#define MAX_TOKENS 4096
#define MAX_LEXEME 256

typedef enum {
    /* Data type keywords */
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_DOUBLE,
    TOKEN_BOOLEAN,
    TOKEN_STRING,
    TOKEN_CHAR,

    /* Control-flow keywords */
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_TRUE,
    TOKEN_FALSE,

    /* Output and Java wrapper keywords */
    TOKEN_PRINT,
    TOKEN_PUBLIC,
    TOKEN_CLASS,
    TOKEN_STATIC,
    TOKEN_VOID,
    TOKEN_RETURN,

    /* Identifiers and literals */
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_BOOLEAN_LITERAL,

    /* Arithmetic operators */
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_MODULO,

    /* Assignment and relational operators */
    TOKEN_ASSIGN,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS,
    TOKEN_GREATER,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,

    /* Logical operators */
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,

    /* Delimiters and punctuation */
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_DOT,

    /* Special tokens */
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_LEXEME];
    int line;
    int column;
} Token;

typedef struct {
    const char *source;
    int position;
    int line;
    int column;

    Token tokens[MAX_TOKENS];
    int tokenCount;

    int hasError;
} Lexer;

/* Initialize lexer */
void initLexer(Lexer *lexer, const char *source);

/* Perform lexical analysis */
int tokenize(Lexer *lexer);

/* Print generated tokens */
void printTokens(const Lexer *lexer);

/* Return readable token name */
const char *tokenTypeName(TokenType type);

#endif
