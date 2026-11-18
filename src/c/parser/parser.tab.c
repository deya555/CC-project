/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 2 "src/c/parser/parser.y"

#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"
#include "../semantic/semantic.h"
#include "../symbol_table/symbol_table.h"
#include "../tac/tac.h"

int yylex(void);
void yyerror(const char *message);

ASTNode *ast_root = NULL;


/* Line 189 of yacc.c  */
#line 87 "src/c/parser/parser.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
#line 18 "src/c/parser/parser.y"

    char *text;
    struct ASTNode *node;



/* Line 214 of yacc.c  */
#line 192 "src/c/parser/parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 204 "src/c/parser/parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   441

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  69
/* YYNRULES -- Number of states.  */
#define YYNSTATES  128

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   317

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    29,    32,    36,    38,    42,    44,
      48,    50,    52,    54,    56,    58,    60,    62,    64,    66,
      71,    76,    81,    86,    91,    94,   101,   102,   105,   113,
     115,   119,   125,   133,   139,   143,   145,   147,   149,   151,
     153,   155,   157,   161,   165,   169,   173,   177,   181,   185,
     189,   193,   197,   201,   205,   209,   213,   216,   219,   222
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      64,     0,    -1,    65,    -1,    -1,    65,    66,    -1,    67,
      -1,    71,    -1,    72,    -1,    73,    -1,    75,    -1,    77,
      -1,    78,    -1,    79,    -1,    22,    80,    53,    -1,    22,
      53,    -1,    70,    68,    53,    -1,    69,    -1,    68,    54,
      69,    -1,    31,    -1,    31,    49,    80,    -1,     3,    -1,
       4,    -1,     5,    -1,     6,    -1,     7,    -1,     8,    -1,
       9,    -1,    10,    -1,    11,    -1,    31,    49,    80,    53,
      -1,    31,    40,    80,    53,    -1,    31,    41,    80,    53,
      -1,    31,    42,    80,    53,    -1,    31,    43,    80,    53,
      -1,    80,    53,    -1,    23,    55,    29,    74,    56,    53,
      -1,    -1,    54,    76,    -1,    24,    55,    29,    54,    76,
      56,    53,    -1,    80,    -1,    76,    54,    80,    -1,    12,
      55,    80,    56,    66,    -1,    12,    55,    80,    56,    66,
      13,    66,    -1,    14,    55,    80,    56,    66,    -1,    57,
      65,    58,    -1,    27,    -1,    28,    -1,    29,    -1,    30,
      -1,    25,    -1,    26,    -1,    31,    -1,    55,    80,    56,
      -1,    80,    44,    80,    -1,    80,    45,    80,    -1,    80,
      46,    80,    -1,    80,    47,    80,    -1,    80,    48,    80,
      -1,    80,    50,    80,    -1,    80,    51,    80,    -1,    80,
      34,    80,    -1,    80,    35,    80,    -1,    80,    32,    80,
      -1,    80,    33,    80,    -1,    80,    36,    80,    -1,    80,
      37,    80,    -1,    52,    80,    -1,    45,    80,    -1,    31,
      38,    -1,    31,    39,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    69,    69,    82,    85,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   110,   117,   126,   131,   139,   143,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   163,
     169,   175,   181,   187,   196,   204,   214,   217,   224,   233,
     238,   246,   252,   262,   271,   281,   283,   285,   287,   289,
     291,   293,   295,   297,   303,   309,   315,   321,   327,   333,
     339,   345,   351,   357,   363,   369,   375,   380,   385,   390
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLOAT_TYPE", "DOUBLE", "CHAR",
  "VOID", "SHORT", "LONG", "SIGNED", "UNSIGNED", "IF", "ELSE", "WHILE",
  "FOR", "DO", "SWITCH", "CASE", "DEFAULT", "BREAK", "CONTINUE", "RETURN",
  "PRINTF", "SCANF", "TRUE", "FALSE", "INTEGER_LITERAL", "FLOAT_LITERAL",
  "STRING_LITERAL", "CHAR_LITERAL", "IDENTIFIER_TOKEN", "EQUAL",
  "NOT_EQUAL", "LESS_EQUAL", "GREATER_EQUAL", "LOGICAL_AND", "LOGICAL_OR",
  "INCREMENT", "DECREMENT", "PLUS_ASSIGN", "MINUS_ASSIGN",
  "MULTIPLY_ASSIGN", "DIVIDE_ASSIGN", "PLUS", "MINUS", "MULTIPLY",
  "DIVIDE", "MODULO", "ASSIGN", "LESS", "GREATER", "LOGICAL_NOT",
  "SEMICOLON", "COMMA", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET",
  "RBRACKET", "UMINUS", "LOWER_THAN_ELSE", "$accept", "program",
  "statement_list", "statement", "declaration", "declarator_list",
  "declarator", "type_specifier", "assignment", "expression_statement",
  "print_statement", "print_arguments", "input_statement", "argument_list",
  "if_statement", "while_statement", "block", "expression", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    63,    64,    65,    65,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    67,    68,    68,    69,    69,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    71,
      71,    71,    71,    71,    72,    73,    74,    74,    75,    76,
      76,    77,    77,    78,    79,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     3,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     4,     4,     4,     2,     6,     0,     2,     7,     1,
       3,     5,     7,     5,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,     0,     0,     0,     0,     0,    49,    50,
      45,    46,    47,    48,    51,     0,     0,     0,     3,     4,
       5,     0,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    51,    14,     0,     0,     0,    68,    69,     0,
       0,     0,     0,     0,    67,    66,     0,     0,    18,     0,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,     0,    13,    36,     0,
       0,     0,     0,     0,     0,    52,    44,     0,    15,     0,
      62,    63,    60,    61,    64,    65,    53,    54,    55,    56,
      57,    58,    59,     0,     0,     0,     0,     0,    30,    31,
      32,    33,    29,    19,    17,    41,    43,    37,    39,     0,
       0,     0,     0,    35,     0,    42,    40,    38
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    29,    30,    59,    60,    31,    32,    33,
      34,   106,    35,   117,    36,    37,    38,    39
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -81
static const yytype_int16 yypact[] =
{
     -81,    27,   112,   -81,   -81,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -25,   -23,   -24,   -16,   -13,   -81,   -81,
     -81,   -81,   -81,   -81,    55,   -12,   -12,   -12,   -81,   -81,
     -81,    38,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   190,
     -12,   -12,   -30,   -81,   212,    42,    59,   -81,   -81,   -12,
     -12,   -12,   -12,   -12,   -81,   -81,   115,    56,    40,    -9,
     -81,   -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,
     -12,   -12,   -12,   -12,   -81,   140,   165,   -81,    37,    45,
     234,   256,   278,   300,   322,   -81,   -81,   -12,   -81,    38,
     390,   390,    29,    29,   372,   364,    81,    81,   -81,   -81,
     -81,    29,    29,   112,   112,   -12,    44,   -12,   -81,   -81,
     -81,   -81,   -81,   344,   -81,    89,   -81,    49,   344,    52,
     -34,   112,   -12,   -81,    53,   -81,   344,   -81
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -81,   -81,    82,   -80,   -81,   -81,    20,   -81,   -81,   -81,
     -81,   -81,   -81,     5,   -81,   -81,   -81,   -15
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      44,    18,    19,    20,    21,    22,    23,    42,    47,    48,
      54,    55,    56,    18,    19,    20,    21,    22,    23,    42,
     122,    25,   124,   115,   116,    75,    76,     3,    26,    43,
      40,    27,    41,    25,    80,    81,    82,    83,    84,    45,
      26,   125,    46,    27,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    58,
      14,    78,   113,    67,    68,    69,    70,    71,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    79,    87,
     118,   105,   118,    47,    48,    49,    50,    51,    52,   107,
     119,    25,   121,   122,    53,   123,   127,   126,    26,   114,
      57,    27,   120,    28,    86,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,     0,    14,    69,    70,    71,
       0,     0,     0,     0,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,     0,     0,    61,    62,    63,
      64,    65,    66,     0,     0,     0,     0,    25,     0,    67,
      68,    69,    70,    71,    26,    72,    73,    27,     0,    28,
       0,    85,    61,    62,    63,    64,    65,    66,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,     0,
      72,    73,     0,     0,     0,     0,   103,    61,    62,    63,
      64,    65,    66,     0,     0,     0,     0,     0,     0,    67,
      68,    69,    70,    71,     0,    72,    73,     0,     0,     0,
       0,   104,    61,    62,    63,    64,    65,    66,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,     0,
      72,    73,     0,    74,    61,    62,    63,    64,    65,    66,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,     0,    72,    73,     0,    77,    61,    62,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,     0,    72,    73,     0,   108,    61,    62,
      63,    64,    65,    66,     0,     0,     0,     0,     0,     0,
      67,    68,    69,    70,    71,     0,    72,    73,     0,   109,
      61,    62,    63,    64,    65,    66,     0,     0,     0,     0,
       0,     0,    67,    68,    69,    70,    71,     0,    72,    73,
       0,   110,    61,    62,    63,    64,    65,    66,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,     0,
      72,    73,     0,   111,    61,    62,    63,    64,    65,    66,
       0,     0,     0,     0,     0,     0,    67,    68,    69,    70,
      71,     0,    72,    73,     0,   112,    61,    62,    63,    64,
      65,    66,     0,     0,     0,     0,     0,     0,    67,    68,
      69,    70,    71,     0,    72,    73,    61,    62,    63,    64,
      65,     0,     0,     0,    61,    62,    63,    64,    67,    68,
      69,    70,    71,     0,    72,    73,    67,    68,    69,    70,
      71,     0,    72,    73,    63,    64,     0,     0,     0,     0,
       0,     0,     0,     0,    67,    68,    69,    70,    71,     0,
      72,    73
};

static const yytype_int8 yycheck[] =
{
      15,    25,    26,    27,    28,    29,    30,    31,    38,    39,
      25,    26,    27,    25,    26,    27,    28,    29,    30,    31,
      54,    45,    56,   103,   104,    40,    41,     0,    52,    53,
      55,    55,    55,    45,    49,    50,    51,    52,    53,    55,
      52,   121,    55,    55,    53,    54,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    31,
      14,    29,    87,    44,    45,    46,    47,    48,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    29,    49,
     105,    54,   107,    38,    39,    40,    41,    42,    43,    54,
      56,    45,    13,    54,    49,    53,    53,   122,    52,    89,
      28,    55,   107,    57,    58,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    -1,    14,    46,    47,    48,
      -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    32,    33,    34,
      35,    36,    37,    -1,    -1,    -1,    -1,    45,    -1,    44,
      45,    46,    47,    48,    52,    50,    51,    55,    -1,    57,
      -1,    56,    32,    33,    34,    35,    36,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,    -1,
      50,    51,    -1,    -1,    -1,    -1,    56,    32,    33,    34,
      35,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      45,    46,    47,    48,    -1,    50,    51,    -1,    -1,    -1,
      -1,    56,    32,    33,    34,    35,    36,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,    -1,
      50,    51,    -1,    53,    32,    33,    34,    35,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    47,
      48,    -1,    50,    51,    -1,    53,    32,    33,    34,    35,
      36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    44,    45,
      46,    47,    48,    -1,    50,    51,    -1,    53,    32,    33,
      34,    35,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    45,    46,    47,    48,    -1,    50,    51,    -1,    53,
      32,    33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    45,    46,    47,    48,    -1,    50,    51,
      -1,    53,    32,    33,    34,    35,    36,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,    -1,
      50,    51,    -1,    53,    32,    33,    34,    35,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    47,
      48,    -1,    50,    51,    -1,    53,    32,    33,    34,    35,
      36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    44,    45,
      46,    47,    48,    -1,    50,    51,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    32,    33,    34,    35,    44,    45,
      46,    47,    48,    -1,    50,    51,    44,    45,    46,    47,
      48,    -1,    50,    51,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    45,    46,    47,    48,    -1,
      50,    51
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    64,    65,     0,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    14,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    45,    52,    55,    57,    66,
      67,    70,    71,    72,    73,    75,    77,    78,    79,    80,
      55,    55,    31,    53,    80,    55,    55,    38,    39,    40,
      41,    42,    43,    49,    80,    80,    80,    65,    31,    68,
      69,    32,    33,    34,    35,    36,    37,    44,    45,    46,
      47,    48,    50,    51,    53,    80,    80,    53,    29,    29,
      80,    80,    80,    80,    80,    56,    58,    49,    53,    54,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    56,    56,    54,    74,    54,    53,    53,
      53,    53,    53,    80,    69,    66,    66,    76,    80,    56,
      76,    13,    54,    53,    56,    66,    80,    53
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1464 of yacc.c  */
#line 70 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_PROGRAM, "Program");

        if ((yyvsp[(1) - (1)].node) != NULL)
            ast_add_child((yyval.node), (yyvsp[(1) - (1)].node));

        ast_root = (yyval.node);
    ;}
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 82 "src/c/parser/parser.y"
    {
        (yyval.node) = NULL;
    ;}
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 86 "src/c/parser/parser.y"
    {
        if ((yyvsp[(1) - (2)].node) == NULL)
            (yyval.node) = ast_create(AST_BLOCK, "Statements");
        else
            (yyval.node) = (yyvsp[(1) - (2)].node);

        ast_add_child((yyval.node), (yyvsp[(2) - (2)].node));
    ;}
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 97 "src/c/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 98 "src/c/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 99 "src/c/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 100 "src/c/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 101 "src/c/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 102 "src/c/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 103 "src/c/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 104 "src/c/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 106 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_RETURN, "return");
          ast_add_child((yyval.node), (yyvsp[(2) - (3)].node));
      ;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 111 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_RETURN, "return");
      ;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 118 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_DECLARATION, "Declaration");
        ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
        ast_add_child((yyval.node), (yyvsp[(2) - (3)].node));
    ;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 127 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_BLOCK, "Variables");
        ast_add_child((yyval.node), (yyvsp[(1) - (1)].node));
    ;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 132 "src/c/parser/parser.y"
    {
        (yyval.node) = (yyvsp[(1) - (3)].node);
        ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 140 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_VARIABLE, (yyvsp[(1) - (1)].text));
    ;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 144 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_VARIABLE, (yyvsp[(1) - (3)].text));
        ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 151 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_TYPE, "int"); ;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 152 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_TYPE, "float"); ;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 153 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_TYPE, "double"); ;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 154 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_TYPE, "char"); ;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 155 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_TYPE, "void"); ;}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 156 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_TYPE, "short"); ;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 157 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_TYPE, "long"); ;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 158 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_TYPE, "signed"); ;}
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 159 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_TYPE, "unsigned"); ;}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 164 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_ASSIGNMENT, "=");
          ast_add_child((yyval.node), ast_create(AST_IDENTIFIER, (yyvsp[(1) - (4)].text)));
          ast_add_child((yyval.node), (yyvsp[(3) - (4)].node));
      ;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 170 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_ASSIGNMENT, "+=");
          ast_add_child((yyval.node), ast_create(AST_IDENTIFIER, (yyvsp[(1) - (4)].text)));
          ast_add_child((yyval.node), (yyvsp[(3) - (4)].node));
      ;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 176 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_ASSIGNMENT, "-=");
          ast_add_child((yyval.node), ast_create(AST_IDENTIFIER, (yyvsp[(1) - (4)].text)));
          ast_add_child((yyval.node), (yyvsp[(3) - (4)].node));
      ;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 182 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_ASSIGNMENT, "*=");
          ast_add_child((yyval.node), ast_create(AST_IDENTIFIER, (yyvsp[(1) - (4)].text)));
          ast_add_child((yyval.node), (yyvsp[(3) - (4)].node));
      ;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 188 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_ASSIGNMENT, "/=");
          ast_add_child((yyval.node), ast_create(AST_IDENTIFIER, (yyvsp[(1) - (4)].text)));
          ast_add_child((yyval.node), (yyvsp[(3) - (4)].node));
      ;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 197 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_EXPRESSION_STATEMENT, "Expression");
        ast_add_child((yyval.node), (yyvsp[(1) - (2)].node));
    ;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 205 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_PRINT, "printf");
        ast_add_child((yyval.node), ast_create(AST_STRING, (yyvsp[(3) - (6)].text)));
        ast_add_child((yyval.node), (yyvsp[(4) - (6)].node));
    ;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 214 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_BLOCK, "Arguments");
    ;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 218 "src/c/parser/parser.y"
    {
        (yyval.node) = (yyvsp[(2) - (2)].node);
    ;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 225 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_PRINT, "scanf");
        ast_add_child((yyval.node), ast_create(AST_STRING, (yyvsp[(3) - (7)].text)));
        ast_add_child((yyval.node), (yyvsp[(5) - (7)].node));
    ;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 234 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_BLOCK, "Arguments");
        ast_add_child((yyval.node), (yyvsp[(1) - (1)].node));
    ;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 239 "src/c/parser/parser.y"
    {
        (yyval.node) = (yyvsp[(1) - (3)].node);
        ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
    ;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 247 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_IF, "if");
        ast_add_child((yyval.node), (yyvsp[(3) - (5)].node));
        ast_add_child((yyval.node), (yyvsp[(5) - (5)].node));
    ;}
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 253 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_IF, "if-else");
        ast_add_child((yyval.node), (yyvsp[(3) - (7)].node));
        ast_add_child((yyval.node), (yyvsp[(5) - (7)].node));
        ast_add_child((yyval.node), (yyvsp[(7) - (7)].node));
    ;}
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 263 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_WHILE, "while");
        ast_add_child((yyval.node), (yyvsp[(3) - (5)].node));
        ast_add_child((yyval.node), (yyvsp[(5) - (5)].node));
    ;}
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 272 "src/c/parser/parser.y"
    {
        (yyval.node) = ast_create(AST_BLOCK, "Block");

        if ((yyvsp[(2) - (3)].node) != NULL)
            ast_add_child((yyval.node), (yyvsp[(2) - (3)].node));
    ;}
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 282 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_INTEGER, (yyvsp[(1) - (1)].text)); ;}
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 284 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_FLOAT, (yyvsp[(1) - (1)].text)); ;}
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 286 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_STRING, (yyvsp[(1) - (1)].text)); ;}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 288 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_CHAR, (yyvsp[(1) - (1)].text)); ;}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 290 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_IDENTIFIER, "true"); ;}
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 292 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_IDENTIFIER, "false"); ;}
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 294 "src/c/parser/parser.y"
    { (yyval.node) = ast_create(AST_IDENTIFIER, (yyvsp[(1) - (1)].text)); ;}
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 296 "src/c/parser/parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 298 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "+");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 304 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "-");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 310 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "*");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 316 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "/");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 322 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "%");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 328 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "<");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 334 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, ">");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 340 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "<=");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 346 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, ">=");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 352 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "==");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 358 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "!=");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 364 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "&&");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 370 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_BINARY_EXPR, "||");
          ast_add_child((yyval.node), (yyvsp[(1) - (3)].node));
          ast_add_child((yyval.node), (yyvsp[(3) - (3)].node));
      ;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 376 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_UNARY_EXPR, "!");
          ast_add_child((yyval.node), (yyvsp[(2) - (2)].node));
      ;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 381 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_UNARY_EXPR, "-");
          ast_add_child((yyval.node), (yyvsp[(2) - (2)].node));
      ;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 386 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_UNARY_EXPR, "++");
          ast_add_child((yyval.node), ast_create(AST_IDENTIFIER, (yyvsp[(1) - (2)].text)));
      ;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 391 "src/c/parser/parser.y"
    {
          (yyval.node) = ast_create(AST_UNARY_EXPR, "--");
          ast_add_child((yyval.node), ast_create(AST_IDENTIFIER, (yyvsp[(1) - (2)].text)));
      ;}
    break;



/* Line 1464 of yacc.c  */
#line 2222 "src/c/parser/parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1684 of yacc.c  */
#line 397 "src/c/parser/parser.y"


void yyerror(const char *message)
{
    fprintf(stderr, "Syntax Error: %s\n", message);
}


int main(void)
{
    printf("C Compiler Parser Started\n");

    if (yyparse() == 0)
    {
        printf("\nSyntax Analysis Successful!\n");

        printf("\n========== ABSTRACT SYNTAX TREE ==========\n");
        ast_print(ast_root);

        SymbolTable table;

        printf("\nStarting Semantic Analysis...\n");

        semantic_analyze(ast_root, &table);

        symbol_table_print(&table);

        printf("\nStarting TAC Generation...\n");

tac_generate(ast_root);
tac_print();
tac_free();

        ast_free(ast_root);
    }
    else
    {
        printf("\nSyntax Analysis Failed!\n");
    }

    return 0;
}

