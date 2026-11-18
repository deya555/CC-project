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
     TRUE = 281,
     FALSE = 282,
     INTEGER_LITERAL = 283,
     FLOAT_LITERAL = 284,
     STRING_LITERAL = 285,
     CHAR_LITERAL = 286,
     IDENTIFIER_TOKEN = 287,
     EQUAL = 288,
     NOT_EQUAL = 289,
     LESS_EQUAL = 290,
     GREATER_EQUAL = 291,
     LOGICAL_AND = 292,
     LOGICAL_OR = 293,
     INCREMENT = 294,
     DECREMENT = 295,
     PLUS_ASSIGN = 296,
     MINUS_ASSIGN = 297,
     MULTIPLY_ASSIGN = 298,
     DIVIDE_ASSIGN = 299,
     PLUS = 300,
     MINUS = 301,
     MULTIPLY = 302,
     DIVIDE = 303,
     MODULO = 304,
     ASSIGN = 305,
     LESS = 306,
     GREATER = 307,
     LOGICAL_NOT = 308,
     SEMICOLON = 309,
     COMMA = 310,
     LPAREN = 311,
     RPAREN = 312,
     LBRACE = 313,
     RBRACE = 314,
     LBRACKET = 315,
     RBRACKET = 316,
     UMINUS = 317,
     LOWER_THAN_ELSE = 318
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
#line 121 "src/c/parser/parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


