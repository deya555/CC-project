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
#line 60 "src/parser/parser.y"

#include "ast.h"



/* Line 1685 of yacc.c  */
#line 45 "build/parser.tab.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     KW_INT = 258,
     KW_FLOAT = 259,
     KW_BOOL = 260,
     KW_IF = 261,
     KW_ELSE = 262,
     KW_WHILE = 263,
     KW_PRINT = 264,
     KW_TRUE = 265,
     KW_FALSE = 266,
     KW_FOR = 267,
     KW_DO = 268,
     KW_SWITCH = 269,
     KW_CASE = 270,
     KW_DEFAULT = 271,
     KW_BREAK = 272,
     KW_CONTINUE = 273,
     KW_RETURN = 274,
     KW_MAIN = 275,
     KW_VOID = 276,
     KW_CONST = 277,
     KW_DOUBLE = 278,
     KW_LONG = 279,
     KW_SHORT = 280,
     KW_COUT = 281,
     KW_CIN = 282,
     KW_ENDL = 283,
     SHL = 284,
     SHR = 285,
     INC = 286,
     DEC = 287,
     ADDEQ = 288,
     SUBEQ = 289,
     MULEQ = 290,
     DIVEQ = 291,
     INT_LIT = 292,
     FLOAT_LIT = 293,
     IDENTIFIER = 294,
     STRING_LIT = 295,
     LE = 296,
     GE = 297,
     EQ = 298,
     NE = 299,
     AND = 300,
     OR = 301,
     LOWER_THAN_ELSE = 302,
     UMINUS = 303
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 64 "src/parser/parser.y"

    long        ival;
    double      fval;
    char*       sval;
    Type        tval;
    Node*       node;
    BlockNode*  block;
    CoutNode*   coutn;
    CinNode*    cinn;
    SwitchNode* swn;



/* Line 1685 of yacc.c  */
#line 124 "build/parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;

