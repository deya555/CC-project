/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 1685 of yacc.c  */
#line 1 "compiler\\src\\parser\\parser.y"

#include "../ast/ast.h"



/* Line 1685 of yacc.c  */
#line 45 "compiler\\src\\parser\\parser.tab.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     FLOAT = 259,
     BOOL = 260,
     IF = 261,
     ELSE = 262,
     WHILE = 263,
     PRINT = 264,
     TRUE = 265,
     FALSE = 266,
     IDENTIFIER = 267,
     INTEGER_LITERAL = 268,
     FLOAT_LITERAL = 269,
     PLUS = 270,
     MINUS = 271,
     MULTIPLY = 272,
     DIVIDE = 273,
     MODULO = 274,
     LT = 275,
     GT = 276,
     LE = 277,
     GE = 278,
     EQ = 279,
     NE = 280,
     AND = 281,
     OR = 282,
     NOT = 283,
     ASSIGN = 284,
     LBRACE = 285,
     RBRACE = 286,
     LPAREN = 287,
     RPAREN = 288,
     SEMICOLON = 289,
     INVALID = 290,
     UMINUS = 291,
     IF_WITHOUT_ELSE = 292
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 39 "compiler\\src\\parser\\parser.y"

    char *text;
    ASTNode *node;



/* Line 1685 of yacc.c  */
#line 106 "compiler\\src\\parser\\parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


