
#include "lexer.h"
#include <stdio.h>

int main() {
    const char *source =
        "public class Main {\n"
        "   public static void main(String[] args) {\n"
        "       int number = 10;\n"
        "       double price = 25.5;\n"
        "       System.out.println(number);\n"
        "   }\n"
        "}";

    Lexer lexer;
    initLexer(&lexer, source);

    printf("========== JAVA LEXICAL ANALYSIS ==========\n");

    Token token;

    do {
        token = getNextToken(&lexer);

        if (token.type != TOKEN_EOF) {
            printf("%-12s %-20s Line: %d\n",
                   tokenTypeName(token.type),
                   token.lexeme,
                   token.line);
        }

    } while (token.type != TOKEN_EOF);

    printf("\nLexical Analysis Completed!\n");

    return 0;
}
