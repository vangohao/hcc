/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */

#line 67 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "parser.y" /* yacc.c:355  */

    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include<map>
    #include "symbol.h"
    #include "node.h"
    
    int yylex();  
    int yyparse();
    int yyerror(const char *);
    typedef struct{
        int number;
        char* name;
        SymbolType type;
        Node* node;
    } foryystype;
    #define YYSTYPE foryystype

#line 117 "y.tab.c" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    EQUAL = 258,
    NOTEQUAL = 259,
    LAND = 260,
    LOR = 261,
    T_INT = 262,
    MAIN = 263,
    IF = 264,
    ELSE = 265,
    WHILE = 266,
    RETURN = 267,
    INTEGER = 268,
    IDENTIFIER = 269,
    NEGA = 270
  };
#endif
/* Tokens.  */
#define EQUAL 258
#define NOTEQUAL 259
#define LAND 260
#define LOR 261
#define T_INT 262
#define MAIN 263
#define IF 264
#define ELSE 265
#define WHILE 266
#define RETURN 267
#define INTEGER 268
#define IDENTIFIER 269
#define NEGA 270

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 165 "y.tab.c" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 21 "parser.y" /* yacc.c:359  */

    SymbolTable * top = new SymbolTable(NULL); 
    SymbolTable * save = NULL;
    namespace Output
    {
        std::vector<std::string> list;
        int gen(std::string s)
        {
            list.push_back(s);
            return list.size() - 1;
        }
        void print()
        {
            for(std::string s : list)
            {
                std::cout<<s;
            }
        }
        void patch(int i,Label& l)
        {
            list.at(i) += l.print() + "\n";
        }
    }
    int Symbol::origCount = 0;
    int Symbol::tempCount = 0;
    int Label::usedCount = 0;

#line 195 "y.tab.c" /* yacc.c:359  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   326

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  131

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   270

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    24,     2,     2,     2,    22,     2,     2,
      29,    30,    20,    18,    28,    19,     2,    21,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    26,
      16,    15,    17,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    25,     2,    27,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,    32,     2,     2,     2,     2,
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
      23
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    69,    69,    71,    72,    74,    74,    74,    76,    84,
      86,    90,    95,    98,   102,   103,   105,   109,   109,   124,
     125,   126,   128,   131,   133,   135,   141,   143,   148,   152,
     152,   156,   161,   168,   176,   183,   184,   185,   188,   191,
     194,   197,   200,   203,   210,   213,   216,   219,   225,   232,
     239,   242,   244,   249,   252,   260,   263,   266,   269,   271
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "EQUAL", "NOTEQUAL", "LAND", "LOR",
  "T_INT", "MAIN", "IF", "ELSE", "WHILE", "RETURN", "INTEGER",
  "IDENTIFIER", "'='", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "NEGA", "'!'", "'['", "';'", "']'", "','", "'('", "')'", "'{'", "'}'",
  "$accept", "Goal", "BeforeMain", "BeforeMainStatement", "VarDefn",
  "VarDecls", "VarDecl", "FuncCreateIdTable", "FuncDefn", "$@1",
  "InsideFuncStatements", "FuncDecl", "MainFunc", "Type", "Statements",
  "M", "N", "Statement", "$@2", "Expression", "Params", "Identifier", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,    61,    60,    62,    43,    45,
      42,    47,    37,   270,    33,    91,    59,    93,    44,    40,
      41,   123,   125
};
# endif

#define YYPACT_NINF -53

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-53)))

#define YYTABLE_NINF -29

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -53,     4,     6,   -53,     7,   -53,   -53,    26,   -53,   -53,
     -53,    23,    16,   -53,     2,   -53,    23,   -53,   -15,    21,
      26,    39,    41,    54,   -53,   -53,    38,   -53,   -53,    43,
     -10,    48,   150,   -53,    51,   -53,    53,    57,   -53,     5,
     -53,   -53,    23,    60,   -53,    -9,    71,   -53,   -53,     5,
      75,   -53,     5,     5,     5,   199,    76,   150,    -3,   -53,
     150,     5,     5,   -53,   150,    22,     5,    85,    85,   103,
       5,     5,   -53,   -53,     5,     5,     5,     5,     5,     5,
       5,     5,   -53,     5,    80,   -53,   223,   149,    26,   -53,
     -53,   -53,   126,   -53,   301,   301,     5,     5,    37,    37,
      62,    62,    85,    85,    85,   174,   271,    20,   -53,   -53,
      98,    33,   150,   -53,   291,   281,   -53,     5,   -53,     5,
      39,   104,   150,   271,   247,   107,   -53,   -53,   -53,   150,
     -53
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     0,     1,    24,     3,     5,    12,     6,     7,
       2,     0,     0,    24,     0,    11,     0,    59,     0,     0,
       0,    17,    13,     0,     8,    16,     0,    10,    22,     0,
       0,     0,     0,    21,     0,    15,     0,     0,    27,     0,
      29,    36,     0,    27,    26,     0,    27,    14,     9,     0,
       0,    50,     0,     0,     0,     0,    51,     0,     0,    23,
       0,     0,     0,    18,     0,     0,     0,    53,    52,     0,
       0,     0,    27,    27,     0,     0,     0,     0,     0,     0,
       0,     0,    37,    58,    27,    25,     0,     0,    12,    19,
      20,    27,     0,    55,    45,    46,     0,     0,    43,    44,
      38,    39,    40,    41,    42,     0,    57,     0,    30,    34,
       0,     0,     0,    27,    47,    48,    49,     0,    54,     0,
       0,    31,     0,    56,     0,     0,    33,    35,    27,     0,
      32
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -53,   -53,   -53,   -53,   124,    46,   115,    63,   -53,   -53,
     -53,    73,   -53,     0,    82,   -37,   -53,   -52,   -53,    19,
     -53,   -11
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,     5,    41,    14,    15,     7,     8,    29,
      46,     9,    10,    42,    43,    60,   125,    44,    57,    55,
     107,    56
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      18,    50,    11,    34,     3,    22,    61,    16,    85,    64,
      23,    24,    90,     4,    25,    12,    62,    35,    51,    17,
      16,    45,    23,    24,    52,    70,    71,    72,    73,    53,
      20,    58,    21,    13,    54,    96,    97,    17,    74,    75,
      76,    77,    78,    79,    80,    19,    45,    81,   117,    45,
     118,    26,    91,    45,   112,    76,    77,    78,    79,    80,
     121,    20,    81,   120,    11,    28,    30,    31,    65,    32,
     126,    67,    68,    69,    33,    36,   122,   130,    47,    48,
      86,    87,    78,    79,    80,    92,    49,    81,    16,    94,
      95,   129,    59,    98,    99,   100,   101,   102,   103,   104,
     105,    45,   106,    63,    66,    83,    70,    71,    72,    73,
      81,    45,   108,   119,   -28,   114,   115,   128,    45,    74,
      75,    76,    77,    78,    79,    80,     6,    88,    81,    70,
      71,    72,    73,    93,   111,    27,   123,    89,   124,    84,
       0,     0,    74,    75,    76,    77,    78,    79,    80,     0,
       0,    81,    70,    71,    72,    73,   113,    13,     0,    37,
       0,    38,    39,     0,    17,    74,    75,    76,    77,    78,
      79,    80,     0,     0,    81,     0,   110,    70,    71,    72,
      73,    40,     0,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,    79,    80,     0,     0,    81,
       0,   116,    70,    71,    72,    73,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
      79,    80,     0,     0,    81,    82,    70,    71,    72,    73,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,    79,    80,     0,     0,    81,   109,
      70,    71,    72,    73,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,    79,    80,
       0,     0,    81,   127,    70,    71,    72,    73,     0,     0,
       0,     0,     0,     0,    70,    71,    72,    74,    75,    76,
      77,    78,    79,    80,    70,    71,    81,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    81,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    81,    74,    75,    76,
      77,    78,    79,    80,     0,     0,    81
};

static const yytype_int16 yycheck[] =
{
      11,    38,     2,    13,     0,    16,    15,     7,    60,    46,
      25,    26,    64,     7,    29,     8,    25,    27,    13,    14,
      20,    32,    25,    26,    19,     3,     4,     5,     6,    24,
      28,    42,    30,     7,    29,    72,    73,    14,    16,    17,
      18,    19,    20,    21,    22,    29,    57,    25,    28,    60,
      30,    30,    30,    64,    91,    18,    19,    20,    21,    22,
     112,    28,    25,    30,    64,    26,    25,    13,    49,    31,
     122,    52,    53,    54,    31,    27,   113,   129,    27,    26,
      61,    62,    20,    21,    22,    66,    29,    25,    88,    70,
      71,   128,    32,    74,    75,    76,    77,    78,    79,    80,
      81,   112,    83,    32,    29,    29,     3,     4,     5,     6,
      25,   122,    32,    15,    10,    96,    97,    10,   129,    16,
      17,    18,    19,    20,    21,    22,     2,    64,    25,     3,
       4,     5,     6,    30,    88,    20,   117,    64,   119,    57,
      -1,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    25,     3,     4,     5,     6,    30,     7,    -1,     9,
      -1,    11,    12,    -1,    14,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    -1,    27,     3,     4,     5,
       6,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    25,
      -1,    27,     3,     4,     5,     6,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    26,     3,     4,     5,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    25,    26,
       3,     4,     5,     6,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,    26,     3,     4,     5,     6,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,    16,    17,    18,
      19,    20,    21,    22,     3,     4,    25,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    34,    35,     0,     7,    36,    37,    40,    41,    44,
      45,    46,     8,     7,    38,    39,    46,    14,    54,    29,
      28,    30,    54,    25,    26,    29,    30,    39,    26,    42,
      25,    13,    31,    31,    13,    27,    27,     9,    11,    12,
      31,    37,    46,    47,    50,    54,    43,    27,    26,    29,
      48,    13,    19,    24,    29,    52,    54,    51,    54,    32,
      48,    15,    25,    32,    48,    52,    29,    52,    52,    52,
       3,     4,     5,     6,    16,    17,    18,    19,    20,    21,
      22,    25,    26,    29,    47,    50,    52,    52,    40,    44,
      50,    30,    52,    30,    52,    52,    48,    48,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    53,    32,    26,
      27,    38,    48,    30,    52,    52,    27,    28,    30,    15,
      30,    50,    48,    52,    52,    49,    50,    26,    10,    48,
      50
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    34,    35,    35,    36,    36,    36,    37,    37,
      38,    38,    38,    39,    39,    39,    40,    42,    41,    43,
      43,    43,    44,    45,    46,    47,    47,    48,    49,    51,
      50,    50,    50,    50,    50,    50,    50,    50,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    53,    53,    53,    54
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     0,     1,     1,     1,     3,     6,
       3,     1,     0,     2,     5,     4,     3,     0,     7,     3,
       3,     0,     4,     7,     1,     3,     1,     0,     0,     0,
       4,     6,    10,     7,     4,     7,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     4,     4,
       1,     1,     2,     2,     4,     3,     3,     1,     0,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

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
#line 69 "parser.y" /* yacc.c:1646  */
    {Output::print();}
#line 1393 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 76 "parser.y" /* yacc.c:1646  */
    {   (yyval.node) = new Node((yyvsp[-1].node),NULL,NodeType::Vardfn,0);
                                    (yyvsp[-1].node)->sym->Declear((yyvsp[-2].type));
                                    (yyvsp[-1].node)->sym->Define();
                                    // printf("var T%d\n",$2->sym->id);
                                    std::stringstream ss;
                                    ss<<"var T"<<(yyvsp[-1].node)->sym->id<<"\n";
                                    Output::gen(ss.str());
                                }
#line 1406 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 86 "parser.y" /* yacc.c:1646  */
    {   (yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::Params,(yyvsp[-2].node)->val + 1);
                                    (yyvsp[0].node)->sym->paramCount = (yyvsp[-2].node)->val;
                                    (yyvsp[0].node)->sym->Define();
}
#line 1415 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 90 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.node) = new Node(NULL,(yyvsp[0].node),NodeType::Params,1);
                                    (yyvsp[0].node)->sym->paramCount = 0;
                                    (yyvsp[0].node)->sym->Define();
}
#line 1425 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 95 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node(NULL,NULL,NodeType::Params,0);
}
#line 1432 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 98 "parser.y" /* yacc.c:1646  */
    {   (yyval.node) = new Node((yyvsp[0].node),NULL,NodeType::VarDcl,0);
                                    (yyvsp[0].node)->sym->Declear((yyvsp[-1].type),0);
                                    (yyval.node)->sym = (yyvsp[0].node)->sym;
}
#line 1441 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 105 "parser.y" /* yacc.c:1646  */
    {save = top;top = new SymbolTable(top);/*函数前的创建符号表*/
                                        (yyval.node) = (yyvsp[-1].node);
}
#line 1449 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 109 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].node)->sym->Declear(SymbolType::FunPtr,0,(yyvsp[-1].node)->val);
                                            (yyvsp[-2].node)->sym->Define();
                                            (yyvsp[-2].node)->sym->print();//printf(" [%d]\n",$2->val);
                                            std::stringstream ss;
                                            ss<<" ["<< (yyvsp[-1].node)->val <<"]\n";
                                            Output::gen(ss.str());
 }
#line 1461 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 118 "parser.y" /* yacc.c:1646  */
    { /* delete top; */ top = save;
         (yyval.node) = new Node((yyvsp[-6].node),(yyvsp[-1].node),NodeType::Fundfn,(yyvsp[-5].node)->val); 
         /* printf("end ");  */ 
         Output::gen("end ");(yyvsp[-6].node)->sym->print();Output::gen("\n");
}
#line 1471 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 129 "parser.y" /* yacc.c:1646  */
    {/* delete top; */ top = save;}
#line 1477 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 133 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = SymbolType::Int;}
#line 1483 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 135 "parser.y" /* yacc.c:1646  */
    { // std::cerr<<"GAY"<<std::endl;
                                            (yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::Stmts,0);
                                            (yyvsp[-2].node)->nextlist.backpatch((yyvsp[-1].node)->instr);
                                            (yyval.node)->nextlist = (yyvsp[0].node)->nextlist;
}
#line 1493 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 141 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1499 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 143 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = new Node(NULL,NULL,NodeType::Empty,0);
            (yyval.node)->instr.Init(); 
            Output::gen((yyval.node)->instr.print()+":");
}
#line 1508 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 148 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = new Node(NULL,NULL,NodeType::Empty,0);
            (yyval.node)->nextlist = Gotolist(Output::gen("goto "));
}
#line 1516 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 152 "parser.y" /* yacc.c:1646  */
    {save = top;top = new SymbolTable(top);}
#line 1522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 153 "parser.y" /* yacc.c:1646  */
    {/* delete top; */ top = save;
                                     (yyval.node) = (yyvsp[-1].node);
}
#line 1530 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 156 "parser.y" /* yacc.c:1646  */
    {   (yyvsp[-3].node)->truelist.backpatch((yyvsp[-1].node)->instr);
                                         (yyval.node) = new Node((yyvsp[-3].node),(yyvsp[0].node),NodeType::If,0);
                                         (yyval.node)->nextlist = (yyvsp[-3].node)->falselist.merge((yyvsp[0].node)->nextlist);
                                        
}
#line 1540 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 161 "parser.y" /* yacc.c:1646  */
    {
                                        (yyval.node) = new Node((yyvsp[-7].node),(yyvsp[-4].node),NodeType::IfElse,0);
                                        (yyvsp[-7].node)->truelist.backpatch((yyvsp[-5].node)->instr);
                                        (yyvsp[-7].node)->falselist.backpatch((yyvsp[-1].node)->instr);
                                        (yyval.node)->nextlist = (yyvsp[-4].node)->nextlist.merge((yyvsp[0].node)->nextlist);
                                        (yyval.node)->nextlist = (yyval.node)->nextlist.merge((yyvsp[-3].node)->nextlist);
}
#line 1552 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 168 "parser.y" /* yacc.c:1646  */
    {
                                        (yyval.node) = new Node((yyvsp[-3].node),(yyvsp[0].node),NodeType::While,0);
                                        (yyvsp[0].node)->nextlist.backpatch((yyvsp[-5].node)->instr);
                                        (yyvsp[-3].node)->truelist.backpatch((yyvsp[-1].node)->instr);
                                        (yyval.node)->nextlist = (yyvsp[-3].node)->falselist;
                                        Output::gen("goto "+(yyvsp[-5].node)->instr.print());
                                        
}
#line 1565 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 176 "parser.y" /* yacc.c:1646  */
    {(yyval.node) =new Node((yyvsp[-3].node),(yyvsp[-1].node),NodeType::Assign,'=');
                                            (yyvsp[-3].node)->sym->print();
                                            Output::gen(" = ");//printf(" = ");
                                            (yyvsp[-1].node)->sym->print();
                                            Output::gen("\n");//printf("\n");

}
#line 1577 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 185 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-1].node),NULL,NodeType::Return,0);
                                            Output::gen("return ");(yyvsp[-1].node)->sym->print();Output::gen("\n");}
#line 1584 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 188 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualArith,'+');
                                           (yyval.node)->sym = Symbol::ProcessDualOp((yyvsp[-2].node)->sym,(yyvsp[0].node)->sym,"+");
}
#line 1592 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 191 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualArith,'-');
                                           (yyval.node)->sym = Symbol::ProcessDualOp((yyvsp[-2].node)->sym,(yyvsp[0].node)->sym,"-");
}
#line 1600 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 194 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualArith,'*');
                                           (yyval.node)->sym = Symbol::ProcessDualOp((yyvsp[-2].node)->sym,(yyvsp[0].node)->sym,"*");
}
#line 1608 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 197 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualArith,'/');
                                           (yyval.node)->sym = Symbol::ProcessDualOp((yyvsp[-2].node)->sym,(yyvsp[0].node)->sym,"/");
}
#line 1616 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 200 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualArith,'%');
                                           (yyval.node)->sym = Symbol::ProcessDualOp((yyvsp[-2].node)->sym,(yyvsp[0].node)->sym,"%%");
}
#line 1624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 203 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualLogic,'<');
                                        //    $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"<");
                                            Output::gen("if ");(yyvsp[-2].node)->sym->print();Output::gen(" < ");
                                            (yyvsp[0].node)->sym->print();
                                            (yyval.node)->truelist = Gotolist(Output::gen(" goto "));
                                            (yyval.node)->falselist = Gotolist(Output::gen("goto "));
}
#line 1636 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 210 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualLogic,'>');
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,">");
}
#line 1644 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 213 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualLogic,EQUAL);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"==");
}
#line 1652 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 216 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualLogic,NOTEQUAL);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"!=");
}
#line 1660 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 219 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-3].node),(yyvsp[-1].node),NodeType::DualLogic,LAND);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"&&");
                                           (yyvsp[-3].node)->truelist.backpatch((yyvsp[-1].node)->instr);
                                           (yyval.node)->truelist = (yyvsp[0].node)->truelist;
                                           (yyval.node)->falselist = (yyvsp[-3].node)->falselist.merge((yyvsp[0].node)->falselist);
}
#line 1671 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 225 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-3].node),(yyvsp[-1].node),NodeType::DualLogic,LOR);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"||");
                                           (yyvsp[-3].node)->falselist.backpatch((yyvsp[-1].node)->instr);
                                           (yyval.node)->falselist = (yyvsp[0].node)->falselist;
                                           (yyval.node)->truelist = (yyvsp[-3].node)->truelist.merge((yyvsp[0].node)->truelist);
                                            
}
#line 1683 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 232 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-3].node),(yyvsp[-1].node),NodeType::DualArith,'[');
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                tmpsym->print();Output::gen(" = ");(yyvsp[-3].node)->sym->print();
                                                Output::gen(" [");(yyvsp[-1].node)->sym->print();Output::gen("]\n");
                                                (yyval.node)->sym = tmpsym;
}
#line 1695 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 239 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node(NULL,NULL,NodeType::Symbol1,(yyvsp[0].number));
                                                (yyval.node)->sym = new Symbol(SymbolType::Immediate,(yyvsp[0].number));
}
#line 1703 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 242 "parser.y" /* yacc.c:1646  */
    {if((yyvsp[0].node)->sym->decleared == false) {(yyvsp[0].node)->sym->ReportError("Undecleared");}
    (yyval.node) = (yyvsp[0].node);}
#line 1710 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 244 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[0].node),NULL,NodeType::SingleLogic,'!');
                                                // $$->sym = Symbol::ProcessSingleOp($2->sym,"!");
                                                (yyval.node)->falselist = (yyvsp[0].node)->truelist;
                                                (yyval.node)->truelist = (yyvsp[0].node)->falselist;
}
#line 1720 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 249 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[0].node),NULL,NodeType::SingleArith,'-');
                                                (yyval.node)->sym = Symbol::ProcessSingleOp((yyvsp[0].node)->sym,"-");
}
#line 1728 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 252 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-3].node),(yyvsp[-1].node),NodeType::Funcall,(yyvsp[-1].node)->val);
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                tmpsym->print();Output::gen(" = ");
                                                Output::gen("call ");(yyvsp[-3].node)->sym->print();Output::gen("\n");
                                                (yyval.node)->sym = tmpsym;
                                                
}
#line 1741 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 260 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node);
}
#line 1748 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 263 "parser.y" /* yacc.c:1646  */
    {   (yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::Params,(yyvsp[-2].node)->val + 1);
                                        Output::gen("param ");(yyvsp[0].node)->sym->print();Output::gen("\n");
}
#line 1756 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 266 "parser.y" /* yacc.c:1646  */
    { (yyval.node)=new Node(NULL,(yyvsp[0].node),NodeType::Params,1);
                                        Output::gen("param ");(yyvsp[0].node)->sym->print();Output::gen("\n");
}
#line 1764 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 269 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node(NULL,NULL,NodeType::Params,0);}
#line 1770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 271 "parser.y" /* yacc.c:1646  */
    {    (yyval.node) = new Node(NULL,NULL,NodeType::Symbol1,0);
                                        Symbol * sym = top->get((yyvsp[0].name));
                                        if(sym == NULL)
                                        {
                                            sym = new Symbol((yyval.node));
                                            top->put((yyvsp[0].name),sym);
                                        }
                                        sym->funName = (char*)malloc(4+strlen((yyvsp[0].name)));
                                        strcpy(sym->funName,"f_");
                                        strcat(sym->funName,(yyvsp[0].name));
                                        (yyval.node)->sym = sym;
}
#line 1787 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1791 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 285 "parser.y" /* yacc.c:1906  */


int yyerror(const char *msg)            //输出错误信息的yyerror()函数
{
std::cerr<<"Error encountered: "<<msg<<std::endl;
return 0;
}
