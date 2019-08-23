// Copyright (c) 2019 Marcus Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "common.h"
#include "parse.h"
#include "lex.h"
#include "stack.h" // Stack
#include "utility.h" // push_timer, pop_timer, get_file_content

#define DEBUG_START \
    xassert(ctx); \
    debug("%s: %s", __func__, token_kind_to_str(kind(ctx)));

typedef struct {
    Token* tokens;
    Token curr;
    Stack* stack;
} ParserContext;

static Token_Kind kind(ParserContext* ctx) {
    return ctx->curr.kind;
}

static char* value(ParserContext* ctx) {
    return token_value(ctx->curr);
}

static void eat(ParserContext* ctx) {
    ctx->curr = *ctx->tokens++;
}

static bool tokIs(ParserContext* ctx, Token_Kind tk) {
    return tk == kind(ctx);
}

static void match(ParserContext* ctx, Token_Kind tk) {
    if (!tokIs(ctx, tk))
        error("expected '%s' got '%s'", token_kind_to_str(tk), value(ctx));
    eat(ctx);
}

static Loc_Info loc(ParserContext* ctx) {
    s64 line = ctx->curr.line;
    s64 col = ctx->curr.col;
    return (Loc_Info){line, col};
}

typedef enum {
    STATE_END = 0,
    STATE_IDENTIFIER,
    _STATE_FINAL_,
    STATE_UNKNOWN,
    _STATE_COUNT_
} State_Kind;

typedef enum {
    ACTION_STOP = 0,
    ACTION_ERROR,
    ACTION_SHIFT,
    ACTION_REDUCE,
    _ACTION_COUNT_
} Action_Kind;

typedef enum {
    EQUIV_EOF = 0,
    EQUIV_STATEMENT,
    EQUIV_EXPRESSION,
    EQUIV_PRIMARY,
    EQUIV_TERMINAL,
    _EQUIV_COUNT_
} Equivalence_Kind;

static char* equivalence_kind_to_str(Equivalence_Kind kind) {
    switch (kind) {
    ERROR_UNHANDLED_KIND(strf("%d", kind));
    case EQUIV_EOF: return        "EQUIV_EOF";
    case EQUIV_STATEMENT: return  "EQUIV_STATEMENT";
    case EQUIV_EXPRESSION: return "EQUIV_EXPRESSION";
    case EQUIV_PRIMARY: return    "EQUIV_PRIMARY";
    case EQUIV_TERMINAL: return   "EQUIV_TERMINAL";
    case _EQUIV_COUNT_: return    "_EQUIV_COUNT_";
    }
    UNREACHABLE;
    return NULL;
}
static char* state_kind_to_str(State_Kind kind) {
    switch (kind) {
    ERROR_UNHANDLED_KIND(strf("%d", kind));
    case STATE_END: return "STATE_END";
    case STATE_UNKNOWN: return "STATE_UNKNOWN";
    case STATE_IDENTIFIER: return "STATE_IDENTIFIER";
    case _STATE_FINAL_: return "_STATE_FINAL_";
    case _STATE_COUNT_: return "_STATE_COUNT_";
    }
    UNREACHABLE;
    return NULL;
}
static char* action_kind_to_str(Action_Kind kind) {
    switch (kind) {
    ERROR_UNHANDLED_KIND(strf("%d", kind));
    case ACTION_STOP:    return "ACTION_STOP";
    case ACTION_ERROR:   return "ACTION_ERROR";
    case ACTION_SHIFT:   return "ACTION_SHIFT";
    case ACTION_REDUCE:  return "ACTION_REDUCE";
    case _ACTION_COUNT_: return "_ACTION_COUNT_";
    }
    UNREACHABLE;
    return NULL;
}

static Action_Kind transition[_STATE_COUNT_][_EQUIV_COUNT_] = {
    
    [STATE_UNKNOWN] [EQUIV_STATEMENT] = ACTION_SHIFT,
    [STATE_UNKNOWN] [EQUIV_EXPRESSION] = ACTION_SHIFT,
    [STATE_UNKNOWN] [EQUIV_PRIMARY] = ACTION_SHIFT,
    
    [STATE_UNKNOWN] [EQUIV_TERMINAL] = ACTION_REDUCE,
    [STATE_UNKNOWN] [EQUIV_EOF] = ACTION_STOP,
};

static Equivalence_Kind equivalence[_TOKEN_COUNT_] = {
    
    [TOKEN_EOF]    = EQUIV_EOF,
    
    [TOKEN_INTEGER]  = EQUIV_PRIMARY,
    [TOKEN_FLOAT]    = EQUIV_PRIMARY,
    [TOKEN_STRING]   = EQUIV_PRIMARY,
    
    [TOKEN_IDENTIFIER]    = EQUIV_STATEMENT,
    [TOKEN_OPEN_PAREN]    = EQUIV_EXPRESSION,
    [TOKEN_CLOSE_PAREN]   = EQUIV_EXPRESSION,
    [TOKEN_EQ_GT]         = EQUIV_EXPRESSION,

    [TOKEN_SEMICOLON]  = EQUIV_TERMINAL,
    [TOKEN_NEWLINE]    = EQUIV_TERMINAL,
};

AST* parse2(char* source) { // @Temp: name
    Token* tokens = lex(source).tokens.data;
    
    Action_Kind action = ACTION_SHIFT;
    State_Kind state = STATE_UNKNOWN;

    Stack* stack = make_stack();
    List* nodes = make_list();
    Token token;

    do {
        do {
            token = *tokens++;
            Equivalence_Kind eq = equivalence[token.kind];
            action = transition[state][eq];
            info("%s %s %s <- %s", state_kind_to_str(state), equivalence_kind_to_str(eq), action_kind_to_str(action), ucolor(token_kind_to_str(token.kind)));
        } while (state > _STATE_FINAL_);

        AST* node = NULL;
        Loc_Info lc = (Loc_Info){token.line, token.col};

        switch (state) {
            ERROR_UNHANDLED_KIND(state_kind_to_str(state));
            case STATE_IDENTIFIER: {
                node = make_ast_ident(lc, token_value(token));
            } break;
        }

        warning("%s", ast_to_str(node));

        stack_foreach(stack) warning("%s", ast_to_str(it->data));

        switch (action) {
            default: break;
            case ACTION_SHIFT: xassert(node); stack_push(stack, node); break;
            case ACTION_REDUCE: list_append(nodes, stack_pop(stack)); break;
        }
    
    } while (action); // ACTION_STOP == 0 @Volatile

    UNREACHABLE; // temp
    return NULL;
}
