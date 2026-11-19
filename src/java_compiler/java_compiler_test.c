
#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h"
#include "tac/tac.h"
#include "runtime/runtime.h"

static int saveJavaSource(
    const char *fileName,
    const char *source
) {
    FILE *file = fopen(fileName, "w");

    if (file == NULL) {
        printf("Error: Could not create Java source file.\n");
        return 0;
    }

    if (fputs(source, file) == EOF) {
        printf("Error: Could not write Java source file.\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int main(void) {

    /* JAVA SOURCE CODE */
const char *source =
    "public class Main {\n"
    "    public static void main(String[] args) {\n"
    "        int number = 10;\n"
    "        boolean result = number > 5;\n"
    "        System.out.println(number);\n"
    "        System.out.println(result);\n"
    "    }\n"
    "}\n";
    printf("========== JAVA COMPILER TEST ==========\n\n");

    /* =====================================
       STEP 1: LEXICAL ANALYSIS
       ===================================== */

    printf("STEP 1: LEXICAL ANALYSIS\n");

    Lexer lexer;
    Token tokens[MAX_TOKENS];

    initLexer(&lexer, source);

    int tokenCount = 0;
    int lexicalSuccess = 0;

    while (tokenCount < MAX_TOKENS) {

        Token token = getNextToken(&lexer);

        if (token.type == TOKEN_UNKNOWN) {
            printf("Lexical Error: Invalid token '%s' at line %d\n",
                   token.lexeme, token.line);
            break;
        }

        tokens[tokenCount++] = token;

        if (token.type == TOKEN_EOF) {
            lexicalSuccess = 1;
            break;
        }
    }

    if (!lexicalSuccess) {
        printf("Lexical Analysis Failed!\n");
        return 1;
    }

    printf("Lexical Analysis Successful!\n\n");

    /* =====================================
       STEP 2: SYNTAX ANALYSIS
       ===================================== */

    printf("STEP 2: SYNTAX ANALYSIS\n");

    Parser parser;

    initParser(&parser, tokens, tokenCount);

    ASTNode *root = parseProgram(&parser);

    if (root == NULL || parser.hasError) {

        printf("Syntax Analysis Failed!\n");
        printf("Parser position: %d\n", parser.position);
        printf("Token count: %d\n", parser.tokenCount);

        if (parser.position < parser.tokenCount) {
            printf("Current token: %s\n",
                   parser.tokens[parser.position].lexeme);
        }

        freeAST(root);
        return 1;
    }

    printf("Syntax Analysis Successful!\n\n");

    printf("========== JAVA ABSTRACT SYNTAX TREE ==========\n");
printAST(root, 0);
printf("\n");

    /* =====================================
       STEP 3: SEMANTIC ANALYSIS
       ===================================== */

    printf("STEP 3: SEMANTIC ANALYSIS\n");

    SemanticAnalyzer analyzer;

    initSemantic(&analyzer);

    int semanticSuccess = analyzeSemantics(&analyzer, root);

    if (semanticSuccess) {
        printf("Semantic Analysis Successful!\n");
    } else {
        printf("Semantic Analysis Failed!\n");
    }

    /* Stop if semantic analysis fails */

    if (!semanticSuccess) {
        freeAST(root);
        return 1;
    }

    /* =====================================
       STEP 4: THREE ADDRESS CODE GENERATION
       ===================================== */

    printf("\nSTEP 4: THREE ADDRESS CODE GENERATION\n");

    TACGenerator tac;

    initTAC(&tac);

    if (!generateTAC(&tac, root)) {

        printf("TAC Generation Failed!\n");

        freeAST(root);
        return 1;
    }

    printf("TAC Generation Successful!\n");

    printTAC(&tac);

    /* =====================================
       STEP 5: SYMBOL TABLE
       ===================================== */

    printf("\nSTEP 5: JAVA SYMBOL TABLE\n");

    printSymbolTable(&analyzer);

    /* =====================================
   STEP 6: JAVA COMPILATION AND EXECUTION
   ===================================== */

printf("\nSTEP 6: JAVA COMPILATION AND EXECUTION\n");

/* Save Java source code */

if (!saveJavaSource(
        "src/java_compiler/runtime/Main.java",
        source
    )) {

    freeAST(root);
    return 1;
}

/* Compile and execute Java program */

if (!runJavaProgram(
        "src/java_compiler/runtime/Main.java",
        "Main"
    )) {

    printf("\nJava Runtime Failed!\n");

    freeAST(root);
    return 1;
}

    /* =====================================
       CLEANUP
       ===================================== */

    freeAST(root);

    printf("\n========== JAVA COMPILER TEST COMPLETE ==========\n");

    return 0;
}
