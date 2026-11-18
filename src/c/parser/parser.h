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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     BOOL = 259,
     FLOAT_TYPE = 260,
     DOUBLE = 261,
     CHAR = 262,
     VOID = 263,
     SHORT = 264,
     LONG = 265,
     SIGNED = 266,
     UNSIGNED = 267,
     IF = 268,
     ELSE = 269,
     WHILE = 270,
     FOR = 271,
     DO = 272,
     SWITCH = 273,
     CASE = 274,
     DEFAULT = 275,
     BREAK = 276,
     CONTINUE = 277,
     RETURN = 278,
     PRINTF = 279,
     SCANF = 280,
     PRINT = 281,
     TRUE = 282,
     FALSE = 283,
     INTEGER_LITERAL = 284,
     FLOAT_LITERAL = 285,
     STRING_LITERAL = 286,
     CHAR_LITERAL = 287,
     IDENTIFIER_TOKEN = 288,
     EQUAL = 289,
     NOT_EQUAL = 290,
     LESS_EQUAL = 291,
     GREATER_EQUAL = 292,
     LOGICAL_AND = 293,
     LOGICAL_OR = 294,
     INCREMENT = 295,
     DECREMENT = 296,
     PLUS_ASSIGN = 297,
     MINUS_ASSIGN = 298,
     MULTIPLY_ASSIGN = 299,
     DIVIDE_ASSIGN = 300,
     PLUS = 301,
     MINUS = 302,
     MULTIPLY = 303,
     DIVIDE = 304,
     MODULO = 305,
     ASSIGN = 306,
     LESS = 307,
     GREATER = 308,
     LOGICAL_NOT = 309,
     SEMICOLON = 310,
     COMMA = 311,
     LPAREN = 312,
     RPAREN = 313,
     LBRACE = 314,
     RBRACE = 315,
     LBRACKET = 316,
     RBRACKET = 317,
     UMINUS = 318,
     LOWER_THAN_ELSE = 319
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 18 "src/c/parser/parser.y"

    char *text;
    struct ASTNode *node;



/* Line 1685 of yacc.c  */
#line 122 "src/c/parser/parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


