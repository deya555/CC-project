
#ifndef JAVA_RUNTIME_H
#define JAVA_RUNTIME_H

#define MAX_RUNTIME_COMMAND 2048

/*
 * Compile a Java source file using javac.
 */
int compileJavaFile(const char *sourceFile);

/*
 * Execute the compiled Java program.
 */
int executeJavaFile(const char *className);

/*
 * Compile and execute a Java program.
 */
int runJavaProgram(const char *sourceFile, const char *className);

#endif
