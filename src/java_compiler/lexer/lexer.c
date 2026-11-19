
#include "lexer.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* =========================================
   HELPER FUNCTIONS
========================================= */

static char currentChar(const Lexer *lexer)
{
    return lexer->source[lexer->position];
}

static char peekChar(const Lexer *lexer)
{
    if (currentChar(lexer) == '\0')
        return '\0';

    return lexer->source[lexer->position + 1];
}

static void advance(Lexer *lexer)
{
    if (currentChar(lexer) == '\0')
        return;

    if (currentChar(lexer) == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }

    lexer->position++;
}

static void addToken(
    Lexer *lexer,
    TokenType type,
    const char *start,
    int length,
    int line,
    int column
)
{
    if (lexer->tokenCount >= MAX_TOKENS) {
        printf("Lexical Error: Maximum token limit exceeded.\n");
        lexer->hasError = 1;
        return;
    }

    Token *token = &lexer->tokens[lexer->tokenCount++];

    token->type = type;
    token->line = line;
    token->column = column;

    if (length >= MAX_LEXEME)
        length = MAX_LEXEME - 1;

    memcpy(token->lexeme, start, length);
    token->lexeme[length] = '\0';
}

static int isIdentifierStart(char c)
{
    return isalpha((unsigned char)c) || c == '_';
}

static int isIdentifierPart(char c)
{
    return isalnum((unsigned char)c) || c == '_';
}

/* =========================================
   KEYWORD RECOGNITION
========================================= */

static TokenType getKeywordType(const char *word)
{
    if (strcmp(word, "int") == 0) return TOKEN_INT;
    if (strcmp(word, "float") == 0) return TOKEN_FLOAT;
    if (strcmp(word, "double") == 0) return TOKEN_DOUBLE;
    if (strcmp(word, "boolean") == 0) return TOKEN_BOOLEAN;
    if (strcmp(word, "String") == 0) return TOKEN_STRING;
    if (strcmp(word, "char") == 0) return TOKEN_CHAR;

    if (strcmp(word, "if") == 0) return TOKEN_IF;
    if (strcmp(word, "else") == 0) return TOKEN_ELSE;
    if (strcmp(word, "while") == 0) return TOKEN_WHILE;

    if (strcmp(word, "true") == 0) return TOKEN_TRUE;
    if (strcmp(word, "false") == 0) return TOKEN_FALSE;

    if (strcmp(word, "print") == 0) return TOKEN_PRINT;

    if (strcmp(word, "public") == 0) return TOKEN_PUBLIC;
    if (strcmp(word, "class") == 0) return TOKEN_CLASS;
    if (strcmp(word, "static") == 0) return TOKEN_STATIC;
    if (strcmp(word, "void") == 0) return TOKEN_VOID;
    if (strcmp(word, "return") == 0) return TOKEN_RETURN;

    return TOKEN_IDENTIFIER;
}

/* =========================================
   IDENTIFIER SCANNING
========================================= */

static void scanIdentifier(Lexer *lexer)
{
    int start = lexer->position;
    int line = lexer->line;
    int column = lexer->column;

    while (isIdentifierPart(currentChar(lexer)))
        advance(lexer);

    int length = lexer->position - start;

    char word[MAX_LEXEME];

    if (length >= MAX_LEXEME)
        length = MAX_LEXEME - 1;

    memcpy(word, lexer->source + start, length);
    word[length] = '\0';

    TokenType type = getKeywordType(word);

    addToken(
        lexer,
        type,
        lexer->source + start,
        length,
        line,
        column
    );
}

/* =========================================
   NUMBER SCANNING
========================================= */

static void scanNumber(Lexer *lexer)
{
    int start = lexer->position;
    int line = lexer->line;
    int column = lexer->column;

    int isFloat = 0;

    while (isdigit((unsigned char)currentChar(lexer)))
        advance(lexer);

    if (currentChar(lexer) == '.' &&
        isdigit((unsigned char)peekChar(lexer))) {

        isFloat = 1;
        advance(lexer);

        while (isdigit((unsigned char)currentChar(lexer)))
            advance(lexer);
    }

    int length = lexer->position - start;

    addToken(
        lexer,
        isFloat ? TOKEN_FLOAT_LITERAL : TOKEN_INTEGER_LITERAL,
        lexer->source + start,
        length,
        line,
        column
    );
}

/* =========================================
   STRING SCANNING
========================================= */

static void scanString(Lexer *lexer)
{
    int start = lexer->position;
    int line = lexer->line;
    int column = lexer->column;

    advance(lexer);

    while (currentChar(lexer) != '\0' &&
           currentChar(lexer) != '"') {

        if (currentChar(lexer) == '\\') {
            advance(lexer);

            if (currentChar(lexer) == '\0')
                break;

            advance(lexer);
            continue;
        }

        if (currentChar(lexer) == '\n') {
            printf(
                "Lexical Error at line %d, column %d: "
                "Unterminated string literal.\n",
                line, column
            );

            lexer->hasError = 1;
            return;
        }

        advance(lexer);
    }

    if (currentChar(lexer) != '"') {
        printf(
            "Lexical Error at line %d, column %d: "
            "Unterminated string literal.\n",
            line, column
        );

        lexer->hasError = 1;
        return;
    }

    advance(lexer);

    addToken(
        lexer,
        TOKEN_STRING_LITERAL,
        lexer->source + start,
        lexer->position - start,
        line,
        column
    );
}

/* =========================================
   CHARACTER SCANNING
========================================= */

static void scanCharacter(Lexer *lexer)
{
    int start = lexer->position;
    int line = lexer->line;
    int column = lexer->column;

    advance(lexer);

    if (currentChar(lexer) == '\0' ||
        currentChar(lexer) == '\n' ||
        currentChar(lexer) == '\'') {

        printf(
            "Lexical Error at line %d, column %d: "
            "Invalid character literal.\n",
            line, column
        );

        lexer->hasError = 1;
        return;
    }

    if (currentChar(lexer) == '\\') {
        advance(lexer);

        if (currentChar(lexer) == '\0') {
            lexer->hasError = 1;
            return;
        }
    }

    advance(lexer);

    if (currentChar(lexer) != '\'') {
        printf(
            "Lexical Error at line %d, column %d: "
            "Character literal must contain one character.\n",
            line, column
        );

        lexer->hasError = 1;

        while (currentChar(lexer) != '\0' &&
               currentChar(lexer) != '\n' &&
               currentChar(lexer) != '\'') {
            advance(lexer);
        }

        if (currentChar(lexer) == '\'')
            advance(lexer);

        return;
    }

    advance(lexer);

    addToken(
        lexer,
        TOKEN_CHAR_LITERAL,
        lexer->source + start,
        lexer->position - start,
        line,
        column
    );
}

/* =========================================
   WHITESPACE AND COMMENT HANDLING
========================================= */

static void skipWhitespaceAndComments(Lexer *lexer)
{
    while (1) {

        char c = currentChar(lexer);

        if (isspace((unsigned char)c)) {
            advance(lexer);
            continue;
        }

        /* Single-line comments */

        if (c == '/' && peekChar(lexer) == '/') {

            while (currentChar(lexer) != '\0' &&
                   currentChar(lexer) != '\n') {
                advance(lexer);
            }

            continue;
        }

        /* Multi-line comments */

        if (c == '/' && peekChar(lexer) == '*') {

            int startLine = lexer->line;
            int startColumn = lexer->column;

            advance(lexer);
            advance(lexer);

            int closed = 0;

            while (currentChar(lexer) != '\0') {

                if (currentChar(lexer) == '*' &&
                    peekChar(lexer) == '/') {

                    advance(lexer);
                    advance(lexer);

                    closed = 1;
                    break;
                }

                advance(lexer);
            }

            if (!closed) {
                printf(
                    "Lexical Error at line %d, column %d: "
                    "Unterminated block comment.\n",
                    startLine, startColumn
                );

                lexer->hasError = 1;
            }

            continue;
        }

        break;
    }
}

/* =========================================
   LEXER INITIALIZATION
========================================= */

void initLexer(Lexer *lexer, const char *source)
{
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->tokenCount = 0;
    lexer->hasError = 0;
}

/* =========================================
   MAIN TOKENIZER
========================================= */

int tokenize(Lexer *lexer)
{
    while (currentChar(lexer) != '\0') {

        skipWhitespaceAndComments(lexer);

        if (currentChar(lexer) == '\0')
            break;

        char c = currentChar(lexer);

        int line = lexer->line;
        int column = lexer->column;
        int start = lexer->position;

        if (isIdentifierStart(c)) {
            scanIdentifier(lexer);
            continue;
        }

        if (isdigit((unsigned char)c)) {
            scanNumber(lexer);
            continue;
        }

        if (c == '"') {
            scanString(lexer);
            continue;
        }

        if (c == '\'') {
            scanCharacter(lexer);
            continue;
        }

        advance(lexer);

        switch (c) {

            case '+':
                addToken(lexer, TOKEN_PLUS, "+", 1, line, column);
                break;

            case '-':
                addToken(lexer, TOKEN_MINUS, "-", 1, line, column);
                break;

            case '*':
                addToken(lexer, TOKEN_MULTIPLY, "*", 1, line, column);
                break;

            case '/':
                addToken(lexer, TOKEN_DIVIDE, "/", 1, line, column);
                break;

            case '%':
                addToken(lexer, TOKEN_MODULO, "%", 1, line, column);
                break;

            case '=':

                if (currentChar(lexer) == '=') {
                    advance(lexer);
                    addToken(lexer, TOKEN_EQUAL, "==", 2, line, column);
                } else {
                    addToken(lexer, TOKEN_ASSIGN, "=", 1, line, column);
                }

                break;

            case '!':

                if (currentChar(lexer) == '=') {
                    advance(lexer);
                    addToken(lexer, TOKEN_NOT_EQUAL, "!=", 2, line, column);
                } else {
                    addToken(lexer, TOKEN_NOT, "!", 1, line, column);
                }

                break;

            case '<':

                if (currentChar(lexer) == '=') {
                    advance(lexer);
                    addToken(lexer, TOKEN_LESS_EQUAL, "<=", 2, line, column);
                } else {
                    addToken(lexer, TOKEN_LESS, "<", 1, line, column);
                }

                break;

            case '>':

                if (currentChar(lexer) == '=') {
                    advance(lexer);
                    addToken(lexer, TOKEN_GREATER_EQUAL, ">=", 2, line, column);
                } else {
                    addToken(lexer, TOKEN_GREATER, ">", 1, line, column);
                }

                break;

            case '&':

                if (currentChar(lexer) == '&') {
                    advance(lexer);
                    addToken(lexer, TOKEN_AND, "&&", 2, line, column);
                } else {
                    printf(
                        "Lexical Error at line %d, column %d: "
                        "Expected '&&'.\n",
                        line, column
                    );

                    lexer->hasError = 1;

                    addToken(
                        lexer, TOKEN_INVALID,
                        lexer->source + start, 1, line, column
                    );
                }

                break;

            case '|':

                if (currentChar(lexer) == '|') {
                    advance(lexer);
                    addToken(lexer, TOKEN_OR, "||", 2, line, column);
                } else {
                    printf(
                        "Lexical Error at line %d, column %d: "
                        "Expected '||'.\n",
                        line, column
                    );

                    lexer->hasError = 1;

                    addToken(
                        lexer, TOKEN_INVALID,
                        lexer->source + start, 1, line, column
                    );
                }

                break;

            case '(':
                addToken(lexer, TOKEN_LEFT_PAREN, "(", 1, line, column);
                break;

            case ')':
                addToken(lexer, TOKEN_RIGHT_PAREN, ")", 1, line, column);
                break;

            case '{':
                addToken(lexer, TOKEN_LEFT_BRACE, "{", 1, line, column);
                break;

            case '}':
                addToken(lexer, TOKEN_RIGHT_BRACE, "}", 1, line, column);
                break;

            case '[':
                addToken(lexer, TOKEN_LEFT_BRACKET, "[", 1, line, column);
                break;

            case ']':
                addToken(lexer, TOKEN_RIGHT_BRACKET, "]", 1, line, column);
                break;

            case ';':
                addToken(lexer, TOKEN_SEMICOLON, ";", 1, line, column);
                break;

            case ',':
                addToken(lexer, TOKEN_COMMA, ",", 1, line, column);
                break;

            case '.':
                addToken(lexer, TOKEN_DOT, ".", 1, line, column);
                break;

            default:

                printf(
                    "Lexical Error at line %d, column %d: "
                    "Invalid character '%c'.\n",
                    line, column, c
                );

                lexer->hasError = 1;

                addToken(
                    lexer,
                    TOKEN_INVALID,
                    lexer->source + start,
                    1,
                    line,
                    column
                );

                break;
        }
    }

    addToken(
        lexer,
        TOKEN_EOF,
        "",
        0,
        lexer->line,
        lexer->column
    );

    return !lexer->hasError;
}

/* =========================================
   TOKEN NAME CONVERSION
========================================= */

const char *tokenTypeName(TokenType type)
{
    switch (type) {

        case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_DOUBLE: return "DOUBLE";
        case TOKEN_BOOLEAN: return "BOOLEAN";
        case TOKEN_STRING: return "STRING";
        case TOKEN_CHAR: return "CHAR";

        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";

        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";

        case TOKEN_PRINT: return "PRINT";

        case TOKEN_PUBLIC: return "PUBLIC";
        case TOKEN_CLASS: return "CLASS";
        case TOKEN_STATIC: return "STATIC";
        case TOKEN_VOID: return "VOID";
        case TOKEN_RETURN: return "RETURN";

        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER_LITERAL: return "INTEGER_LITERAL";
        case TOKEN_FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TOKEN_STRING_LITERAL: return "STRING_LITERAL";
        case TOKEN_CHAR_LITERAL: return "CHAR_LITERAL";
        case TOKEN_BOOLEAN_LITERAL: return "BOOLEAN_LITERAL";

        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_MODULO: return "MODULO";

        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_NOT_EQUAL: return "NOT_EQUAL";
        case TOKEN_LESS: return "LESS";
        case TOKEN_GREATER: return "GREATER";
        case TOKEN_LESS_EQUAL: return "LESS_EQUAL";
        case TOKEN_GREATER_EQUAL: return "GREATER_EQUAL";

        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";

        case TOKEN_LEFT_PAREN: return "LEFT_PAREN";
        case TOKEN_RIGHT_PAREN: return "RIGHT_PAREN";
        case TOKEN_LEFT_BRACE: return "LEFT_BRACE";
        case TOKEN_RIGHT_BRACE: return "RIGHT_BRACE";
        case TOKEN_LEFT_BRACKET: return "LEFT_BRACKET";
        case TOKEN_RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_DOT: return "DOT";

        case TOKEN_EOF: return "EOF";
        case TOKEN_INVALID: return "INVALID";

        default: return "UNKNOWN";
    }
}

/* =========================================
   PRINT TOKENS
========================================= */

void printTokens(const Lexer *lexer)
{
    printf("\n========== LEXICAL TOKENS ==========\n");

    printf(
        "%-22s %-22s %-8s %-8s\n",
        "TOKEN TYPE",
        "LEXEME",
        "LINE",
        "COLUMN"
    );

    printf("------------------------------------------------------------\n");

    for (int i = 0; i < lexer->tokenCount; i++) {

        const Token *token = &lexer->tokens[i];

        printf(
            "%-22s %-22s %-8d %-8d\n",
            tokenTypeName(token->type),
            token->lexeme,
            token->line,
            token->column
        );
    }

    printf("====================================\n");

    if (lexer->hasError)
        printf("Lexical Analysis Failed!\n");
    else
        printf("Lexical Analysis Successful!\n");
}
