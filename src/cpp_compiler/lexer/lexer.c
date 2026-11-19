
#include "lexer.h"

#include <ctype.h>
#include <string.h>

/* Get the current character. */
static char current_char(CPPLexer *lexer)
{
    return lexer->source[lexer->position];
}

/* Get the next character without consuming it. */
static char peek_char(CPPLexer *lexer)
{
    return lexer->source[lexer->position + 1];
}

/* Consume one character and update line/column. */
static char advance_char(CPPLexer *lexer)
{
    char ch = current_char(lexer);

    if (ch == '\0')
        return '\0';

    lexer->position++;

    if (ch == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }

    return ch;
}

/* Match and consume an expected character. */
static int match_char(CPPLexer *lexer, char expected)
{
    if (current_char(lexer) != expected)
        return 0;

    advance_char(lexer);
    return 1;
}

/* Create a token. */
static CPPToken make_token(
    CPPTokenType type,
    const char *start,
    int length,
    int line,
    int column
)
{
    CPPToken token;

    token.type = type;
    token.line = line;
    token.column = column;

    if (length >= CPP_MAX_LEXEME)
        length = CPP_MAX_LEXEME - 1;

    if (length < 0)
        length = 0;

    memcpy(token.lexeme, start, (size_t)length);
    token.lexeme[length] = '\0';

    return token;
}

/* Initialize the lexer. */
void cpp_lexer_init(CPPLexer *lexer, const char *source)
{
    if (lexer == NULL)
        return;

    lexer->source = source != NULL ? source : "";
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
}

/* Check whether an identifier is a C++ keyword. */
static CPPTokenType get_keyword_type(const char *word)
{
    struct Keyword {
        const char *name;
        CPPTokenType type;
    };

    static const struct Keyword keywords[] = {
        {"int", CPP_TOKEN_INT},
        {"float", CPP_TOKEN_FLOAT},
        {"double", CPP_TOKEN_DOUBLE},
        {"char", CPP_TOKEN_CHAR},
        {"bool", CPP_TOKEN_BOOL},
        {"void", CPP_TOKEN_VOID},
        {"if", CPP_TOKEN_IF},
        {"else", CPP_TOKEN_ELSE},
        {"while", CPP_TOKEN_WHILE},
        {"for", CPP_TOKEN_FOR},
        {"return", CPP_TOKEN_RETURN},
        {"class", CPP_TOKEN_CLASS},
        {"public", CPP_TOKEN_PUBLIC},
        {"private", CPP_TOKEN_PRIVATE},
        {"protected", CPP_TOKEN_PROTECTED},
        {"using", CPP_TOKEN_USING},
        {"namespace", CPP_TOKEN_NAMESPACE},
        {"include", CPP_TOKEN_INCLUDE},
        {"const", CPP_TOKEN_CONST},
        {"static", CPP_TOKEN_STATIC},
        {"struct", CPP_TOKEN_STRUCT},
        {"true", CPP_TOKEN_TRUE},
        {"false", CPP_TOKEN_FALSE},
        {"new", CPP_TOKEN_NEW},
        {"delete", CPP_TOKEN_DELETE},
        {"this", CPP_TOKEN_THIS},
        {"break", CPP_TOKEN_BREAK},
        {"continue", CPP_TOKEN_CONTINUE}
    };

    size_t count = sizeof(keywords) / sizeof(keywords[0]);

    for (size_t i = 0; i < count; i++) {
        if (strcmp(word, keywords[i].name) == 0)
            return keywords[i].type;
    }

    return CPP_TOKEN_IDENTIFIER;
}

/* Skip whitespace and comments. */
static void skip_ignored(CPPLexer *lexer)
{
    while (1) {
        char ch = current_char(lexer);

        if (isspace((unsigned char)ch)) {
            advance_char(lexer);
            continue;
        }

        /* Single-line comment. */
        if (ch == '/' && peek_char(lexer) == '/') {
            while (current_char(lexer) != '\0' &&
                   current_char(lexer) != '\n') {
                advance_char(lexer);
            }
            continue;
        }

        /* Multi-line comment. */
        if (ch == '/' && peek_char(lexer) == '*') {
            advance_char(lexer);
            advance_char(lexer);

            while (current_char(lexer) != '\0') {
                if (current_char(lexer) == '*' &&
                    peek_char(lexer) == '/') {
                    advance_char(lexer);
                    advance_char(lexer);
                    break;
                }

                advance_char(lexer);
            }

            continue;
        }

        break;
    }
}

/* Read an identifier or keyword. */
static CPPToken read_identifier(CPPLexer *lexer)
{
    const char *start = &lexer->source[lexer->position];
    int line = lexer->line;
    int column = lexer->column;

    while (isalnum((unsigned char)current_char(lexer)) ||
           current_char(lexer) == '_') {
        advance_char(lexer);
    }

    int length = (int)(&lexer->source[lexer->position] - start);

    CPPToken token = make_token(
        CPP_TOKEN_IDENTIFIER, start, length, line, column
    );

    token.type = get_keyword_type(token.lexeme);

    return token;
}

/* Read an integer or floating-point literal. */
static CPPToken read_number(CPPLexer *lexer)
{
    const char *start = &lexer->source[lexer->position];
    int line = lexer->line;
    int column = lexer->column;
    int is_float = 0;

    while (isdigit((unsigned char)current_char(lexer)))
        advance_char(lexer);

    if (current_char(lexer) == '.' &&
        isdigit((unsigned char)peek_char(lexer))) {
        is_float = 1;
        advance_char(lexer);

        while (isdigit((unsigned char)current_char(lexer)))
            advance_char(lexer);
    }

    if (current_char(lexer) == 'e' ||
        current_char(lexer) == 'E') {
        is_float = 1;
        advance_char(lexer);

        if (current_char(lexer) == '+' ||
            current_char(lexer) == '-') {
            advance_char(lexer);
        }

        if (!isdigit((unsigned char)current_char(lexer))) {
            int length = (int)(
                &lexer->source[lexer->position] - start
            );

            return make_token(
                CPP_TOKEN_UNKNOWN, start, length, line, column
            );
        }

        while (isdigit((unsigned char)current_char(lexer)))
            advance_char(lexer);
    }

    int length = (int)(&lexer->source[lexer->position] - start);

    return make_token(
        is_float ? CPP_TOKEN_FLOAT_LITERAL :
                   CPP_TOKEN_INTEGER_LITERAL,
        start,
        length,
        line,
        column
    );
}

/* Read a string or character literal. */
static CPPToken read_quoted_literal(
    CPPLexer *lexer,
    char quote,
    CPPTokenType type
)
{
    const char *start = &lexer->source[lexer->position];
    int line = lexer->line;
    int column = lexer->column;

    advance_char(lexer);

    while (current_char(lexer) != '\0') {
        char ch = advance_char(lexer);

        if (ch == '\\') {
            if (current_char(lexer) != '\0')
                advance_char(lexer);

            continue;
        }

        if (ch == quote) {
            int length = (int)(
                &lexer->source[lexer->position] - start
            );

            return make_token(type, start, length, line, column);
        }

        if (ch == '\n')
            break;
    }

    int length = (int)(&lexer->source[lexer->position] - start);

    return make_token(
        CPP_TOKEN_UNKNOWN, start, length, line, column
    );
}

/* Return the next token. */
CPPToken cpp_lexer_next_token(CPPLexer *lexer)
{
    if (lexer == NULL || lexer->source == NULL) {
        return make_token(CPP_TOKEN_EOF, "", 0, 1, 1);
    }

    skip_ignored(lexer);

    char ch = current_char(lexer);

    int line = lexer->line;
    int column = lexer->column;

    const char *start = &lexer->source[lexer->position];

    if (ch == '\0')
        return make_token(CPP_TOKEN_EOF, "", 0, line, column);

    if (isalpha((unsigned char)ch) || ch == '_')
        return read_identifier(lexer);

    if (isdigit((unsigned char)ch))
        return read_number(lexer);

    if (ch == '"')
        return read_quoted_literal(
            lexer, '"', CPP_TOKEN_STRING_LITERAL
        );

    if (ch == '\'')
        return read_quoted_literal(
            lexer, '\'', CPP_TOKEN_CHAR_LITERAL
        );

    advance_char(lexer);

    CPPTokenType type = CPP_TOKEN_UNKNOWN;

    switch (ch) {
        case '+':
            if (match_char(lexer, '+'))
                type = CPP_TOKEN_INCREMENT;
            else if (match_char(lexer, '='))
                type = CPP_TOKEN_PLUS_ASSIGN;
            else
                type = CPP_TOKEN_PLUS;
            break;

        case '-':
            if (match_char(lexer, '-'))
                type = CPP_TOKEN_DECREMENT;
            else if (match_char(lexer, '='))
                type = CPP_TOKEN_MINUS_ASSIGN;
            else if (match_char(lexer, '>'))
                type = CPP_TOKEN_ARROW;
            else
                type = CPP_TOKEN_MINUS;
            break;

        case '*':
            type = match_char(lexer, '=') ?
                CPP_TOKEN_MULTIPLY_ASSIGN : CPP_TOKEN_MULTIPLY;
            break;

        case '/':
            type = match_char(lexer, '=') ?
                CPP_TOKEN_DIVIDE_ASSIGN : CPP_TOKEN_DIVIDE;
            break;

        case '%':
            type = match_char(lexer, '=') ?
                CPP_TOKEN_MODULO_ASSIGN : CPP_TOKEN_MODULO;
            break;

        case '=':
            type = match_char(lexer, '=') ?
                CPP_TOKEN_EQUAL : CPP_TOKEN_ASSIGN;
            break;

        case '!':
            type = match_char(lexer, '=') ?
                CPP_TOKEN_NOT_EQUAL : CPP_TOKEN_LOGICAL_NOT;
            break;

        case '<':
            if (match_char(lexer, '<')) {
                if (match_char(lexer, '='))
                    type = CPP_TOKEN_LEFT_SHIFT_ASSIGN;
                else
                    type = CPP_TOKEN_LEFT_SHIFT;
            } else if (match_char(lexer, '=')) {
                type = CPP_TOKEN_LESS_EQUAL;
            } else {
                type = CPP_TOKEN_LESS;
            }
            break;

        case '>':
            if (match_char(lexer, '>')) {
                if (match_char(lexer, '='))
                    type = CPP_TOKEN_RIGHT_SHIFT_ASSIGN;
                else
                    type = CPP_TOKEN_RIGHT_SHIFT;
            } else if (match_char(lexer, '=')) {
                type = CPP_TOKEN_GREATER_EQUAL;
            } else {
                type = CPP_TOKEN_GREATER;
            }
            break;

        case '&':
            if (match_char(lexer, '&'))
                type = CPP_TOKEN_LOGICAL_AND;
            else if (match_char(lexer, '='))
                type = CPP_TOKEN_AND_ASSIGN;
            else
                type = CPP_TOKEN_BITWISE_AND;
            break;

        case '|':
            if (match_char(lexer, '|'))
                type = CPP_TOKEN_LOGICAL_OR;
            else if (match_char(lexer, '='))
                type = CPP_TOKEN_OR_ASSIGN;
            else
                type = CPP_TOKEN_BITWISE_OR;
            break;

        case '^':
            type = match_char(lexer, '=') ?
                CPP_TOKEN_XOR_ASSIGN : CPP_TOKEN_BITWISE_XOR;
            break;

        case '~':
            type = CPP_TOKEN_BITWISE_NOT;
            break;

        case ':':
            type = match_char(lexer, ':') ?
                CPP_TOKEN_SCOPE : CPP_TOKEN_COLON;
            break;

        case ';': type = CPP_TOKEN_SEMICOLON; break;
        case ',': type = CPP_TOKEN_COMMA; break;
        case '.': type = CPP_TOKEN_DOT; break;
        case '#': type = CPP_TOKEN_HASH; break;

        case '(': type = CPP_TOKEN_LEFT_PAREN; break;
        case ')': type = CPP_TOKEN_RIGHT_PAREN; break;
        case '{': type = CPP_TOKEN_LEFT_BRACE; break;
        case '}': type = CPP_TOKEN_RIGHT_BRACE; break;
        case '[': type = CPP_TOKEN_LEFT_BRACKET; break;
        case ']': type = CPP_TOKEN_RIGHT_BRACKET; break;

        default:
            type = CPP_TOKEN_UNKNOWN;
            break;
    }

    int length = (int)(&lexer->source[lexer->position] - start);

    return make_token(type, start, length, line, column);
}

/* Get a readable token name. */
const char *cpp_token_type_name(CPPTokenType type)
{
#define TOKEN_NAME(t) case t: return #t

    switch (type) {
        TOKEN_NAME(CPP_TOKEN_EOF);
        TOKEN_NAME(CPP_TOKEN_UNKNOWN);
        TOKEN_NAME(CPP_TOKEN_IDENTIFIER);
        TOKEN_NAME(CPP_TOKEN_INTEGER_LITERAL);
        TOKEN_NAME(CPP_TOKEN_FLOAT_LITERAL);
        TOKEN_NAME(CPP_TOKEN_STRING_LITERAL);
        TOKEN_NAME(CPP_TOKEN_CHAR_LITERAL);

        TOKEN_NAME(CPP_TOKEN_INT);
        TOKEN_NAME(CPP_TOKEN_FLOAT);
        TOKEN_NAME(CPP_TOKEN_DOUBLE);
        TOKEN_NAME(CPP_TOKEN_CHAR);
        TOKEN_NAME(CPP_TOKEN_BOOL);
        TOKEN_NAME(CPP_TOKEN_VOID);
        TOKEN_NAME(CPP_TOKEN_IF);
        TOKEN_NAME(CPP_TOKEN_ELSE);
        TOKEN_NAME(CPP_TOKEN_WHILE);
        TOKEN_NAME(CPP_TOKEN_FOR);
        TOKEN_NAME(CPP_TOKEN_RETURN);
        TOKEN_NAME(CPP_TOKEN_CLASS);
        TOKEN_NAME(CPP_TOKEN_PUBLIC);
        TOKEN_NAME(CPP_TOKEN_PRIVATE);
        TOKEN_NAME(CPP_TOKEN_PROTECTED);
        TOKEN_NAME(CPP_TOKEN_USING);
        TOKEN_NAME(CPP_TOKEN_NAMESPACE);
        TOKEN_NAME(CPP_TOKEN_INCLUDE);
        TOKEN_NAME(CPP_TOKEN_CONST);
        TOKEN_NAME(CPP_TOKEN_STATIC);
        TOKEN_NAME(CPP_TOKEN_STRUCT);
        TOKEN_NAME(CPP_TOKEN_TRUE);
        TOKEN_NAME(CPP_TOKEN_FALSE);
        TOKEN_NAME(CPP_TOKEN_NEW);
        TOKEN_NAME(CPP_TOKEN_DELETE);
        TOKEN_NAME(CPP_TOKEN_THIS);
        TOKEN_NAME(CPP_TOKEN_BREAK);
        TOKEN_NAME(CPP_TOKEN_CONTINUE);

        TOKEN_NAME(CPP_TOKEN_PLUS);
        TOKEN_NAME(CPP_TOKEN_MINUS);
        TOKEN_NAME(CPP_TOKEN_MULTIPLY);
        TOKEN_NAME(CPP_TOKEN_DIVIDE);
        TOKEN_NAME(CPP_TOKEN_MODULO);
        TOKEN_NAME(CPP_TOKEN_INCREMENT);
        TOKEN_NAME(CPP_TOKEN_DECREMENT);

        TOKEN_NAME(CPP_TOKEN_ASSIGN);
        TOKEN_NAME(CPP_TOKEN_PLUS_ASSIGN);
        TOKEN_NAME(CPP_TOKEN_MINUS_ASSIGN);
        TOKEN_NAME(CPP_TOKEN_MULTIPLY_ASSIGN);
        TOKEN_NAME(CPP_TOKEN_DIVIDE_ASSIGN);
        TOKEN_NAME(CPP_TOKEN_MODULO_ASSIGN);

        TOKEN_NAME(CPP_TOKEN_EQUAL);
        TOKEN_NAME(CPP_TOKEN_NOT_EQUAL);
        TOKEN_NAME(CPP_TOKEN_LESS);
        TOKEN_NAME(CPP_TOKEN_GREATER);
        TOKEN_NAME(CPP_TOKEN_LESS_EQUAL);
        TOKEN_NAME(CPP_TOKEN_GREATER_EQUAL);

        TOKEN_NAME(CPP_TOKEN_LOGICAL_AND);
        TOKEN_NAME(CPP_TOKEN_LOGICAL_OR);
        TOKEN_NAME(CPP_TOKEN_LOGICAL_NOT);
        TOKEN_NAME(CPP_TOKEN_BITWISE_AND);
        TOKEN_NAME(CPP_TOKEN_BITWISE_OR);
        TOKEN_NAME(CPP_TOKEN_BITWISE_XOR);
        TOKEN_NAME(CPP_TOKEN_BITWISE_NOT);
        TOKEN_NAME(CPP_TOKEN_LEFT_SHIFT);
        TOKEN_NAME(CPP_TOKEN_RIGHT_SHIFT);

        TOKEN_NAME(CPP_TOKEN_AND_ASSIGN);
        TOKEN_NAME(CPP_TOKEN_OR_ASSIGN);
        TOKEN_NAME(CPP_TOKEN_XOR_ASSIGN);
        TOKEN_NAME(CPP_TOKEN_LEFT_SHIFT_ASSIGN);
        TOKEN_NAME(CPP_TOKEN_RIGHT_SHIFT_ASSIGN);
        TOKEN_NAME(CPP_TOKEN_SCOPE);
        TOKEN_NAME(CPP_TOKEN_ARROW);

        TOKEN_NAME(CPP_TOKEN_SEMICOLON);
        TOKEN_NAME(CPP_TOKEN_COMMA);
        TOKEN_NAME(CPP_TOKEN_DOT);
        TOKEN_NAME(CPP_TOKEN_COLON);
        TOKEN_NAME(CPP_TOKEN_HASH);
        TOKEN_NAME(CPP_TOKEN_LEFT_PAREN);
        TOKEN_NAME(CPP_TOKEN_RIGHT_PAREN);
        TOKEN_NAME(CPP_TOKEN_LEFT_BRACE);
        TOKEN_NAME(CPP_TOKEN_RIGHT_BRACE);
        TOKEN_NAME(CPP_TOKEN_LEFT_BRACKET);
        TOKEN_NAME(CPP_TOKEN_RIGHT_BRACKET);

        default:
            return "UNKNOWN_TOKEN";
    }

#undef TOKEN_NAME
}
