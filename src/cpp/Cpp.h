#pragma once
// The typed C/C++ front-end (src/cpp/).
//
//   preprocess  (CppLex.cpp)    tokens, #include "local" (+ the matching
//                               implementation files), macros, conditionals
//   parse       (CppParse.cpp)  a C++ AST with full declarator types
//   lower       (CppLower.cpp)  name lookup, overloads and static types, then
//                               the ordinary Quantum AST with C++ semantics
//                               spelled out: integer division, copies at copy
//                               points, tagged variants, typed brace-init,
//                               char as a number, formatted streams. The
//                               runtime helpers it calls are __cpp_* natives
//                               (src/vm/VmCpp.cpp).
//
// Anything outside the supported subset throws Unsupported; compileSource()
// then falls back to the older dialect pipeline. QUANTUM_DEBUG_CPPFE=1 prints
// why.

#include "AST.h"

#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace cpp
{

// A real error in the program (not a gap in the supported subset): reported,
// not handed to the fallback pipeline.
struct LinkError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

struct Unsupported : std::runtime_error
{
    std::string file;
    int line;
    Unsupported(const std::string &msg, const std::string &f = "", int l = 0)
        : std::runtime_error(msg), file(f), line(l) {}
};

// ─── Tokens ────────────────────────────────────────────────────────────────

enum class TK
{
    Ident,
    Int,
    Float,
    Char,
    String,
    Punct,
    End
};

struct Token
{
    TK kind = TK::End;
    std::string text;  // as written (Punct/Ident/numbers), or the literal's spelling
    std::string value; // decoded bytes of a string / char literal
    int file = 0;
    int line = 0;
    bool bol = false;          // first token on its line
    bool spaceBefore = false;  // whitespace precedes it (for # stringification)
    int tu = 0;                // translation unit (anonymous namespaces)
};

struct Preprocessed
{
    std::vector<Token> tokens; // ends with an End token
    std::vector<std::string> files;
};

// The program: `path` and, appended, the implementation files of the local
// headers it includes (x.h → x.cpp / src/x.cpp), each a translation unit.
Preprocessed preprocess(const std::string &path, const std::string &source);

// ─── Syntax ────────────────────────────────────────────────────────────────

struct Expr;
struct Stmt;
struct TypeName;
struct Decl;
using ExprP = std::shared_ptr<Expr>;
using StmtP = std::shared_ptr<Stmt>;
using TypeNameP = std::shared_ptr<TypeName>;
using DeclP = std::shared_ptr<Decl>;

// A template argument: a type, or an expression (std::array<int, 3>).
struct TArg
{
    TypeNameP type;
    ExprP expr;
};

// A type as written: `const std::vector<Row>&`, `int (*)(int)`, `char[16]`.
struct TypeName
{
    std::vector<std::string> name;        // qualified, e.g. {"std","vector"}; builtins as one word ("unsigned long")
    std::vector<std::vector<TArg>> targs; // per name component (empty when none)
    bool hasTargs = false;
    bool isConst = false;
    int pointers = 0;
    std::vector<bool> constPtr; // `* const`
    bool ref = false;           // T&
    bool rref = false;          // T&&
    std::vector<ExprP> arrayDims; // T x[N][M]; null = []
    // Function types: std::function<R(Args)>, function pointers.
    bool isFunction = false;
    TypeNameP ret;
    std::vector<TypeNameP> params;
    bool isAuto() const { return name.size() == 1 && name[0] == "auto"; }
    std::string str() const;
};

struct Param
{
    TypeNameP type;
    std::string name;
    ExprP def;
};

struct Lambda;
using LambdaP = std::shared_ptr<Lambda>;

enum class EK
{
    Int,
    Float,
    Char,
    Str,
    Bool,
    Nullptr,
    Name,     // (qualified) id, optional template args
    This,
    Unary,    // op: - + ! ~ ++ -- * & (prefix)
    Postfix,  // ++ --
    Binary,
    Assign,   // = += ...
    Cond,
    Call,
    Member,   // a.b / a->b (arrow)
    Index,
    Cast,     // static_cast<T>(e), (T)e, T(e)
    SizeofT,
    SizeofE,
    New,
    Delete,
    Lambda,
    InitList, // { a, b }
    Construct, // T{args} / T(args) with a known type
    Comma,
    Throw,
};

struct Expr
{
    EK k;
    int line = 0;
    std::string op;            // operator / member name / cast kind
    std::string str;           // decoded literal bytes
    double num = 0;
    bool isUnsigned = false, isLong = false, isFloatSuffix = false;
    std::vector<std::string> name;        // EK::Name / member: qualified parts
    std::vector<TArg> targs;              // template args on the last name component
    bool hasTargs = false;
    std::vector<ExprP> kids;
    TypeNameP type;            // casts, new, sizeof, construct
    bool arrow = false;        // Member via ->
    bool braced = false;       // Construct with {} / New with {}
    bool newArray = false;
    LambdaP lambda;
};

struct VarDeclarator
{
    std::string name;
    TypeNameP type; // the declarator applied to the base type
    enum Init
    {
        None,
        Copy,   // = e
        Direct, // (a, b)
        Brace,  // {a, b} or = {a, b}
    } init = None;
    std::vector<ExprP> args;
    int line = 0;
    // Structured binding: auto [a, b] = e;
    std::vector<std::string> bindings;
};

enum class SK
{
    Expr,
    Decl,
    Block,
    If,
    While,
    Do,
    For,
    RangeFor,
    Switch,
    Case,
    Default,
    Break,
    Continue,
    Return,
    Try,
    Empty,
    LocalClass, // struct/enum/using defined inside a function body
};

struct Catch
{
    TypeNameP type; // null = catch (...)
    std::string name;
    StmtP body;
};

struct Stmt
{
    SK k;
    int line = 0;
    ExprP expr;                  // Expr, conditions, Return value, Case value, Switch value
    std::vector<VarDeclarator> decls; // Decl
    bool isStatic = false;       // Decl: static local
    std::vector<StmtP> body;     // Block, Switch body statements
    StmtP init, then, els;       // If/For/While init (decl), branches; For body = then
    ExprP cond2, step;           // For condition / step
    StmtP condDecl;              // if (auto p = ...), while (T x = ...)
    std::vector<Catch> catches;  // Try (body in `then`)
    DeclP local;                 // LocalClass
};

struct Lambda
{
    std::vector<Param> params;
    TypeNameP ret; // null = deduced
    StmtP body;
    bool captureAllByRef = false, captureAllByValue = false;
    std::vector<std::string> captures; // explicit (prefixed by & when by reference)
    bool isMutable = false;
};

enum class DK
{
    Func,
    Var,
    Class,
    Enum,
    Alias,
    Namespace,
    UsingDirective,
    UsingDecl,
    StaticAssert,
    Empty,
};

struct MemInit
{
    std::string name; // member or base class
    std::vector<ExprP> args;
    bool braced = false;
};

struct Decl
{
    DK k;
    int line = 0;
    int file = 0;
    int tu = 0;
    std::vector<std::string> name; // qualified (out-of-class: {"Parser","peek"})
    std::vector<std::string> templateParams; // template<typename T, ...>
    bool isTemplate = false;
    // Func
    TypeNameP ret;
    std::vector<Param> params;
    bool variadic = false;
    bool isConst = false, isStatic = false, isVirtual = false, isPure = false, isExplicit = false;
    bool isCtor = false, isDtor = false, isDefault = false, isDeleted = false, isOperator = false;
    bool isFriend = false, isInline = false, isConstexpr = false, noreturn = false;
    std::vector<MemInit> inits;
    StmtP body; // null = prototype
    // Var (one Decl per declarator)
    VarDeclarator var;
    bool isExtern = false;
    // Class
    bool isStruct = false, isUnion = false;
    std::vector<TypeNameP> bases;
    std::vector<DeclP> members;
    bool forwardOnly = false;
    // Enum
    bool scoped = false;
    TypeNameP underlying;
    std::vector<std::pair<std::string, ExprP>> enumerators;
    // Alias: using X = T / typedef T X
    TypeNameP aliased;
    // Namespace
    std::vector<DeclP> decls;
    bool anonymous = false;
    // using-declaration / directive: name
    bool inheritingCtor = false; // using Base::Base;
};

struct Program
{
    std::vector<DeclP> decls;
    std::vector<std::string> files;
};

Program parse(const Preprocessed &pp);

// ─── Entry point ───────────────────────────────────────────────────────────

// Preprocesses, parses and lowers the C/C++ program at `path`; the result is
// the Quantum AST for compileSource() to compile. Throws Unsupported.
ASTNodePtr compileProgram(const std::string &path, const std::string &source);

} // namespace cpp
