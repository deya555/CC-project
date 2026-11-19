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
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 16 "src/parser/parser.y"

#include <cstdio>
#include <cstdlib>
#include <string>
#include "ast.h"
#include "diagnostics.h"

extern int yylex();
extern int yylineno;
extern int g_lastLexErrorLine;
void yyerror(const char* msg);

BlockNode* g_program = NULL;   /* root of the AST, read by main */

/* Facts about 'main' collected while parsing; main.cpp turns them into errors. */
int   g_mainCount = 0;         /* how many main() definitions were found */
int   g_secondMainLine = 0;    /* line of the 2nd definition (redefinition error) */
int   g_mainVoidLine = 0;      /* line of 'void main' (main must return int) */
Node* g_mainBlock = NULL;      /* the body block of the first main() */

static Node* registerMain(Node* body, int line, bool isVoid) {
    g_mainCount++;
    if (g_mainCount == 1) g_mainBlock = body;
    else if (!g_secondMainLine) g_secondMainLine = line;
    if (isVoid && !g_mainVoidLine) g_mainVoidLine = line;
    return body;
}

/* build  name = name <op> rhs  (used by +=, -=, ++ ...) */
static Node* compound(const char* name, const char* op, Node* rhs, int line) {
    return new AssignNode(name, new BinaryNode(op, new IdentNode(name, line), rhs, line), line);
}

/* give every declarator of 'int a, b = 2;' its type; a single declarator stays a plain VarDecl */
static Node* finishDecl(Type t, BlockNode* group) {
    for (size_t i = 0; i < group->stmts.size(); i++)
        static_cast<VarDeclNode*>(group->stmts[i])->declType = t;
    return group->stmts.size() == 1 ? group->stmts[0] : group;
}


/* Line 189 of yacc.c  */
#line 114 "build/parser.tab.cpp"

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

/* "%code requires" blocks.  */

/* Line 209 of yacc.c  */
#line 60 "src/parser/parser.y"

#include "ast.h"



/* Line 209 of yacc.c  */
#line 144 "build/parser.tab.cpp"

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

/* Line 214 of yacc.c  */
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



/* Line 214 of yacc.c  */
#line 223 "build/parser.tab.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

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


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 248 "build/parser.tab.cpp"

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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYLAST   674

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  66
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNRULES -- Number of states.  */
#define YYNSTATES  219

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   303

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    57,     2,     2,     2,    56,     2,     2,
      62,    63,    54,    52,    64,    53,     2,    55,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    49,    59,
      50,    65,    51,    48,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    60,     2,    61,     2,     2,     2,     2,
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
      45,    46,    47,    58
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    27,    29,    31,    33,    35,    38,
      41,    43,    46,    50,    55,    61,    68,    74,    79,    81,
      83,    85,    87,    89,    92,    94,    98,   103,   105,   109,
     111,   115,   118,   122,   126,   130,   134,   138,   141,   144,
     147,   150,   156,   164,   170,   178,   188,   189,   191,   196,
     197,   199,   200,   202,   210,   211,   217,   222,   224,   227,
     231,   234,   238,   243,   245,   247,   249,   257,   267,   271,
     275,   279,   284,   286,   290,   296,   300,   304,   308,   312,
     316,   320,   324,   328,   332,   336,   340,   344,   348,   351,
     354,   358,   360,   362,   364,   366
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      67,     0,    -1,    68,    -1,    -1,    68,    69,    -1,    73,
      -1,    76,    -1,    78,    -1,    79,    -1,    80,    -1,    81,
      -1,    88,    -1,    89,    -1,    71,    -1,    70,    -1,    90,
      -1,    92,    -1,    85,    -1,    17,    59,    -1,    18,    59,
      -1,    59,    -1,     1,    59,    -1,    60,    68,    61,    -1,
      60,    68,     1,    61,    -1,     3,    20,    62,    63,    70,
      -1,     3,    20,    62,    21,    63,    70,    -1,    21,    20,
      62,    63,    70,    -1,    20,    62,    63,    70,    -1,     3,
      -1,     4,    -1,    23,    -1,     5,    -1,    24,    -1,    24,
      24,    -1,    25,    -1,    72,    74,    59,    -1,    22,    72,
      74,    59,    -1,    75,    -1,    74,    64,    75,    -1,    39,
      -1,    39,    65,    94,    -1,    77,    59,    -1,    39,    65,
      94,    -1,    39,    33,    94,    -1,    39,    34,    94,    -1,
      39,    35,    94,    -1,    39,    36,    94,    -1,    39,    31,
      -1,    39,    32,    -1,    31,    39,    -1,    32,    39,    -1,
       6,    62,    94,    63,    69,    -1,     6,    62,    94,    63,
      69,     7,    69,    -1,     8,    62,    94,    63,    69,    -1,
      13,    69,     8,    62,    94,    63,    59,    -1,    12,    62,
      82,    59,    83,    59,    84,    63,    69,    -1,    -1,    77,
      -1,    72,    39,    65,    94,    -1,    -1,    94,    -1,    -1,
      77,    -1,    14,    62,    94,    63,    60,    86,    61,    -1,
      -1,    86,    15,    87,    49,    68,    -1,    86,    16,    49,
      68,    -1,    37,    -1,    53,    37,    -1,     9,    94,    59,
      -1,    19,    59,    -1,    19,    94,    59,    -1,    26,    29,
      91,    59,    -1,    94,    -1,    40,    -1,    28,    -1,    62,
      94,    48,    40,    49,    40,    63,    -1,    91,    29,    62,
      94,    48,    40,    49,    40,    63,    -1,    91,    29,    94,
      -1,    91,    29,    40,    -1,    91,    29,    28,    -1,    27,
      30,    93,    59,    -1,    39,    -1,    93,    30,    39,    -1,
      94,    48,    94,    49,    94,    -1,    94,    46,    94,    -1,
      94,    45,    94,    -1,    94,    43,    94,    -1,    94,    44,
      94,    -1,    94,    50,    94,    -1,    94,    51,    94,    -1,
      94,    41,    94,    -1,    94,    42,    94,    -1,    94,    52,
      94,    -1,    94,    53,    94,    -1,    94,    54,    94,    -1,
      94,    55,    94,    -1,    94,    56,    94,    -1,    57,    94,
      -1,    53,    94,    -1,    62,    94,    63,    -1,    37,    -1,
      38,    -1,    10,    -1,    11,    -1,    39,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   109,   109,   113,   114,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   138,   139,   144,   145,   146,   147,   153,   154,
     155,   156,   157,   158,   159,   163,   164,   168,   169,   173,
     174,   178,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   195,   197,   202,   206,   211,   221,   222,   223,   227,
     228,   232,   233,   237,   242,   243,   249,   258,   259,   263,
     267,   268,   273,   277,   280,   283,   286,   290,   293,   295,
     297,   303,   307,   308,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KW_INT", "KW_FLOAT", "KW_BOOL", "KW_IF",
  "KW_ELSE", "KW_WHILE", "KW_PRINT", "KW_TRUE", "KW_FALSE", "KW_FOR",
  "KW_DO", "KW_SWITCH", "KW_CASE", "KW_DEFAULT", "KW_BREAK", "KW_CONTINUE",
  "KW_RETURN", "KW_MAIN", "KW_VOID", "KW_CONST", "KW_DOUBLE", "KW_LONG",
  "KW_SHORT", "KW_COUT", "KW_CIN", "KW_ENDL", "SHL", "SHR", "INC", "DEC",
  "ADDEQ", "SUBEQ", "MULEQ", "DIVEQ", "INT_LIT", "FLOAT_LIT", "IDENTIFIER",
  "STRING_LIT", "LE", "GE", "EQ", "NE", "AND", "OR", "LOWER_THAN_ELSE",
  "'?'", "':'", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'",
  "UMINUS", "';'", "'{'", "'}'", "'('", "')'", "','", "'='", "$accept",
  "program", "stmt_list", "stmt", "block", "main_def", "type", "decl",
  "declarators", "declarator", "assign_stmt", "simple_assign", "if_stmt",
  "while_stmt", "do_stmt", "for_stmt", "for_init", "for_cond", "for_step",
  "switch_stmt", "case_list", "case_value", "print_stmt", "return_stmt",
  "cout_stmt", "cout_list", "cin_stmt", "cin_list", "expr", 0
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
     295,   296,   297,   298,   299,   300,   301,   302,    63,    58,
      60,    62,    43,    45,    42,    47,    37,    33,   303,    59,
     123,   125,    40,    41,    44,    61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    66,    67,    68,    68,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    69,
      69,    69,    70,    70,    71,    71,    71,    71,    72,    72,
      72,    72,    72,    72,    72,    73,    73,    74,    74,    75,
      75,    76,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    78,    78,    79,    80,    81,    82,    82,    82,    83,
      83,    84,    84,    85,    86,    86,    86,    87,    87,    88,
      89,    89,    90,    91,    91,    91,    91,    91,    91,    91,
      91,    92,    93,    93,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     2,     3,     4,     5,     6,     5,     4,     1,     1,
       1,     1,     1,     2,     1,     3,     4,     1,     3,     1,
       3,     2,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     5,     7,     5,     7,     9,     0,     1,     4,     0,
       1,     0,     1,     7,     0,     5,     4,     1,     2,     3,
       2,     3,     4,     1,     1,     1,     7,     9,     3,     3,
       3,     4,     1,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       3,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,     0,    28,    29,    31,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      30,    32,    34,     0,     0,     0,     0,     0,    20,     3,
       4,    14,    13,     0,     5,     6,     0,     7,     8,     9,
      10,    17,    11,    12,    15,    16,    21,     0,     0,     0,
     103,   104,   101,   102,   105,     0,     0,     0,     0,    56,
       0,     0,    18,    19,    70,     0,     0,     0,    28,     0,
      33,     0,     0,    49,    50,    47,    48,     0,     0,     0,
       0,     0,     0,    39,     0,    37,    41,     0,     0,     0,
      99,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,     0,    57,
       0,     0,     0,    71,     0,     0,     0,    75,    74,     0,
       0,    73,    82,     0,    43,    44,    45,    46,    42,     0,
      22,     0,    35,     0,     0,     0,     0,     0,   100,    91,
      92,    87,    88,    86,    85,     0,    89,    90,    93,    94,
      95,    96,    97,     0,    59,     0,     0,    27,     0,    36,
       0,     0,    72,     0,    81,    23,    40,    38,     0,    24,
      51,    53,     0,     0,     0,    60,     0,    64,    26,     0,
      80,    79,     0,    78,    83,    25,     0,    84,    58,    61,
       0,     0,     0,     0,    52,    62,     0,    54,     0,     0,
      63,     0,     0,     0,    67,     0,     0,     3,     0,     0,
      55,    68,     3,     0,    76,     0,     0,     0,    77
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    30,    31,    32,    33,    34,    84,    85,
      35,    36,    37,    38,    39,    40,   110,   174,   196,    41,
     191,   206,    42,    43,    44,   120,    45,   123,   145
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -58
static const yytype_int16 yypact[] =
{
     -58,     4,   246,   -58,   -43,    -1,   -58,   -58,   -15,    -3,
     423,     5,   351,    16,    12,    21,   356,    19,    66,    80,
     -58,    63,   -58,    77,    79,    74,    88,    29,   -58,   -58,
     -58,   -58,   -58,    94,   -58,   -58,    57,   -58,   -58,   -58,
     -58,   -58,   -58,   -58,   -58,   -58,   -58,    72,   423,   423,
     -58,   -58,   -58,   -58,   -58,   423,   423,   423,   541,   209,
     128,   423,   -58,   -58,   -58,   560,    78,    81,   -58,    94,
     -58,   100,   103,   -58,   -58,   -58,   -58,   423,   423,   423,
     423,   423,   290,    83,   -22,   -58,   -58,   -18,   282,   445,
     -58,   -58,   461,   423,   423,   423,   423,   423,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   -58,   112,   -58,
      93,    92,   477,   -58,    95,    96,    18,   -58,   -58,   423,
     -20,   595,   -58,   -10,   595,   595,   595,   595,   595,    -4,
     -58,   423,   -58,    94,    98,    95,   351,   351,   -58,    67,
      67,   618,   618,   611,   414,   579,    67,    67,    75,    75,
     -58,   -58,   -58,   101,   423,   423,   105,   -58,    95,   -58,
     493,   107,   -58,   119,   -58,   -58,   595,   -58,    95,   -58,
     156,   -58,   423,   423,   111,   595,   509,   -58,   -58,   386,
     -58,   -58,   423,   595,   -58,   -58,   351,   595,   595,    76,
     113,    -8,   122,   525,   -58,   -58,   110,   -58,     1,   126,
     -58,   136,   392,   351,   -58,   141,   131,   -58,   118,   133,
     -58,   -58,   -58,     9,   -58,   143,   184,   123,   -58
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -58,   -58,   -28,   -12,   -56,   -58,   -13,   -58,   125,    62,
     -58,   -57,   -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,
     -58,   -58,   -58,   -58,   -58,   -58,   -58,   -58,    -5
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -67
static const yytype_int16 yytable[] =
{
      60,    82,   109,   134,     3,    58,    69,   198,   199,   161,
       4,    65,     5,     6,     7,     8,    46,     9,    10,    47,
     163,    11,    12,    13,   -66,   -66,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,   132,   204,   162,
      25,    26,   133,    88,    89,   135,   108,    48,    27,   164,
      90,    91,    92,   200,   205,    46,   112,   165,   157,    49,
      75,    76,    77,    78,    79,    80,   121,    59,    28,    29,
     -66,    62,   124,   125,   126,   127,   128,   159,    61,   169,
      63,    66,   133,    68,     6,     7,    67,    70,   139,   140,
     141,   142,   143,   144,    81,   146,   147,   148,   149,   150,
     151,   152,   178,    20,    21,    22,    71,    25,    26,    72,
      50,    51,   185,    73,   160,    27,    86,    50,    51,   102,
     103,   104,   105,   106,   170,   171,   166,    74,   117,   104,
     105,   106,   195,    83,    87,   180,   111,    52,    53,    54,
     118,   114,   122,   115,    52,    53,    54,   181,   131,   175,
     176,   153,   154,    55,   155,    29,   183,    56,   184,   158,
      55,   168,   119,   186,    56,   177,   173,   187,   188,   182,
     189,   201,   197,   203,   194,   207,   208,   193,   211,   213,
     212,   214,   215,   217,   216,     4,   218,     5,     6,     7,
       8,   210,     9,    10,   116,   167,    11,    12,    13,   -65,
     -65,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    68,     6,     7,    25,    26,     0,     0,     0,
       0,     0,     0,    27,     0,     0,     0,     0,     0,     0,
       0,     0,    20,    21,    22,     0,     0,     0,     0,     0,
      25,    26,     0,    28,    29,   -65,    -2,     4,    27,     5,
       6,     7,     8,     0,     9,    10,     0,     0,    11,    12,
      13,     0,     0,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,     0,     0,    25,    26,     0,
       0,     0,     0,     0,     0,    27,     0,     0,     0,     0,
       0,   129,     0,     5,     6,     7,     8,     0,     9,    10,
       0,     0,    11,    12,    13,    28,    29,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,     0,
       0,    25,    26,    93,    94,    95,    96,    97,    98,    27,
      99,     0,   100,   101,   102,   103,   104,   105,   106,     0,
       0,     0,     0,     0,     0,   136,     0,     0,     0,    28,
      29,   130,     4,     0,     5,     6,     7,     8,     0,     9,
      10,     0,     0,    11,    12,    13,    50,    51,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,     0,    25,    26,     0,     0,     0,     0,     0,     0,
      27,     0,     0,    52,    53,    54,    50,    51,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,     0,    55,
      28,    29,     0,    56,     0,    64,     0,     0,    57,     0,
       0,     0,     0,    52,    53,    54,   192,     0,     0,    52,
      53,    54,   209,    50,    51,     0,     0,     0,     0,    55,
       0,     0,     0,    56,     0,    55,     0,     0,    57,    56,
       0,     0,     0,     0,    57,    93,    94,    95,    96,    97,
      52,    53,    54,     0,   100,   101,   102,   103,   104,   105,
     106,     0,     0,     0,     0,     0,    55,     0,     0,     0,
      56,     0,     0,     0,     0,    57,    93,    94,    95,    96,
      97,    98,     0,    99,     0,   100,   101,   102,   103,   104,
     105,   106,    93,    94,    95,    96,    97,    98,   137,    99,
       0,   100,   101,   102,   103,   104,   105,   106,    93,    94,
      95,    96,    97,    98,   138,    99,     0,   100,   101,   102,
     103,   104,   105,   106,    93,    94,    95,    96,    97,    98,
     156,   179,     0,   100,   101,   102,   103,   104,   105,   106,
      93,    94,    95,    96,    97,    98,   138,    99,     0,   100,
     101,   102,   103,   104,   105,   106,    93,    94,    95,    96,
      97,    98,   190,   202,     0,   100,   101,   102,   103,   104,
     105,   106,    93,    94,    95,    96,    97,    98,   138,    99,
       0,   100,   101,   102,   103,   104,   105,   106,     0,     0,
     107,    93,    94,    95,    96,    97,    98,     0,    99,     0,
     100,   101,   102,   103,   104,   105,   106,     0,     0,   113,
      93,    94,    95,    96,    97,    98,     0,    99,   172,   100,
     101,   102,   103,   104,   105,   106,    93,    94,    95,    96,
      97,    98,     0,    99,     0,   100,   101,   102,   103,   104,
     105,   106,    93,    94,    95,    96,     0,     0,     0,    93,
      94,   100,   101,   102,   103,   104,   105,   106,   100,   101,
     102,   103,   104,   105,   106
};

static const yytype_int16 yycheck[] =
{
      12,    29,    59,    21,     0,    10,    19,    15,    16,    29,
       1,    16,     3,     4,     5,     6,    59,     8,     9,    20,
      30,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    59,    37,    59,
      31,    32,    64,    48,    49,    63,    59,    62,    39,    59,
      55,    56,    57,    61,    53,    59,    61,    61,   114,    62,
      31,    32,    33,    34,    35,    36,    71,    62,    59,    60,
      61,    59,    77,    78,    79,    80,    81,    59,    62,   135,
      59,    62,    64,     3,     4,     5,    20,    24,    93,    94,
      95,    96,    97,    98,    65,   100,   101,   102,   103,   104,
     105,   106,   158,    23,    24,    25,    29,    31,    32,    30,
      10,    11,   168,    39,   119,    39,    59,    10,    11,    52,
      53,    54,    55,    56,   136,   137,   131,    39,    28,    54,
      55,    56,   189,    39,    62,    28,     8,    37,    38,    39,
      40,    63,    39,    62,    37,    38,    39,    40,    65,   154,
     155,    39,    59,    53,    62,    60,   161,    57,    39,    63,
      53,    63,    62,     7,    57,    60,    65,   172,   173,    62,
      59,    49,    59,    63,   186,    49,    40,   182,    37,   207,
      49,    63,    49,    40,   212,     1,    63,     3,     4,     5,
       6,   203,     8,     9,    69,   133,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     3,     4,     5,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    59,    60,    61,     0,     1,    39,     3,
       4,     5,     6,    -1,     8,     9,    -1,    -1,    12,    13,
      14,    -1,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,     5,     6,    -1,     8,     9,
      -1,    -1,    12,    13,    14,    59,    60,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    -1,    -1,
      -1,    31,    32,    41,    42,    43,    44,    45,    46,    39,
      48,    -1,    50,    51,    52,    53,    54,    55,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    59,
      60,    61,     1,    -1,     3,     4,     5,     6,    -1,     8,
       9,    -1,    -1,    12,    13,    14,    10,    11,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    37,    38,    39,    10,    11,    -1,    -1,
      -1,    -1,    10,    11,    -1,    -1,    -1,    -1,    -1,    53,
      59,    60,    -1,    57,    -1,    59,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    37,    38,    39,    40,    -1,    -1,    37,
      38,    39,    40,    10,    11,    -1,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    57,    -1,    53,    -1,    -1,    62,    57,
      -1,    -1,    -1,    -1,    62,    41,    42,    43,    44,    45,
      37,    38,    39,    -1,    50,    51,    52,    53,    54,    55,
      56,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    62,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    50,    51,    52,    53,    54,
      55,    56,    41,    42,    43,    44,    45,    46,    63,    48,
      -1,    50,    51,    52,    53,    54,    55,    56,    41,    42,
      43,    44,    45,    46,    63,    48,    -1,    50,    51,    52,
      53,    54,    55,    56,    41,    42,    43,    44,    45,    46,
      63,    48,    -1,    50,    51,    52,    53,    54,    55,    56,
      41,    42,    43,    44,    45,    46,    63,    48,    -1,    50,
      51,    52,    53,    54,    55,    56,    41,    42,    43,    44,
      45,    46,    63,    48,    -1,    50,    51,    52,    53,    54,
      55,    56,    41,    42,    43,    44,    45,    46,    63,    48,
      -1,    50,    51,    52,    53,    54,    55,    56,    -1,    -1,
      59,    41,    42,    43,    44,    45,    46,    -1,    48,    -1,
      50,    51,    52,    53,    54,    55,    56,    -1,    -1,    59,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    41,    42,    43,    44,
      45,    46,    -1,    48,    -1,    50,    51,    52,    53,    54,
      55,    56,    41,    42,    43,    44,    -1,    -1,    -1,    41,
      42,    50,    51,    52,    53,    54,    55,    56,    50,    51,
      52,    53,    54,    55,    56
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    67,    68,     0,     1,     3,     4,     5,     6,     8,
       9,    12,    13,    14,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    31,    32,    39,    59,    60,
      69,    70,    71,    72,    73,    76,    77,    78,    79,    80,
      81,    85,    88,    89,    90,    92,    59,    20,    62,    62,
      10,    11,    37,    38,    39,    53,    57,    62,    94,    62,
      69,    62,    59,    59,    59,    94,    62,    20,     3,    72,
      24,    29,    30,    39,    39,    31,    32,    33,    34,    35,
      36,    65,    68,    39,    74,    75,    59,    62,    94,    94,
      94,    94,    94,    41,    42,    43,    44,    45,    46,    48,
      50,    51,    52,    53,    54,    55,    56,    59,    72,    77,
      82,     8,    94,    59,    63,    62,    74,    28,    40,    62,
      91,    94,    39,    93,    94,    94,    94,    94,    94,     1,
      61,    65,    59,    64,    21,    63,    63,    63,    63,    94,
      94,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    39,    59,    62,    63,    70,    63,    59,
      94,    29,    59,    30,    59,    61,    94,    75,    63,    70,
      69,    69,    49,    65,    83,    94,    94,    60,    70,    48,
      28,    40,    62,    94,    39,    70,     7,    94,    94,    59,
      63,    86,    40,    94,    69,    77,    84,    59,    15,    16,
      61,    49,    48,    63,    37,    53,    87,    49,    40,    40,
      69,    37,    49,    68,    63,    49,    68,    40,    63
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
		  Type, Value, Location); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
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
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

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

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

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
       `yyls': related to locations.

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

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
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
  yylsp = yyls;

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1464 of yacc.c  */
#line 109 "src/parser/parser.y"
    { g_program = (yyvsp[(1) - (1)].block); (yyval.block) = (yyvsp[(1) - (1)].block); ;}
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 113 "src/parser/parser.y"
    { (yyval.block) = new BlockNode(NodeKind::Program, yylineno); ;}
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 114 "src/parser/parser.y"
    { if ((yyvsp[(2) - (2)].node)) (yyvsp[(1) - (2)].block)->stmts.push_back((yyvsp[(2) - (2)].node)); (yyval.block) = (yyvsp[(1) - (2)].block); ;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 128 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].coutn); ;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 129 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].cinn); ;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 130 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].swn); ;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 131 "src/parser/parser.y"
    { (yyval.node) = new JumpNode(NodeKind::Break, (yylsp[(1) - (2)]).first_line); ;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 132 "src/parser/parser.y"
    { (yyval.node) = new JumpNode(NodeKind::Continue, (yylsp[(1) - (2)]).first_line); ;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 133 "src/parser/parser.y"
    { (yyval.node) = new BlockNode(NodeKind::Block, (yylsp[(1) - (1)]).first_line); ;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 134 "src/parser/parser.y"
    { yyerrok; (yyval.node) = NULL; ;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 138 "src/parser/parser.y"
    { (yyvsp[(2) - (3)].block)->kind = NodeKind::Block; (yyval.node) = (yyvsp[(2) - (3)].block); ;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 139 "src/parser/parser.y"
    { yyerrok; (yyvsp[(2) - (4)].block)->kind = NodeKind::Block; (yyval.node) = (yyvsp[(2) - (4)].block); ;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 144 "src/parser/parser.y"
    { (yyval.node) = registerMain((yyvsp[(5) - (5)].node), (yylsp[(1) - (5)]).first_line, false); ;}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 145 "src/parser/parser.y"
    { (yyval.node) = registerMain((yyvsp[(6) - (6)].node), (yylsp[(1) - (6)]).first_line, false); ;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 146 "src/parser/parser.y"
    { (yyval.node) = registerMain((yyvsp[(5) - (5)].node), (yylsp[(1) - (5)]).first_line, true); ;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 148 "src/parser/parser.y"
    { reportError(PH_SYNTAX, (yylsp[(1) - (4)]).first_line, "'main' has no return type: write 'int main()'");
          (yyval.node) = registerMain((yyvsp[(4) - (4)].node), (yylsp[(1) - (4)]).first_line, false); ;}
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 153 "src/parser/parser.y"
    { (yyval.tval) = TY_INT; ;}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 154 "src/parser/parser.y"
    { (yyval.tval) = TY_FLOAT; ;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 155 "src/parser/parser.y"
    { (yyval.tval) = TY_FLOAT; ;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 156 "src/parser/parser.y"
    { (yyval.tval) = TY_BOOL; ;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 157 "src/parser/parser.y"
    { (yyval.tval) = TY_INT; ;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 158 "src/parser/parser.y"
    { (yyval.tval) = TY_INT; ;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 159 "src/parser/parser.y"
    { (yyval.tval) = TY_INT; ;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 163 "src/parser/parser.y"
    { (yyval.node) = finishDecl((yyvsp[(1) - (3)].tval), (yyvsp[(2) - (3)].block)); ;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 164 "src/parser/parser.y"
    { (yyval.node) = finishDecl((yyvsp[(2) - (4)].tval), (yyvsp[(3) - (4)].block)); ;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 168 "src/parser/parser.y"
    { (yyval.block) = new BlockNode(NodeKind::DeclGroup, (yylsp[(1) - (1)]).first_line); (yyval.block)->stmts.push_back((yyvsp[(1) - (1)].node)); ;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 169 "src/parser/parser.y"
    { (yyvsp[(1) - (3)].block)->stmts.push_back((yyvsp[(3) - (3)].node)); (yyval.block) = (yyvsp[(1) - (3)].block); ;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 173 "src/parser/parser.y"
    { (yyval.node) = new VarDeclNode(TY_ERROR, (yyvsp[(1) - (1)].sval), NULL, (yylsp[(1) - (1)]).first_line); free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 174 "src/parser/parser.y"
    { (yyval.node) = new VarDeclNode(TY_ERROR, (yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].node), (yylsp[(1) - (3)]).first_line); free((yyvsp[(1) - (3)].sval)); ;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 178 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 183 "src/parser/parser.y"
    { (yyval.node) = new AssignNode((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].node), (yylsp[(1) - (3)]).first_line); free((yyvsp[(1) - (3)].sval)); ;}
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 184 "src/parser/parser.y"
    { (yyval.node) = compound((yyvsp[(1) - (3)].sval), "+", (yyvsp[(3) - (3)].node), (yylsp[(1) - (3)]).first_line); free((yyvsp[(1) - (3)].sval)); ;}
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 185 "src/parser/parser.y"
    { (yyval.node) = compound((yyvsp[(1) - (3)].sval), "-", (yyvsp[(3) - (3)].node), (yylsp[(1) - (3)]).first_line); free((yyvsp[(1) - (3)].sval)); ;}
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 186 "src/parser/parser.y"
    { (yyval.node) = compound((yyvsp[(1) - (3)].sval), "*", (yyvsp[(3) - (3)].node), (yylsp[(1) - (3)]).first_line); free((yyvsp[(1) - (3)].sval)); ;}
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 187 "src/parser/parser.y"
    { (yyval.node) = compound((yyvsp[(1) - (3)].sval), "/", (yyvsp[(3) - (3)].node), (yylsp[(1) - (3)]).first_line); free((yyvsp[(1) - (3)].sval)); ;}
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 188 "src/parser/parser.y"
    { (yyval.node) = compound((yyvsp[(1) - (2)].sval), "+", new IntLitNode(1, (yylsp[(1) - (2)]).first_line), (yylsp[(1) - (2)]).first_line); free((yyvsp[(1) - (2)].sval)); ;}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 189 "src/parser/parser.y"
    { (yyval.node) = compound((yyvsp[(1) - (2)].sval), "-", new IntLitNode(1, (yylsp[(1) - (2)]).first_line), (yylsp[(1) - (2)]).first_line); free((yyvsp[(1) - (2)].sval)); ;}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 190 "src/parser/parser.y"
    { (yyval.node) = compound((yyvsp[(2) - (2)].sval), "+", new IntLitNode(1, (yylsp[(1) - (2)]).first_line), (yylsp[(1) - (2)]).first_line); free((yyvsp[(2) - (2)].sval)); ;}
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 191 "src/parser/parser.y"
    { (yyval.node) = compound((yyvsp[(2) - (2)].sval), "-", new IntLitNode(1, (yylsp[(1) - (2)]).first_line), (yylsp[(1) - (2)]).first_line); free((yyvsp[(2) - (2)].sval)); ;}
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 196 "src/parser/parser.y"
    { (yyval.node) = new IfNode((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node), NULL, (yylsp[(1) - (5)]).first_line); ;}
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 198 "src/parser/parser.y"
    { (yyval.node) = new IfNode((yyvsp[(3) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].node), (yylsp[(1) - (7)]).first_line); ;}
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 202 "src/parser/parser.y"
    { (yyval.node) = new WhileNode((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node), (yylsp[(1) - (5)]).first_line); ;}
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 206 "src/parser/parser.y"
    { (yyval.node) = new DoWhileNode((yyvsp[(2) - (7)].node), (yyvsp[(5) - (7)].node), (yylsp[(1) - (7)]).first_line); ;}
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 212 "src/parser/parser.y"
    {
            BlockNode* outer = new BlockNode(NodeKind::Block, (yylsp[(1) - (9)]).first_line);
            if ((yyvsp[(3) - (9)].node)) outer->stmts.push_back((yyvsp[(3) - (9)].node));
            outer->stmts.push_back(new WhileNode((yyvsp[(5) - (9)].node), (yyvsp[(9) - (9)].node), (yylsp[(1) - (9)]).first_line, (yyvsp[(7) - (9)].node)));
            (yyval.node) = outer;
        ;}
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 221 "src/parser/parser.y"
    { (yyval.node) = NULL; ;}
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 222 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 223 "src/parser/parser.y"
    { (yyval.node) = new VarDeclNode((yyvsp[(1) - (4)].tval), (yyvsp[(2) - (4)].sval), (yyvsp[(4) - (4)].node), (yylsp[(1) - (4)]).first_line); free((yyvsp[(2) - (4)].sval)); ;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 227 "src/parser/parser.y"
    { (yyval.node) = new BoolLitNode(true, yylineno); ;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 228 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 232 "src/parser/parser.y"
    { (yyval.node) = NULL; ;}
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 233 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 238 "src/parser/parser.y"
    { (yyvsp[(6) - (7)].swn)->expr = (yyvsp[(3) - (7)].node); (yyvsp[(6) - (7)].swn)->line = (yylsp[(1) - (7)]).first_line; (yyval.swn) = (yyvsp[(6) - (7)].swn); ;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 242 "src/parser/parser.y"
    { (yyval.swn) = new SwitchNode(NULL, yylineno); ;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 244 "src/parser/parser.y"
    {
            SwitchCase c; c.isDefault = false; c.value = (yyvsp[(3) - (5)].ival); c.line = (yylsp[(2) - (5)]).first_line;
            (yyvsp[(5) - (5)].block)->kind = NodeKind::DeclGroup; c.body = (yyvsp[(5) - (5)].block);
            (yyvsp[(1) - (5)].swn)->cases.push_back(c); (yyval.swn) = (yyvsp[(1) - (5)].swn);
        ;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 250 "src/parser/parser.y"
    {
            SwitchCase c; c.isDefault = true; c.value = 0; c.line = (yylsp[(2) - (4)]).first_line;
            (yyvsp[(4) - (4)].block)->kind = NodeKind::DeclGroup; c.body = (yyvsp[(4) - (4)].block);
            (yyvsp[(1) - (4)].swn)->cases.push_back(c); (yyval.swn) = (yyvsp[(1) - (4)].swn);
        ;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 258 "src/parser/parser.y"
    { (yyval.ival) = (yyvsp[(1) - (1)].ival); ;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 259 "src/parser/parser.y"
    { (yyval.ival) = -(yyvsp[(2) - (2)].ival); ;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 263 "src/parser/parser.y"
    { (yyval.node) = new PrintNode((yyvsp[(2) - (3)].node), (yylsp[(1) - (3)]).first_line); ;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 267 "src/parser/parser.y"
    { (yyval.node) = new ReturnNode(NULL, (yylsp[(1) - (2)]).first_line); ;}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 268 "src/parser/parser.y"
    { (yyval.node) = new ReturnNode((yyvsp[(2) - (3)].node), (yylsp[(1) - (3)]).first_line); ;}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 273 "src/parser/parser.y"
    { (yyvsp[(3) - (4)].coutn)->line = (yylsp[(1) - (4)]).first_line; (yyval.coutn) = (yyvsp[(3) - (4)].coutn); ;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 278 "src/parser/parser.y"
    { (yyval.coutn) = new CoutNode((yylsp[(1) - (1)]).first_line);
          CoutItem it; it.kind = 0; it.expr = (yyvsp[(1) - (1)].node); (yyval.coutn)->items.push_back(it); ;}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 281 "src/parser/parser.y"
    { (yyval.coutn) = new CoutNode((yylsp[(1) - (1)]).first_line);
          CoutItem it; it.kind = 1; it.expr = NULL; it.text = (yyvsp[(1) - (1)].sval); (yyval.coutn)->items.push_back(it); free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 284 "src/parser/parser.y"
    { (yyval.coutn) = new CoutNode((yylsp[(1) - (1)]).first_line);
          CoutItem it; it.kind = 2; it.expr = NULL; (yyval.coutn)->items.push_back(it); ;}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 287 "src/parser/parser.y"
    { (yyval.coutn) = new CoutNode((yylsp[(1) - (7)]).first_line);
          CoutItem it; it.kind = 3; it.expr = (yyvsp[(2) - (7)].node); it.text = (yyvsp[(4) - (7)].sval); it.text2 = (yyvsp[(6) - (7)].sval);
          (yyval.coutn)->items.push_back(it); free((yyvsp[(4) - (7)].sval)); free((yyvsp[(6) - (7)].sval)); ;}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 291 "src/parser/parser.y"
    { CoutItem it; it.kind = 3; it.expr = (yyvsp[(4) - (9)].node); it.text = (yyvsp[(6) - (9)].sval); it.text2 = (yyvsp[(8) - (9)].sval);
          (yyvsp[(1) - (9)].coutn)->items.push_back(it); free((yyvsp[(6) - (9)].sval)); free((yyvsp[(8) - (9)].sval)); (yyval.coutn) = (yyvsp[(1) - (9)].coutn); ;}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 294 "src/parser/parser.y"
    { CoutItem it; it.kind = 0; it.expr = (yyvsp[(3) - (3)].node); (yyvsp[(1) - (3)].coutn)->items.push_back(it); (yyval.coutn) = (yyvsp[(1) - (3)].coutn); ;}
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 296 "src/parser/parser.y"
    { CoutItem it; it.kind = 1; it.expr = NULL; it.text = (yyvsp[(3) - (3)].sval); (yyvsp[(1) - (3)].coutn)->items.push_back(it); free((yyvsp[(3) - (3)].sval)); (yyval.coutn) = (yyvsp[(1) - (3)].coutn); ;}
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 298 "src/parser/parser.y"
    { CoutItem it; it.kind = 2; it.expr = NULL; (yyvsp[(1) - (3)].coutn)->items.push_back(it); (yyval.coutn) = (yyvsp[(1) - (3)].coutn); ;}
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 303 "src/parser/parser.y"
    { (yyvsp[(3) - (4)].cinn)->line = (yylsp[(1) - (4)]).first_line; (yyval.cinn) = (yyvsp[(3) - (4)].cinn); ;}
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 307 "src/parser/parser.y"
    { (yyval.cinn) = new CinNode((yylsp[(1) - (1)]).first_line); (yyval.cinn)->vars.push_back(new IdentNode((yyvsp[(1) - (1)].sval), (yylsp[(1) - (1)]).first_line)); free((yyvsp[(1) - (1)].sval)); ;}
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 308 "src/parser/parser.y"
    { (yyvsp[(1) - (3)].cinn)->vars.push_back(new IdentNode((yyvsp[(3) - (3)].sval), (yylsp[(3) - (3)]).first_line)); free((yyvsp[(3) - (3)].sval)); (yyval.cinn) = (yyvsp[(1) - (3)].cinn); ;}
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 312 "src/parser/parser.y"
    { (yyval.node) = new TernaryNode((yyvsp[(1) - (5)].node), (yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node), (yylsp[(2) - (5)]).first_line); ;}
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 313 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("||", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 314 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("&&", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 87:

/* Line 1464 of yacc.c  */
#line 315 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("==", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 316 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("!=", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 317 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("<", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 318 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode(">", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 319 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("<=", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 320 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode(">=", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 321 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("+", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 322 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("-", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 323 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("*", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 324 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("/", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 325 "src/parser/parser.y"
    { (yyval.node) = new BinaryNode("%", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), (yylsp[(2) - (3)]).first_line); ;}
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 326 "src/parser/parser.y"
    { (yyval.node) = new UnaryNode("!", (yyvsp[(2) - (2)].node), (yylsp[(1) - (2)]).first_line); ;}
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 327 "src/parser/parser.y"
    { (yyval.node) = new UnaryNode("-", (yyvsp[(2) - (2)].node), (yylsp[(1) - (2)]).first_line); ;}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 328 "src/parser/parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 329 "src/parser/parser.y"
    { (yyval.node) = new IntLitNode((yyvsp[(1) - (1)].ival), (yylsp[(1) - (1)]).first_line); ;}
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 330 "src/parser/parser.y"
    { (yyval.node) = new FloatLitNode((yyvsp[(1) - (1)].fval), (yylsp[(1) - (1)]).first_line); ;}
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 331 "src/parser/parser.y"
    { (yyval.node) = new BoolLitNode(true, (yylsp[(1) - (1)]).first_line); ;}
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 332 "src/parser/parser.y"
    { (yyval.node) = new BoolLitNode(false, (yylsp[(1) - (1)]).first_line); ;}
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 333 "src/parser/parser.y"
    { (yyval.node) = new IdentNode((yyvsp[(1) - (1)].sval), (yylsp[(1) - (1)]).first_line); free((yyvsp[(1) - (1)].sval)); ;}
    break;



/* Line 1464 of yacc.c  */
#line 2449 "build/parser.tab.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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

  yyerror_range[0] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  yyerror_range[0] = yylsp[1-yylen];
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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
		 yytoken, &yylval, &yylloc);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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
#line 336 "src/parser/parser.y"


void yyerror(const char* msg) {
    /* a bad token on this line was already reported by the lexer: don't pile on */
    if (g_lastLexErrorLine == yylineno) return;
    reportError(PH_SYNTAX, yylineno, msg);
}

