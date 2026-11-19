
#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static const char *keywords[] = {
    "public", "private", "protected", "static",
    "void", "int", "float", "double", "char",
    "boolean", "String", "class", "new",
    "if", "else", "while", "for", "return",
    "true", "false", "null", "break", "continue",
    "this", "final", "extends", "implements",
    "import", "package", "System", "out",
    "println", "print", NULL
};

void initLexer(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->position = 0;
    lexer->line = 1;
}

static char currentChar(Lexer *lexer) {
    return lexer->source[lexer->position];
}

static char peekChar(Lexer *lexer) {
    if (lexer->source[lexer->position] == '\0')
        return '\0';

    return lexer->source[lexer->position + 1];
}

static void skipWhitespace(Lexer *lexer) {
    while (isspace((unsigned char)currentChar(lexer))) {
        if (currentChar(lexer) == '\n')
            lexer->line++;

        lexer->position++;
    }
}

static int isKeyword(const char *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }

    return 0;
}

static Token makeToken(TokenType type, const char *text, int line) {
    Token token;

    token.type = type;
    token.line = line;

    strncpy(token.lexeme, text, MAX_LEXEME - 1);
    token.lexeme[MAX_LEXEME - 1] = '\0';

    return token;
}

Token getNextToken(Lexer *lexer) {
    skipWhitespace(lexer);

    int line = lexer->line;
    char ch = currentChar(lexer);

    if (ch == '\0')
        return makeToken(TOKEN_EOF, "EOF", line);

    char buffer[MAX_LEXEME];
    int index = 0;

    if (isalpha((unsigned char)ch) || ch == '_') {
        while (isalnum((unsigned char)currentChar(lexer)) ||
               currentChar(lexer) == '_') {

            if (index < MAX_LEXEME - 1)
                buffer[index++] = currentChar(lexer);

            lexer->position++;
        }

        buffer[index] = '\0';

        return makeToken(
            isKeyword(buffer) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER,
            buffer, line
        );
    }

    if (isdigit((unsigned char)ch)) {
        int isFloat = 0;

        while (isdigit((unsigned char)currentChar(lexer)) ||
               (currentChar(lexer) == '.' &&
                !isFloat &&
                isdigit((unsigned char)peekChar(lexer)))) {

            if (currentChar(lexer) == '.')
                isFloat = 1;

            if (index < MAX_LEXEME - 1)
                buffer[index++] = currentChar(lexer);

            lexer->position++;
        }

        buffer[index] = '\0';

        return makeToken(
            isFloat ? TOKEN_FLOAT : TOKEN_INTEGER,
            buffer, line
        );
    }

    if (ch == '"') {
        lexer->position++;

        while (currentChar(lexer) != '"' &&
               currentChar(lexer) != '\0') {

            if (currentChar(lexer) == '\n')
                lexer->line++;

            if (currentChar(lexer) == '\\' &&
                peekChar(lexer) != '\0') {

                if (index < MAX_LEXEME - 1)
                    buffer[index++] = currentChar(lexer);

                lexer->position++;
            }

            if (index < MAX_LEXEME - 1)
                buffer[index++] = currentChar(lexer);

            lexer->position++;
        }

        buffer[index] = '\0';

        if (currentChar(lexer) == '"') {
            lexer->position++;
        } else {
            return makeToken(TOKEN_UNKNOWN, "Unterminated string", line);
        }

        return makeToken(TOKEN_STRING, buffer, line);
    }

    if (ch == '/' && peekChar(lexer) == '/') {
        while (currentChar(lexer) != '\n' &&
               currentChar(lexer) != '\0') {
            lexer->position++;
        }

        return getNextToken(lexer);
    }

    if (ch == '/' && peekChar(lexer) == '*') {
        lexer->position += 2;

        while (currentChar(lexer) != '\0') {
            if (currentChar(lexer) == '\n')
                lexer->line++;

            if (currentChar(lexer) == '*' && peekChar(lexer) == '/') {
                lexer->position += 2;
                return getNextToken(lexer);
            }

            lexer->position++;
        }

        return makeToken(TOKEN_UNKNOWN, "Unterminated comment", line);
    }

    if (strchr("+-*/%=!<>&|", ch) != NULL) {
        buffer[index++] = ch;
        lexer->position++;

        if ((ch == '=' && currentChar(lexer) == '=') ||
            (ch == '!' && currentChar(lexer) == '=') ||
            (ch == '<' && currentChar(lexer) == '=') ||
            (ch == '>' && currentChar(lexer) == '=') ||
            (ch == '+' && currentChar(lexer) == '+') ||
            (ch == '-' && currentChar(lexer) == '-') ||
            (ch == '&' && currentChar(lexer) == '&') ||
            (ch == '|' && currentChar(lexer) == '|')) {

            buffer[index++] = currentChar(lexer);
            lexer->position++;
        }

        buffer[index] = '\0';

        return makeToken(TOKEN_OPERATOR, buffer, line);
    }

    if (strchr("(){}[];,.", ch) != NULL) {
        buffer[0] = ch;
        buffer[1] = '\0';

        lexer->position++;

        return makeToken(TOKEN_SEPARATOR, buffer, line);
    }

    buffer[0] = ch;
    buffer[1] = '\0';

    lexer->position++;

    return makeToken(TOKEN_UNKNOWN, buffer, line);
}

const char *tokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER: return "INTEGER";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STRING: return "STRING";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_SEPARATOR: return "SEPARATOR";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        case TOKEN_EOF: return "EOF";
        default: return "INVALID";
    }
}
