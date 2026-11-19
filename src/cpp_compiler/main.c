
#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h"
#include "tac/tac.h"

/* Read source code from a file */
static char *read_source_file(const char *filename) {
    FILE *file = fopen(filename, "rb");

    if (!file) {
        printf("Error: Cannot open file '%s'.\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size < 0) {
        fclose(file);
        return NULL;
    }

    char *source = malloc((size_t)size + 1);

    if (!source) {
        fclose(file);
        printf("Memory allocation failed.\n");
        return NULL;
    }

    size_t bytes_read = fread(source, 1, (size_t)size, file);
    source[bytes_read] = '\0';

    fclose(file);
    return source;
}

/* Stage 1: Lexical Analysis */
static void run_lexer(const char *source) {
    CPPLexer lexer;
    cpp_lexer_init(&lexer, source);

    printf("\n========== STEP 1: LEXICAL ANALYSIS ==========\n");

    while (1) {
        CPPToken token = cpp_lexer_next_token(&lexer);

        printf(
            "Line: %-3d Column: %-3d Token: %-30s Lexeme: %s\n",
            token.line,
            token.column,
            cpp_token_type_name(token.type),
            token.lexeme
        );

        if (token.type == CPP_TOKEN_EOF)
            break;
    }

    printf("Lexical Analysis Completed!\n");
}

/* Complete compiler pipeline */
int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: cpp_compiler <source_file>\n");
        return 1;
    }

    char *source = read_source_file(argv[1]);

    if (!source)
        return 1;

    printf("\n============================================\n");
    printf("          C++ COMPILER PIPELINE             \n");
    printf("============================================\n");

    /* STEP 1: Lexical Analysis */
    run_lexer(source);

    /* STEP 2: Syntax Analysis */
    printf("\n========== STEP 2: SYNTAX ANALYSIS ==========\n");

    CPPParser parser;
    cpp_parser_init(&parser, source);

    CPPASTNode *root = cpp_parse_program(&parser);

    if (!root || cpp_parser_had_error(&parser)) {
        printf("Syntax Analysis Failed!\n");
        cpp_ast_free(root);
        free(source);
        return 1;
    }

    printf("Syntax Analysis Successful!\n");

    printf("\n========== ABSTRACT SYNTAX TREE ==========\n");
    cpp_ast_print(root, 0);

    /* STEP 3: Semantic Analysis */
    printf("\n========== STEP 3: SEMANTIC ANALYSIS ==========\n");

    CPPSemanticAnalyzer semantic;
    cpp_semantic_init(&semantic);

    int semantic_success =
        cpp_semantic_analyze(&semantic, root);

    cpp_semantic_print_errors(&semantic);

    if (!semantic_success) {
        printf("\nCompilation stopped: Semantic errors found.\n");

        cpp_ast_free(root);
        free(source);
        return 1;
    }

    /* STEP 4: TAC Generation */
    printf("\n========== STEP 4: TAC GENERATION ==========\n");

    CPPTAC tac;
    cpp_tac_init(&tac);

    int tac_success = cpp_tac_generate(&tac, root);

    if (!tac_success) {
        printf("TAC Generation Failed!\n");

        cpp_ast_free(root);
        free(source);
        return 1;
    }

    cpp_tac_print(&tac);

    printf("\n============================================\n");
    printf("       C++ COMPILATION PIPELINE SUCCESS     \n");
    printf("============================================\n");

    cpp_ast_free(root);
    free(source);

    return 0;
}
