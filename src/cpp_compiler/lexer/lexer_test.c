
#include <stdio.h>
#include "lexer.h"

int main(void)
{
    const char *source =
        "#include <iostream>\n"
        "using namespace std;\n"
        "int main() {\n"
        "    int number = 10;\n"
        "    if (number > 5) {\n"
        "        cout << \"greater\" << endl;\n"
        "    }\n"
        "    return 0;\n"
        "}\n";

    CPPLexer lexer;
    cpp_lexer_init(&lexer, source);

    printf("========== C++ LEXER TEST ==========\\n");

    while (1) {
        CPPToken token = cpp_lexer_next_token(&lexer);

        printf(
            "Line: %-3d Column: %-3d Token: %-35s Lexeme: %s\n",
            token.line,
            token.column,
            cpp_token_type_name(token.type),
            token.lexeme
        );

        if (token.type == CPP_TOKEN_EOF)
            break;
    }

    printf("\n========== LEXER TEST COMPLETE ==========\n");

    return 0;
}
