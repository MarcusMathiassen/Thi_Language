#ifndef CST_H
#define CST_H

#include "lexer.h"  // Token_Kind
#include "list.h"   // List

//------------------------------------------------------------------------------
//                               ast.h
//------------------------------------------------------------------------------
typedef struct CST    CST;
typedef enum CST_Kind CST_Kind;

enum CST_Kind {

    CST_TOKEN,

    CST_INT,
    CST_FLOAT,
    CST_STRING,

    CST_IDENT,

    CST_BLOCK,

    CST_IF,
    CST_CASE,
    CST_FOR,

    CST_COUNT
};

//------------------------------------------------------------------------------
//                          AST Expression Structures
//------------------------------------------------------------------------------

struct CST {

    CST_Kind kind;
    
    union {

        Token token;

        typedef struct { s64 val; } Int;
        typedef struct { f64 val; } Float;
        typedef struct { char* val; } String;

        typedef struct { char* name; } Ident;

        typedef struct { List* cst; } Block;

        typedef struct {} If;
        typedef struct {} For;
        typedef struct {} Case;

        typedef struct {} Break;
        typedef struct {} Continue;
        typedef struct {} Return;
    };
};

char* cst_to_json(CST* cst);
char* cst_to_str(CST* cst);
char* cst_kind_to_str(CST_Kind kind);

#endif
