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
    #include<stack>
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

#line 118 "y.tab.c" /* yacc.c:355  */

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

#line 166 "y.tab.c" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 22 "parser.y" /* yacc.c:359  */

    SymbolTable * top = new SymbolTable(NULL); 
    std::stack<SymbolTable*> save;
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
        void patch(int i,Label& l,int x =0)
        {
            if(x ==0)
            list.at(i) += l.print() + "\n";
            else list.at(i) = l.print() + ":\n";
        }
    }
    int Symbol::origCount = 0;
    int Symbol::tempCount = 0;
    int Label::usedCount = 0;

#line 198 "y.tab.c" /* yacc.c:359  */

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
#define YYLAST   392

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  33
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  63
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  137

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
       0,    72,    72,    74,    75,    77,    77,    77,    79,    93,
     109,   113,   118,   121,   133,   144,   156,   160,   160,   174,
     178,   183,   184,   186,   191,   191,   201,   203,   209,   211,
     215,   219,   219,   223,   224,   229,   236,   245,   251,   251,
     264,   265,   268,   271,   274,   277,   280,   283,   290,   297,
     304,   311,   317,   324,   333,   336,   338,   343,   346,   354,
     357,   360,   363,   365
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
  "InsideFuncStatements", "FuncDecl", "MainFunc", "$@2", "Type",
  "Statements", "M", "N", "Statement", "$@3", "$@4", "Expression",
  "Params", "Identifier", YY_NULLPTR
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

#define YYPACT_NINF -41

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-41)))

#define YYTABLE_NINF -31

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -41,    12,    10,   -41,     1,   -41,   -41,    32,   -41,   -41,
     -41,     5,    15,   -41,    -6,   -41,     5,   -41,   -19,    18,
      32,    19,    22,    36,   -41,   -41,   -41,   -41,   -41,    20,
     -12,    23,    33,    87,    31,   -41,    37,    87,    40,   -41,
     353,   -41,   -41,   -41,    32,   -41,   -41,   -41,   -11,   -41,
     -41,   -41,   353,    41,   -41,   353,   353,   353,   251,    43,
      87,     2,     9,   353,   353,   171,   118,   353,    48,    48,
     147,   353,   353,   -41,   -41,   353,   353,   353,   353,   353,
     353,   353,   353,   -41,   353,     5,    34,   -41,    19,   -41,
     -41,   -41,   275,   201,   -41,   -41,   170,   -41,   367,   367,
     353,   353,   190,   190,     6,     6,    48,    48,    48,   226,
     323,     8,    17,   -41,    87,   -41,    59,    87,   -41,   343,
     333,   -41,   353,   -41,   -41,   -41,    80,    87,   323,   353,
      81,   -41,   299,   -41,   -41,    87,   -41
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     0,     1,    26,     3,     5,    12,     6,     7,
       2,     0,     0,    26,     0,    11,     0,    63,     0,     0,
       0,    17,    13,     0,     8,    16,    24,    10,    23,     0,
       0,     0,     0,     0,     0,    15,     0,     0,     0,    29,
       0,    33,    31,    40,    12,    29,    21,    22,     0,    14,
       9,    29,     0,     0,    54,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    56,
       0,     0,     0,    29,    29,     0,     0,     0,     0,     0,
       0,     0,     0,    41,    62,     0,    29,    28,     0,    18,
      19,    20,     0,     0,    25,    29,     0,    59,    49,    50,
       0,     0,    47,    48,    42,    43,    44,    45,    46,     0,
      61,     0,     0,    32,     0,    37,     0,     0,    29,    51,
      52,    53,     0,    58,    27,    38,    34,     0,    60,     0,
       0,    36,     0,    29,    39,     0,    35
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -41,   -41,   -41,   -41,    93,    53,    82,    98,   -41,   -41,
      66,     0,   -41,   -41,    -7,   -41,   -40,   -41,    27,   -41,
     -41,     4,   -41,    -8
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     5,    43,    14,    15,    44,     8,    29,
      45,    46,    10,    32,    11,    86,    53,   130,    47,    60,
     129,    58,   111,    59
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      16,    34,     9,    18,    63,    62,    23,    24,    22,    12,
      25,    65,     3,    16,    64,    35,    13,     4,    38,    17,
      39,    40,    20,    17,    21,    48,    79,    80,    81,    48,
      20,    82,    88,   100,   101,    41,   122,    16,   123,    13,
      42,    89,    23,    24,    19,    28,   114,    30,    26,    31,
      36,    33,    48,    85,    48,   117,    66,    48,    49,    68,
      69,    70,    90,    50,    37,    90,   113,    92,    93,    52,
      67,    96,    84,    82,   125,    98,    99,   112,   127,   102,
     103,   104,   105,   106,   107,   108,   109,    87,   110,    91,
     -30,   133,    91,   135,    13,     6,    38,    61,    39,    40,
       7,    17,    27,    51,   119,   120,    48,    85,     0,    48,
      85,     0,     0,    41,     0,     0,     0,     0,    42,    48,
      85,    71,    72,    73,    74,     0,   128,    48,    85,     0,
       0,     0,     0,   132,    75,    76,    77,    78,    79,    80,
      81,   124,     0,    82,   126,     0,     0,     0,    95,     0,
      71,    72,    73,    74,   131,     0,     0,     0,     0,     0,
       0,     0,   136,    75,    76,    77,    78,    79,    80,    81,
       0,     0,    82,    71,    72,    73,    74,    97,    13,     0,
      38,     0,    39,    40,     0,    17,    75,    76,    77,    78,
      79,    80,    81,     0,     0,    82,     0,    41,     0,     0,
     118,     0,    42,    94,    71,    72,    73,    74,    77,    78,
      79,    80,    81,     0,     0,    82,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,    82,     0,   116,    71,
      72,    73,    74,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,    79,    80,    81,     0,
       0,    82,     0,   121,    71,    72,    73,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,    80,    81,     0,     0,    82,    83,    71,    72,
      73,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,    79,    80,    81,     0,     0,
      82,   115,    71,    72,    73,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,    79,
      80,    81,     0,     0,    82,   134,    71,    72,    73,    74,
       0,     0,     0,     0,     0,     0,    71,    72,    73,    75,
      76,    77,    78,    79,    80,    81,    71,    72,    82,    75,
      76,    77,    78,    79,    80,    81,     0,     0,    82,    75,
      76,    77,    78,    79,    80,    81,    54,    17,    82,     0,
       0,     0,    55,     0,     0,     0,     0,    56,     0,     0,
       0,     0,    57,    75,    76,    77,    78,    79,    80,    81,
       0,     0,    82
};

static const yytype_int16 yycheck[] =
{
       7,    13,     2,    11,    15,    45,    25,    26,    16,     8,
      29,    51,     0,    20,    25,    27,     7,     7,     9,    14,
      11,    12,    28,    14,    30,    33,    20,    21,    22,    37,
      28,    25,    30,    73,    74,    26,    28,    44,    30,     7,
      31,    32,    25,    26,    29,    26,    86,    25,    30,    13,
      27,    31,    60,    60,    62,    95,    52,    65,    27,    55,
      56,    57,    62,    26,    31,    65,    32,    63,    64,    29,
      29,    67,    29,    25,    15,    71,    72,    85,   118,    75,
      76,    77,    78,    79,    80,    81,    82,    60,    84,    62,
      10,    10,    65,   133,     7,     2,     9,    44,    11,    12,
       2,    14,    20,    37,   100,   101,   114,   114,    -1,   117,
     117,    -1,    -1,    26,    -1,    -1,    -1,    -1,    31,   127,
     127,     3,     4,     5,     6,    -1,   122,   135,   135,    -1,
      -1,    -1,    -1,   129,    16,    17,    18,    19,    20,    21,
      22,   114,    -1,    25,   117,    -1,    -1,    -1,    30,    -1,
       3,     4,     5,     6,   127,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   135,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25,     3,     4,     5,     6,    30,     7,    -1,
       9,    -1,    11,    12,    -1,    14,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    25,    -1,    26,    -1,    -1,
      30,    -1,    31,    32,     3,     4,     5,     6,    18,    19,
      20,    21,    22,    -1,    -1,    25,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    -1,    27,     3,
       4,     5,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    25,    -1,    27,     3,     4,     5,     6,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    26,     3,     4,
       5,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      25,    26,     3,     4,     5,     6,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    25,    26,     3,     4,     5,     6,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,    16,
      17,    18,    19,    20,    21,    22,     3,     4,    25,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    25,    16,
      17,    18,    19,    20,    21,    22,    13,    14,    25,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    -1,    29,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    25
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    34,    35,     0,     7,    36,    37,    40,    41,    44,
      45,    47,     8,     7,    38,    39,    47,    14,    56,    29,
      28,    30,    56,    25,    26,    29,    30,    39,    26,    42,
      25,    13,    46,    31,    13,    27,    27,    31,     9,    11,
      12,    26,    31,    37,    40,    43,    44,    51,    56,    27,
      26,    43,    29,    49,    13,    19,    24,    29,    54,    56,
      52,    38,    49,    15,    25,    49,    54,    29,    54,    54,
      54,     3,     4,     5,     6,    16,    17,    18,    19,    20,
      21,    22,    25,    26,    29,    47,    48,    51,    30,    32,
      44,    51,    54,    54,    32,    30,    54,    30,    54,    54,
      49,    49,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    55,    56,    32,    49,    26,    27,    49,    30,    54,
      54,    27,    28,    30,    51,    15,    51,    49,    54,    53,
      50,    51,    54,    10,    26,    49,    51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    33,    34,    35,    35,    36,    36,    36,    37,    37,
      38,    38,    38,    39,    39,    39,    40,    42,    41,    43,
      43,    43,    43,    44,    46,    45,    47,    48,    48,    49,
      50,    52,    51,    51,    51,    51,    51,    51,    53,    51,
      51,    51,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      55,    55,    55,    56
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     0,     1,     1,     1,     3,     6,
       3,     1,     0,     2,     5,     4,     3,     0,     8,     3,
       3,     1,     1,     4,     0,     9,     1,     3,     1,     0,
       0,     0,     4,     1,     6,    10,     7,     4,     0,     8,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     4,     1,     1,     2,     2,     4,     3,
       3,     1,     0,     1
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
#line 72 "parser.y" /* yacc.c:1646  */
    {Output::print();}
#line 1414 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 79 "parser.y" /* yacc.c:1646  */
    {   (yyval.node) = new Node((yyvsp[-1].node),NULL,NodeType::Vardfn,0);
                                    if((yyvsp[-1].node)->val == 1)
                                    {
                                        Symbol * tmp = (yyvsp[-1].node)->sym;
                                        (yyvsp[-1].node)->sym = new Symbol((yyvsp[-1].node));
                                        (yyvsp[-1].node)->sym->funName = tmp->funName;
                                        top->put(tmp->funName+2,(yyvsp[-1].node)->sym);
                                    }
                                    (yyvsp[-1].node)->sym->Declear((yyvsp[-2].type));
                                    (yyvsp[-1].node)->sym->Define();
                                    std::stringstream ss;
                                    ss<<"var T"<<(yyvsp[-1].node)->sym->id<<"\n";
                                    Output::gen(ss.str());
                                }
#line 1433 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 93 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.node) = new Node((yyvsp[-4].node),NULL,NodeType::Vardfn,4 * (yyvsp[-2].number));
                                    if((yyvsp[-4].node)->val == 1)
                                    {
                                        Symbol * tmp = (yyvsp[-4].node)->sym;
                                        (yyvsp[-4].node)->sym = new Symbol((yyvsp[-4].node));
                                        (yyvsp[-4].node)->sym->funName = tmp->funName;
                                        top->put(tmp->funName+2,(yyvsp[-4].node)->sym);
                                    }
                                    (yyvsp[-4].node)->sym->Declear(SymbolType::IntPtr,-1,4 * (yyvsp[-2].number));
                                    (yyvsp[-4].node)->sym->Define();
                                    std::stringstream ss;
                                    ss<<"var "<<4 * (yyvsp[-2].number)<<" T"<<(yyvsp[-4].node)->sym->id<<"\n";
                                    Output::gen(ss.str());
}
#line 1453 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 109 "parser.y" /* yacc.c:1646  */
    {   (yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::Params,(yyvsp[-2].node)->val + 1);
                                    (yyvsp[0].node)->sym->paramCount = (yyvsp[-2].node)->val;
                                    (yyvsp[0].node)->sym->Define();
}
#line 1462 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 113 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.node) = new Node(NULL,(yyvsp[0].node),NodeType::Params,1);
                                    (yyvsp[0].node)->sym->paramCount = 0;
                                    (yyvsp[0].node)->sym->Define();
}
#line 1472 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 118 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node(NULL,NULL,NodeType::Params,0);
}
#line 1479 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 121 "parser.y" /* yacc.c:1646  */
    {   (yyval.node) = new Node((yyvsp[0].node),NULL,NodeType::VarDcl,0);
                                    if((yyvsp[0].node)->val == 1)
                                    {
                                        Symbol * tmp = (yyvsp[0].node)->sym;
                                        (yyvsp[0].node)->sym = new Symbol((yyvsp[0].node));
                                        (yyvsp[0].node)->sym->funName = tmp->funName;
                                        // std::cerr<<tmp->funName +2<<std::endl;
                                        top->put(tmp->funName+2,(yyvsp[0].node)->sym);
                                    }
                                    (yyvsp[0].node)->sym->Declear((yyvsp[-1].type),0);
                                    (yyval.node)->sym = (yyvsp[0].node)->sym;
}
#line 1496 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 133 "parser.y" /* yacc.c:1646  */
    {   (yyval.node) = new Node((yyvsp[-3].node),NULL,NodeType::VarDcl,4 * (yyvsp[-1].number));
                                    if((yyvsp[-3].node)->val == 1)
                                    {
                                        Symbol * tmp = (yyvsp[-3].node)->sym;
                                        (yyvsp[-3].node)->sym = new Symbol((yyvsp[-3].node));
                                        (yyvsp[-3].node)->sym->funName = tmp->funName;
                                        top->put(tmp->funName+2,(yyvsp[-3].node)->sym);
                                    }
                                    (yyvsp[-3].node)->sym->Declear(SymbolType::IntPtr,0,4 * (yyvsp[-1].number));
                                    (yyval.node)->sym = (yyvsp[-3].node)->sym;
}
#line 1512 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 144 "parser.y" /* yacc.c:1646  */
    {   (yyval.node) = new Node((yyvsp[-2].node),NULL,NodeType::VarDcl,0);
                                    if((yyvsp[-2].node)->val == 1)
                                    {
                                        Symbol * tmp = (yyvsp[-2].node)->sym;
                                        (yyvsp[-2].node)->sym = new Symbol((yyvsp[-2].node));
                                        (yyvsp[-2].node)->sym->funName = tmp->funName;
                                        top->put(tmp->funName+2,(yyvsp[-2].node)->sym);
                                    }
                                    (yyvsp[-2].node)->sym->Declear(SymbolType::IntPtr,0,0);
                                    (yyval.node)->sym = (yyvsp[-2].node)->sym;
}
#line 1528 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 156 "parser.y" /* yacc.c:1646  */
    {save.push(top);top = new SymbolTable(top);/*函数前的创建符号表*/
                                        (yyval.node) = (yyvsp[-1].node);
}
#line 1536 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 160 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].node)->sym->Define(SymbolType::FunPtr,0,(yyvsp[-1].node)->val);
                                            (yyvsp[-2].node)->sym->print();//printf(" [%d]\n",$2->val);
                                            std::stringstream ss;
                                            ss<<" ["<< (yyvsp[-1].node)->val <<"]\n";
                                            Output::gen(ss.str());
 }
#line 1547 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 168 "parser.y" /* yacc.c:1646  */
    {   delete top;  top = save.top();save.pop();//恢复符号表
         (yyval.node) = new Node((yyvsp[-7].node),(yyvsp[-2].node),NodeType::Fundfn,(yyvsp[-6].node)->val); 
         (yyvsp[-2].node)->nextlist.backpatch((yyvsp[-1].node)->instr);
         Output::gen("end ");(yyvsp[-7].node)->sym->print();Output::gen("\n");
}
#line 1557 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 174 "parser.y" /* yacc.c:1646  */
    { 
                                            (yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::Stmts,0);
                                            (yyvsp[-2].node)->nextlist.backpatch((yyvsp[-1].node)->instr);
}
#line 1566 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 178 "parser.y" /* yacc.c:1646  */
    { 
                                            (yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::Stmts,0);
                                            (yyvsp[-2].node)->nextlist.backpatch((yyvsp[-1].node)->instr);
                                            (yyval.node)->nextlist = (yyvsp[0].node)->nextlist;
}
#line 1576 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 183 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1582 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 184 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1588 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 187 "parser.y" /* yacc.c:1646  */
    {     (yyval.node) = new Node((yyvsp[-3].node),(yyvsp[-2].node),NodeType::Fundcl,(yyvsp[-2].node)->val);
            (yyvsp[-3].node)->sym->Declear(SymbolType::FunPtr,0,(yyvsp[-2].node)->val);
            delete top;  top = save.top(); save.pop();}
#line 1596 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 191 "parser.y" /* yacc.c:1646  */
    {   save.push(top);top = new SymbolTable(top);/*函数前的创建符号表*/ 
                                            Output::gen("f_main [0]\n");
}
#line 1604 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 194 "parser.y" /* yacc.c:1646  */
    {   
                                            (yyvsp[-2].node)->nextlist.backpatch((yyvsp[-1].node)->instr);
                                            Output::gen("end f_main\n");
                                            delete top;
                                            top = save.top(); save.pop(); //恢复符号表
                                    }
#line 1615 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 201 "parser.y" /* yacc.c:1646  */
    {(yyval.type) = SymbolType::Int;}
#line 1621 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 203 "parser.y" /* yacc.c:1646  */
    { 
                                            (yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::Stmts,0);
                                            (yyvsp[-2].node)->nextlist.backpatch((yyvsp[-1].node)->instr);
                                            (yyval.node)->nextlist = (yyvsp[0].node)->nextlist;
}
#line 1631 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 209 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[0].node);}
#line 1637 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 211 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = new Node(NULL,NULL,NodeType::Empty,0); 
            (yyval.node)->instr.Init(Output::gen(""));
}
#line 1645 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 215 "parser.y" /* yacc.c:1646  */
    { (yyval.node) = new Node(NULL,NULL,NodeType::Empty,0);
            (yyval.node)->nextlist = Gotolist(Output::gen("goto "));
}
#line 1653 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 219 "parser.y" /* yacc.c:1646  */
    {save.push(top);top = new SymbolTable(top);}
#line 1659 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 220 "parser.y" /* yacc.c:1646  */
    { delete top;  top = save.top(); save.pop();
                                     (yyval.node) = (yyvsp[-1].node);
}
#line 1667 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 223 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node(NULL,NULL,NodeType::Stmts,0);}
#line 1673 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 224 "parser.y" /* yacc.c:1646  */
    {   (yyvsp[-3].node)->truelist.backpatch((yyvsp[-1].node)->instr);
                                         (yyval.node) = new Node((yyvsp[-3].node),(yyvsp[0].node),NodeType::If,0);
                                         (yyval.node)->nextlist = (yyvsp[-3].node)->falselist.merge((yyvsp[0].node)->nextlist);
                                        
}
#line 1683 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 229 "parser.y" /* yacc.c:1646  */
    {
                                        (yyval.node) = new Node((yyvsp[-7].node),(yyvsp[-4].node),NodeType::IfElse,0);
                                        (yyvsp[-7].node)->truelist.backpatch((yyvsp[-5].node)->instr);
                                        (yyvsp[-7].node)->falselist.backpatch((yyvsp[-1].node)->instr);
                                        (yyval.node)->nextlist = (yyvsp[-4].node)->nextlist.merge((yyvsp[0].node)->nextlist);
                                        (yyval.node)->nextlist = (yyval.node)->nextlist.merge((yyvsp[-3].node)->nextlist);
}
#line 1695 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 236 "parser.y" /* yacc.c:1646  */
    {
                                        (yyval.node) = new Node((yyvsp[-3].node),(yyvsp[0].node),NodeType::While,0);
                                        (yyvsp[0].node)->nextlist.backpatch((yyvsp[-5].node)->instr);
                                        (yyvsp[-3].node)->truelist.backpatch((yyvsp[-1].node)->instr);
                                        (yyval.node)->nextlist = (yyvsp[-3].node)->falselist;
                                        Gotolist(Output::gen("goto ")).backpatch((yyvsp[-5].node)->instr);
                                        // Output::gen("goto "+$2->instr.print());
                                        
}
#line 1709 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 245 "parser.y" /* yacc.c:1646  */
    {(yyval.node) =new Node((yyvsp[-3].node),(yyvsp[-1].node),NodeType::Assign,'=');
                                            (yyvsp[-3].node)->sym->print();
                                            Output::gen(" = ");
                                            (yyvsp[-1].node)->sym->print();
                                            Output::gen("\n");
}
#line 1720 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 251 "parser.y" /* yacc.c:1646  */
    {
                                        Symbol * four = new Symbol(SymbolType::Immediate,4);
                                        (yyvsp[-2].node)->sym = Symbol::ProcessDualOp(four,(yyvsp[-2].node)->sym,"*");

}
#line 1730 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 256 "parser.y" /* yacc.c:1646  */
    {(yyval.node) =new Node((yyvsp[-7].node),(yyvsp[-5].node),NodeType::ArrayAssign,'=');
                                            (yyvsp[-7].node)->sym->print();
                                            Output::gen(" [");
                                            (yyvsp[-5].node)->sym->print();
                                            Output::gen("] = ");
                                            (yyvsp[-1].node)->sym->print();
                                            Output::gen("\n");
}
#line 1743 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 265 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-1].node),NULL,NodeType::Return,0);
                                            Output::gen("return ");(yyvsp[-1].node)->sym->print();Output::gen("\n");}
#line 1750 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 268 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualArith,'+');
                                           (yyval.node)->sym = Symbol::ProcessDualOp((yyvsp[-2].node)->sym,(yyvsp[0].node)->sym,"+");
}
#line 1758 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 271 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualArith,'-');
                                           (yyval.node)->sym = Symbol::ProcessDualOp((yyvsp[-2].node)->sym,(yyvsp[0].node)->sym,"-");
}
#line 1766 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 274 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualArith,'*');
                                           (yyval.node)->sym = Symbol::ProcessDualOp((yyvsp[-2].node)->sym,(yyvsp[0].node)->sym,"*");
}
#line 1774 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 277 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualArith,'/');
                                           (yyval.node)->sym = Symbol::ProcessDualOp((yyvsp[-2].node)->sym,(yyvsp[0].node)->sym,"/");
}
#line 1782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 280 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualArith,'%');
                                           (yyval.node)->sym = Symbol::ProcessDualOp((yyvsp[-2].node)->sym,(yyvsp[0].node)->sym,"%%");
}
#line 1790 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 283 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualLogic,'<');
                                        //    $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"<");
                                            Output::gen("if ");(yyvsp[-2].node)->sym->print();Output::gen(" < ");
                                            (yyvsp[0].node)->sym->print();
                                            (yyval.node)->truelist = Gotolist(Output::gen(" goto "));
                                            (yyval.node)->falselist = Gotolist(Output::gen("goto "));
}
#line 1802 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 290 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualLogic,'>');
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,">");
                                           Output::gen("if ");(yyvsp[-2].node)->sym->print();Output::gen(" > ");
                                            (yyvsp[0].node)->sym->print();
                                            (yyval.node)->truelist = Gotolist(Output::gen(" goto "));
                                            (yyval.node)->falselist = Gotolist(Output::gen("goto "));
}
#line 1814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 297 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualLogic,EQUAL);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"==");
                                           Output::gen("if ");(yyvsp[-2].node)->sym->print();Output::gen(" == ");
                                            (yyvsp[0].node)->sym->print();
                                            (yyval.node)->truelist = Gotolist(Output::gen(" goto "));
                                            (yyval.node)->falselist = Gotolist(Output::gen("goto "));
}
#line 1826 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 304 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::DualLogic,NOTEQUAL);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"!=");
                                           Output::gen("if ");(yyvsp[-2].node)->sym->print();Output::gen(" != ");
                                            (yyvsp[0].node)->sym->print();
                                            (yyval.node)->truelist = Gotolist(Output::gen(" goto "));
                                            (yyval.node)->falselist = Gotolist(Output::gen("goto "));
}
#line 1838 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 311 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-3].node),(yyvsp[-1].node),NodeType::DualLogic,LAND);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"&&");
                                           (yyvsp[-3].node)->truelist.backpatch((yyvsp[-1].node)->instr);
                                           (yyval.node)->truelist = (yyvsp[0].node)->truelist;
                                           (yyval.node)->falselist = (yyvsp[-3].node)->falselist.merge((yyvsp[0].node)->falselist);
}
#line 1849 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 317 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-3].node),(yyvsp[-1].node),NodeType::DualLogic,LOR);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"||");
                                           (yyvsp[-3].node)->falselist.backpatch((yyvsp[-1].node)->instr);
                                           (yyval.node)->falselist = (yyvsp[0].node)->falselist;
                                           (yyval.node)->truelist = (yyvsp[-3].node)->truelist.merge((yyvsp[0].node)->truelist);
                                            
}
#line 1861 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 324 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-3].node),(yyvsp[-1].node),NodeType::DualArith,'[');
                                                Symbol * four = new Symbol(SymbolType::Immediate,4);
                                                Symbol * tmpans = Symbol::ProcessDualOp(four,(yyvsp[-1].node)->sym,"*");
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                tmpsym->print();Output::gen(" = ");(yyvsp[-3].node)->sym->print();
                                                Output::gen(" [");tmpans->print();Output::gen("]\n");
                                                (yyval.node)->sym = tmpsym;
}
#line 1875 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 333 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node(NULL,NULL,NodeType::Symbol1,(yyvsp[0].number));
                                                (yyval.node)->sym = new Symbol(SymbolType::Immediate,(yyvsp[0].number));
}
#line 1883 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 336 "parser.y" /* yacc.c:1646  */
    {if((yyvsp[0].node)->sym->decleared == false) {(yyvsp[0].node)->sym->ReportError("Undecleared");}
    (yyval.node) = (yyvsp[0].node);}
#line 1890 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 338 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[0].node),NULL,NodeType::SingleLogic,'!');
                                                // $$->sym = Symbol::ProcessSingleOp($2->sym,"!");
                                                (yyval.node)->falselist = (yyvsp[0].node)->truelist;
                                                (yyval.node)->truelist = (yyvsp[0].node)->falselist;
}
#line 1900 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 343 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[0].node),NULL,NodeType::SingleArith,'-');
                                                (yyval.node)->sym = Symbol::ProcessSingleOp((yyvsp[0].node)->sym,"-");
}
#line 1908 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 346 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node((yyvsp[-3].node),(yyvsp[-1].node),NodeType::Funcall,(yyvsp[-1].node)->val);
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                tmpsym->print();Output::gen(" = ");
                                                Output::gen("call ");(yyvsp[-3].node)->sym->print();Output::gen("\n");
                                                (yyval.node)->sym = tmpsym;
                                                
}
#line 1921 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 354 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = (yyvsp[-1].node);
}
#line 1928 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 357 "parser.y" /* yacc.c:1646  */
    {   (yyval.node) = new Node((yyvsp[-2].node),(yyvsp[0].node),NodeType::Params,(yyvsp[-2].node)->val + 1);
                                        Output::gen("param ");(yyvsp[0].node)->sym->print();Output::gen("\n");
}
#line 1936 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 360 "parser.y" /* yacc.c:1646  */
    { (yyval.node)=new Node(NULL,(yyvsp[0].node),NodeType::Params,1);
                                        Output::gen("param ");(yyvsp[0].node)->sym->print();Output::gen("\n");
}
#line 1944 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 363 "parser.y" /* yacc.c:1646  */
    {(yyval.node) = new Node(NULL,NULL,NodeType::Params,0);}
#line 1950 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 365 "parser.y" /* yacc.c:1646  */
    {    (yyval.node) = new Node(NULL,NULL,NodeType::Symbol1,0);
                                        Symbol * sym = top->get((yyvsp[0].name));
                                        Symbol * here = top->gethere((yyvsp[0].name));
                                        if(sym == NULL)
                                        {
                                            sym = new Symbol((yyval.node));
                                            top->put((yyvsp[0].name),sym);
                                        }
                                        else if(here == NULL)
                                        {
                                            (yyval.node)->val = 1;//标记该标识符在最内层函数中未定义。
                                        }
                                        sym->funName = (char*)malloc(4+strlen((yyvsp[0].name)));
                                        strcpy(sym->funName,"f_");
                                        strcat(sym->funName,(yyvsp[0].name));
                                        (yyval.node)->sym = sym;
}
#line 1972 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1976 "y.tab.c" /* yacc.c:1646  */
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
#line 384 "parser.y" /* yacc.c:1906  */


int yyerror(const char *msg)            //输出错误信息的yyerror()函数
{
std::cerr<<"Error encountered: "<<msg<<std::endl;
return 0;
}
