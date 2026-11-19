#ifndef CPP_LEXER_H
#define CPP_LEXER_H

#define CPP_MAX_LEXEME 256

typedef enum {
    CPP_TOKEN_EOF = 0,
    CPP_TOKEN_UNKNOWN,

    /* Identifiers and literals */
    CPP_TOKEN_IDENTIFIER,
    CPP_TOKEN_INTEGER_LITERAL,
    CPP_TOKEN_FLOAT_LITERAL,
    CPP_TOKEN_STRING_LITERAL,
    CPP_TOKEN_CHAR_LITERAL,

    /* Keywords */
    CPP_TOKEN_INT,
    CPP_TOKEN_FLOAT,
    CPP_TOKEN_DOUBLE,
    CPP_TOKEN_CHAR,
    CPP_TOKEN_BOOL,
    CPP_TOKEN_VOID,
    CPP_TOKEN_IF,
    CPP_TOKEN_ELSE,
    CPP_TOKEN_WHILE,
    CPP_TOKEN_FOR,
    CPP_TOKEN_RETURN,
    CPP_TOKEN_CLASS,
    CPP_TOKEN_PUBLIC,
    CPP_TOKEN_PRIVATE,
    CPP_TOKEN_PROTECTED,
    CPP_TOKEN_USING,
    CPP_TOKEN_NAMESPACE,
    CPP_TOKEN_INCLUDE,
    CPP_TOKEN_CONST,
    CPP_TOKEN_STATIC,
    CPP_TOKEN_STRUCT,
    CPP_TOKEN_TRUE,
    CPP_TOKEN_FALSE,
    CPP_TOKEN_NEW,
    CPP_TOKEN_DELETE,
    CPP_TOKEN_THIS,
    CPP_TOKEN_BREAK,
    CPP_TOKEN_CONTINUE,

    /* Arithmetic operators */
    CPP_TOKEN_PLUS,
    CPP_TOKEN_MINUS,
    CPP_TOKEN_MULTIPLY,
    CPP_TOKEN_DIVIDE,
    CPP_TOKEN_MODULO,

    /* Increment and decrement */
    CPP_TOKEN_INCREMENT,
    CPP_TOKEN_DECREMENT,

    /* Assignment operators */
    CPP_TOKEN_ASSIGN,
    CPP_TOKEN_PLUS_ASSIGN,
    CPP_TOKEN_MINUS_ASSIGN,
    CPP_TOKEN_MULTIPLY_ASSIGN,
    CPP_TOKEN_DIVIDE_ASSIGN,
    CPP_TOKEN_MODULO_ASSIGN,

    /* Comparison operators */
    CPP_TOKEN_EQUAL,
    CPP_TOKEN_NOT_EQUAL,
    CPP_TOKEN_LESS,
    CPP_TOKEN_GREATER,
    CPP_TOKEN_LESS_EQUAL,
    CPP_TOKEN_GREATER_EQUAL,

    /* Logical and bitwise operators */
    CPP_TOKEN_LOGICAL_AND,
    CPP_TOKEN_LOGICAL_OR,
    CPP_TOKEN_LOGICAL_NOT,
    CPP_TOKEN_BITWISE_AND,
    CPP_TOKEN_BITWISE_OR,
    CPP_TOKEN_BITWISE_XOR,
    CPP_TOKEN_BITWISE_NOT,
    CPP_TOKEN_LEFT_SHIFT,
    CPP_TOKEN_RIGHT_SHIFT,

    /* Other operators */
    CPP_TOKEN_AND_ASSIGN,
    CPP_TOKEN_OR_ASSIGN,
    CPP_TOKEN_XOR_ASSIGN,
    CPP_TOKEN_LEFT_SHIFT_ASSIGN,
    CPP_TOKEN_RIGHT_SHIFT_ASSIGN,
    CPP_TOKEN_SCOPE,
    CPP_TOKEN_ARROW,

    /* Punctuation */
    CPP_TOKEN_SEMICOLON,
    CPP_TOKEN_COMMA,
    CPP_TOKEN_DOT,
    CPP_TOKEN_COLON,
    CPP_TOKEN_HASH,
    CPP_TOKEN_LEFT_PAREN,
    CPP_TOKEN_RIGHT_PAREN,
    CPP_TOKEN_LEFT_BRACE,
    CPP_TOKEN_RIGHT_BRACE,
    CPP_TOKEN_LEFT_BRACKET,
    CPP_TOKEN_RIGHT_BRACKET
} CPPTokenType;

typedef struct {
    CPPTokenType type;
    char lexeme[CPP_MAX_LEXEME];
    int line;
    int column;
} CPPToken;

typedef struct {
    const char *source;
    int position;
    int line;
    int column;
} CPPLexer;

/* Initialize the lexer with source code. */
void cpp_lexer_init(CPPLexer *lexer, const char *source);

/* Read and return the next token. */
CPPToken cpp_lexer_next_token(CPPLexer *lexer);

/* Return a readable name for a token type. */
const char *cpp_token_type_name(CPPTokenType type);

#endif