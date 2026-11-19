
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "runtime.h"

int compileJavaFile(const char *sourceFile)
{
    char command[MAX_RUNTIME_COMMAND];

    snprintf(
        command,
        sizeof(command),
        "javac \"%s\"",
        sourceFile
    );

    printf("\n========== JAVA COMPILATION ==========\n");

    int result = system(command);

    if (result != 0)
    {
        printf("Java Compilation Failed!\n");
        return 0;
    }

    printf("Java Compilation Successful!\n");
    return 1;
}

int executeJavaFile(const char *className)
{
    char command[MAX_RUNTIME_COMMAND];

    snprintf(
        command,
        sizeof(command),
        "java -cp \"src/java_compiler/runtime\" %s",
        className
    );

    printf("\n========== JAVA PROGRAM OUTPUT ==========\n");

    int result = system(command);

    if (result != 0)
    {
        printf("Java Execution Failed!\n");
        return 0;
    }

    printf("\nJava Execution Successful!\n");
    return 1;
}

int runJavaProgram(
    const char *sourceFile,
    const char *className
)
{
    if (!compileJavaFile(sourceFile))
    {
        return 0;
    }

    if (!executeJavaFile(className))
    {
        return 0;
    }

    return 1;
}
