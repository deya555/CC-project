
#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h"
#include "tac/tac.h"

/* Read Java source code from a file. */
static char *readSourceFile(const char *fileName)
{
    FILE *file = fopen(fileName, "rb");

    if (file == NULL)
    {
        printf("Error: Cannot open Java source file.\n");
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        fclose(file);
        return NULL;
    }

    long fileSize = ftell(file);

    if (fileSize < 0 || fseek(file, 0, SEEK_SET) != 0)
    {
        fclose(file);
        return NULL;
    }

    char *source = malloc((size_t)fileSize + 1);

    if (source == NULL)
    {
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(source, 1, (size_t)fileSize, file);

    if (bytesRead != (size_t)fileSize)
    {
        free(source);
        fclose(file);
        return NULL;
    }

    source[bytesRead] = '\0';
    fclose(file);

    return source;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: java_compiler_cli.exe <source.java>\n");
        return 1;
    }

    char *source = readSourceFile(argv[1]);

    if (source == NULL)
    {
        printf("Source file reading failed!\n");
        return 1;
    }

    printf("========== JAVA COMPILER ==========\n\n");

    /* STEP 1: LEXICAL ANALYSIS */

    printf("STEP 1: LEXICAL ANALYSIS\n");

    Lexer lexer;
    Token tokens[MAX_TOKENS];

    initLexer(&lexer, source);

    int tokenCount = 0;
    int lexicalSuccess = 0;

    while (tokenCount < MAX_TOKENS)
    {
        Token token = getNextToken(&lexer);

        if (token.type == TOKEN_UNKNOWN)
        {
            printf(
                "Lexical Error: Invalid token '%s' at line %d\n",
                token.lexeme,
                token.line
            );
            break;
        }

        tokens[tokenCount++] = token;

        if (token.type == TOKEN_EOF)
        {
            lexicalSuccess = 1;
            break;
        }
    }

    if (!lexicalSuccess)
    {
        printf("Lexical Analysis Failed!\n");
        free(source);
        return 1;
    }

    printf("Lexical Analysis Successful!\n\n");

    /* STEP 2: SYNTAX ANALYSIS */

    printf("STEP 2: SYNTAX ANALYSIS\n");

    Parser parser;

    initParser(&parser, tokens, tokenCount);

    ASTNode *root = parseProgram(&parser);

    if (root == NULL || parser.hasError)
    {
        printf("Syntax Analysis Failed!\n");
        freeAST(root);
        free(source);
        return 1;
    }

    printf("Syntax Analysis Successful!\n\n");

    printf("========== JAVA ABSTRACT SYNTAX TREE ==========\n");
    printAST(root, 0);
    printf("\n");

    /* STEP 3: SEMANTIC ANALYSIS */

    printf("STEP 3: SEMANTIC ANALYSIS\n");

    SemanticAnalyzer analyzer;

    initSemantic(&analyzer);

    int semanticSuccess = analyzeSemantics(&analyzer, root);

    if (!semanticSuccess)
    {
        printf("Semantic Analysis Failed!\n");
        freeAST(root);
        free(source);
        return 1;
    }

    printf("Semantic Analysis Successful!\n");

    /* STEP 4: THREE ADDRESS CODE */

    printf("\nSTEP 4: THREE ADDRESS CODE GENERATION\n");

    TACGenerator tac;

    initTAC(&tac);

    if (!generateTAC(&tac, root))
    {
        printf("TAC Generation Failed!\n");
        freeAST(root);
        free(source);
        return 1;
    }

    printf("TAC Generation Successful!\n");
    printTAC(&tac);

    /* STEP 5: SYMBOL TABLE */

    printf("\nSTEP 5: JAVA SYMBOL TABLE\n");

    printSymbolTable(&analyzer);

    printf("\nJAVA COMPILER ANALYSIS SUCCESSFUL!\n");

    freeAST(root);
    free(source);

    return 0;
}
