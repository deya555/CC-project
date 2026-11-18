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
     FLOAT_TYPE = 259,
     DOUBLE = 260,
     CHAR = 261,
     VOID = 262,
     SHORT = 263,
     LONG = 264,
     SIGNED = 265,
     UNSIGNED = 266,
     IF = 267,
     ELSE = 268,
     WHILE = 269,
     FOR = 270,
     DO = 271,
     SWITCH = 272,
     CASE = 273,
     DEFAULT = 274,
     BREAK = 275,
     CONTINUE = 276,
     RETURN = 277,
     PRINTF = 278,
     SCANF = 279,
     TRUE = 280,
     FALSE = 281,
     INTEGER_LITERAL = 282,
     FLOAT_LITERAL = 283,
     STRING_LITERAL = 284,
     CHAR_LITERAL = 285,
     IDENTIFIER_TOKEN = 286,
     EQUAL = 287,
     NOT_EQUAL = 288,
     LESS_EQUAL = 289,
     GREATER_EQUAL = 290,
     LOGICAL_AND = 291,
     LOGICAL_OR = 292,
     INCREMENT = 293,
     DECREMENT = 294,
     PLUS_ASSIGN = 295,
     MINUS_ASSIGN = 296,
     MULTIPLY_ASSIGN = 297,
     DIVIDE_ASSIGN = 298,
     PLUS = 299,
     MINUS = 300,
     MULTIPLY = 301,
     DIVIDE = 302,
     MODULO = 303,
     ASSIGN = 304,
     LESS = 305,
     GREATER = 306,
     LOGICAL_NOT = 307,
     SEMICOLON = 308,
     COMMA = 309,
     LPAREN = 310,
     RPAREN = 311,
     LBRACE = 312,
     RBRACE = 313,
     LBRACKET = 314,
     RBRACKET = 315,
     UMINUS = 316,
     LOWER_THAN_ELSE = 317
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 15 "src/c/parser/parser.y"

    char *text;
    struct ASTNode *node;



/* Line 1685 of yacc.c  */
#line 120 "src/c/parser/parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


