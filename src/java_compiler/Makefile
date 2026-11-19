
CC = gcc

CFLAGS = -Wall -Wextra -std=c11

TARGET = java_compiler.exe

SRC = main.c \
      lexer/lexer.c \
      parser/parser.c \
      ast/ast.c \
      symbol_table/symbol_table.c \
      semantic/semantic.c \
      tac/tac.c \
      optimizer/optimizer.c

INCLUDES = -Ilexer \
           -Iparser \
           -Iast \
           -Isymbol_table \
           -Isemantic \
           -Itac \
           -Ioptimizer

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) examples/sample.java

clean:
	rm -f $(TARGET)
